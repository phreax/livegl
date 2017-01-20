#ifndef __UNIFORM_H
#define __UNIFORM_H

#include <string>
#include <stdio.h>

class Uniform {

public:
    
    Uniform(const char* name, unsigned int dimension=1);
    ~Uniform();

    void update_shader(int shader_id);

    void incr(float v);
    void set(float v1);
    void set(float v1, float v2);
    void set(float v1, float v2, float v3);
    void set(float v1, float v2, float v3, float v4);
    void set_value(unsigned int index, float v);
    inline const char* name() { return _name; }
    inline float* data() { return _data; }

private:

    float *_data;
    int _dimension;
    const char* _name;
};

#endif
