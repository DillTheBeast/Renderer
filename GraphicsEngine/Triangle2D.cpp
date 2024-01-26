//g++ -o GraphicsEngine Triangle2D.cpp -std=c++11 -I/opt/homebrew/Cellar/glfw/3.3.9/include -I/opt/homebrew/Cellar/glew/2.2.0_1/include -I/opt/homebrew/Cellar/glm/0.9.9.8/include -I/opt/homebrew/Cellar/freeglut/3.4.0/include -L/opt/homebrew/Cellar/glfw/3.3.9/lib -L/opt/homebrew/Cellar/glew/2.2.0_1/lib -L/opt/homebrew/Cellar/freeglut/3.4.0/lib -lglfw -lGLEW -lglut -framework OpenGL
// Including the necessary libraries for OpenGL.
#define GL_SILENCE_DEPRECATION
#include <GL/glut.h>

// Using a namespace to prevent potential naming conflicts and to logically group
// associated functionalities.
namespace OpenGLUtils {

    /**
    * @class SpinningTriangle
    * Represents a spinning triangle in OpenGL.
    */
    class SpinningTriangle {
    public:
        /**
        * Initializes the OpenGL environment and starts the spinning triangle animation.
        */
        void StartAnimation() {
            // Initialize GLUT and create a window.
            glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
            glutInitWindowSize(800, 600);
            glutCreateWindow("Spinning Triangle");

            // Set the display function.
            glutDisplayFunc(Display);

            // Set the idle function.
            glutIdleFunc(Idle);

            // Start the main loop.
            glutMainLoop();
        }

    private:
        /**
        * Renders the spinning triangle on the screen.
        */
        static void Display() {
        // Clear the color buffer.
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the triangle with RGB colors.
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);  // Red
        glVertex2f(-0.5f, -0.5f);

        glColor3f(0.0f, 1.0f, 0.0f);  // Green
        glVertex2f(0.5f, -0.5f);

        glColor3f(0.0f, 0.0f, 1.0f);  // Blue
        glVertex2f(0.0f, 0.5f);
        glEnd();

        // Swap the buffers.
        glutSwapBuffers();
    }


        /**
        * Updates the rotation angle of the triangle and triggers a redraw.
        */
        static void Idle() {
            // Update the rotation angle.
            static float angle = 0.0f;
            angle += 0.01f;

            // Set the rotation matrix.
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glRotatef(angle, 0.0f, 0.0f, 1.0f);

            // Redraw the scene.
            glutPostRedisplay();
        }
    };
}

int main(int argc, char** argv) {
    // Initialize GLUT.
    glutInit(&argc, argv);

    // Create an instance of the SpinningTriangle class.
    OpenGLUtils::SpinningTriangle spinningTriangle;

    // Start the spinning triangle animation.
    spinningTriangle.StartAnimation();

    return 0;
}