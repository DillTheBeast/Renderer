import pygame
import numpy as np
from math import *

paused = False
forward = False
triangle = False
pyramid = False
cube = True
moveUp = False
moveDown = False
moveLeft = False
moveRight = False

WHITE = (255, 255, 255)
RED = (255, 0, 0)
BLACK = (0, 0, 0)

WIDTH, HEIGHT = 1440, 845
pygame.display.set_caption("3d Renderer")
screen = pygame.display.set_mode((WIDTH, HEIGHT))

scale = 96
circle_pos = [WIDTH / 2, HEIGHT / 2]
angle = 0

cubePoints = []
trianglePoints = []
pyramidPoints = []

# For Cube
cubePoints.append(np.matrix([-1, -1, 1]))
cubePoints.append(np.matrix([1, -1, 1]))
cubePoints.append(np.matrix([1, 1, 1]))
cubePoints.append(np.matrix([-1, 1, 1]))
cubePoints.append(np.matrix([-1, -1, -1]))
cubePoints.append(np.matrix([1, -1, -1]))
cubePoints.append(np.matrix([1, 1, -1]))
cubePoints.append(np.matrix([-1, 1, -1]))

# For Triangle
trianglePoints.append(np.matrix([-1, -1, 1]))  # Base Point 1
trianglePoints.append(np.matrix([1, -1, 1]))  # Base Point 2
trianglePoints.append(np.matrix([0, -1, -1]))  # Base Point 3
trianglePoints.append(np.matrix([0, 1, 0]))  # Apex

# For Pyramid
pyramidPoints.append(np.matrix([-1, -1, 1]))  # Base Point 1
pyramidPoints.append(np.matrix([1, -1, 1]))  # Base Point 2
pyramidPoints.append(np.matrix([1, -1, -1]))  # Base Point 3
pyramidPoints.append(np.matrix([-1, -1, -1]))  # Base Point 4
pyramidPoints.append(np.matrix([0, 1, 0]))  # Apex

projection_matrix = np.matrix([
    [1, 0, 0],
    [0, 1, 0]
])

cubeProjectedPoints = [
    [n, n] for n in range(len(cubePoints))
]
triangleProjectedPoints = [
    [n, n] for n in range(len(trianglePoints))
]
pyramidProjectedPoints = [
    [n, n] for n in range(len(pyramidPoints))
]


def connectPoints(i, j, points):
    pygame.draw.line(screen, BLACK, (points[i][0], points[i][1]), (points[j][0], points[j][1]))


def thing(points, projectedPoints, i, cube, triangle, pyramid):
    for point in points:
        rotated2D = np.dot(rotationZ, point.reshape((3, 1)))
        rotated2D = np.dot(rotationX, rotated2D)
        rotated2D = np.dot(rotationY, rotated2D)

        projected2D = np.dot(projection_matrix, rotated2D)

        x = int(projected2D[0, 0] * scale) + circle_pos[0]
        y = int(projected2D[1, 0] * scale) + circle_pos[1]

        projectedPoints[i] = [x, y]
        pygame.draw.circle(screen, BLACK, (x, y), 5)
        i += 1
    if cube:
        for p in range(4):
            connectPoints(p, (p + 1) % 4, projectedPoints)
            connectPoints(p + 4, ((p + 1) % 4) + 4, projectedPoints)
            connectPoints(p, p + 4, projectedPoints)
    elif triangle:
        for p in range(3):
            connectPoints(p, (p + 1) % 3, projectedPoints)
            connectPoints(p, 3, projectedPoints)
    elif pyramid:
        for p in range(4):
            connectPoints(p, (p + 1) % 4, projectedPoints)
            connectPoints(p, 4, projectedPoints)

clock = pygame.time.Clock()

while True:

    clock.tick(60)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            exit()
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                pygame.quit()
                exit()
            if event.key == pygame.K_SPACE:  # Check if the key pressed is space
                paused = not paused
            if event.key == pygame.K_r:  # Check if the key pressed is space
                forward = not forward
            if event.key == pygame.K_UP:
                scale += 96
            if event.key == pygame.K_DOWN:
                scale -= 96
            if event.key == pygame.K_c:
                cube = not cube
                if pyramid:
                    pyramid = False
                elif triangle:
                    triangle = False
            if event.key == pygame.K_p:
                pyramid = not pyramid
                if cube:
                    cube = False
                elif triangle:
                    triangle = False
            if event.key == pygame.K_t:
                triangle = not triangle
                if pyramid:
                    pyramid = False
                elif cube:
                    cube = False
        if event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:  # Check if left mouse button is clicked
                x, y = pygame.mouse.get_pos()
                circle_pos = [x,y]

    #Doing these keys pressed this way so it also works for holding    
    keys = pygame.key.get_pressed()  # get the state of all the keys
    if keys[pygame.K_w]:
        circle_pos[1] -= 5
    if keys[pygame.K_s]:
        circle_pos[1] += 5
    if keys[pygame.K_d]:
        circle_pos[0] += 5
    if keys[pygame.K_a]:
        circle_pos[0] -= 5
    if not paused:
        # update stuff
        rotationZ = np.matrix([
            [cos(angle), -sin(angle), 0],
            [sin(angle), cos(angle), 0],
            [0, 0, 1]
        ])
        rotationY = np.matrix([
            [cos(angle), 0, sin(angle)],
            [0, 1, 0],
            [-sin(angle), 0, cos(angle)]
        ])
        rotationX = np.matrix([
            [1, 0, 0],
            [0, cos(angle), -sin(angle)],
            [0, sin(angle), cos(angle)]
        ])

        if forward:
            angle += 0.01
        else:
            angle -= 0.01

        screen.fill(WHITE)
        # drawing stuff
        i = 0
        if cube:
            thing(cubePoints, cubeProjectedPoints, i, True, False, False)
        elif triangle:
            thing(trianglePoints, triangleProjectedPoints, i, False, True, False)
        elif pyramid:
            thing(pyramidPoints, pyramidProjectedPoints, i, False, False, True)

    pygame.display.update()
