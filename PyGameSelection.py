import pygame
import numpy as np
from math import *
import sys
from Cube import Cube
from Triangle import Triangle
from Pyramid import Pyramid
from Sphere import Sphere
from HexPrism import HexPrism

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

BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)
GREY = (192,192,192)
PINK = (255,182,193)
CYAN = (0,255,255)
ORANGE = (255,165,0)
PURPLE = (186,85,211)

#Button Colors
buttonText = (255, 87, 51)
buttonShell = (255, 255, 237)
colorLight = (170,170,170)  
colorDark = (100,100,100)

# defining a font
smallfont = pygame.font.SysFont('Corbel', 35)
# Rendering headers + button texts
texts = []
header1 = smallfont.render('Background Color', True, buttonText)
texts.append(header1)
color1 = smallfont.render('Black', True, buttonText)
texts.append(color1)
color2 = smallfont.render('White', True, buttonText)
texts.append(color2)
color3 = smallfont.render('Blue', True, buttonText)
texts.append(color3)
color4 = smallfont.render('Red', True, buttonText)
texts.append(color4)
color5 = smallfont.render('Green', True, buttonText)
texts.append(color5)
color6 = smallfont.render('Grey', True, buttonText)
texts.append(color6)
color7 = smallfont.render('Pink', True, buttonText)
texts.append(color7)
color8 = smallfont.render('Cyan', True, buttonText)
texts.append(color8)
color9 = smallfont.render('Orange', True, buttonText)
texts.append(color9)
color10 = smallfont.render('Purple', True, buttonText)
texts.append(color10)

header2 = smallfont.render('Shape Color', True, buttonText)
backgroundColor = BLACK
shapeColor = WHITE

WIDTH, HEIGHT = 1440, 845
pygame.display.set_caption("3d Renderer")
screen = pygame.display.set_mode((WIDTH, HEIGHT))

scale = 96
circle_pos = [WIDTH / 2, HEIGHT / 2]
angle = 0

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

projection_matrix = np.matrix([
    [1, 0, 0],
    [0, 1, 0]
])

def connectPoints(i, j, points):
    pygame.draw.line(screen, shapeColor, (points[i][0], points[i][1]), (points[j][0], points[j][1]))


def check(object1, object2, object3, object4):
    if object1:
        object1 = not object1
    elif object2: 
        object2 = not object2
    elif object3:
        object3 = not object3
    elif object4:
        object4 = not object4

    return object1, object2, object3, object4


def movement(points, projectedPoints, i, cube, triangle, pyramid, hexPrism, sphere):
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
        pygame.draw.circle(screen, shapeColor, (x, y), 5)
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


def colors(x, y, backgroundColor, shapeColor):
    if 20 <= x <= 100:
        if 60 <= y <= 80:
            #Black
            backgroundColor = BLACK
        elif 100 <= y <= 115:
            #White
            backgroundColor = WHITE
        elif 135 <= y <= 150:
            #Blue
            backgroundColor = BLUE
        elif 170 <= y <= 185:
            #Red
            backgroundColor = RED
        elif 205 <= y <= 220:
            #Green
            backgroundColor = GREEN
        elif 240 <= y <= 255:
            #Grey
            backgroundColor = GREY
        elif 275 <= y <= 290:
            #Pink
            backgroundColor = PINK
        elif 310 <= y <= 325:
            #Cyan
            backgroundColor = CYAN
        elif 345 <= y <= 360:
            #Orange
            backgroundColor = ORANGE
        elif 380 <= y <= 395:
            #Purple
            backgroundColor = PURPLE

        #Now onto shape colors
        elif 490 <= y <= 505:
            #Black2
            shapeColor = BLACK
        elif 525 <= y <= 540:
            #White2
            shapeColor = WHITE
        elif 560 <= y <= 575:
            #Blue2
            shapeColor = BLUE
        elif 595 <= y <= 610:
            #Red2
            shapeColor = RED
        elif 630 <= y <= 645:
            #Green2
            shapeColor = GREEN
        elif 665 <= y <= 680:
            #Grey2
            shapeColor = GREY
        elif 700 <= y <= 715:
            #Pink2
            shapeColor = PINK
        elif 735 <= y <= 750:
            #Cyan2
            shapeColor = CYAN
        elif 770 <= y <= 785:
            #Orange2
            shapeColor = ORANGE
        elif 805 <= y <= 820:
            #Purple2
            shapeColor = PURPLE
    return backgroundColor, shapeColor


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
            if event.key == pygame.K_SPACE:
                paused = not paused
            if event.key == pygame.K_r: 
                forward = not forward
            if event.key == pygame.K_UP:
                scale += 96
            if event.key == pygame.K_DOWN:
                scale -= 96
            if event.key == pygame.K_c:
                cube = not cube
                pyramid, triangle, hexPrism, sphere = check(pyramid, triangle, hexPrism, sphere)
            if event.key == pygame.K_p:
                pyramid = not pyramid
                cube, triangle, hexPrism, sphere = check(cube, triangle, hexPrism, sphere)
            if event.key == pygame.K_t:
                triangle = not triangle
                pyramid, cube, hexPrism, sphere = check(pyramid, cube, hexPrism, sphere)
            if event.key == pygame.K_h:
                hexPrism = not hexPrism
                pyramid, cube, triangle, sphere = check(pyramid, cube, triangle, sphere)
            if event.key == pygame.K_l:
                sphere = not sphere
                pyramid, cube, triangle, hexPrism = check(pyramid, cube, triangle, hexPrism)
        if event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:  # Check if left mouse button is clicked
                x, y = pygame.mouse.get_pos()
                backgroundColor, shapeColor = colors(x, y, backgroundColor, shapeColor)
                if x > 100:
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
            angle += 0.01
        else:
            angle -= 0.01

        screen.fill(backgroundColor)
        #Finding out which shape I want to display and displaying it
        i = 0
        if cube:
            movement(Cube.cubePoints, Cube.cubeProjectedPoints, i, True, False, False, False, False)
        elif triangle:
            movement(Triangle.trianglePoints, Triangle.triangleProjectedPoints, i, False, True, False, False, False)
        elif pyramid:
            movement(Pyramid.pyramidPoints, Pyramid.pyramidProjectedPoints, i, False, False, True, False, False)
        elif hexPrism:
            movement(HexPrism.HexPrismPoints, HexPrism.HexPrismProjectedPoints, i, False, False, False, True, False)
        elif sphere:
            movement(Sphere.spherePoints, Sphere.sphereProjectedPoints, i, False, False, False, False, True)
    place = 20
    for i in range(2):
        if i == 0:
            header = header1
        else:
            header = header2
            
        screen.blit(header, (20, place))
        place += 35
        
        for j in range(1, len(texts)):
            screen.blit(texts[j], (20, place))
            place += 35
        place += 40

    pygame.display.update()
