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
from Physics import Physics

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
xAngle = 0
yAngle = 0
angleAddition = 0.01
wall_thickness = 10
gravity = 0.5
bounceStop = 0.3
viewing_angle = 0

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
#For Sp65555555here
Sphere = Sphere()
Sphere.appendSpherePoints()

#For the colors and the texts displayed
ColorsAndText = ColorsAndText()
ColorsAndText.addColors()

#For the physics
# physics = Physics()

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


def movement(shape_instance):
    points = shape_instance.cubePoints
    projectedPoints = shape_instance.cubeProjectedPoints
    i = 0

    # Making dots at updated/rotated points
    for point in points:
        rotated2D = np.dot(rotationZ, point.reshape((3, 1)))
        rotated2D = np.dot(rotationX, rotated2D)
        rotated2D = np.dot(rotationY, rotated2D)

        projected2D = np.dot(projection_matrix, rotated2D)

        x = int(projected2D[0, 0] * scale) + circle_pos[0]
        y = int(projected2D[1, 0] * scale) + circle_pos[1]

        if i >= len(projectedPoints):
            projectedPoints.append([x, y])
        else:
            projectedPoints[i] = [x, y]
        pygame.draw.circle(screen, shape_instance.color, (x, y), 5)
        i += 1

    # Connecting the points
    shape_instance.connectCubePoints(screen, projectedPoints, connectPoints)


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
                    
    keys = pygame.key.get_pressed()

    keys = pygame.key.get_pressed()

    if keys[pygame.K_w]:
        yAngle += angleAddition
    if keys[pygame.K_s]:
        yAngle -= angleAddition
    if keys[pygame.K_d]:
        xAngle += angleAddition
    if keys[pygame.K_a]:
        xAngle -= angleAddition

    # update rotation matrices based on angles
    rotationZ = np.matrix([
        [cos(viewing_angle), -sin(viewing_angle), 0],
        [sin(viewing_angle), cos(viewing_angle), 0],
        [0, 0, 1]
    ])

    rotationY = np.matrix([
        [cos(xAngle), 0, sin(xAngle)],
        [0, 1, 0],
        [-sin(xAngle), 0, cos(xAngle)]
    ])

    rotationX = np.matrix([
        [1, 0, 0],
        [0, cos(yAngle), -sin(yAngle)],
        [0, sin(yAngle), cos(yAngle)]
    ])
    
    if not paused:
        
        if ColorsAndText.falling:
            #Need to move object down
            circle_pos[1] += 5
        # update shape rotations
        # rotationZ = np.matrix([
        #     [cos(viewing_angle), -sin(viewing_angle), 0],
        #     [sin(viewing_angle), cos(viewing_angle), 0],
        #     [0, 0, 1]
        # ])
        # rotationY = np.matrix([
        #     [cos(xAngle), 0, sin(xAngle)],
        #     [0, 1, 0],
        #     [-sin(xAngle), 0, cos(xAngle)]
        # ])
        # rotationX = np.matrix([
        #     [1, 0, 0],
        #     [0, cos(yAngle), -sin(yAngle)],
        #     [0, sin(yAngle), cos(yAngle)]
        # ])

        #if forward:
        #    angle += angleAddition
        #else:
        #    angle -= angleAddition

        screen.fill(ColorsAndText.backgroundColor)
        #Finding out which shape I want to display and displaying it
        if cube:
            movement(Cube)
        elif triangle:
            movement(Triangle)
        elif pyramid:
            movement(Pyramid)
        elif hexPrism:
            movement(HexPrism)
        elif sphere:
            movement(Sphere)

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
