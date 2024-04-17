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

.PHONY : all dynamic static clean


# todo: windows
BUILDDIR = build
LIBPREFIX = lib
LIBSUFFIX_DYN = so
LIBSUFFIX_STATIC = a

LIBNAME = $(LIBPREFIX)Zitatespucker

ifneq ($(DEBUG),)
	CFLAGS += -g
	CFLAGS += -Wpedantic
endif

CFLAGS += -std=c99 -I.

objects = $(BUILDDIR)/Zitatespucker_common.o

# -fPIC needs to be added due to the build failing with "relocation R_X86_64_PC32 against symbol `stderr@@GLIBC_2.2.5' can not be used when making a shared object" otherwise
# gcc' manual recommends adding flags to both compiler and linker flags
ifneq ($(ENABLE_JSON_C),)
	CFLAGS += -D ZITATESPUCKER_FEATURE_JSON_C -fPIC
	ifneq ($(ENABLE_JSON_C_STATIC),)
		LDFLAGS += -Wl,-Bstatic
	endif
	LDFLAGS += -ljson-c -fPIC
		objects += $(BUILDDIR)/Zitatespucker_json-c.o 
endif

# todo: install and uninstall target
# todo: echoing (https://www.gnu.org/software/make/manual/html_node/Echoing.html)
# https://www.gnu.org/prep/standards/html_node/Standard-Targets.html
# todo: 'check' target for tests
all : dynamic static

dynamic : $(objects)
	$(CC) $(LDFLAGS) -Wl,-Bdynamic -lc -shared $^ -o $(BUILDDIR)/$(LIBNAME).$(LIBSUFFIX_DYN)

static : $(objects)
	$(AR) $(ARFLAGS) $(BUILDDIR)/$(LIBNAME).$(LIBSUFFIX_STATIC) $^

$(BUILDDIR)/Zitatespucker_common.o : src/Zitatespucker_common.c
	mkdir -p $(BUILDDIR)
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILDDIR)/Zitatespucker_json-c.o : src/Zitatespucker_json-c.c
	mkdir -p $(BUILDDIR)
	$(CC) -c $(CFLAGS) $^ -o $@

src/Zitatespucker_common.c : Zitatespucker/Zitatespucker_common.h

src/Zitatespucker_json-c.c : Zitatespucker/Zitatespucker_json-c.h Zitatespucker/Zitatespucker_common.h

clean : 
	-rm -rf $(BUILDDIR)
