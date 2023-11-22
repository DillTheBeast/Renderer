import pygame
import numpy as np

pygame.init()

#Defining pygame needed variables
WIDTH, HEIGHT = 1400, 800
pygame.display.set_caption("Graph")
screen = pygame.display.set_mode((WIDTH, HEIGHT))
font = pygame.font.Font(None, 36)

#Defining color codes
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
BLUE = (50, 120, 255)
IDK = (255, 120, 50)

#Array used to store all of the poins
#Going to be a 2d array b/c 1 for x and 1 for y axis
points = []

#Going to make lines for the equations
def connectPoints(i, j, points, COLOR):
    #Defining 2 points for each line
    x1, y1 = points[i]
    x2, y2 = points[j]

    # Draw the line within the screen
    pygame.draw.line(screen, COLOR, (x1, y1), (x2, y2), 3)

    # Calculate the slope and y-intercept
    m = (y2 - y1) / (x2 - x1) if (x2 - x1) != 0 else float('inf')
    b = y1 - m * x1

    # Calculate the extended line points
    extended_x1 = -WIDTH
    extended_y1 = m * extended_x1 + b
    extended_x2 = 2 * WIDTH
    extended_y2 = m * extended_x2 + b

    # Draw the extended line
    pygame.draw.line(screen, COLOR, (extended_x1, extended_y1), (extended_x2, extended_y2), 3)

#Function to make the grid lines + origin point
def drawGrid():
    #Drawing origin point
    pygame.draw.circle(screen, WHITE, (WIDTH // 2, HEIGHT // 2), 5)
    x = 0  
    y = 0
    #Making x + y grid lines
    #Each line is 50 pixels apart on x + y axis
    for i in range((1400 // 50) + 1):  
        x += 50  
        pygame.draw.line(screen, WHITE, (x, 0), (x, HEIGHT))
    for i in range((800 // 50) + 1):
        y += 50
        pygame.draw.line(screen, WHITE, (0, y), (WIDTH, y))

def wrap_text(text, font, max_width):
    lines = []
    words = text.split()
    current_line = words[0]

    for word in words[1:]:
        test_line = current_line + " " + word
        test_width, _ = font.size(test_line)
        if test_width <= max_width:
            current_line = test_line
        else:
            lines.append(current_line)
            current_line = word

    lines.append(current_line)
    return lines

def inputText(display_text, initial_value=""):
    input_window = pygame.display.set_mode((1400, 800))
    pygame.display.set_caption("Graph Calculator")
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

        input_window.fill(BLACK)  # Clear the window each frame

        # Wrap the text to a new line if it exceeds the width
        wrapped_lines = wrap_text(display_text + user_text, font, WIDTH - 20)

        # Draw the wrapped lines
        for i, line in enumerate(wrapped_lines):
            text_surface = font.render(line, True, WHITE)
            input_window.blit(text_surface, (10, 10 + i * font.get_linesize()))

        pygame.display.flip()

def findPoints():
    bP = 0
    mDenP1 = 0
    mDenP2 = 0
    mDenP3 = 0
    mDenP4 = 0
    mNumP1 = HEIGHT / 2 - bP
    mNumP2 = HEIGHT / 2 - bP
    mNumP3 = HEIGHT / 2 - bP
    mNumP4 = HEIGHT / 2 - bP
    #Stuff needs to be a little different with neg slope
    negChoice = int(inputText("If m is a negative number press 1. Otherwise click any other number: \n"))
    mNum = int(inputText("What is m's numerator: \n"))
    mDen = int(inputText("What is m's denominator: \n"))
    bAns = int(inputText("What is b: \n"))

    if negChoice == 1:
        print("Test")

    if bAns > 0:
        #Making y intercept
        for i in range(bAns):
            bP += 50
        #Making the numerator + denominator points
        for i in range(mNum):
            mNumP1 -= 50
            mNumP2 += 50
            mNumP3 -= 50
            mNumP4 += 50
        for i in range(mDen):
            mDenP1 += 50
            mDenP2 -= 50
            mDenP3 += 50
            mDenP4 -= 50
        #Adding all of the points created
        points.append((WIDTH / 2, HEIGHT / 2 - bP))
        points.append((WIDTH/2 + mDenP1, mNumP1))
        points.append((WIDTH/2 + mDenP2, mNumP2))
        points.append((WIDTH/2 + mDenP3, mNumP3))
        points.append((WIDTH/2 + mDenP4, mNumP4))


running = True
#Making the amount of line equations there will be
for i in range(2):
    findPoints()

#Main loop
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                running = False

    #Setting everything up
    screen.fill(BLACK)
    drawGrid()
    for i in range(9):
        if i > 4: pygame.draw.circle(screen, BLUE, (int(points[i][0]), int(points[i][1])), 8)
        else: pygame.draw.circle(screen, IDK, (int(points[i][0]), int(points[i][1])), 8)
    connectPoints(3, 4, points, IDK)
    connectPoints(8, 9, points, BLUE)
    pygame.display.flip()


pygame.quit()
