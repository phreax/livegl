#ifndef __UNIFORM_H
#define __UNIFORM_H

#include <string>
#include <stdio.h>

class UniformBase {

public:
    UniformBase();
    inline UniformBase(const char *name) : _name(name) {};

    virtual void update_shader(int shader_id) = 0;
    inline const char* name() { return _name; }

protected:

    const char* _name;
};

class Uniform : public UniformBase {

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
    inline float* data() { return _data; }

private:

    int _dimension;
    float *_data;
};

class UniformIv : public UniformBase {

public:
    
    UniformIv(const char* name, unsigned int size=1);
    ~UniformIv();

    void update_shader(int shader_id);

    void set_value(unsigned int index, int value);
    inline int* data() { return _data; }

private:

    int *_data;
    unsigned int  _size;
};


#endif
