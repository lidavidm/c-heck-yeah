CC=clang
LDFLAGS=
CFLAGS=-g -O0 -Wall -Werror -Wextra -Wno-error=unused-parameter -Wno-error=unused-variable
SOURCES=main.c entity.c screens.c
OBJECTS=$(SOURCES:.c=.o)
#Hacky solution to have a single makefile. TODO: Make this not suck
UNAME_S=$(shell uname -s)
#David's Configs
ifeq ($(UNAME_S),Linux)
	CFLAGS+=-fsanitize=address
	INC=/usr/include/SDL2 ./include/chipmunk
# Hacky solution to get chipmunk/TMX statically linked on Linux
	STATICLIBPARAMS=libchipmunk.a
	LIB=
#Tyler's Configs
else
	INC=/usr/local/Cellar/sdl2/2.0.3/include/SDL2 /usr/local/Cellar/sdl2_image/2.0.0_1/include/SDL2 /usr/local/Cellar/sdl2_ttf/2.0.12/include/SDL2 /usr/local/Cellar/chipmunk/6.2.1/include/chipmunk
	STATICLIBPARAMS=
	LIB=chipmunk
endif
LIB+=SDL2 SDL2_ttf SDL2_image
INCPARAMS=$(foreach d, $(INC), -I$d)
LIBPARAMS=$(foreach d, $(LIB), -l$d)

all: $(OBJECTS) main

.PHONY: main

main:
	$(CC) $(LIBPARAMS) $(INCPARAMS) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $@ $(STATICLIBPARAMS)

%.o: %.c
	$(CC) $(INCPARAMS) $(CFLAGS) -c $< -o $@

clean:
	rm *.o
	rm main
