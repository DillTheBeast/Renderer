//g++ -o GraphicsEngine test.cpp -std=c++11 -I/opt/homebrew/Cellar/glfw/3.3.9/include -I/opt/homebrew/Cellar/glew/2.2.0_1/include -I/opt/homebrew/Cellar/glm/0.9.9.8/include -I/opt/homebrew/Cellar/freeglut/3.4.0/include -L/opt/homebrew/Cellar/glfw/3.3.9/lib -L/opt/homebrew/Cellar/glew/2.2.0_1/lib -L/opt/homebrew/Cellar/freeglut/3.4.0/lib -lglfw -lGLEW -lglut -framework OpenGL
#define GL_SILENCE_DEPRECATION
#include <GL/glut.h>
#include <iostream>

namespace OpenGLUtils {

    class SpinningTriangle {
    public:
        SpinningTriangle() {
            // Set the instance pointer in the constructor
            instance = this;
        }

        void StartAnimation() {
            glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
            glutInitWindowSize(800, 600);
            glutCreateWindow("Spinning Triangle");

            glEnable(GL_DEPTH_TEST);

            // Pass the 'this' pointer as user data to be used in trampoline functions
            glutDisplayFunc(TrampolineDisplay);
            glutIdleFunc(TrampolineIdle);

            glutMainLoop();
        }

    private:
        static SpinningTriangle* instance;  // Hold the instance to be used in trampoline functions

        static void TrampolineDisplay() {
            if (instance) {
                instance->Display();
            }
        }

        static void TrampolineIdle() {
            if (instance) {
                instance->Idle();
            }
        }

        void Display() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glBegin(GL_TRIANGLES);
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(-0.5f, -0.5f, 0.0f);
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(0.5f, -0.5f, 0.0f);
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(0.0f, 0.5f, 0.0f);
            glEnd();
            glutSwapBuffers();
        }

        void Idle() {
            static float angle = 0.0f;
            angle += 0.01f;
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glTranslatef(0.0f, 0.0f, -5.0f);
            glRotatef(angle, 1.0f, 1.0f, 1.0f);
            glutPostRedisplay();
        }
    };

    // Initialize the instance pointer
    SpinningTriangle* SpinningTriangle::instance = nullptr;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    // Create an instance of the SpinningTriangle class.
    OpenGLUtils::SpinningTriangle spinningTriangle;

    // Start the spinning triangle animation.
    spinningTriangle.StartAnimation();

    return 0;
}
