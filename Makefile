CC=clang
LDFLAGS=
CFLAGS=-g -O0 -Wall -Werror -Wextra -fsanitize=address -Wno-error=unused-parameter
SOURCES=main.c entity.c screens.c
OBJECTS=$(SOURCES:.c=.o)
INC=/usr/include/SDL2
LIB=SDL2
INCPARAMS=$(foreach d, $(INC), -I$d)
LIBPARAMS=$(foreach d, $(LIB), -l$d)

all: $(OBJECTS) main

.PHONY: main

main:
	$(CC) $(LIBPARAMS) $(INCPARAMS) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(INCPARAMS) $(CFLAGS) -c $< -o $@

clean:
	rm *.o
	rm main