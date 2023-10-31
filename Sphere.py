import numpy as np

class Sphere:
    def __init__(self):
        self.spherePoints = []

    def appendSpherePoints(self):
        self.cubePoints.append(np.array([-1, -1, 1]))
        self.cubePoints.append(np.array([1, -1, 1]))
        self.cubePoints.append(np.array([1, 1, 1]))
        self.cubePoints.append(np.array([-1, 1, 1]))
        self.cubePoints.append(np.array([-1, -1, -1]))
        self.cubePoints.append(np.array([1, -1, -1]))
        self.cubePoints.append(np.array([1, 1, -1]))
        self.cubePoints.append(np.array([-1, 1, -1]))

    def connectSpherePoints(self, connectPoints, projectedPoints):
        for p in range(4):
            connectPoints(p, (p + 1) % 4, projectedPoints)
            connectPoints(p + 4, ((p + 1) % 4) + 4, projectedPoints)
            connectPoints(p, p + 4, projectedPoints)
