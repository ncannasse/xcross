all:

linux: clean linux_ui clean2 linux_console

linux_ui:
	(cd ../.. && make -f libs/xcross/Makefile.real linux)

linux_console:
	(cd ../.. && make -f libs/xcross/Makefile.real CONSOLE_MODE=1 linux)

osx:
	(cd ../.. && make -f libs/xcross/Makefile.real MACOSX=1 OSX_UNIVERSAL=1 osx)

clean:
	(cd ../.. && make -f libs/xcross/Makefile.real xcross_clean)

clean2:
	(cd ../.. && make -f libs/xcross/Makefile.real CONSOLE_MODE=1 xcross_clean)

release:
	mkdir xcross-release
	haxe run.hxml	
	cp -r xcross src bin Run.hx run.n haxelib.xml xcross-release
	rm -rf xcross-release/*/.svn xcross-release/*/*/.svn xcross-release.zip
	zip -r xcross-release.zip xcross-release
	rm -rf xcross-release
	-haxelib remove xcross
	haxelib test xcross-release.zip

test:
	(cd test && haxe -lib xcross -main Test -neko test.n && haxelib run xcross -bundle "xcross test" -x test.n)

.PHONY: all linux osx clean release test

