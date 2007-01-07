all:

linux:
	(cd ../.. && make -f libs/xcross/Makefile.real linux)

osx:
	(cd ../.. && make -f libs/xcross/Makefile.real MACOSX=1 OSX_UNIVERSAL=1 osx)

clean:
	(cd ../.. && make -f libs/xcross/Makefile.real xcross_clean)

release:
	mkdir xcross-release
	haxe run.hxml	
	cp -r xcross src bin Run.hx run.n haxelib.xml xcross-release
	rm -rf xcross-release/*/.svn
	zip -r xcross-release.zip xcross-release
	rm -rf xcross-release
	-haxelib remove xcross
	haxelib test xcross-release.zip

test:
	(cd test && haxe -lib xcross -main Test -neko test.n && haxelib run xcross -x test.n)

.PHONY: all linux osx clean release test

