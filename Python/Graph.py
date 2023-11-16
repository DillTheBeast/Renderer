import pygame
import numpy as np

pygame.init()

WIDTH, HEIGHT = 1400, 800
pygame.display.set_caption("Graph")
screen = pygame.display.set_mode((WIDTH, HEIGHT))
font = pygame.font.Font(None, 36)

BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
BLUE = (0, 0, 255)
points = []

def connectPoints(i, j, points):
    pygame.draw.line(screen, BLUE, (points[i][0], points[i][1]), (points[j][0], points[j][1]), 3)

def drawGrid():
    pygame.draw.circle(screen, WHITE, (WIDTH // 2, HEIGHT // 2), 5)
    x = 0  
    y = 0 
    for i in range((1400 // 50) + 1):  
        x += 50  
        pygame.draw.line(screen, WHITE, (x, 0), (x, HEIGHT))
    for i in range((800 // 50) + 1):
        y += 50
        pygame.draw.line(screen, WHITE, (0, y), (WIDTH, y))

def inputText(display_text, initial_value=""):
    input_window = pygame.display.set_mode((400, 100))
    pygame.display.set_caption("Text Input")
    user_text = initial_value

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit()
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_RETURN:
                    return user_text
                elif event.key == pygame.K_BACKSPACE:
                    user_text = user_text[:-1]
                else:
                    user_text += event.unicode

        input_window.fill(BLACK)
        text_surface = font.render(display_text + user_text, True, WHITE)
        input_window.blit(text_surface, (10, 10))
        pygame.display.flip()

def findPoints():
    print("Use the line equation y = mx + b")
    neg_choice = int(inputText("If m is a negative number press 1. Otherwise click any other number"))
    m_num = int(inputText("What is m's numerator: "))
    m_den = int(inputText("What is m's denominator: "))
    b = int(inputText("What is b: "))
    points.append((WIDTH/2, b))

running = True
findPoints()
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                running = False

    screen.fill(BLACK)
    drawGrid()
    pygame.draw.circle(screen, WHITE, (int(points[0][0]), int(points[0][1])), 5)
    pygame.display.flip()

pygame.quit()
