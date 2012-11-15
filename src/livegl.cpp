#include <math.h>
#include "server.h"

LiveGLServer *shader_server;
float t = 0.0;
int mouse_x, mouse_y;
float angle_x, angle_y;
float cam_x, cam_y,cam_z; // cam position
float lx, ly, lz;
bool blocking = false;

float macro[2];

float width  = 800;
float height = 600;

void mouseButton(int button, int state, int x, int y) {
    
    if(button == GLUT_LEFT_BUTTON) {
        if(state == GLUT_UP) {
            
            mouse_x = -1;
            mouse_y = -1;
        }
        else {
            mouse_x = x;
            mouse_y = y;
        }
    }
}

void mouseMove(int x, int y) {
    if(mouse_x>=0) {
        int dx = mouse_x -x;
        int dy = mouse_y -y;

        angle_x += dx/800.0;
        angle_y += dy/600.0;
        if(angle_x>=360.0) angle_x -=360.0;
        if(angle_y>=360.0) angle_y -=360.0;

        if(angle_x<0.0) angle_x +=360.0;
        if(angle_y<0.0) angle_y +=360.0;

        lx = sin(M_PI*angle_x/180.0);
        ly = cos(M_PI*angle_y/180.0);
        int loccam = glGetUniformLocation(shader_server->id(), "camd");
        glUniform3f(loccam, lx,ly,lz);
    }    
}

void keyPressed(unsigned char k, int x, int y) {
    switch(k) {
        case 'j':
            if(macro[0]>-1.0)
                macro[0] -= 0.01;
            break; 
        case 'k':
            if(macro[0]<1.0)
                macro[0] += 0.01;
            break;
        case 'h':
            if(macro[1]>-1.0)
                macro[1] -= 0.01;
            break; 
        case 'l':
            if(macro[1]<1.0)
                macro[1] += 0.01;
            break;
        case 'J':
            if(macro[0]>-1.0)
                macro[0] -= 0.001;
            break; 
        case 'K':
            if(macro[0]<1.0)
                macro[0] += 0.001;
            break;
        case 'H':
            if(macro[1]>-1.0)
                macro[1] -= 0.001;
            break; 
        case 'L':
            if(macro[1]<1.0)
                macro[1] += 0.001;
            break;
        default:
            return;
    }

    printf("Changed macro to %.3f,%.3f\n",macro[0],macro[1]);
}

void renderScene(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // set camara position
    gluLookAt(cam_x,cam_y,cam_z, 
              cam_x+lx,cam_y+ly,cam_z+lz,
               0.0,1.0,0.0);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glColor4f(0.0,0.0,0.0,1.0);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f(1.0,1.0,-1.0);
    glVertex3f(1.0,-1.0,-1.0);
    glVertex3f(-1.0,1.0,-1.0);
    glVertex3f(-1.0,-1.0,-1.0);
    glEnd();
    
    glutSwapBuffers();
}

void init(int argc, char **argv) {

    bool blocking = false;
    if(argc>1) {
        if(strcmp(argv[1],"-b")==0) blocking = true;
    }
    // init GLUT
    glutInit(&argc,argv);
    
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
    glEnable(GL_BLEND|GL_TEXTURE_2D|GL_TEXTURE_1D|GL_DEPTH_TEST);

    // initilize shader server
    shader_server = new LiveGLServer();
    shader_server->bind();

    // init mouse variables
    mouse_x=0;
    mouse_y=0;
    angle_x = 0.0;
    angle_y = 0.0;
    lx = 0.f;
    ly = 0.f;
    lz = 0.f;
    cam_x = 0.f;
    cam_y = 0.4f;
    cam_z = 3.0f;
    macro[0] = 0.0f;
    macro[1] = 0.0f;
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

    width  = w;
    height = h;
}


void idleFunc(void) {

    t += 0.01;

    // TODO: move uniform accessor to LiveGLServer
    int resloc = glGetUniformLocation(shader_server->id(), "resolution");
    int timeloc = glGetUniformLocation(shader_server->id(), "time");
    int macroloc = glGetUniformLocation(shader_server->id(), "macro");

    glUniform2f(resloc,width,height);
    glUniform1f(timeloc,t);
    glUniform2fv(macroloc,1,macro);

    shader_server->poll();
    glutPostRedisplay();
    if(blocking) usleep(500000);
}

int main(int argc, char **argv) {

    init(argc,argv);

    // init app
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(idleFunc);

    // register mouse
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);

    glutKeyboardFunc(keyPressed);

    glutMainLoop();
       
    return 0;
}
