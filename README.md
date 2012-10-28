Readme
======

This is a basic live coding framework for rapid prototyping GLSL shaders and demos with audio feedback. It comes with a vim plugin which sends the current source code of a shader programm to a server, which then compiles and loads it into the current OpenGL rendering context. 
Further it includes a simple sound analyzer and beat matching engine that can process a
audiostream in realtime and makes the result available in the shader through uniforms and textures.

Setup
-----

### Vim Plugin

First you need to install the Vim plugin by copying all files in *vim* directory to *~/.vim*
It contains a tiny python script to send a current buffer to zmq socket, and a *ftplugin* for *GLSL* files.
Shader types are distinguished by their names, thus they need to be either *.vert* or *.frag*.

### Audio

In order to use the realtime audio processing module, you need to have PulseAudio installed
and running as your audio server. LiveGL grabs the audio signal from the monitor channel
of the current playing soundstream. In order to use is, you need unmute it first with
the following command:

    pacmd set-source-mute alsa_output.pci-0000_00_1b.0.analog-stereo.monitor false

### LiveGL Server

To start the LiveGL server you need to run the folllowing:
    
    ./livegl

It sets up a OpenGL rendering context and starts a server to listen for the vim client. 
Currently it will output only a single quad (two triangles) to fill the screen size.


Usage
-----

Now you can start opening a new shader file. When saving it, the it will be send to the server. 
Any compilation errors will be displayed in the server log. Try some examples to get an idea:

    vim examples/dancingjulia.frag

Hit save and you should see a message in you log, as well as a nice animation of the julia
set.


### Variables

The following variables will be available as *uniforms* in your shader and can be used freely for your
animations:

`float time`: The time value, it is increased every frame by 0.01
`vec2  resolution`: The screen resolution, it is updated when the window size changes.
`sampler1D spec`: This is a 1D rgba texture that contains the spectrum of the current processed 
audiosample. Currently only the red and green channel are used, where the red channel contains
the unfiltered spectrum and the green channel contains the spectral flux, which is the gradient
over time of the spectrum.
`vec3  sound`: This vector contains the a smoothed, avaraged energy value for 3 bands, low, middle and high, with the cutoff frequencies: 200Hz (low) and 8000Hz (high).

An example of how to use the output of the spectral analyzer can be found in `examples/spec.frag`.


### Organizing Code

When hacking with new algorithms for lightening, raymarching or what ever cool shit your are
playing with, it is often desirable to outsource
frequently used functions into a different file. The LiveGL gives you the possibility
to use a special tag to import code from other files.

**Example:**

    // require "toolbox.glsl" 


This will import all functions from the file `toolbox.glsl`. Note that the path will
be relative to the current directory, i.e. the one you started vim in.


Requirements
------------

* **vim**         with python extension
* **OpenGL**      
* **glut**
* **glew**
* **zeromq**
* **jsoncpp**     get it here: http://jsoncpp.sourceforge.net/
* **pulse-audio**


Planned Features
----------------

* Noise generator (Perlin or similar)
