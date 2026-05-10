SRCS = main.c \
       menu.c \
       $(wildcard N/*.c) \
       $(wildcard Z/*.c) \
       $(wildcard Q/*.c) \
       $(wildcard P/*.c)

all:
	gcc -fno-common -std=c11 -g -Wall -Werror  $(SRCS) -o dm

clean:
	rm -f dm
