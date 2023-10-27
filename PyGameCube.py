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

projectedPoints = [
    [n, n] for n in range(len(points))
]

def connectPoints(i, j, points):
    pygame.draw.line(screen, BLACK, (points[i][0], points[i][1]), (points[j][0], points[j][1]))

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

    angle += 0.01

    screen.fill(WHITE)
    #drawing stuff
    i = 0
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
    # connectPoints(0, 1, projectedPoints)
    # connectPoints(1, 2, projectedPoints)
    # connectPoints(2, 3, projectedPoints)
    # connectPoints(3, 0, projectedPoints)
    #
    # connectPoints(4, 5, projectedPoints)
    # connectPoints(5, 6, projectedPoints)
    # connectPoints(6, 7, projectedPoints)
    # connectPoints(7, 4, projectedPoints)
    #
    # connectPoints(0, 4, projectedPoints)
    # connectPoints(1, 5, projectedPoints)
    # connectPoints(2, 6, projectedPoints)
    # connectPoints(3, 7, projectedPoints)

    for p in range(4):
        connectPoints(p, (p+1) % 4, projectedPoints)
        connectPoints(p+4, ((p+1) % 4) + 4, projectedPoints)
        connectPoints(p, p+4, projectedPoints)

    pygame.display.update()