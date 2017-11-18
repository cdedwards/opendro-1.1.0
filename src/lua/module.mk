# OpenDRO Project
#
# Copyright (C) 2011 ShumaTech http://www.shumatech.com/
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

MOD_SRC = lapi.c lcode.c ldebug.c ldo.c ldump.c lfunc.c lgc.c llex.c \
          lmem.c lobject.c lopcodes.c lparser.c lstate.c lstring.c \
          ltable.c ltm.c lundump.c lvm.c lzio.c lauxlib.c lbaselib.c \
          liolib.c lmathlib.c ltablib.c lstrlib.c \
          loadlib.c linit.c lrotable.c legc.c

MOD_CFLAGS = -DLUA_OPTIMIZE_MEMORY=2
