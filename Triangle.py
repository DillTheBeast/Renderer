import numpy as np

class Triangle:
    def __init__(self):
        self.trianglePoints = []

    def appendTrianglePoints(self):
        self.trianglePoints.append(np.matrix([-1, -1, 1]))  # Base Point 1
        self.trianglePoints.append(np.matrix([1, -1, 1]))  # Base Point 2
        self.trianglePoints.append(np.matrix([0, -1, -1]))  # Base Point 3
        self.trianglePoints.append(np.matrix([0, 1, 0]))  # Apex

    def connectTrianglePoints(self, connectPoints, projectedPoints):
        for p in range(3):
            connectPoints(p, (p + 1) % 3, projectedPoints)
            connectPoints(p, 3, projectedPoints)
