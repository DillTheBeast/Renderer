#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>
#include <vector>
#include <cmath>

class Cube {
public:
    std::vector<std::vector<float>> cubePoints;
    std::vector<std::vector<float>> cubeProjectedPoints;
    std::vector<std::vector<int>> cubeFaces;

    Cube() {
        cubeFaces = {
            {0, 1, 2, 3},  // front face
            {4, 5, 6, 7},  // back face
            {0, 1, 5, 4},  // bottom face
            {3, 2, 6, 7},  // top face
            {0, 3, 7, 4},  // left face
            {1, 2, 6, 5}   // right face
        };

        appendCubePoints();
    }

    void appendCubePoints() {
        cubePoints.push_back({-1.0, -1.0, 1.0});
        cubePoints.push_back({1.0, -1.0, 1.0});
        cubePoints.push_back({1.0, 1.0, 1.0});
        cubePoints.push_back({-1.0, 1.0, 1.0});
        cubePoints.push_back({-1.0, -1.0, -1.0});
        cubePoints.push_back({1.0, -1.0, -1.0});
        cubePoints.push_back({1.0, 1.0, -1.0});
        cubePoints.push_back({-1.0, 1.0, -1.0});
    }

    void connectCubePoints() {
        // Implement connection logic here
    }
};

bool paused = false;
bool forward = false;
bool triangle = false;
bool pyramid = false;
bool cubeFlag = true;
bool hexPrism = false;
bool sphere = false;
bool moveUp = false;
bool moveDown = false;
bool moveLeft = false;
bool moveRight = false;

int WIDTH = 1440;
int HEIGHT = 845;
float scale = 96.0;
std::vector<float> circle_pos = {static_cast<float>(WIDTH / 2), static_cast<float>(HEIGHT / 2)};
float angle = 0.0;
float angleAddition = 0.01;
float wall_thickness = 10.0;
float gravity = 0.5;
float bounceStop = 0.3;

float projection_matrix[2][3] = {
    {1, 0, 0},
    {0, 1, 0}
};

float rotationZ[3][3];
float rotationY[3][3];
float rotationX[3][3];

void connectPoints(int i, int j, std::vector<std::vector<float>>& points) {
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2f(points[i][0], points[i][1]);
    glVertex2f(points[j][0], points[j][1]);
    glEnd();
}

void checkShapes(bool& change, bool& object1, bool& object2, bool& object3, bool& object4) {
    if (object1) {
        object1 = false;
    } else if (object2) {
        object2 = false;
    } else if (object3) {
        object3 = false;
    } else if (object4) {
        object4 = false;
    }

    change = !change;
}

void movement(std::vector<std::vector<float>>& points, int i, bool cube, bool triangle, bool pyramid, bool hexPrism) {
    for (auto& point : points) {
        float rotated2D[3] = {0};
        for (int j = 0; j < 3; j++) {
            rotated2D[j] = rotationZ[j][0] * point[0] + rotationZ[j][1] * point[1] + rotationZ[j][2] * point[2];
        }
        for (int j = 0; j < 3; j++) {
            rotated2D[j] = rotationX[j][0] * rotated2D[0] + rotationX[j][1] * rotated2D[1] + rotationX[j][2] * rotated2D[2];
        }
        for (int j = 0; j < 3; j++) {
            rotated2D[j] = rotationY[j][0] * rotated2D[0] + rotationY[j][1] * rotated2D[1] + rotationY[j][2] * rotated2D[2];
        }

        float projected2D[2] = {0};
        for (int j = 0; j < 2; j++) {
            projected2D[j] = projection_matrix[j][0] * rotated2D[0] + projection_matrix[j][1] * rotated2D[1] + projection_matrix[j][2] * rotated2D[2];
        }

        int x = static_cast<int>(projected2D[0] * scale) + static_cast<int>(circle_pos[0]);
        int y = static_cast<int>(projected2D[1] * scale) + static_cast<int>(circle_pos[1]);

        if (i >= points.size()) {
            points.push_back({x, y});
        } else {
            points[i] = {x, y};
        }

        glPointSize(5);
        glBegin(GL_POINTS);
        glColor3f(1.0, 1.0, 1.0);
        glVertex2f(x, y);
        glEnd();
        i++;
    }

    if (cube) {
        // Call cube connection function
        // cube.connectCubePoints();
    } else if (triangle) {
        // Call triangle connection function
    } else if (pyramid) {
        // Call pyramid connection function
    } else if (hexPrism) {
        // Call hexPrism connection function
    }
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (!paused) {
        angle += forward ? angleAddition : -angleAddition;

        rotationZ[0][0] = cos(angle);
        rotationZ[0][1] = -sin(angle);
        rotationZ[0][2] = 0;
        rotationZ[1][0] = sin(angle);
        rotationZ[1][1] = cos(angle);
        rotationZ[1][2] = 0;
        rotationZ[2][0] = 0;
        rotationZ[2][1] = 0;
        rotationZ[2][2] = 1;

        rotationY[0][0] = cos(angle);
        rotationY[0][1] = 0;
        rotationY[0][2] = sin(angle);
        rotationY[1][0] = 0;
        rotationY[1][1] = 1;
        rotationY[1][2] = 0;
        rotationY[2][0] = -sin(angle);
        rotationY[2][1] = 0;
        rotationY[2][2] = cos(angle);

        rotationX[0][0] = 1;
        rotationX[0][1] = 0;
        rotationX[0][2] = 0;
        rotationX[1][0] = 0;
        rotationX[1][1] = cos(angle);
        rotationX[1][2] = -sin(angle);
        rotationX[2][0] = 0;
        rotationX[2][1] = sin(angle);
        rotationX[2][2] = cos(angle);

        movement(Cube.cubePoints, 0, cubeFlag, triangle, pyramid, hexPrism);
    }

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("3D Renderer");
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(render);
    glutIdleFunc(render);

    glutMainLoop();

    return 0;
}
