#ifndef _MIDI_INPUT_H
#define _MIDI_INPUT_H

#include <iostream>
#include <cstdlib>
#include <RtMidi.h>
#include "midi_message.h"

class MidiInput {

public:
    MidiInput(unsigned int port);
    ~MidiInput();

    MidiMessage *get_message();

private:

    RtMidiIn *_midiin;
};

#endif

