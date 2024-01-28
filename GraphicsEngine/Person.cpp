#define GL_SILENCE_DEPRECATION
#include <GL/glut.h>

/* Global variables */
char title[] = "Connected Wireframe Person";
float angle = 0.0f;
float spinSpeed = 1.0f;

/* Initialize OpenGL Graphics */
void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Set background color to black and opaque
    glClearDepth(1.0f);                      // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);                // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);                 // Set the type of depth-test
    glShadeModel(GL_SMOOTH);                // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

/* Handler for window-repaint event */
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);

    // Draw the head
    glLoadIdentity();
    glTranslatef(0.0f, 2.0f, -6.0f);
    glutWireSphere(1.0f, 16, 16);  // Draw a wireframe sphere

    // Draw the body (torso)
    glLoadIdentity();
    glTranslatef(0.0f, -1.0f, -6.0f);
    glutWireCube(2.0f);  // Draw a wireframe cube

    // Draw the right arm
    glLoadIdentity();
    glTranslatef(1.5f, 0.5f, -6.0f);
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);  // Rotate around the z-axis
    glutWireCylinder(0.25f, 2.0f, 16, 1);  // Draw a wireframe cylinder

    // Draw the left arm
    glLoadIdentity();
    glTranslatef(-1.5f, 0.5f, -6.0f);
    glRotatef(-45.0f, 0.0f, 0.0f, 1.0f);  // Rotate around the z-axis
    glutWireCylinder(0.25f, 2.0f, 16, 1);  // Draw a wireframe cylinder

    // Draw the right leg
    glLoadIdentity();
    glTranslatef(0.75f, -3.0f, -6.0f);
    glRotatef(30.0f, 0.0f, 0.0f, 1.0f);  // Rotate around the z-axis
    glutWireCylinder(0.25f, 3.0f, 16, 1);  // Draw a wireframe cylinder

    // Draw the left leg
    glLoadIdentity();
    glTranslatef(-0.75f, -3.0f, -6.0f);
    glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);  // Rotate around the z-axis
    glutWireCylinder(0.25f, 3.0f, 16, 1);  // Draw a wireframe cylinder

    glutSwapBuffers();
}

/* Handler for window re-size event */
void reshape(GLsizei width, GLsizei height) {
    if (height == 0) height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Timer function for animation */
void timer(int value) {
    angle += spinSpeed;  // Increment the rotation angle
    glutPostRedisplay(); // Request a redraw to update the display
    glutTimerFunc(16, timer, 0);  // Set up the next timer callback for 60 frames per second
}

/* Main function */
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(50, 50);
    glutCreateWindow(title);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    initGL();

    glutTimerFunc(0, timer, 0);  // Start the timer for animation

    glutMainLoop();
    return 0;
}
