Readme
======

This is basic live coding framework for rapid prototyping GLSL shaders. It comes with a  vim plugin which sends
the current source code of a shader programm to a server, which then compiles and loads it into the current
OpenGL rendering context. 

Usage
-----

### Vim Plugin

First you need to install the Vim plugin by copying all files in *vim* directory to *~/.vim*
It contains a tiny python script to send a current buffer to zmq socket, and a *ftplugin* for *GLSL* files.
Shader types are distinguished by their names, thus they need to be either *.vert* or *.frag*

### LiveGL Server

The LiveGL Server can simply be started with running 
    
    ./livegl

It sets up a OpenGL rendering context and listens on tcp/4223 for incomming json requests. 


Requirements
------------

* **vim**         with python extension
* **OpenGL**      
* **glut**
* **glew**
* **zeromq**
* **jsoncpp**     get it here: http://jsoncpp.sourceforge.net/


