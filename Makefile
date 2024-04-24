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

.PHONY : all dynamic static install install-headers install-dynamic install-static uninstall uninstall-headers uninstall-dynamic uninstall-static clean check


# todo: windows
BUILDDIR = build
LIBPREFIX = lib
LIBSUFFIX_DYN = so
LIBSUFFIX_STATIC = a
EXESUFFIX = 
MAJOR = 0
MINOR = 0
PATCH = 1
ifeq ($(DESTDIR),)
	DESTDIR = $(BUILDDIR)
endif
ifeq ($(PREFIX),)
	PREFIX = usr/local
endif

LIBNAME = $(LIBPREFIX)Zitatespucker
LIBNAME_DYN = $(LIBNAME).$(LIBSUFFIX_DYN)
LIBNAME_STATIC = $(LIBNAME).$(LIBSUFFIX_STATIC)

HEADERS = Zitatespucker/Zitatespucker.h Zitatespucker/Zitatespucker_common.h

ifneq ($(DEBUG),)
	CFLAGS += -g
	CFLAGS += -Wpedantic
endif

CFLAGS += -std=c99 -I. -D ZITATESPUCKER_VERSION_MAJOR=$(MAJOR) -D ZITATESPUCKER_VERSION_MINOR=$(MINOR) -D ZITATESPUCKER_VERSION_PATCH=$(PATCH)

objects = $(BUILDDIR)/Zitatespucker_common.o

# -fPIC needs to be added due to the build failing with "relocation R_X86_64_PC32 against symbol `stderr@@GLIBC_2.2.5' can not be used when making a shared object" otherwise
# gcc' manual recommends adding flags to both compiler and linker flags
ifneq ($(ENABLE_JSON_C),)
	HEADERS += Zitatespucker/Zitatespucker_json-c.h
	CFLAGS += -D ZITATESPUCKER_FEATURE_JSON_C -fPIC
	ifneq ($(ENABLE_JSON_C_STATIC),)
		LDFLAGS += -Wl,-Bstatic
	endif
	LDFLAGS += -ljson-c -fPIC
	objects += $(BUILDDIR)/Zitatespucker_json-c.o 
endif

# todo: echoing (https://www.gnu.org/software/make/manual/html_node/Echoing.html)
# https://www.gnu.org/prep/standards/html_node/Standard-Targets.html
# todo: 'check' target for tests
all : dynamic static

# question: should a crossbuild for Windows omit "-soname"?
dynamic : $(objects)
	$(CC) $(LDFLAGS) -Wl,-Bdynamic -lc -shared -Wl,-soname,$(LIBNAME_DYN).$(MAJOR) $^ -o $(BUILDDIR)/$(LIBNAME_DYN).$(MAJOR).$(MINOR).$(PATCH)

static : $(objects)
	$(AR) $(ARFLAGS) $(BUILDDIR)/$(LIBNAME_STATIC) $^

$(BUILDDIR)/Zitatespucker_common.o : src/Zitatespucker_common.c
	mkdir -p $(BUILDDIR)
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILDDIR)/Zitatespucker_json-c.o : src/Zitatespucker_json-c.c
	mkdir -p $(BUILDDIR)
	$(CC) -c $(CFLAGS) $^ -o $@

src/Zitatespucker_common.c : Zitatespucker/Zitatespucker_common.h

src/Zitatespucker_json-c.c : Zitatespucker/Zitatespucker_json-c.h Zitatespucker/Zitatespucker_common.h

install : install-headers install-dynamic install-static

# would it be better to put the headers as a prerequisite here?
install-headers : 
	install -d "$(DESTDIR)"/"$(PREFIX)"/include/Zitatespucker
	install -m 0644 -t "$(DESTDIR)"/"$(PREFIX)"/include/Zitatespucker $(HEADERS)

install-dynamic : install-headers dynamic
	install -d "$(DESTDIR)"/"$(PREFIX)"/lib
	install -t "$(DESTDIR)"/"$(PREFIX)"/lib $(BUILDDIR)/$(LIBNAME_DYN).$(MAJOR).$(MINOR).$(PATCH)
	ln -s $(LIBNAME_DYN).$(MAJOR).$(MINOR).$(PATCH) "$(DESTDIR)"/"$(PREFIX)"/lib/$(LIBNAME_DYN).$(MAJOR)
	ln -s $(LIBNAME_DYN).$(MAJOR) "$(DESTDIR)"/"$(PREFIX)"/lib/$(LIBNAME_DYN)

install-static : install-headers static
	install -d "$(DESTDIR)"/"$(PREFIX)"/lib
	install -t "$(DESTDIR)"/"$(PREFIX)"/lib $(BUILDDIR)/$(LIBNAME_STATIC)

uninstall : uninstall-headers uninstall-dynamic uninstall-static

uninstall-headers :
	-rm -rf "$(DESTDIR)"/"$(PREFIX)"/include/Zitatespucker

uninstall-dynamic : 
	-rm -f "$(DESTDIR)"/"$(PREFIX)"/lib/$(LIBNAME_DYN).$(MAJOR).$(MINOR).$(PATCH)
	-rm -f "$(DESTDIR)"/"$(PREFIX)"/lib/$(LIBNAME_DYN).$(MAJOR)
	-rm -f "$(DESTDIR)"/"$(PREFIX)"/lib/$(LIBNAME_DYN)

uninstall-static : 
	-rm -f "$(DESTDIR)"/"$(PREFIX)"/lib/$(LIBNAME_STATIC)

clean : 
	-rm -rf $(BUILDDIR)
	-rm -rf tests/build

check :
	mkdir -p tests/build
	$(CC) ./tests/Zitatespucker_json-c_tests.c -I. -L./$(BUILDDIR) -lZitatespucker -ljson-c -o ./tests/build/Zitatespucker_json-c_tests$(EXESUFFIX) 
	cd tests/build && ./Zitatespucker_json-c_tests$(EXESUFFIX)
