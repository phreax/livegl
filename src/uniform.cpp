#include "uniform.h"
#include <iostream>

#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/glut.h>
#include <GL/freeglut.h>


Uniform::Uniform(const char* name, unsigned int dimension)
    : _dimension(dimension), _name(name) 
{
    if(dimension > 4 || dimension < 1) {
        throw "Invalid value for dimension: 1 <= x <= 4";
    }
    _data = new float[_dimension]();
}

Uniform::~Uniform() { delete _data; }

void Uniform::update_shader(int shader_id) {

    int location = glGetUniformLocation(shader_id, _name);
    switch(_dimension) {
    case 1:
        glUniform1f(location, _data[0]);
        break;
    case 2:
        glUniform2fv(location, 1, _data);
        break;
    case 3:
        glUniform3fv(location, 1, _data);
        break;
    case 4:
        glUniform4fv(location, 1, _data);
        break;

    }
}

void Uniform::set(float v1) {
    set_value(0, v1);
}

void Uniform::incr(float v) {
    _data[0] += v;
}

void Uniform::set(float v1, float v2) {
    set_value(0, v1);
    set_value(1, v2);
}

void Uniform::set(float v1, float v2, float v3) {
    set_value(0, v1);
    set_value(1, v2);
    set_value(2, v3);
}

void Uniform::set(float v1, float v2, float v3, float v4) {
    set_value(0, v1);
    set_value(1, v2);
    set_value(2, v4);
}

void Uniform::set_value(unsigned int index, float v1) {

    if(index > _dimension-1) {
        throw "Index out of range";
    }
    _data[index] = v1;
}
