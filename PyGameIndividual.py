import pygame
import numpy as np
from math import *

paused = False
forward = False

WHITE = (255, 255, 255)
RED = (255, 0, 0)
BLACK = (0, 0, 0)

WIDTH, HEIGHT = 1440, 845
pygame.display.set_caption("3d Cube")
screen = pygame.display.set_mode((WIDTH, HEIGHT))

scale = 96
circle_pos = [WIDTH/2, HEIGHT/2]
angle = 0

points = []

#For Cube
# points.append(np.matrix([-1, -1, 1]))
# points.append(np.matrix([1, -1, 1]))
# points.append(np.matrix([1, 1, 1]))
# points.append(np.matrix([-1, 1, 1]))
# points.append(np.matrix([-1, -1, -1]))
# points.append(np.matrix([1, -1, -1]))
# points.append(np.matrix([1, 1, -1]))
# points.append(np.matrix([-1, 1, -1]))

#For Triangle
# points.append(np.matrix([-1, -1, 1]))   # Base Point 1
# points.append(np.matrix([1, -1, 1]))    # Base Point 2
# points.append(np.matrix([0, -1, -1]))   # Base Point 3
# points.append(np.matrix([0, 1, 0]))     # Apex

#For Pyramid
# points.append(np.matrix([-1, -1, 1]))   # Base Point 1
# points.append(np.matrix([1, -1, 1]))    # Base Point 2
# points.append(np.matrix([1, -1, -1]))   # Base Point 3
# points.append(np.matrix([-1, -1, -1]))   # Base Point 4
# points.append(np.matrix([0, 1, 0]))     # Apex


points.append(np.matrix([0, 1, 0]))     # Apex



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
            if event.key == pygame.K_SPACE:  # Check if the key pressed is space
                paused = not paused
            if event.key == pygame.K_r:  # Check if the key pressed is space
                forward = not forward
            if event.key == pygame.K_UP:
                scale += 96
            if event.key == pygame.K_DOWN:
                scale -= 96

    if not paused:
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

        if forward:
            angle += 0.01
        else:
            angle -= 0.01


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

        #For Cube
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

        # for p in range(4):
        #     connectPoints(p, (p+1) % 4, projectedPoints)
        #     connectPoints(p+4, ((p+1) % 4) + 4, projectedPoints)
        #     connectPoints(p, p+4, projectedPoints)

        #For Triangle
        # Connect the base points
        # connectPoints(0, 1, projectedPoints)
        # connectPoints(1, 2, projectedPoints)
        # connectPoints(2, 0, projectedPoints)

        # Connect each base point to the apex
        # connectPoints(0, 3, projectedPoints)
        # connectPoints(1, 3, projectedPoints)
        # connectPoints(2, 3, projectedPoints)

        # for p in range(3):
        #     connectPoints(p, (p+1) % 3, projectedPoints)
        #     connectPoints(p, 3, projectedPoints)

        #For Pyramid
        # connectPoints(0, 1, projectedPoints)  # Connect Base Point 1 to Base Point 2
        # connectPoints(1, 2, projectedPoints)  # Connect Base Point 2 to Base Point 3
        # connectPoints(2, 3, projectedPoints)  # Connect Base Point 3 to Base Point 4
        # connectPoints(3, 0, projectedPoints)  # Connect Base Point 4 to Base Point 1
        #
        # connectPoints(0, 4, projectedPoints)  # Connect Base Point 1 to Apex
        # connectPoints(1, 4, projectedPoints)  # Connect Base Point 2 to Apex
        # connectPoints(2, 4, projectedPoints)  # Connect Base Point 3 to Apex
        # connectPoints(3, 4, projectedPoints)  # Connect Base Point 4 to Apex

        # for p in range(4):
        #     connectPoints(p, (p+1) % 4, projectedPoints)
        #     connectPoints(p, 4, projectedPoints)


    pygame.display.update()