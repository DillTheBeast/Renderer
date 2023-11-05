import numpy as np

class HexPrism:
    def __init__(self):
        self.HexPrismPoints = []
        self.HexPrismProjectedPoints = [
            [n, n] for n in range(len(self.HexPrismPoints))
        ]

    def appendHexPrismPoints(self):
        h = 2  # assuming the height of the prism is 2 units
        # Top hexagon
        self.HexPrismPoints.append(np.matrix([np.sqrt(3)/2, 0.5, h/2]))
        self.HexPrismPoints.append(np.matrix([np.sqrt(3)/2, -0.5, h/2]))
        self.HexPrismPoints.append(np.matrix([0, 1, h/2]))
        self.HexPrismPoints.append(np.matrix([0, -1, h/2]))
        self.HexPrismPoints.append(np.matrix([-np.sqrt(3)/2, 0.5, h/2]))
        self.HexPrismPoints.append(np.matrix([-np.sqrt(3)/2, -0.5, h/2]))

        # Bottom hexagon
        self.HexPrismPoints.append(np.matrix([np.sqrt(3)/2, 0.5, -h/2]))
        self.HexPrismPoints.append(np.matrix([np.sqrt(3)/2, -0.5, -h/2]))
        self.HexPrismPoints.append(np.matrix([0, 1, -h/2]))
        self.HexPrismPoints.append(np.matrix([0, -1, -h/2]))
        self.HexPrismPoints.append(np.matrix([-np.sqrt(3)/2, 0.5, -h/2]))
        self.HexPrismPoints.append(np.matrix([-np.sqrt(3)/2, -0.5, -h/2]))

    def connectHexPrismPoints(self, connectPoints, projectedPoints):
        for p in range(0, 11):
            if p != 5 and p != 10 and p != 4:
                connectPoints(p, p + 2, projectedPoints)
            if p == 6 or p == 0 or p == 10 or p == 4:
                connectPoints(p, p + 1, projectedPoints)
            if p < 6:
                connectPoints(p, p + 6, projectedPoints)
