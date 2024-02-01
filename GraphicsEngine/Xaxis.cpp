//g++ -o GraphicsEngine Xaxis.cpp -std=c++11 -I/opt/homebrew/Cellar/glfw/3.3.9/include -I/opt/homebrew/Cellar/glew/2.2.0_1/include -I/opt/homebrew/Cellar/glm/0.9.9.8/include -I/opt/homebrew/Cellar/freeglut/3.4.0/include -L/opt/homebrew/Cellar/glfw/3.3.9/lib -L/opt/homebrew/Cellar/glew/2.2.0_1/lib -L/opt/homebrew/Cellar/freeglut/3.4.0/lib -lglfw -lGLEW -lglut -framework OpenGL
#define GL_SILENCE_DEPRECATION
#include <GL/glut.h>
#include <cmath>

/* Global variables */
char title[] = "Camera Rotating Around Object";
float angle = 0.0f;
float spinSpeed = 1.0f;  // Initial rotation speed
float acceleration = 0.1f;  // Speed acceleration factor

float cameraRadius = 5.0f;  // Initial distance from the object
float cameraAngle = 0.0f;   // Initial camera rotation angle
float cameraSpeed = 0.05f;  // Initial camera rotation speed

/* Initialize OpenGL Graphics */
void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Set background color to black and opaque
    glClearDepth(1.0f);                      // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);                // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);                 // Set the type of depth-test
    glShadeModel(GL_SMOOTH);                // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

    // Set up material properties for the teapot
    glutSetColor(0, 1.0f, 0.5f, 0.0f);  // Set the color to orange
}

/* Handler for window-repaint event */
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);

    // Calculate camera position in circular motion around the object
    float cameraX = cameraRadius * sin(cameraAngle);
    float cameraZ = cameraRadius * cos(cameraAngle);

    // Set up camera position
    gluLookAt(cameraX, 0.0f, cameraZ,      // Camera position
              0.0f, 0.0f, -1.0f,           // Look at point
              0.0f, 1.0f, 0.0f);           // Up vector

    // Draw the first 3D object (a rotating cube)
    glPushMatrix();  // Save the current modelview matrix
    glTranslatef(1.5f, 0.0f, -7.0f);
    glutSolidCube(2.0f);  // Draw a solid cube
    glPopMatrix();  // Restore the previous modelview matrix

    // Draw the second 3D object (a rotating teapot) with the set material
    glLoadIdentity();
    gluLookAt(cameraX, 0.0f, cameraZ,      // Camera position
              0.0f, 0.0f, -1.0f,           // Look at point
              0.0f, 1.0f, 0.0f);           // Up vector

    glTranslatef(-1.5f, 0.0f, -6.0f);
    glutSolidTeapot(1.5f);  // Draw a solid teapot

    glutSwapBuffers();
}

/* Timer function for animation */
void timer(int value) {
    angle += spinSpeed;  // Increment the rotation angle
    cameraAngle += cameraSpeed;  // Increment the camera rotation angle
    glutPostRedisplay(); // Request a redraw to update the display
    glutTimerFunc(16, timer, 0);  // Set up the next timer callback for 60 frames per second
}

/* Keyboard callback function */
void keyboard(unsigned char key, int x, int y) {
    if (key == 'w') {
        // Increase rotation speed when 'w' key is pressed
        spinSpeed += acceleration;
    } else if (key == 's') {
        spinSpeed -= acceleration;
    }
}

/* Reshape function for window resizing */
void reshape(GLsizei width, GLsizei height) {
    if (height == 0) height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Main function */
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(50, 50);
    glutCreateWindow(title);

    // Call the initialization function
    initGL();

    // Register callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);  // Start the timer for animation
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
