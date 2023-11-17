import pygame
import numpy as np

pygame.init()

WIDTH, HEIGHT = 1400, 800
pygame.display.set_caption("Graph")
screen = pygame.display.set_mode((WIDTH, HEIGHT))
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
BLUE = (0, 0, 255)
points = []

def connectPoints(i, j, points):
    pygame.draw.line(screen, BLUE, (points[i][0], points[i][1]), (points[j][0], points[j][1]), 3)

def drawGrid(points):
    pygame.draw.circle(screen, WHITE, (WIDTH // 2, HEIGHT // 2), 5)
    
    for point in points:
        pygame.draw.circle(screen, WHITE, (int(point[0]), int(point[1])), 5)
    
    x = 0
    y = 0
    for i in range((1400 // 50) + 1):
        x += 50
        pygame.draw.line(screen, WHITE, (x, 0), (x, HEIGHT))
        
    for i in range((800 // 50) + 1):
        y += 50
        pygame.draw.line(screen, WHITE, (0, y), (WIDTH, y))

def findPoints():
    print("Use the line equation y = mx + b")
    negChoice = int(input("If m is a negative number press 1. Otherwise click any other number\n"))
    mNum = int(input("What is m's numerator:\n"))
    mDen = int(input("What is m's denominator:\n"))
    b = int(input("What is b: "))
    points.append((0, b))

running = True

findPoints()  # Call findPoints before entering the main loop

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                running = False

    screen.fill(BLACK)
    drawGrid(points)
    pygame.display.flip()

pygame.quit()
