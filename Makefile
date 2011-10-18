INCPATH = -I./ -I/usr/include
LIBPATH = -L/usr/lib 
CFLAGS  = -O2 -g
LDFLAGS = -Wl,--rpath -Wl,/usr/lib -Wno-write-strings -lm  -lstdc++  -lGL -lglut -lGLU -lGLEW -pthread -lzmq -ljson_linux-gcc-4.4.5_libmt

RM 		= /bin/rm -f
CC		= colorgcc 
SOURCES = shader.cpp server.cpp livegl.cpp
OBJECTS = $(SOURCES:.c=.o)
MAIN	= livegl

all: $(MAIN) 

$(MAIN): $(OBJECTS)
	$(CC) $(LIBPATH) $(LDFLAGS) $(CFLAGS) $^ -o $@

.c.o:
	$(CC) $(INCPATH) $(CFLAGS) -c $< 

clean:
	$(RM) *~ *.o

