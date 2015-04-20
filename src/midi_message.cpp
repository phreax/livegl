#include "midi_message.h"
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

MidiMessage::MidiMessage(double timestamp, std::vector<unsigned char> message) 
    : _timestamp(timestamp), _message(message)
{
}

bool MidiMessage::is_cc() {
    return (_message[0] >> 4) == 0xb;
}

bool MidiMessage::is_note_on() {
    return (_message[0] >> 4) == 0x9;
}

bool MidiMessage::is_note_off() {
    return (_message[0] >> 4) == 0x8;
}

int MidiMessage::note_chan() {
    return (int)(_message[0] & 0xf) + 1;
}

int MidiMessage::note_value() {
    return (int)(_message[1]);
}

int MidiMessage::note_velocity() {
    return (int)(_message[2]);
}

int MidiMessage::cc_chan() {
    return (int)(_message[0] & 0xf) + 1;
}

int MidiMessage::cc_number() {
    return (int)(_message[1]);
}

int MidiMessage::cc_value() {
    return (int)(_message[2]);
}

string MidiMessage::to_string() {
    ostringstream stream;

    stream << setfill('0');
    if(is_note_on()) {
        stream << "CHAN" << setw(2) <<  note_chan() << " Note ";
        stream << setw(3) << note_value() << " " << setw(3) << note_velocity();
    }
    else if(is_cc()) {
        stream << "CHAN" << setw(2) <<  note_chan() << " CC#  ";
        stream << setw(3) << note_value() << " " << setw(3) << note_velocity();
    }
    else {

        int nbytes = _message.size();
        for ( int i=0; i < nbytes; i++  ) 
            stream << "Byte " << i << " = " << (int)_message[i] << ", ";
    }
    return stream.str();
}
