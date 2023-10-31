import numpy as np

class Cube:
    def __init__(self):
        self.cubePoints = []

    def appendCubePoints(self):
        self.cubePoints.append(np.array([-1, -1, 1]))
        self.cubePoints.append(np.array([1, -1, 1]))
        self.cubePoints.append(np.array([1, 1, 1]))
        self.cubePoints.append(np.array([-1, 1, 1]))
        self.cubePoints.append(np.array([-1, -1, -1]))
        self.cubePoints.append(np.array([1, -1, -1]))
        self.cubePoints.append(np.array([1, 1, -1]))
        self.cubePoints.append(np.array([-1, 1, -1]))
    
    def connectCubePoints(self, connectPoints, projectedPoints):
        for p in range(4):
            connectPoints(p, (p + 1) % 4, projectedPoints)
            connectPoints(p + 4, ((p + 1) % 4) + 4, projectedPoints)
            connectPoints(p, p + 4, projectedPoints)
