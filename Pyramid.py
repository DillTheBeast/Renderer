import numpy as np

class Pyramid:
    def __init__(self):
        self.pyramidPoints = []
        self.pyramidProjectedPoints = [
            [n, n] for n in range(len(self.pyramidPoints))
        ]

    def appendPyramidPoints(self):
        self.pyramidPoints.append(np.matrix([-1, -1, 1]))  # Base Point 1
        self.pyramidPoints.append(np.matrix([1, -1, 1]))  # Base Point 2
        self.pyramidPoints.append(np.matrix([1, -1, -1]))  # Base Point 3
        self.pyramidPoints.append(np.matrix([-1, -1, -1]))  # Base Point 4
        self.pyramidPoints.append(np.matrix([0, 1, 0]))  # Apex

    def connectPyramidPoints(self, connectPoints, projectedPoints):
        for p in range(4):
            connectPoints(p, (p + 1) % 4, projectedPoints)
            connectPoints(p, 4, projectedPoints)
