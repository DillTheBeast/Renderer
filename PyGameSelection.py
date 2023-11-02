import pygame
import numpy as np
from math import *
import sys
from Cube import Cube
from Triangle import Triangle
from Pyramid import Pyramid
from Sphere import Sphere
from HexPrism import HexPrism
from ColorsAndText import ColorsAndText

pygame.init()

paused = False
forward = False
triangle = False
pyramid = False
cube = True
hexPrism = False
sphere = False
moveUp = False
moveDown = False
moveLeft = False
moveRight = False

WIDTH, HEIGHT = 1440, 845
pygame.display.set_caption("3d Renderer")
screen = pygame.display.set_mode((WIDTH, HEIGHT))

scale = 96
circle_pos = [WIDTH / 2, HEIGHT / 2]
angle = 0
angleAddition = 0.01

# For Cube
Cube = Cube()
Cube.appendCubePoints()

# For Triangle
Triangle = Triangle()
Triangle.appendTrianglePoints()

# For Pyramid
Pyramid = Pyramid()
Pyramid.appendPyramidPoints()

#For Hexagon Prism
HexPrism = HexPrism()
HexPrism.appendHexPrismPoints()

#For Sphere
Sphere = Sphere()
Sphere.appendSpherePoints()

#For the colors and the texts displayed
ColorsAndText = ColorsAndText()
ColorsAndText.addColors()

projection_matrix = np.matrix([
    [1, 0, 0],
    [0, 1, 0]
])

def connectPoints(i, j, points):
    pygame.draw.line(screen, ColorsAndText.shapeColor, (points[i][0], points[i][1]), (points[j][0], points[j][1]))


def checkShapes(change, object1, object2, object3, object4):
    if object1:
        object1 = not object1
    elif object2: 
        object2 = not object2
    elif object3:
        object3 = not object3
    elif object4:
        object4 = not object4

    change = not change

    return change, object1, object2, object3, object4


def movement(points, projectedPoints, i, cube, triangle, pyramid, hexPrism):
    #Making dots at updated/rotated points
    for point in points:
        rotated2D = np.dot(rotationZ, point.reshape((3, 1)))
        rotated2D = np.dot(rotationX, rotated2D)
        rotated2D = np.dot(rotationY, rotated2D)

        projected2D = np.dot(projection_matrix, rotated2D)

        x = int(projected2D[0, 0] * scale) + circle_pos[0]
        y = int(projected2D[1, 0] * scale) + circle_pos[1]

        # print("i:", i)
        # print("length of projectedPoints:", len(projectedPoints))

        if i >= len(projectedPoints):
            projectedPoints.append([x, y])
        else:
            projectedPoints[i] = [x, y]
        pygame.draw.circle(screen, ColorsAndText.shapeColor, (x, y), 5)
        i += 1
    #Connecting the points
    if cube:
        Cube.connectCubePoints(connectPoints, projectedPoints)
    elif triangle:
        Triangle.connectTrianglePoints(connectPoints, projectedPoints)
    elif pyramid:
        Pyramid.connectPyramidPoints(connectPoints, projectedPoints)
    elif hexPrism:
        HexPrism.connectHexPrismPoints(connectPoints, projectedPoints)


clock = pygame.time.Clock()

while True:

    clock.tick(60)
    for event in pygame.event.get():
        #Checking what key is pressed and then executing specific action
        if event.type == pygame.QUIT:
            pygame.quit()
            exit()
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                pygame.quit()
                exit()
            if event.key == pygame.K_SPACE: paused = not paused
            if event.key == pygame.K_r: forward = not forward
            if event.key == pygame.K_UP: scale += 96
            if event.key == pygame.K_DOWN: scale -= 96
            if event.key == pygame.K_c: cube, pyramid, triangle, hexPrism, sphere = checkShapes(cube, pyramid, triangle, hexPrism, sphere)
            if event.key == pygame.K_p: pyramid, cube, triangle, hexPrism, sphere = checkShapes(pyramid, cube, triangle, hexPrism, sphere)
            if event.key == pygame.K_t: triangle, pyramid, cube, hexPrism, sphere = checkShapes(triangle, pyramid, cube, hexPrism, sphere)
            if event.key == pygame.K_h: hexPrism, pyramid, cube, triangle, sphere = checkShapes(hexPrism, pyramid, cube, triangle, sphere)
            if event.key == pygame.K_l: sphere, pyramid, cube, triangle, hexPrism = checkShapes(sphere, pyramid, cube, triangle, hexPrism)
            if event.key == pygame.K_f: angleAddition += 0.01
            if event.key == pygame.K_b: angleAddition -= 0.01
        if event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:  # Check if left mouse button is clicked
                ColorsAndText.x, ColorsAndText.y = pygame.mouse.get_pos()
                ColorsAndText.changeColors()
                if ColorsAndText.x > 100 and (ColorsAndText.x < 1000 and ColorsAndText.y > 35):
                    circle_pos = [ColorsAndText.x, ColorsAndText.y]

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
        # update shape rotations
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
            angle += angleAddition
        else:
            angle -= angleAddition

        screen.fill(ColorsAndText.backgroundColor)
        #Finding out which shape I want to display and displaying it
        if cube:
            movement(Cube.cubePoints, Cube.cubeProjectedPoints, 0, True, False, False, False)
        elif triangle:
            movement(Triangle.trianglePoints, Triangle.triangleProjectedPoints, 0, False, True, False, False)
        elif pyramid:
            movement(Pyramid.pyramidPoints, Pyramid.pyramidProjectedPoints, 0, False, False, True, False)
        elif hexPrism:
            movement(HexPrism.HexPrismPoints, HexPrism.HexPrismProjectedPoints, 0, False, False, False, True)
        elif sphere:
            movement(Sphere.spherePoints, Sphere.sphereProjectedPoints, 0, False, False, False, False)
    place = 20
    for i in range(2):
        if i == 0:
            header = ColorsAndText.header1
        else:
            header = ColorsAndText.header2
            
        screen.blit(header, (20, place))
        place += 35
        
        for j in range(1, len(ColorsAndText.texts)):
            screen.blit(ColorsAndText.texts[j], (20, place))
            place += 35
        place += 40
    screen.blit(ColorsAndText.runButton, (WIDTH-55, 10))

    pygame.display.update()
