# todo: windows
LIBPREFIX = lib
LIBSUFFIX = so

ifeq ($(DEBUG), 1)
	CFLAGS += -g
endif

CFLAGS += -std=c99 -I.
LDFLAGS += -shared

objects = build/Zitatespucker_common.o

# -fPIC needs to be added due to the build failing with relocation R_X86_64_PC32 against symbol `stderr@@GLIBC_2.2.5' can not be used when making a shared object otherwise
ifeq ($(ZITATESPUCKER_FEATURE_JSON_C), 1)
	CFLAGS += -D ZITATESPUCKER_FEATURE_JSON_C -fPIC
	LDFLAGS += -ljson-c
	objects += build/Zitatespucker_json-c.o 
endif

all : $(objects)
	$(CC) $(LDFLAGS) $^ -o build/$(LIBPREFIX)Zitatespucker.$(LIBSUFFIX)
	rm -f build/*.o

build/Zitatespucker_common.o : src/Zitatespucker_common.c #Zitatespucker/Zitatespucker_common.h
	mkdir -p build
	$(CC) -c $(CFLAGS) $^ -o $@

build/Zitatespucker_json-c.o : src/Zitatespucker_json-c.c #Zitatespucker/Zitatespucker_json-c.h Zitatespucker/Zitatespucker_common.h
	mkdir -p build
	$(CC) -c $(CFLAGS) $^ -o $@

.PHONY : clean
clean : 
	-rm -rf build
