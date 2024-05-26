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

Currently, both JSON (via json-c) and SQL (via sqlite3) are supported.


## Building

A (GNU) Makefile is provided with the project.
By default, the library has little functionality.

The following switches are recognized:

'DESTDIR' and 'PREFIX' (when set, control behavior of the 'install' and 'uninstall' targets)
'TARGET=win32' (which tries a crossbuild for Windows, best used on something like MSYS)
'DEBUG' (when set, passes '-g' and '-Wpedantic' to the compiler)
'ENABLE_JSON_C' (when set, builds and links the json-c backend)
'ENABLE_JSON_C_STATIC' (when set, link json-c statically)
'ENABLE_SQLITE' (when set, builds and links the sqlite3 backend)
'ENABLE_SQLITE_STATIC' (when set, link sqlite3 statically)


## Dependencies

Build:
gcc (or any syntactically identical compiler)
libc
json-c (only if ENABLE_JSON_C is set)
sqlite3 (only if ENABLE_SQLITE is set)

Runtime:
libc
json-c (only if ENABLE_JSON_C is set)
sqlite3 (only if ENABLE_SQLITE is set)


## Usage

Each of the backends is controlled via a preprocessor definition:
ZITATESPUCKER_FEATURE_JSON_C for json-c
ZITATESPUCKER_FEATURE_SQLITE for sqlite3

You need simply #define them to enable the feature.

Usage of the specific backends is described within their respective headers.
Example files can be found within the 'examples' directory.

Then, pass -lZitatespucker to the linker, and you should be good.


## Tests

Run 'make check' after you built the library.

Friendly warning:
The 'check' target expects the library to be built with all backends enabled, and parts will fail otherwise.


## License/Copying conditions

This project is licensed the GNU LGPL 3.0-only,
as specified in the file 'LICENSE.LGPL-3.0-only',
except the directories 'examples', 'external', 'tests', and their respective contents.

The directories 'examples' and 'tests' and their respective contents are licensed under
the Zero-Clause BSD license, as specified within the file 'LICENSE.0BSD'.

The directory 'external' contains files from external projects,
and thus the licensing terms of the original external projects apply
to the respective files.

For the purpose of convenience, most files contain a license header.