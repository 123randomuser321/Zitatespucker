#	SPDX-License-Identifier: 0BSD
#
#	Zitatespucker: Library to spit out quotes (and relating information)
#	Makefile
#
#	Copyright (C) 2024 by Sembo Sadur <labmailssadur@gmail.com>
#	
#	Permission to use, copy, modify, and/or distribute this software
#	for any purpose with or without fee is hereby granted.
#
#	THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
#	WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
#	IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES
#	OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
#	NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

.PHONY : all dynamic static install install-headers install-dynamic install-static uninstall uninstall-headers uninstall-dynamic uninstall-static clean check


# todo: windows
BUILDDIR = build

include Makefile.defs

ifeq ($(DESTDIR),)
	DESTDIR = $(BUILDDIR)
endif
ifeq ($(PREFIX),)
	PREFIX = usr/local
endif

ifeq ($(TARGET), win32)
	LIBPREFIX =
	LIBSUFFIX_DYN = dll
	LIBSUFFIX_STATIC = lib
	C_LIB_NAME = msvcrt
else
	LIBPREFIX = lib
	LIBSUFFIX_DYN = so
	LIBSUFFIX_STATIC = a
	C_LIB_NAME = c
endif

LIBNAME = $(LIBPREFIX)Zitatespucker
LIBNAME_DYN = $(LIBNAME).$(LIBSUFFIX_DYN)
LIBNAME_STATIC = $(LIBNAME).$(LIBSUFFIX_STATIC)
ifneq ($(TARGET), win32)
	LIBNAME_DYN_SUFFIX = .$(MAJOR).$(MINOR).$(PATCH)
endif

HEADERS = Zitatespucker/Zitatespucker.h Zitatespucker/Zitatespucker_common.h

ifneq ($(DEBUG),)
	override CFLAGS += -g
	override CFLAGS += -Wpedantic
endif

override CFLAGS += -std=c99 -I. -D ZITATESPUCKER_VERSION_MAJOR=$(MAJOR) -D ZITATESPUCKER_VERSION_MINOR=$(MINOR) -D ZITATESPUCKER_VERSION_PATCH=$(PATCH)

ifneq ($(NOPRINT),)
	override CFLAGS += -D ZITATESPUCKER_NOPRINT=1
endif

objects = $(BUILDDIR)/Zitatespucker_common.o

# -fPIC needs to be added due to the build failing with "relocation R_X86_64_PC32 against symbol `stderr@@GLIBC_2.2.5' can not be used when making a shared object" otherwise
# gcc's manual recommends adding flags to both compiler and linker flags
ifneq ($(ENABLE_JSON_C),)
	HEADERS += Zitatespucker/Zitatespucker_json.h
	override CFLAGS += -D ZITATESPUCKER_JSON -D ZITATESPUCKER_FEATURE_JSON_C -fPIC
	ifneq ($(ENABLE_JSON_C_STATIC),)
		override LDFLAGS += -Wl,-Bstatic
	endif
	override LDFLAGS += -ljson-c -fPIC
	objects += $(BUILDDIR)/Zitatespucker_json-c.o
endif

ifneq ($(ENABLE_JANSSON),)
	HEADERS += Zitatespucker/Zitatespucker_json.h
	override CFLAGS += -D ZITATESPUCKER_JSON -fPIC
	ifneq ($(ENABLE_JANSSON_STATIC),)
		override LDFLAGS += -Wl,-Bstatic
	endif
	override LDFLAGS += -ljansson -fPIC
	objects += $(BUILDDIR)/Zitatespucker_jansson.o
endif

ifneq ($(ENABLE_SQLITE),)
	HEADERS += Zitatespucker/Zitatespucker_sqlite.h
	override CFLAGS += -D ZITATESPUCKER_SQL -fPIC
	ifneq ($(ENABLE_SQLITE_STATIC),)
		override LDFLAGS += -Wl,-Bstatic
	endif
	override LDFLAGS += -lsqlite3 -fPIC
	objects += $(BUILDDIR)/Zitatespucker_sqlite.o
endif

# todo: echoing (https://www.gnu.org/software/make/manual/html_node/Echoing.html)
# https://www.gnu.org/prep/standards/html_node/Standard-Targets.html
all : dynamic static

# mingw seems to simply ignore "-soname"?
dynamic : $(objects)
	$(CC) $^ -o $(BUILDDIR)/$(LIBNAME_DYN)$(LIBNAME_DYN_SUFFIX) $(LDFLAGS) -Wl,-Bdynamic -l$(C_LIB_NAME) -shared -Wl,-soname,$(LIBNAME_DYN).$(MAJOR)

static : $(objects)
	$(AR) $(ARFLAGS) $(BUILDDIR)/$(LIBNAME_STATIC) $^

$(BUILDDIR)/Zitatespucker_common.o : src/Zitatespucker_common.c
	-mkdir $(BUILDDIR)
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILDDIR)/Zitatespucker_json-c.o : src/Zitatespucker_json-c.c
	-mkdir $(BUILDDIR)
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILDDIR)/Zitatespucker_jansson.o : src/Zitatespucker_jansson.c
	-mkdir $(BUILDDIR)
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILDDIR)/Zitatespucker_sqlite.o : src/Zitatespucker_sqlite.c
	-mkdir $(BUILDDIR)
	$(CC) -c $(CFLAGS) $^ -o $@

src/Zitatespucker_common.c : Zitatespucker/Zitatespucker_common.h

src/Zitatespucker_json-c.c : Zitatespucker/Zitatespucker_json.h Zitatespucker/Zitatespucker_common.h

src/Zitatespucker_jansson.c : Zitatespucker/Zitatespucker_json.h Zitatespucker/Zitatespucker_common.h

src/Zitatespucker_sqlite.c : Zitatespucker/Zitatespucker_sqlite.h Zitatespucker/Zitatespucker_common.h

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
	-rm -rf lib

check :
	@echo "The tests assume that the library was built with all backends switched on."
	@echo "Expect failures otherwise."
	mkdir tests/build
	$(CC) ./tests/Zitatespucker_json-c_tests.c -I. -L./$(BUILDDIR) -lZitatespucker -ljson-c -o ./tests/build/Zitatespucker_json-c_tests 
	$(CC) ./tests/Zitatespucker_sqlite_tests.c -I. -L./$(BUILDDIR) -lZitatespucker -lsqlite3 -o ./tests/build/Zitatespucker_sqlite_tests
	cd tests/build && ./Zitatespucker_json-c_tests && ./Zitatespucker_sqlite_tests
