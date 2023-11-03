import numpy as np
import pygame

class Cube:
    def __init__(self):
        self.cubePoints = []
        self.cubeProjectedPoints = [
            [n, n] for n in range(len(self.cubePoints))
        ]
        self.cubeFaces = [
            [0, 1, 2, 3],  # front face
            [4, 5, 6, 7],  # back face
            [0, 1, 5, 4],  # bottom face
            [3, 2, 6, 7],  # top face
            [0, 3, 7, 4],  # left face
            [1, 2, 6, 5]   # right face
        ]


    def appendCubePoints(self):
        self.cubePoints.append(np.array([-1, -1, 1]))
        self.cubePoints.append(np.array([1, -1, 1]))
        self.cubePoints.append(np.array([1, 1, 1]))
        self.cubePoints.append(np.array([-1, 1, 1]))
        self.cubePoints.append(np.array([-1, -1, -1]))
        self.cubePoints.append(np.array([1, -1, -1]))
        self.cubePoints.append(np.array([1, 1, -1]))
        self.cubePoints.append(np.array([-1, 1, -1]))
    
    def connectCubePoints(self, screen, connectPoints, projectedPoints):
        purple = (128, 0, 128)

        # Drawing filled polygons for each face
        for face in self.cubeFaces:
            points = [projectedPoints[i] for i in face]
            pygame.draw.polygon(screen, purple, points)

        # Drawing edges
        for p in range(4):
            connectPoints(p, (p + 1) % 4, projectedPoints)
            connectPoints(p + 4, ((p + 1) % 4) + 4, projectedPoints)
            connectPoints(p, p + 4, projectedPoints)

