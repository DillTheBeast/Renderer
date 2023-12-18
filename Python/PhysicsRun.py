import pygame
import numpy as np
from math import *
from Cube import Cube

pygame.init()

paused = False
forward = False
cube = True

WIDTH, HEIGHT = 1440, 845
pygame.display.set_caption("3d Renderer")
screen = pygame.display.set_mode((WIDTH, HEIGHT))

scale = 96
circle_pos = [WIDTH / 2, HEIGHT / 2]
angle = 0
angleAddition = 0.01
gravity = 0.5

Cube = Cube()
Cube.appendCubePoints()

projection_matrix = np.matrix([
    [1, 0, 0],
    [0, 1, 0]
])

rotation_z = np.matrix([
    [cos(angle), -sin(angle), 0],
    [sin(angle), cos(angle), 0],
    [0, 0, 1]
])

rotation_y = np.matrix([
    [cos(angle), 0, sin(angle)],
    [0, 1, 0],
    [-sin(angle), 0, cos(angle)]
])

rotation_x = np.matrix([
    [1, 0, 0],
    [0, cos(angle), -sin(angle)],
    [0, sin(angle), cos(angle)]
])

def connect_points(i, j, points):
    pygame.draw.line(screen, (255, 255, 255), (points[i][0], points[i][1]), (points[j][0], points[j][1]))

def movement(points):
    projected_points = []
    for point in points:
        rotated_2d = np.dot(rotation_z, point.reshape((3, 1)))
        rotated_2d = np.dot(rotation_x, rotated_2d)
        rotated_2d = np.dot(rotation_y, rotated_2d)

        projected_2d = np.dot(projection_matrix, rotated_2d)

        x = int(projected_2d[0, 0] * scale) + circle_pos[0]
        y = int(projected_2d[1, 0] * scale) + circle_pos[1]

        pygame.draw.circle(screen, (255, 255, 255), (x, y), 5)
        projected_points.append([x, y])

    # Connecting the points
    for i in range(len(projected_points)):
        connect_points(i, (i + 1) % len(projected_points), projected_points)

def directCube(points, keys):
    pressed = False
    before_x, before_y = circle_pos[0], circle_pos[1]

    if keys[pygame.K_w]:
        circle_pos[1] -= 5
        pressed = True
    if keys[pygame.K_s]:
        circle_pos[1] += 5
        pressed = True
    if keys[pygame.K_a]:
        circle_pos[0] -= 5
        pressed = True
    if keys[pygame.K_d]:
        circle_pos[0] += 5
        pressed = True

    if pressed:
        # Handle boundary checks
        if circle_pos[1] >= HEIGHT - scale or circle_pos[1] <= 0 + scale:
            circle_pos[1] = before_y
        if circle_pos[0] >= WIDTH - scale or circle_pos[0] <= 0 + scale:
            circle_pos[0] = before_x

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

    keys = pygame.key.get_pressed()
    directCube(Cube.cubePoints, keys)
    screen.fill((0, 0, 0))
    movement(Cube.cubePoints)

    pygame.display.update()
