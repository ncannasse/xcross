# xCross Help #

xCross is a tool to easily create cross-platform Win/Mac/Linux applications.

Even if you don't have the three operating system installed, you can create binaries for all of them from your favorite OS.

# How does it works ? #

xCross is using [NekoVM](http://nekovm.org) and [haXe](http://haxe.org). NekoVM is a small, crossplatform and fast embeddable Virtual Machine which is executing the Neko language. haXe is an highlevel OO programming language which targets Neko. An haXe program can then be compiled to Neko bytecode (.n file) and the bytecode is crossplatform (like Java .class files).

xCross is a _Fat VM_ which includes the NekoVM, but also some standard libraries for accessing the OS filesystem, sockets, xml, Zip files, regular expressions... All these libraries are linked statically into xCross, so to run a xCross binary you don't need to install any other library on your system.

xCross distribution includes three versions of the VM binaries : for Windows, Linux and OSX (Universal binary, native on both PPC and Intel). A small tool included in xCross distribution can be used to inject a Neko bytecode file into a xCross binary, so when the binary is loaded the VM will execute the bytecode.

# Tutorial #

  * Download and install [haXe](http://haxe.org/download) (and Neko if needed)
  * Install latest xCross distribution, availabe as an haxelib package, by running the following command :
```
    haxelib install xcross
```
  * Create the following Test.hx file :
```
class Test {
    static function main() {
         xcross.Api.message("Hello","Hello World !");
    }
}
```
  * Compile it using haXe/Neko :
```
    haxe -neko test.n -lib xcross -main Test
```
  * Run xcross to build binaries :
```
    haxelib run xcross test.n
```
_(you might be required to provide an absolute path to test.n)_

This will produce three binaries :
  * test-win.exe
  * test-osx
  * test-linux

You can launch the one which is on your system to see the Hello message.

For further instructions, watch the [haXe API](http://haxe.org/api) and ask on the haXe mailing list.

# xCross File Size #

A xCross binary size consists in the system-specific binary plus the bytecode size. Bytecode size tends to be quite low, around 200KB for a good application. System-specific binaries size are currently the following :

  * Windows : 112 KB
  * Linux : 192 KB
  * OSX : 300 KB (Universal, it contains both PPC and Intel versions)

# Compiling xCross #

  * Download [Neko CVS](http://nekovm.org)
```
    cvs -d:pserver:anonymous@cvs.motion-twin.com co neko
```
  * Download [xCross SVN](http://code.google.com/p/xcross/source) in neko/libs/xcross
```
    cd neko/libs
    svn checkout http://xcross.googlecode.com/svn/trunk/ xcross
```

Compile :

  * for Windows, use VS.Net 2003 and open xcross.sln , you might need MSVCRT60.LIB which is the VC++ 6.0 MSVCRT DLL interface (prevent requiring MSVCRT71.DLL)
  * for Linux/OSX, install the following libraries :
    * libgc-dev
    * libpcre3-dev
    * libz-dev
    * libgtk2.0-dev (for linux only)
  * for Linux, run the following commands :
```
    cd xcross
    make linux   
```
  * for OSX, run the following commands :
```
    cd xcross
    make osx
```

To reduce xcross binary size, you can use [UPX](http://upx.sourceforge.net) : at least 2.92beta for Linux at at least current SVN (should become 2.94) for OSX.