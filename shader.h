#ifndef __SHADER_H
#define __SHADER_H

#include <string>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <stdio.h>

static const char* default_vp = 
"void main() {" 
"    gl_FrontColor = gl_Color;"
"    gl_Position = ftransform();"
"}";

static const char* default_fp = 
"void main() {" 
"    gl_FragColor = vec4(0.0,0.0,1.0,1.0);"
"}";



class Shader {

public:
	Shader(const char *vsSource=default_vp, const char *fsSource=default_fp);
	~Shader();
	
    void init(const char *vsSource, const char *fsSource);
    void replaceShader(const char *type, const char *source);
    
	void bind();
	void unbind();
	
	unsigned int id();
	bool valid();
	
private:
	unsigned int shader_id;
	unsigned int shader_vp;
	unsigned int shader_fp;
    bool         _valid;
    bool         _active;
};


#endif
