PKGS    = jsoncpp gl glu glew libzmq fftw3 libpulse libpulse-simple rtmidi
CFLAGS  = -O2 -g `pkg-config --cflags $(PKGS)`
LDFLAGS = -Wl,--rpath -Wl,/usr/lib -Wno-write-strings -lm -lstdc++ `pkg-config --libs $(PKGS)`  -pthread -lglut
RM 		= /bin/rm -f
CC		= g++
SOURCES = shader.cpp server.cpp livegl.cpp pasink.cpp spectral.cpp midi_message.cpp midi_input.cpp uniform.cpp
OBJECTS = $(SOURCES:.cpp=.o)
MAIN	= livegl
TEST	= spectral_test
TEST_OBJ= pasink.o spectral.o spectral_test.o

all: $(MAIN) 

test: $(TEST)

$(MAIN): $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@

$(TEST): $(TEST_OBJ)
	$(CC) $^ $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $< 

clean:
	$(RM) *~ *.o $(MAIN)

