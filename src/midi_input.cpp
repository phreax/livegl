#include "midi_input.h"

MidiInput::MidiInput(unsigned int port) 
    : _midiin(new RtMidiIn())
{
    _midiin->openPort(port);
    _midiin->ignoreTypes( false, false, false  );
}

MidiMessage *MidiInput::get_message() {
    std::vector<unsigned char> message;
    double stamp = _midiin->getMessage( &message );
    int nBytes = message.size();
    if(message.size() > 0 && message[0] > 0) 
        return new MidiMessage(stamp, message);
    else
        return NULL;
}
