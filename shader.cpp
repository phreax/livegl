#include "shader.h"
#include <string.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

static char* textFileRead(const char *fileName) {
	char* text;
    
	if (fileName != NULL) {
        FILE *file = fopen(fileName, "rt");
        
		if (file != NULL) {
            fseek(file, 0, SEEK_END);
            int count = ftell(file);
            rewind(file);
            
			if (count > 0) {
				text = (char*)malloc(sizeof(char) * (count + 1));
				count = fread(text, sizeof(char), count, file);
				text[count] = '\0';
			}
			fclose(file);
		}
	}
	return text;
}

static int validateShader(GLuint shader, const char* file = 0) {
	const unsigned int BUFFER_SIZE = 512;
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	GLsizei length = 0;
    
	glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);
	if (length > 0) {
		cout << "Shader " << shader << " (" << (file?file:"") << ") compile error: " << buffer << endl;
        return -1;
	}
    return 0;
}

static int validateProgram(GLuint program) {
	const unsigned int BUFFER_SIZE = 512;
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	GLsizei length = 0;
    
	memset(buffer, 0, BUFFER_SIZE);
	glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);
	if (length > 0) {
		cout << "Program " << program << " link error: " << buffer << endl;
        return -1;
    }
    
	glValidateProgram(program);
	GLint status;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
	if (status == GL_FALSE) {
		cout << "Error validating shader " << program << endl;
        return -1;
    }
    return 0;
}

Shader::Shader(const char *vsSource, const char *fsSource)
    : _active(false), _valid(true)
{
    cout << "here: " <<__LINE__ << endl;
    init(vsSource, fsSource);
}

void Shader::init(const char *vsSource, const char *fsSource) {
    cout << "here: " <<__LINE__ << endl;
	shader_vp = glCreateShader(GL_VERTEX_SHADER);
	shader_fp = glCreateShader(GL_FRAGMENT_SHADER);
    cout << "here: " <<__LINE__ << endl;

    glShaderSource(shader_vp, 1, &vsSource, 0);
	glShaderSource(shader_fp, 1, &fsSource, 0);
    
    cout << "here: " <<__LINE__ << endl;
	glCompileShader(shader_vp);
	_valid = validateShader(shader_vp) == 0 && _valid;
	glCompileShader(shader_fp);
	_valid = validateShader(shader_fp) == 0 && _valid;
    
    cout << "here: " <<__LINE__ << endl;
	shader_id = glCreateProgram();
	glAttachShader(shader_id, shader_fp);
	glAttachShader(shader_id, shader_vp);
	glLinkProgram(shader_id);
	_valid = validateProgram(shader_id) == 0 && _valid;

    cout << "here: " <<__LINE__ << endl;
    if(!_valid)
        throw "Validation failed";

}

void Shader::replaceShader(const char *type, const char *source) {

    int shader_type;
   
    cout << "replacing " << type << " programm" <<endl;
    
    if(strcmp(type, "shader_vp") == 0) shader_type = GL_VERTEX_SHADER;
    else if(strcmp(type, "shader_fp") == 0) shader_type = GL_FRAGMENT_SHADER;
    else throw "Invalid shader type";

    int shader_new = glCreateShader(shader_type);
    int program_new = glCreateProgram();

    glShaderSource(shader_new, 1, &source, 0);
    glCompileShader(shader_new);

    if(validateShader(shader_new)<0) 
        throw "Shader failed validation";

    // attach new shader 
    glAttachShader(program_new,shader_new);

    // attach old shader (the one which is not replaced)
    if(shader_type == GL_VERTEX_SHADER)
        glAttachShader(program_new,shader_fp);
    else
        glAttachShader(program_new,shader_vp);

    glLinkProgram(program_new);

    //if(validateProgram(program_new)<0)
    //    throw "Program failed validation";
    
    validateProgram(program_new);

    // delete old program
    if(shader_type == GL_VERTEX_SHADER) {
        glAttachShader(shader_id,shader_vp);
        glDeleteShader(shader_vp);
        shader_vp = shader_new;
    }
    else {
        glAttachShader(shader_id,shader_fp);
        glDeleteShader(shader_fp);
        shader_fp = shader_new;
    }

    cout << "replaceShader(): "<<__LINE__ << endl;
    glDeleteProgram(shader_id);
    cout << "replaceShader(): "<<__LINE__ << endl;

    // reset state  to new shader/program
    shader_id = program_new;

    // use new program
    if(_active) bind();

}

Shader::~Shader() {
	glDetachShader(shader_id, shader_fp);
	glDetachShader(shader_id, shader_vp);
    
	glDeleteShader(shader_fp);
	glDeleteShader(shader_vp);
	glDeleteProgram(shader_id);
}

unsigned int Shader::id() {
	return shader_id;
}

bool Shader::valid() {
    return _valid;
}

void Shader::bind() {
    if(_valid) {
        glUseProgram(shader_id);
        _active = true;
    }
    else
        cout << "bind(): shader failed validation" << endl;
}

void Shader::unbind() {
	glUseProgram(0);
    _active=false;
}
