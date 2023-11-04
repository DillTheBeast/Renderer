import numpy as np
import pygame
from math import degrees

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

        # Load the texture
        self.texture = pygame.image.load('texture.jpeg')  # Replace with the actual path of your texture file

        # Create surfaces for each face
        self.face_surfaces = []
        for _ in self.cubeFaces:
            self.face_surfaces.append(pygame.Surface((self.texture.get_width(), self.texture.get_height()), pygame.SRCALPHA))
            self.face_surfaces[-1].blit(self.texture, (0, 0))

    def appendCubePoints(self):
        self.cubePoints.append(np.array([-1, -1, 1]))
        self.cubePoints.append(np.array([1, -1, 1]))
        self.cubePoints.append(np.array([1, 1, 1]))
        self.cubePoints.append(np.array([-1, 1, 1]))
        self.cubePoints.append(np.array([-1, -1, -1]))
        self.cubePoints.append(np.array([1, -1, -1]))
        self.cubePoints.append(np.array([1, 1, -1]))
        self.cubePoints.append(np.array([-1, 1, -1]))

    def connectCubePoints(self, screen, connectPoints, projectedPoints, angle):
        for i, face in enumerate(self.cubeFaces):
            points = [projectedPoints[i] for i in face]

            # Apply the same rotation to the texture coordinates
            rotated_texture = pygame.transform.rotate(self.face_surfaces[i], -degrees(angle))

            # Calculate the position for blitting the rotated texture
            bounding_rect = rotated_texture.get_rect()

            # Calculate the center of the face
            face_center = pygame.Vector2(sum(points[j][0] for j in range(4)) / 4,
                                        sum(points[j][1] for j in range(4)) / 4)

            # Adjust the bounding rectangle position to center the texture
            bounding_rect.center = face_center

            # Get the width and height of the face for scaling
            square_width = abs(points[1][0] - points[0][0])
            square_height = abs(points[3][1] - points[0][1])

            # Make sure the dimensions are valid before scaling
            if square_width > 0 and square_height > 0:
                scaled_texture = pygame.transform.scale(rotated_texture, (square_width, square_height))

                # Adjust the position of the texture to center it
                texture_rect = scaled_texture.get_rect(center=face_center)

                screen.blit(scaled_texture, texture_rect.topleft)

            for p in range(4):
                connectPoints(p, (p + 1) % 4, projectedPoints)
                connectPoints(p + 4, ((p + 1) % 4) + 4, projectedPoints)
                connectPoints(p, p + 4, projectedPoints)