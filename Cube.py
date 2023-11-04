import numpy as np
import pygame

class Cube:
    def __init__(self):
        self.cubePoints = []
        self.cubeProjectedPoints = []
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

    def connectCubePoints(self, screen, projectedPoints, connectPoints):
        purple = (128, 0, 128)

        # Drawing filled polygons for each face
        for face in self.cubeFaces:
            points = [projectedPoints[i] for i in face]
            pygame.draw.polygon(screen, purple, points)

        # Drawing edges
        edges = [
            (0, 1), (1, 2), (2, 3), (3, 0),  # front face
            (4, 5), (5, 6), (6, 7), (7, 4),  # back face
            (0, 4), (1, 5), (2, 6), (3, 7)   # connecting edges
        ]

        for edge in edges:
            start_point = projectedPoints[edge[0]]
            end_point = projectedPoints[edge[1]]
            pygame.draw.line(screen, purple, start_point, end_point)
        
        for p in range(4):
            connectPoints(p, (p + 1) % 4, projectedPoints)
            connectPoints(p + 4, ((p + 1) % 4) + 4, projectedPoints)
            connectPoints(p, p + 4, projectedPoints)
