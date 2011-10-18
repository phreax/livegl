#include <math.h>
#include "server.h"

LiveGLServer *shader_server;
float t = 0.0;

void renderScene(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glColor4f(0.0,0.0,0.0,1.0);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f(1.0,1.0,-1.0);
    glVertex3f(1.0,-1.0,-1.0);
    glVertex3f(-1.0,1.0,-1.0);
    glVertex3f(-1.0,-1.0,-1.0);
    glEnd();

    t += 0.01;

    int resloc = glGetUniformLocation(shader_server->id(), "resolution");
    int timeloc = glGetUniformLocation(shader_server->id(), "time");
    glUniform2f(resloc,800.0,600.0);
    glUniform1f(timeloc,t);

    // updatetime

    glutSwapBuffers();

    shader_server->poll();
}

void init() {

    glutInitContextVersion(3, 0);

    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(800,600);
    glutCreateWindow("hello shader");

    
    glewExperimental=true;  // hack around for GL 3.3
    GLenum err = glewInit();   
    if(err!=GLEW_OK) {
        printf("glewInit failed, aborting.\n");
    }
    
    printf("OpenGL version = %s\n", glGetString(GL_VERSION));
    printf("GL Shading Language version = %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // configure opengel
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClearDepth(1.0f);
    glEnable(GL_BLEND|GL_TEXTURE_2D|GL_DEPTH_TEST);

    // initilize shader
    shader_server = new LiveGLServer();

    shader_server->bind();
    
        
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio =  w * 1.0 / h;
	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 1.0f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}


void idleFunc(void) {
    glutPostRedisplay();
}



int main(int argc, char **argv) {

    // init GLUT
    glutInit(&argc,argv);
    
    init();

    // init app
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(idleFunc);

    glutSwapBuffers();
    glutMainLoop();

    return 0;
}
