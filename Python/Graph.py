import pygame
import numpy as np
from math import *

pygame.init()

WIDTH, HEIGHT = 1440, 845
pygame.display.set_caption("Graph")
screen = pygame.display.set_mode((WIDTH, HEIGHT))

def connectPoints(i, j, points):
    pygame.draw.line(screen, ColorsAndText.shapeColor, (points[i][0], points[i][1]), (points[j][0], points[j][1]))