#define GL_SILENCE_DEPRECATION
#include <GL/glut.h>

namespace OpenGLUtils {

    class SpinningTriangle {
    public:
        void StartAnimation() {
            glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);  // Enable depth buffer
            glutInitWindowSize(800, 600);
            glutCreateWindow("Spinning Triangle");

            glEnable(GL_DEPTH_TEST);  // Enable depth testing

            glutDisplayFunc(Display);
            glutIdleFunc(Idle);

            glutMainLoop();
        }

    private:
        static void Display() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear color and depth buffers

            glBegin(GL_TRIANGLES);
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(-0.5f, -0.5f, 0.0f);  // Specify Z-coordinate for each vertex

            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(0.5f, -0.5f, 0.0f);

            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(0.0f, 0.5f, 0.0f);
            glEnd();

            glutSwapBuffers();
        }

        static void Idle() {
            static float angle = 0.0f;
            angle += 0.01f;

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glTranslatef(0.0f, 0.0f, -5.0f);  // Move the triangle away from the camera
            glRotatef(angle, 1.0f, 1.0f, 1.0f);  // Rotate around all axes

            glutPostRedisplay();
        }
    };
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    OpenGLUtils::SpinningTriangle spinningTriangle;

    spinningTriangle.StartAnimation();

    return 0;
}
