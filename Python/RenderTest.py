import pygame
import numpy as np
from math import cos, sin
from CubeTest import Cube
from Triangle import Triangle
from Pyramid import Pyramid
from Sphere import Sphere
from HexPrism import HexPrism
from ColorsAndText import ColorsAndText
from OpenGL.GL import *
from OpenGL.GLU import *
from pygame.locals import *

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
render_cube = True  # Initialize render_cube variable

WIDTH, HEIGHT = 1440, 845
pygame.display.set_caption("3D Renderer")
screen = pygame.display.set_mode((WIDTH, HEIGHT), OPENGL | DOUBLEBUF)

glEnable(GL_DEPTH_TEST)
glEnable(GL_TEXTURE_2D)
glShadeModel(GL_SMOOTH)
glMatrixMode(GL_PROJECTION)
glLoadIdentity()
gluPerspective(45, (WIDTH / HEIGHT), 0.1, 50.0)
glMatrixMode(GL_MODELVIEW)
glTranslatef(0.0, 0.0, -5)

# Load textures
texture_files = [
    "front.jpeg",
    "back.jpeg",
    "right.jpeg",
    "left.jpeg",
    "top.jpeg",
    "bottom.jpeg"
]

textures = {}

for i, file in enumerate(texture_files):
    image = pygame.image.load(file)
    texture_data = pygame.image.tostring(image, "RGBA", 1)
    width, height = image.get_size()

    texture_id = glGenTextures(1)
    glBindTexture(GL_TEXTURE_2D, texture_id)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data)

    textures[i] = texture_id

angle = 0
angleAddition = 0.01
scale = 96
circle_pos = [WIDTH / 2, HEIGHT / 2]
rotationZ = np.identity(3)
rotationX = np.identity(3)
rotationY = np.identity(3)

# For Cube
Cube = Cube()
Cube.appendCubePoints()

# Create an instance of ColorsAndText
colors_and_text = ColorsAndText()

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
            if event.key == pygame.K_SPACE:
                paused = not paused
            if event.key == pygame.K_r:
                forward = not forward
            if event.key == pygame.K_UP:
                scale += 96
            if event.key == pygame.K_DOWN:
                scale -= 96
            if event.key == pygame.K_c:
                cube, pyramid, triangle, hexPrism, sphere = checkShapes(cube, pyramid, triangle, hexPrism, sphere)
            if event.key == pygame.K_p:
                pyramid, cube, triangle, hexPrism, sphere = checkShapes(pyramid, cube, triangle, hexPrism, sphere)
            if event.key == pygame.K_t:
                triangle, pyramid, cube, hexPrism, sphere = checkShapes(triangle, pyramid, cube, hexPrism, sphere)
            if event.key == pygame.K_h:
                hexPrism, pyramid, cube, triangle, sphere = checkShapes(hexPrism, pyramid, cube, triangle, sphere)
            if event.key == pygame.K_l:
                sphere, pyramid, cube, triangle, hexPrism = checkShapes(sphere, pyramid, cube, triangle, hexPrism)
            if event.key == pygame.K_f:
                angleAddition += 0.01
            if event.key == pygame.K_b:
                angleAddition -= 0.01
        if event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:
                colors_and_text.x, colors_and_text.y = pygame.mouse.get_pos()
                colors_and_text.changeColors()
                if 100 < colors_and_text.x < 1000 and colors_and_text.y > 35:
                    circle_pos = [colors_and_text.x, colors_and_text.y]

    keys = pygame.key.get_pressed()
    if keys[pygame.K_w]:
        circle_pos[1] -= 5
    if keys[pygame.K_s]:
        circle_pos[1] += 5
    if keys[pygame.K_d]:
        circle_pos[0] += 5
    if keys[pygame.K_a]:
        circle_pos[0] -= 5

    if not paused:
        if colors_and_text.falling:
            circle_pos[1] += 5

        rotationZ = np.array([
            [cos(angle), -sin(angle), 0],
            [sin(angle), cos(angle), 0],
            [0, 0, 1]
        ])
        rotationY = np.array([
            [cos(angle), 0, sin(angle)],
            [0, 1, 0],
            [-sin(angle), 0, cos(angle)]
        ])
        rotationX = np.array([
            [1, 0, 0],
            [0, cos(angle), -sin(angle)],
            [0, sin(angle), cos(angle)]
        ])

        if forward:
            angle += angleAddition
        else:
            angle -= angleAddition

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glLoadIdentity()
        gluPerspective(45, (WIDTH / HEIGHT), 0.1, 50.0)
        glTranslatef(0.0, 0.0, -5)

        # Render your shape based on the chosen shape (cube, pyramid, etc.)
        if render_cube:
            Cube.connectCubePoints(screen, Cube.cubeProjectedPoints, connectPoints)

        # Draw UI elements
        place = 20
        for i in range(2):
            if i == 0:
                header = colors_and_text.header1
            else:
                header = colors_and_text.header2

            screen.blit(header, (20, place))
            place += 35

            for j in range(1, len(colors_and_text.texts)):
                screen.blit(colors_and_text.texts[j], (20, place))
                place += 35
            place += 40

        screen.blit(colors_and_text.runButton, (WIDTH - 55, 10))

    pygame.display.flip()