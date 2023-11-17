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

input_text = ""

def connectPoints(i, j, points):
    pygame.draw.line(screen, BLUE, (points[i][0], points[i][1]), (points[j][0], points[j][1]), 3)

def drawGrid():
    pygame.draw.circle(screen, WHITE, (WIDTH // 2, HEIGHT // 2), 5)
    x = 0
    y = 0
    for i in range((WIDTH // 50) + 1):
        x += 50
        pygame.draw.line(screen, WHITE, (x, 0), (x, HEIGHT))
    for i in range((HEIGHT // 50) + 1):
        y += 50
        pygame.draw.line(screen, WHITE, (0, y), (WIDTH, y))

def findPoints():
    global input_text
    displayText("Use the line equation y = mx + b", (10, 10))
    displayText("If m is a negative number, press 1. Otherwise, press any other number", (10, 50))
    displayText("Press Enter to submit", (10, 90))
    displayText("Input: " + input_text, (10, 130))

def processInput():
    global input_text, points
    # Process the input and update the points list
    # You need to implement this part based on your requirements
    # For example, you can parse the input_text and update points accordingly
    pass

def displayText(text, position):
    text_surface = font.render(text, True, WHITE)
    screen.blit(text_surface, position)

running = True
findPoints()

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                running = False
            elif event.key == pygame.K_RETURN:
                # Process the input when Enter is pressed
                try:
                    processInput()
                except ValueError:
                    print("Invalid input. Please try again.")
                input_text = ""
            elif event.key == pygame.K_BACKSPACE:
                # Handle backspace to delete characters
                input_text = input_text[:-1]
            else:
                # Add the pressed key to the input text
                input_text += event.unicode

    screen.fill(BLACK)
    drawGrid()

    if points:  # Check if the points list is not empty
        pygame.draw.circle(screen, WHITE, (int(points[0][0]), int(points[0][1])), 5)

    displayText("Press ESC to exit", (10, HEIGHT - 40))
    findPoints()
    pygame.display.flip()

pygame.quit()
