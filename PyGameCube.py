import pygame
import numpy as np
from math import *

WHITE = (255, 255, 255)
RED = (255, 0, 0)
BLACK = (0, 0, 0)

WIDTH, HEIGHT = 800, 600
pygame.display.set_caption("3d Cube")
screen = pygame.display.set_mode((WIDTH, HEIGHT))

scale = 100
circle_pos = [WIDTH/2, HEIGHT/2]
angle = 0

points = []

points.append(np.matrix([-1, -1, 1]))
points.append(np.matrix([1, -1, 1]))
points.append(np.matrix([1, 1, 1]))
points.append(np.matrix([-1, 1, 1]))
points.append(np.matrix([-1, -1, -1]))
points.append(np.matrix([1, -1, -1]))
points.append(np.matrix([1, 1, -1]))
points.append(np.matrix([-1, 1, -1]))

projection_matrix = np.matrix([
    [1, 0, 0],
    [0, 1, 0]
])
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

    #update stuff
    rotationZ = np.matrix([
        [cos(angle), -sin(angle), 0],
        [sin(angle), cos(angle), 0],
        [0, 0, 1]
    ])

    angle += 0.01

    screen.fill(WHITE)
    #drawing stuff
    for point in points:
        rotated2D = np.dot(rotationZ, point.reshape((3, 1)))
        projected2D = np.dot(projection_matrix, rotated2D)
        x = int(projected2D[0, 0] * scale) + circle_pos[0]
        y = int(projected2D[1, 0] * scale) + circle_pos[1 ]
        pygame.draw.circle(screen, BLACK, (x, y), 5)

    pygame.display.update()