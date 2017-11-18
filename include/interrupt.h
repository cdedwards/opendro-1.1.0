#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "fifo.h"

void
interrupt_fiq(void);

void
interrupt_irq(void);

void
interrupt_spu(void);

void
interrupt_disable(void);

void
interrupt_enable(void);

fifo32_t gpio_fifo;

#endif /*INTERRUPT_H_*/
