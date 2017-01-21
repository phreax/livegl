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

bool MidiMessage::is_system_exclusive() {
    return _message[0] == 0xf0;
}

bool MidiMessage::is_start() {
    if(_message.size() > 5) {
       return _message[3] == 0x6 && _message[4] == 0x2;
    } else {
       return false;
    }
}

bool MidiMessage::is_stop() {
    if(_message.size() > 5) {
       return _message[3] == 0x6 && _message[4] == 0x1;
    } else {
       return false;
    }
}

int MidiMessage::note_chan() {
    return (int)(_message[0] & 0xf) + 1;
}

int MidiMessage::note_value() {
    return (int)(_message[1]);
}

// quantized to to scale 0-11 
int MidiMessage::note_value_scaled() {
    return note_value() % 12;
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
        stream << "CHAN" << setw(2) <<  note_chan() << " Note On ";
        stream << setw(3) << note_value() << " " << setw(3) << note_velocity();
    }
    else if(is_note_off()) {
        stream << "CHAN" << setw(2) <<  note_chan() << " Note Off ";
        stream << setw(3) << note_value() << " ";
    }
    else if(is_cc()) {
        stream << "CHAN" << setw(2) <<  note_chan() << " CC#  ";
        stream << setw(3) << note_value() << " " << setw(3) << note_velocity();
    }
    else if(is_system_exclusive()) {
        stream << "EXCL ";
        if(is_start()) stream << "start";
        if(is_stop()) stream << "stop";
    }
    else {
        int nbytes = _message.size();
        for ( int i=0; i < nbytes; i++  ) 
            stream << "Byte " << i << " = " << (int)_message[i] << ", ";
    }
    return stream.str();
}
