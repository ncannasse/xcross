all:

linux:
	(cd ../.. && make -f libs/xcross/Makefile.real linux)

osx:
	(cd ../.. && make -f libs/xcross/Makefile.real MACOSX=1 OSX_UNIVERSAL=1 osx)

clean:
	(cd ../.. && make -f libs/xcross/Makefile.real clean)

.PHONY: all linux osx clean

