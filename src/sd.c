///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// SD card driver using bit-banged SPI
//
// Copyright (C) 2011 ShumaTech http://www.shumatech.com/
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////////

#include "sd.h"
#include "gpio.h"
#include "timer.h"
#include "irq.h"
#include "trace.h"
#include "swi.h"

static bool card_detect;
static bool card_ready;
static bool card_wp;
static bool block_addr;
static timer_node_t sd_timer;

#define READY_RETRIES       10000
#define RESP_RETRIES        100
#define START_RETRIES       2500
#define IDLE_RETRIES        2500

#define RESP_IDLE           0x1
#define RESP_ERROR(resp)    ((resp) & 0xfe)
#define RESP_ERROR_ERASE    0x02
#define RESP_ERROR_CMD      0x04
#define RESP_ERROR_CRC      0x08
#define RESP_ERROR_SEQ      0x10
#define RESP_ERROR_ADDR     0x20
#define RESP_ERROR_PARAM    0x40
#define RESP_ERROR_TIMEOUT  0x80

#define SD_VOLTAGE_RANGE    (0x3 << 20)

#define CMD_START           0x40

#define CMD0                0
#define CMD8                8
#define CMD9                9
#define CMD10               10
#define CMD16               16
#define CMD55               55
#define CMD58               58
#define CMD58_OCR_CCS       (1 << 30)
#define ACMD41              41
#define ACMD41_HCS          (1 << 30)

#define CMD0_CRC            0x95
#define CMD8_CRC            0x87

#define CMD8_CHECK          0xaa
#define CMD8_VHS_2_7V_3_3V  0x100

#define RESP_START          0x80

#define START_BLOCK         0xfe

#define DATA_ERROR          0xe0

#define DATA_RESPONSE       0x1f
#define DATA_ACCEPT         0x05

#define SPI_HIGH            0xff

static uint8_t
sd_spi_xfer(uint8_t out)
{
    uint8_t in = 0;
    uint8_t bit = 0x80;
    
    // Before the SD card is ready, we must keep the SPI
    // clock less than 400kHz so the version in the
    // else close has 1 microsecond delays after each 
    // clock transition.
    if (card_ready)
    {
        while (bit)
        {
            if (out & bit)
                GPIO_HIGH(SD_CARD_PORT, SD_CARD_CMD)
            else
                GPIO_LOW(SD_CARD_PORT, SD_CARD_CMD)
            
            GPIO_HIGH(SD_CARD_PORT, SD_CARD_SCLK);
            
            if (GPIO_IS_HIGH(SD_CARD_PORT, SD_CARD_D0))
                in |= bit;
            
            bit >>= 1;
            
            GPIO_LOW(SD_CARD_PORT, SD_CARD_SCLK);
        }
        
        return in;
    }
    else
    {
        while (bit)
        {
            if (out & bit)
                GPIO_HIGH(SD_CARD_PORT, SD_CARD_CMD)
            else
                GPIO_LOW(SD_CARD_PORT, SD_CARD_CMD)
            
            timer_delay();

            GPIO_HIGH(SD_CARD_PORT, SD_CARD_SCLK);
            
            if (GPIO_IS_HIGH(SD_CARD_PORT, SD_CARD_D0))
                in |= bit;
            
            timer_delay();
            
            bit >>= 1;

            GPIO_LOW(SD_CARD_PORT, SD_CARD_SCLK);
        }
        
        return in;
    }
}

static void
sd_select(void)
{
    // Turn the LED on
    GPIO_LOW(SD_CARD_PORT, SD_CARD_IND);

    // Set chip select
    GPIO_LOW(SD_CARD_PORT, SD_CARD_CS);
}

static void
sd_deselect(void)
{
    // Clear chip select
    GPIO_HIGH(SD_CARD_PORT, SD_CARD_CS);
    
    // Turn the LED off
    GPIO_HIGH(SD_CARD_PORT, SD_CARD_IND);
}

static void
sd_cmd_response(uint8_t cmd, uint32_t args, uint8_t* response, uint32_t len)
{
    uint32_t retries;
    
    for (retries = 0; retries < READY_RETRIES; retries++)
    {
        if (sd_spi_xfer(SPI_HIGH) == SPI_HIGH)
            break;
    }
    
    if (retries == READY_RETRIES)
    {
        TRACE_ERROR("SD cmd exceeded ready retries\n");
        *response = RESP_ERROR_TIMEOUT;
        return;
    }
    
    sd_spi_xfer(CMD_START | cmd);
    sd_spi_xfer((args >> 24) & 0xff);
    sd_spi_xfer((args >> 16) & 0xff);
    sd_spi_xfer((args >> 8) & 0xff);
    sd_spi_xfer(args & 0xff);
    sd_spi_xfer(cmd == CMD8 ? CMD8_CRC : CMD0_CRC);
    sd_spi_xfer(SPI_HIGH);
 
    for (retries = 0; retries < RESP_RETRIES; retries++)
    {
        *response = sd_spi_xfer(SPI_HIGH);
        if (!(*response & RESP_START))
            break;
    }       

    if (retries == RESP_RETRIES)
    {
        TRACE_ERROR("SD cmd exceeded response retries\n");
        *response = RESP_ERROR_TIMEOUT;
        return;
    }

    while (--len)
        *++response = sd_spi_xfer(SPI_HIGH);
}

static bool
sd_block_read(uint8_t* block, uint32_t len)
{
    uint8_t token;
    uint32_t retries;
    
    // Wait for start block token
    for (retries = 0; retries < START_RETRIES; retries++)
    {
        token = sd_spi_xfer(SPI_HIGH);
        if (token == START_BLOCK)
            break;
        if (!(token & DATA_ERROR))
        {
            TRACE_ERROR("Block read data error\n");
            return false;
        }
    }       
    
    // Too many retries?
    if (retries == START_RETRIES)
    {
        TRACE_ERROR("Block read exceeded start retries\n");
        return false;
    }

    // Read the block
    while (len--)
        *block++ = sd_spi_xfer(SPI_HIGH);

    // Read dummy CRC
    sd_spi_xfer(SPI_HIGH);
    sd_spi_xfer(SPI_HIGH);

    return true;
}

static bool
sd_block_write(const uint8_t* block, uint32_t len)
{
    uint8_t token;
    
    // Write the start block token
    sd_spi_xfer(START_BLOCK);
    
    // Write the block
    while (len--)
        sd_spi_xfer(*block++);

    // Write dummy CRC
    sd_spi_xfer(SPI_HIGH);
    sd_spi_xfer(SPI_HIGH);

    // Read data response token
    token = sd_spi_xfer(SPI_HIGH);
    if ((token & DATA_RESPONSE) != DATA_ACCEPT)
    {
        TRACE_ERROR("Block write data not accepted\n");
        return false;
    }
        
    return true;
}

static void
sd_init_isr(void)
{
    uint32_t init;
    uint32_t retries;
    uint8_t response;
    uint32_t ocr;
    bool sd_v2;
    
    // Send 80 SPI clocks to put card in SPI mode
    for (init = 0; init < 10; init++)
        sd_spi_xfer(SPI_HIGH);
    
    // Reset card
    response = sd_cmd_r1(CMD0, 0);
    if (RESP_ERROR(response))
    {
        TRACE_ERROR("Unable to reset SD card: %s\n", sd_error(response));
        return;
    }
    
    // Send the host voltage
    response = sd_cmd_r1(CMD8, CMD8_VHS_2_7V_3_3V | CMD8_CHECK);
    if (response & RESP_ERROR_CMD)
    {
        TRACE_INFO("Card is SDv1\n");
        sd_v2 = false;
    }
    else
    {
        TRACE_INFO("Card is SDv2\n");
        sd_v2 = true;
        if (RESP_ERROR(response))
        {
            TRACE_ERROR("Incompatible host voltage: %s\n", sd_error(response));
            return;
        }
    }
    
    // Check the card voltage
    response = sd_cmd_r3(CMD58, 0, &ocr);
    if (RESP_ERROR(response))
    {
        TRACE_WARNING("Cannot check SD card voltage: %s\n", sd_error(response));
    }
    else if (!(ocr & SD_VOLTAGE_RANGE))
    {
        TRACE_ERROR("SD card voltage out of range\n");
        return;
    }
        
    // Check for idle state
    for (retries = 0; retries < IDLE_RETRIES; retries++)
    {
        response = sd_cmd_r1(CMD55, 0);
        if (RESP_ERROR(response))
        {
            TRACE_WARNING("CMD55 error: %s\n", sd_error(response));
            if (response & RESP_ERROR_TIMEOUT)
                return;
        }
        else
        {
            response = sd_cmd_r1(ACMD41, sd_v2 ? ACMD41_HCS : 0);
            if (RESP_ERROR(response))
            {
                TRACE_WARNING("ACMD41 error: %s\n", sd_error(response));
                if (response & RESP_ERROR_TIMEOUT)
                    return;
            }
            else if (!(response & RESP_IDLE))
            {
                break;
            }
        }
    }
    
    if (retries == IDLE_RETRIES)
    {
        TRACE_ERROR("SD card exceeded idle retries\n");
        return;
    }
    
    // Default to byte addressing
    block_addr = false;
    
    if (sd_v2)
    {
        // Get the CCS
        response = sd_cmd_r3(CMD58, 0, &ocr);
        if (RESP_ERROR(response))
        {
            TRACE_WARNING("Cannot get CCS: %s\n", sd_error(response));
        }
        else if (ocr & CMD58_OCR_CCS)
        {
            TRACE_INFO("Card is SDHC\n");
            
            // SDHD uses block addressing
            block_addr = true;
        }
    }
    else
    {
        // Set the block length to 512
        response = sd_cmd_r1(CMD16, 512);
        if (RESP_ERROR(response))
        {
            TRACE_WARNING("Cannot set block length: %s\n", response);
        }
    }
    
    TRACE_INFO("SD card ready\n");
    card_ready = true;
}

static void
sd_timeout(void* user)
{
    irq_trigger(IRQ_SD_INIT);
}

static void
card_detect_isr(void)
{
    uint32_t status;
    
    // Read the status to clear the interrupt
    status = GPIO_INT_STATUS(SD_CARD_PORT);
    
    if (GPIO_IS_LOW(SD_CARD_PORT, SD_CARD_CD))
    {
        if (!card_detect)
        {
            card_detect = true;
            card_wp = false;
            TRACE_INFO("SD card detected\n");
            timer_start(&sd_timer, 100 * MSEC_TICKS, TIMER_ONE_SHOT, sd_timeout, NULL);
#if 0 // LCD-200 write protect is disabled
            if (GPIO_IS_HIGH(SD_CARD_PORT, SD_CARD_WP))
            {
                TRACE_WARNING("SD card is write protected\n");
                card_wp = true;
            }
#endif
            swi_mount(0);
        }
    }
    else
    {
        if (card_detect)
        {
            swi_umount(0);
            card_ready = false;
            card_detect = false;
            TRACE_INFO("SD card removed\n");
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// P U B L I C   A P I
///////////////////////////////////////////////////////////////////////////////

bool
sd_card_ready(void)
{
    return card_ready;
}

bool
sd_card_wp(void)
{
    return card_wp;
}

void
sd_show(FILE* out)
{
    uint8_t cid[16];
    
#define FIELD_WIDTH     12

    fprintf(out, "%-*s : %s\n", FIELD_WIDTH, "Card Detect", card_detect ? "yes" : "no");
    if (!card_detect)
        return;
        
    fprintf(out, "%-*s : %s\n", FIELD_WIDTH, "Card Ready", card_ready ? "yes" : "no");
    if (!card_ready)
        return;

    if (!sd_cmd_read(CMD10, 0, cid, sizeof(cid)))
        return;
        
    fprintf(out, "%-*s : 0x%x\n", FIELD_WIDTH, "Manufact ID", cid[0]);
    fprintf(out, "%-*s : %c%c\n", FIELD_WIDTH, "OEM ID", cid[1], cid[2]);
    fprintf(out, "%-*s : %c%c%c%c%c\n", FIELD_WIDTH, "Product", cid[3], cid[4], cid[5], cid[6], cid[7]);
    fprintf(out, "%-*s : %d.%d\n", FIELD_WIDTH, "Revision", cid[8] >> 4, cid[8] & 0xf);
    fprintf(out, "%-*s : %02x%02x%02x%02x\n", FIELD_WIDTH, "Serial Num", cid[9], cid[10], cid[11], cid[12]);
    fprintf(out, "%-*s : %d/20%d%d\n", FIELD_WIDTH, "Date", cid[14] & 0xf, cid[13] & 0xf, cid[14] >> 4);
    
    fprintf(out, "%-*s : %"PRIu32"K\n", FIELD_WIDTH, "Size", sd_card_sectors() / 2);
}

uint32_t
sd_card_sectors(void)
{
    uint8_t csd[16];
    uint32_t read_bl_len;
    uint32_t c_size;
    uint32_t c_size_mult;
    uint32_t mem_size;
    
    if (!sd_cmd_read(CMD9, 0, csd, sizeof(csd)))
        return 0;
    
    // READ_BL_LEN [83:80]
    read_bl_len = ((uint32_t) csd[5] & 0xf);
    
    // C_SIZE [72:62]
    c_size = ((uint32_t) csd[6] & 0x3) << 10 |
             ((uint32_t) csd[7]) << 2 |
             ((uint32_t) csd[8]) >> 6;
    
    // C_SIZE_MULT [49:47]
    c_size_mult = ((uint32_t) csd[9] & 0x3) << 1 |
                  (uint32_t) csd[10] >> 7;
    
    // memory size = (C_SIZE + 1) * 2 ^ (C_SIZE_MULT + 2) * 2 ^ READ_BL_LEN
    mem_size = (c_size + 1) * (1 << (2 + c_size_mult)) * (1 << read_bl_len);
    
    return mem_size / 512;
}

const char*
sd_error(uint8_t response)
{
    if (response & RESP_ERROR_TIMEOUT)
        return "timeout error";
    else if (response & RESP_ERROR_PARAM)
        return "parameter error";
    else if (response & RESP_ERROR_ADDR)
        return "address error";
    else if (response & RESP_ERROR_SEQ)
        return "erase sequence error";
    else if (response & RESP_ERROR_CRC)
        return "communication CRC error";
    else if (response & RESP_ERROR_CMD)
        return "illegal command";
    else if (response & RESP_ERROR_ERASE)
        return "erase error";
    else
        return "no error";
}

uint8_t
sd_cmd_r1(uint8_t cmd, uint32_t args)
{
    uint8_t response;
    
    TRACE_DEBUG("sd_cmd_r1(%d 0x%x)\n", cmd, args);
    sd_select();
    sd_cmd_response(cmd, args, &response, 1);
    sd_deselect();
    TRACE_DEBUG("sd_cmd_r1=%d\n", response);
    
    return response;
}

uint8_t
sd_cmd_r2(uint8_t cmd, uint32_t args, uint8_t* value)
{
    uint8_t buffer[2];
    
    TRACE_DEBUG("sd_cmd_r2(%d 0x%x)\n", cmd, args);
    sd_select();
    sd_cmd_response(cmd, args, buffer, sizeof(buffer));
    *value = buffer[1];
    sd_deselect();
    TRACE_DEBUG("sd_cmd_r2=%d 0x%x\n", buffer[0], value);
    
    return buffer[0];
}

uint8_t
sd_cmd_r3(uint8_t cmd, uint32_t args, uint32_t* value)
{
    uint8_t buffer[5];
    
    TRACE_DEBUG("sd_cmd_r3(%d 0x%x)\n", cmd, args);
    sd_select();
    sd_cmd_response(cmd, args, buffer, sizeof(buffer));
    *value = (uint32_t) buffer[1] << 24 |
             (uint32_t) buffer[2] << 16 |
             (uint32_t) buffer[3] << 8 |
             (uint32_t) buffer[4];
    sd_deselect();
    TRACE_DEBUG("sd_cmd_r3=%d 0x%x\n", buffer[0], value);

    return buffer[0];
}

bool
sd_cmd_read(uint8_t cmd, uint32_t args, uint8_t* block, uint32_t len)
{
    uint8_t success = false;
    uint8_t response;
    
    if (block_addr)
        args /= 512;
        
    TRACE_DEBUG("sd_cmd_read(%d %d %p %u)\n", cmd, args, block, len);
    sd_select();
    sd_cmd_response(cmd, args, &response, 1);
    if (!RESP_ERROR(response))
        if (sd_block_read(block, len))
            success = true;
    sd_deselect();
    TRACE_DEBUG("sd_cmd_read=%d\n", success);
    
    return success;
}

bool
sd_cmd_write(uint8_t cmd, uint32_t args, const uint8_t* block, uint32_t len)
{
    uint8_t success = false;
    uint8_t response;

    if (block_addr)
        args /= 512;

    TRACE_DEBUG("sd_cmd_write(%d %d %p %u)\n", cmd, args, block, len);
    sd_select();
    sd_cmd_response(cmd, args, &response, 1);
    if (!RESP_ERROR(response))
        if (sd_block_write(block, len))
            success = true;
    sd_deselect();
    TRACE_DEBUG("sd_cmd_write=%d\n", success);
    
    return success;
}

void
sd_init(void)
{
    // Configure CS, CMD, SCLK, and IND as outputs
    GPIO_LOW(SD_CARD_PORT, SD_CARD_SCLK);
    GPIO_HIGH(SD_CARD_PORT, SD_CARD_CS | SD_CARD_CMD | SD_CARD_IND);
    GPIO_NOPULLUP(SD_CARD_PORT, SD_CARD_CS | SD_CARD_CMD | SD_CARD_SCLK | SD_CARD_IND);
    GPIO_OPENDRAIN(SD_CARD_PORT, SD_CARD_IND);
    GPIO_OUTPUT(SD_CARD_PORT, SD_CARD_CS | SD_CARD_CMD | SD_CARD_SCLK | SD_CARD_IND);
    
    // Software interrupt for card init
    irq_register(IRQ_SD_INIT, IRQ_PRIORITY_LOWEST, IRQ_TYPE_EDGE, sd_init_isr);
    
    // Interrupt on card detect
    GPIO_INTERRUPT(SD_CARD_PORT, SD_CARD_CD);
    irq_register(IRQ_SD_DETECT, IRQ_PRIORITY_LOWEST, IRQ_TYPE_LEVEL, card_detect_isr);
    
    // Card detect
    card_detect_isr();
}

///////////////////////////////////////////////////////////////////////////////
// L U A
///////////////////////////////////////////////////////////////////////////////

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "lrotable.h"
#include "luash.h"
#define MIN_OPT_LEVEL 2
#include "lrodefs.h"

static int
sd_show_lua(lua_State* L)
{
    sd_show(luash_out(L));
    return 0;
}

static int
sd_dir_lua(lua_State* L)
{
    FILE* out;
    DIR* dir;
    struct dirent* dent;
    
    out = luash_out(L);
    
    dir = swi_opendir(luaL_checkstring(L, 1));
    if (!dir)
        return luaL_error(L, "unable to open directory");
    
    while ((dent = swi_readdir(dir)))
    {
        fprintf(out, "%c%c%c%c%c %10u %04d-%02d-%02d %02d:%02d %s\n",
                (dent->d_attrib & ATTRIB_DIR) ? 'd' : '-',
                (dent->d_attrib & ATTRIB_RO) ? 'r' : '-',
                (dent->d_attrib & ATTRIB_HID) ? 'h' : '-',
                (dent->d_attrib & ATTRIB_SYS) ? 's' : '-',
                (dent->d_attrib & ATTRIB_ARC) ? 'a' : '-',
                dent->d_size, dent->d_time.tm_year + 1900, dent->d_time.tm_mon,
                dent->d_time.tm_mday, dent->d_time.tm_hour, dent->d_time.tm_min,
                dent->d_name);
    }
    
    return 0;
}

static int
sd_mkdir_lua(lua_State* L)
{
    if (!swi_mkdir(luaL_checkstring(L, 1)))
        return luaL_error(L, "unable to make directory");
    
    return 0;
}

static int
sd_rename_lua(lua_State* L)
{
    if (!swi_rename(luaL_checkstring(L, 1), luaL_checkstring(L, 2)))
        return luaL_error(L, "unable to rename file");
    
    return 0;
}

static int
sd_remove_lua(lua_State* L)
{
    if (!swi_unlink(luaL_checkstring(L, 1)))
        return luaL_error(L, "unable to remove file");
    
    return 0;
}

static int
sd_read_lua(lua_State* L)
{
    if (!luash_cat(luaL_checkstring(L, 1)))
        return luaL_error(L, "unable to open file");
    
    return 0;
}

static int
sd_write_lua(lua_State* L)
{
    if (!luash_pipe(luaL_checkstring(L, 1)))
        return luaL_error(L, "unable to open file");
    
    return 0;
}

static int
sd_hd_lua(lua_State* L)
{
    FILE* file;
    FILE* out;
    size_t len;
    uint8_t buffer[128];
    int32_t addr;
    int32_t offset;
    
    out = luash_out(L);
    
    file = fopen(luaL_checkstring(L, 1), "r");
    if (!file)
        return luaL_error(L, "unable to open file");

    addr = 0;
    while ((len = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        for (offset = 0; offset < len; offset++)
        {
            if (addr % 16 == 0)
                fprintf(out, "%08"PRIx32" ", addr);
            fprintf(out, "%02x%s", buffer[offset],
                    (addr % 16 == 15) ? "\n" : " ");
            addr++;
        }
    }
    if (addr % 16 != 0)
        fprintf(out, "\n");
        
    fclose(file);
    
    return 0;
}

const LUA_REG_TYPE sd_map_lua[] =
{
    { LSTRKEY("show"), LFUNCVAL(sd_show_lua) },
    { LSTRKEY("dir"), LFUNCVAL(sd_dir_lua) },
    { LSTRKEY("mkdir"), LFUNCVAL(sd_mkdir_lua) },
    { LSTRKEY("rename"), LFUNCVAL(sd_rename_lua) },
    { LSTRKEY("remove"), LFUNCVAL(sd_remove_lua) },
    { LSTRKEY("read"), LFUNCVAL(sd_read_lua) },
    { LSTRKEY("write"), LFUNCVAL(sd_write_lua) },
    { LSTRKEY("hd"), LFUNCVAL(sd_hd_lua) },
    { LNILKEY, LNILVAL }
};

LUALIB_API int
sd_open_lua(lua_State* L)
{
    LREGISTER(L, "sd", sd_map_lua);
}
