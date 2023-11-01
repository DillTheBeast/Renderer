import numpy as np

class Sphere:
    def __init__(self):
        self.spherePoints = []
        self.sphereProjectedPoints = [
            [n, n] for n in range(len(self.spherePoints))
        ]

    def appendSpherePoints(self):
        radius = 1
        resolution = 20
        num_points = 1000
        theta_values = np.linspace(0, np.pi, resolution)
        phi_values = np.linspace(0, 2*np.pi, 2*resolution)

        for theta in theta_values:
            for phi in phi_values:
                x = radius * np.sin(theta) * np.cos(phi)
                y = radius * np.sin(theta) * np.sin(phi)
                z = radius * np.cos(theta)
                self.spherePoints.append(np.matrix([x, y, z]))
