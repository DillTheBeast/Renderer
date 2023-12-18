import pygame
import numpy as np
import math
import time
import random

# Initialize Pygame
pygame.init()

# Set up display
width, height = 800, 600
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption('DVD')

# Set up colors
black = (0, 0, 0)
white = (255, 255, 255)
green = (0, 255, 0)
blue = (0, 0, 255)
red = (255, 0, 0)
colors = [white, blue, green, red]
randomColor = random.choice(colors)
print(random)
scale = 0.5
speed = 1
forward = True
down = True

# Draw oval
center_x, center_y = width // 2, height // 2
radius_x, radius_y = 100, 50  # Adjust these values for the desired oval shape

# Create points for the ellipse using numpy and math
theta = np.linspace(0, 2 * math.pi, 100)
x = center_x + radius_x * np.cos(theta) * scale
y = center_y + radius_y * np.sin(theta) * scale

# Main loop
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Clear the screen
    screen.fill(black)

    if (x >= 800).any() or (x <= 0).any():
        forward = not forward
        randomColor = random.choice(colors)

    if (y >= 600).any() or (y <= 0).any():
        down = not down
        randomColor = random.choice(colors)

    if forward:
        x += speed 
    else:
        x -= speed

    if down:
        y -= speed
    else:
        y += speed

    # Convert points to integer values
    points = list(zip(x.astype(int), y.astype(int)))

    # Draw the ellipse
    pygame.draw.polygon(screen, randomColor, points, 1)

    # Update the display
    pygame.display.flip()


    time.sleep(0.01)

# Quit Pygame
pygame.quit()
