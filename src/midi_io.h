#include <iostream>
#include <cstdlib>
#include <RtMidi.h>

class MidiInput {

public:
    MidiInput(unsigned int port);
    ~MidiInput();

    MidiData get_message();

private:

    RtMidiIn *_midiin;
}
