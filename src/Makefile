INCPATH = -I./ -I/usr/include
LIBPATH = -L/usr/lib -L/usr/lib/i386-linux-gnu 
CFLAGS  = -O2 -g
LDFLAGS = -Wl,--rpath -Wl,/usr/lib -Wno-write-strings -lm  -lstdc++  -lGL -lglut -lGLU -lGLEW -pthread -lzmq -ljson_linux-gcc-4.4.5_libmt -lfftw3 -lpulse -lpulse-simple

RM 		= /bin/rm -f
CC		= colorgcc 
SOURCES = shader.cpp server.cpp livegl.cpp pasink.cpp spectral.cpp
OBJECTS = $(SOURCES:.c=.o)
MAIN	= livegl
TEST	= spectral_test
TEST_OBJ= pasink.o spectral.o spectral_test.o

all: $(MAIN) 

test: $(TEST)

$(MAIN): $(OBJECTS)
	$(CC) $^ $(LIBPATH) $(LDFLAGS) $(CFLAGS)  -o $@

$(TEST): $(TEST_OBJ)
	$(CC) $^ $(LIBPATH) $(LDFLAGS) $(CFLAGS)  -o $@

.c.o:
	$(CC) $(INCPATH) $(CFLAGS) -c $< 

clean:
	$(RM) *~ *.o $(MAIN)

