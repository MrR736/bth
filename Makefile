ifeq ($(OS),Windows_NT)
    file = Makefile.mingw32
else
    file = Makefile.gcc
endif

all: select

select:
	@make -f $(file)

arch:
	@make -f Makefile.gcc arch
	@make -f Makefile.mingw32

gcc:
	@make -f Makefile.gcc

clang:
	@make -f Makefile.clang

mingw32:
	@make -f Makefile.mingw32

install:
	@make -f Makefile.gcc install

uninstall:
	@make -f Makefile.gcc uninstall

clean:
	@for files in Makefile.gcc Makefile.clang Makefile.mingw32; do \
		if [ -f $$files ]; then \
			make -f $$files clean; \
		fi; \
	done
