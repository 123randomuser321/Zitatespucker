## Description

This is a library written to let you extract quotes (Zitat is the German word) from various file formats.
The information is then stored into a doubly linked list of structs, which store:

The author
The quote itself
A comment on the quote
Day of utterance
Month of utterance
Year of utterance
Whether or not it happened BC or AD

The library currently supports:
JSON (aka .json files); via json-c or jansson
SQL files; via sqlite3


## Building

A (GNU) Makefile is provided with the project.
By default, the library has little functionality.

The following switches are recognized:

'DESTDIR' and 'PREFIX' (when set, control behavior of the 'install' and 'uninstall' targets)
'TARGET=win32' (which tries a crossbuild for Windows, best used on something like MSYS)
'DEBUG' (when set, passes '-g' and '-Wpedantic' to the compiler)
'NOPRINT' (when set, the library will never fprintf by itself)
'ENABLE_JSON_C' (when set, builds and links the json-c backend)
'ENABLE_JSON_C_STATIC' (when set, link json-c statically)
'ENABLE_JANSSON' (when set, builds and links the jansson backend)
'ENABLE_JANSSON_STATIC' (when set, link jansson statically)
'ENABLE_SQLITE' (when set, builds and links the sqlite3 backend)
'ENABLE_SQLITE_STATIC' (when set, link sqlite3 statically)

If you are building on Windows, do not forget to pass the correct include and link directories via CFLAGS and LDFLAGS.

WARNING: Enabling more than one backend for a file format will fail to build (linker will complain about duplicate symbols).


## Building for Nintendo DS (static library)

Invoke make using the 'Makefile.blocksds' Makefile.
This will require you to have installed BlocksDS as per upstream documentation.

Note:
Currently, only the jansson backend is supported on the Nintendo DS (because I couldn't get the others to build).


## Dependencies

Build:
gcc (or any syntactically identical compiler)
libc
json-c (only if ENABLE_JSON_C is set)
jansson (only if ENABLE_JANSSON is set)
sqlite3 (only if ENABLE_SQLITE is set)

Runtime:
libc
json-c (only if ENABLE_JSON_C is set)
jansson (only if ENABLE_JANSSON is set)
sqlite3 (only if ENABLE_SQLITE is set)


## Usage

The core library is more or less a no-op.
For use of specific backends, please define the following preprocessor definitions:
'ZITATESPUCKER_JSON' for JSON stuff
'ZITATESPUCKER_SQL' for SQL stuff

Usage of the specific backends is described within their respective headers.
Example files can be found within the 'examples' directory.

Then, pass -lZitatespucker to the linker, and you should be good.


## Tests

Run 'make check' after you built the library.

Friendly warning:
The 'check' target expects the library to be built with a backend for each possible file format enabled, and parts will fail otherwise.
