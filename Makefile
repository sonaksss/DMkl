SRCS = main.c \
       menu.c \
       $(wildcard N/*.c) \
       $(wildcard Z/*.c) \
       $(wildcard Q/*.c) \
       $(wildcard P/*.c)
TEST = test_main.c \
       menu.c \
       $(wildcard N/*.c) \
       $(wildcard Z/*.c) \
       $(wildcard Q/*.c) \
       $(wildcard P/*.c)

all:
	gcc -fno-common -std=c11 -g -Wall -Werror  $(SRCS) -o dm
	gcc -fno-common -std=c11 -g -Wall -Werror  $(TEST) -o dt
clean:
	rm -f dmi
