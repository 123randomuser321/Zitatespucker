#	Zitatespucker: Library to spit out quotes (and relating information)
#	Makefile
#
#	Copyright (C) 2024  Sembo Sadur
#
#	This program is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	This program is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with this program.  If not, see <https://www.gnu.org/licenses/>.


# todo: windows
LIBPREFIX = lib
LIBSUFFIX = so

ifeq ($(DEBUG), 1)
	CFLAGS += -g
endif

CFLAGS += -std=c99 -I.
LDFLAGS += -shared

objects = build/Zitatespucker_common.o

# -fPIC needs to be added due to the build failing with "relocation R_X86_64_PC32 against symbol `stderr@@GLIBC_2.2.5' can not be used when making a shared object" otherwise
ifeq ($(ZITATESPUCKER_FEATURE_JSON_C), 1)
	CFLAGS += -D ZITATESPUCKER_FEATURE_JSON_C -fPIC
	LDFLAGS += -ljson-c
	objects += build/Zitatespucker_json-c.o 
endif

# todo: target for building all objects without linking
# todo: install and uninstall target
# todo: static:, etc
# https://www.gnu.org/prep/standards/html_node/Standard-Targets.html
# https://www.howtogeek.com/427086/how-to-use-linuxs-ar-command-to-create-static-libraries/
all : $(objects)
	$(CC) $(LDFLAGS) $^ -o build/$(LIBPREFIX)Zitatespucker.$(LIBSUFFIX)
	rm -f build/*.o

build/Zitatespucker_common.o : src/Zitatespucker_common.c
	mkdir -p build
	$(CC) -c $(CFLAGS) $^ -o $@

build/Zitatespucker_json-c.o : src/Zitatespucker_json-c.c
	mkdir -p build
	$(CC) -c $(CFLAGS) $^ -o $@

src/Zitatespucker_common.c : Zitatespucker/Zitatespucker_common.h

src/Zitatespucker_json-c.c : Zitatespucker/Zitatespucker_json-c.h Zitatespucker/Zitatespucker_common.h

.PHONY : clean
clean : 
	-rm -rf build
