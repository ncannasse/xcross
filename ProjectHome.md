xCross is a tool that enable you to build standalone binaries from an haXe/Neko program. By running a single command, xCross will create three binaries, for Windows, OSX (Universal) and Linux with no required dependencies.


To install :

- install haXe + Neko from http://haxe.org
- run "haxelib install xcross"

To run :

- run "haxelib run xcross myfile.n"
- this will produce three standalone executables "myfile-win.exe", "myfile-osx" and "myfile-linux"
- you can create an OSX bundle instead of a binary with "haxelib run xcross -bundle MyBundle myfile.n"

xCross includes / statically link :
- the NekoVM itself (neko.dll or libneko.so)
- the Boehm GC used by NekoVM (gc.dll or libgc.so)
- the Neko standard library (std.ndll) which contains filesystem access, sockets, threads, etc.
- the zlib.ndll and regexp.ndll libraries (for zip and regular expressions support)
- a very tiny UI toolkit used by haXe Installer (see http://code.google.com/p/xcross/source/browse/#svn%2Ftrunk%2Fxcross)

If you want to use additional libraries, you'll have to make a custom build of xCross.