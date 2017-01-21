#ifndef _MIDI_MESSAGE_H
#define _MIDI_MESSAGE_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <RtMidi.h>

class MidiMessage {

public:
    
    MidiMessage(double timestamp, std::vector<unsigned char> message);

    bool is_cc();
    bool is_note_on();
    bool is_note_off();
    bool is_start();
    bool is_stop();
    bool is_system_exclusive();

    int note_chan();
    int note_value();
    int note_value_scaled();
    int note_velocity();

    inline float note_value_f() { return midi_to_f(note_value()); }
    float note_velocity_f() { return midi_to_f(note_velocity()); }

    int cc_chan();
    int cc_number();
    int cc_value();

    inline float cc_value_f() { return midi_to_f(cc_value()); }

    inline float timestamp() { return _timestamp; };
    std::string to_string();

private:

    inline float midi_to_f(int value) { return value/127.0; }

    double                     _timestamp;
    std::vector<unsigned char> _message; 
};

#endif

