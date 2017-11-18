# OpenDRO Project
#
# Copyright (C) 2008-2012 ShumaTech http://www.shumatech.com/
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

#
# Project name used for binaries
#
PROJECT   = opendro

#
# Software versioning
#
OPENDRO_MAJOR	= 1
OPENDRO_MINOR	= 1
OPENDRO_MAINT	= 0

RELEASE = $(OPENDRO_MAJOR).$(OPENDRO_MINOR).$(OPENDRO_MAINT)

#
# Platforms
#
PLATFORMS = dro375 dro550 dpu550

#
# Configurations
#
CONFIGS_dro375 = base
CONFIGS_dpu550 = base lite
CONFIGS_dro550 = base lcd

#
# Modules
#
MODULES = lua usb/common/core usb/common/cdc usb/device/core usb/device/cdc-serial
MODULES_dro375 = dro375
MODULES_dro550 = fatfs dro550
MODULES_dpu550 = dpu550

#
# Directory names
#
OBJDIR    = obj
SRCDIR    = src
INCDIR    = include
BINDIR    = bin
TOOLSDIR  = tools
RELDIR    = releases/$(RELEASE)

#
# Targets 
#
TARGET    = $(BINDIR)/$(PROJECT)-$(PLATFORM)-$(CONFIG)

#
# Windows Installers
#
INSTALLS  = install/opendro-$(RELEASE).msi install/opendro64-$(RELEASE).msi

#
# Tools
#
Q        ?= @
CROSS     = arm-elf-
CC        = $(Q)$(CROSS)gcc
OBJCOPY   = $(Q)$(CROSS)objcopy
SIZE      = $(Q)$(CROSS)size
RM        = $(Q)rm -rf
MKDIR     = $(Q)mkdir -p

#
# Compiler flags
#
MACHINE   = -mthumb -mthumb-interwork -mcpu=arm7tdmi
OPTIMIZE  = -Os -std=gnu99 -fshort-enums
WARNING   = -Wall -Werror
INCLUDE   = -I$(INCDIR) -I$(SRCDIR)
DEBUG     = -g -DDEBUG
DEPENDS   = -MT $(@) -MD -MP -MF $(@:%.o=%.d)
CFLAGS    = $(MACHINE) $(OPTIMIZE) $(WARNING) $(INCLUDE) $(DEBUG) $(DEPENDS)
DEFINES   = -DOPENDRO_MAJOR=$(OPENDRO_MAJOR) -DOPENDRO_MINOR=$(OPENDRO_MINOR) -DOPENDRO_MAINT=$(OPENDRO_MAINT) -DTRACE_FEATURE

#
# Linker flags
#
LD_dro375 = at91sam7s256-flash.ld
LD_dro550 = at91sam7se256-flash.ld
LD_dpu550 = at91sam7s256-flash.ld
LFLAGS    = -nostartfiles
LIBS      = -lm

#
# Default make rule is to make all platforms
#
all: $(PLATFORMS)

#
# Help rule
#
help:
	@echo "The following targets are available:"
	@echo "all                 Make all possible images."
	@echo "<platform>          Make all configs for <platform>."
	@echo "<platform>-<config> Make image only for <platform>-<config>."
	@echo "release             Make the release package."
	@echo "publish             Publish a release to SourceForge."
	@echo "clean               Delete all objects and binaries."
	@echo 
	@echo "Supported platforms: " $(PLATFORMS)

#
# Clean rules
#
clean:
	$(RM) $(OBJDIR) $(BINDIR)

#
# Documentation
#
.PHONY: docs
docs:
	doxygen doxyfile

#
# Release package
#
define PLATFORM_release
	$(foreach config,$(CONFIGS_$(1)),cp $(BINDIR)/$(PROJECT)-$(1)-$(config).bin $(RELDIR)/$(PROJECT)-$(1)-$(config)-$(RELEASE).bin;)
endef
release: $(INSTALLS) | $(RELDIR)
	$(MKDIR) $(RELDIR)
	$(foreach platform,$(PLATFORMS),$(call PLATFORM_release,$(platform)))
	for install in $(INSTALLS); do cp $$install $(RELDIR); done
	cp RELNOTES.txt $(RELDIR)
	tar cvzf $(RELDIR)/$(PROJECT)-$(RELEASE).tar.gz --no-recursion --transform 'flags=r;s,^,$(PROJECT)-$(RELEASE)/,' `svn ls -rHEAD -R`

publish:
	scp $(RELDIR)/* shumatech,opendro@frs.sourceforge.net:/home/frs/project/o/op/opendro/opendro/$(RELEASE)

#
# Module object rules
#
define PLATFORM_CONFIG_MODULE_obj
MOD_SRC=
MOD_SRC_$(1)=
MOD_SRC_$(1)_$(2)=
MOD_CFLAGS=
MOD_DEFINES=
$(eval include $(SRCDIR)$(3)/module.mk)
$(eval CSRC = $(filter %.c,$(MOD_SRC) $(MOD_SRC_$(1)) $(MOD_SRC_$(1)_$(2))))
$(eval ASRC = $(filter %.s,$(MOD_SRC) $(MOD_SRC_$(1)) $(MOD_SRC_$(1)_$(2))))
OBJ_$(1)_$(2) += $(CSRC:%.c=$(OBJDIR)/$(1)-$(2)$(3)/%.o) $(ASRC:%.s=$(OBJDIR)/$(1)-$(2)$(3)/%.o)
DEP += $(CSRC:%.c=$(OBJDIR)$(3)/%.d)
$(OBJDIR)/$(1)-$(2)$(3):
	$(MKDIR) $$(@)
$(OBJDIR)/$(1)-$(2)$(3)/%.o: $(SRCDIR)$(3)/%.c | $(OBJDIR)/$(1)-$(2)$(3)
	@echo "  CC $$(<)"
	$$(CC) $(MOD_CFLAGS) $$(CFLAGS) $(MOD_DEFINES) $$(DEFINES) -DPLATFORM_$(1) -DCONFIG_$(2) -c -o $$(@) $$(<)
$(OBJDIR)/$(1)-$(2)$(3)/%.o: $(SRCDIR)$(3)/%.s | $(OBJDIR)/$(1)-$(2)$(3)
	@echo "  AS $$(<)"
	$$(CC) $(MOD_CFLAGS) $$(CFLAGS) $(MOD_DEFINES) $$(DEFINES) -DPLATFORM_$(1) -DCONFIG_$(2) -c -o $$(@) $$(<)
endef
define PLATFORM_CONFIG_obj
$(eval $(call PLATFORM_CONFIG_MODULE_obj,$(1),$(2)))
$(foreach module,$(MODULES) $(MODULES_$(1)),$(eval $(call PLATFORM_CONFIG_MODULE_obj,$(1),$(2),/$(module))))
endef
define PLATFORM_obj
$(foreach config,$(CONFIGS_$(1)),$(eval $(call PLATFORM_CONFIG_obj,$(1),$(config))))
endef
$(foreach platform,$(PLATFORMS),$(eval $(call PLATFORM_obj,$(platform))))

#
# Build the platform targets
#
define PLATFORM_CONFIG_target
$(1)-$(2): $(BINDIR)/$(PROJECT)-$(1)-$(2).bin
$(BINDIR)/$(PROJECT)-$(1)-$(2).bin: $(BINDIR)/$(PROJECT)-$(1)-$(2).elf
	$(SIZE) -t $$(OBJ_$(1)_$(2))
	$(SIZE) -t $$(<)
	$(OBJCOPY) -O binary $$(<) $$(@)
$(BINDIR)/$(PROJECT)-$(1)-$(2).elf: $$(OBJ_$(1)_$(2)) $(LD_$(1)) | $(OBJDIR) $(BINDIR)
	@echo "  LD $$(@)"
	$(CC) $$(CFLAGS) $$(LFLAGS) -T $$(LD_$(1)) -o $$(@) $$(OBJ_$(1)_$(2)) $$(LIBS)
endef
define PLATFORM_target
$(1): $(foreach config,$(CONFIGS_$(1)),$(1)-$(config))
$(foreach config,$(CONFIGS_$(1)),$(eval $(call PLATFORM_CONFIG_target,$(1),$(config))))
endef
$(foreach platform,$(PLATFORMS),$(eval $(call PLATFORM_target,$(platform))))

#
# Directory creation rules
#
$(OBJDIR):
	$(MKDIR) $@

$(BINDIR):
	$(MKDIR) $@

$(RELDIR):
	$(MKDIR) $@

#
# Make rules
#
$(TARGET).bin: $(TARGET).elf
	$(SIZE) -t $(OBJ)
	$(SIZE) -t $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).elf: $(OBJ) $(LD) | $(OBJDIR) $(BINDIR)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $(OBJ) $(LIBS)

-include $(DEP)


