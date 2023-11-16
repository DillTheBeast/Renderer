import pygame
import sys

# Initialize Pygame
pygame.init()

# Set up window dimensions
window_width = 400
window_height = 300

# Create two separate windows
window1 = pygame.display.set_mode((window_width, window_height))
window2 = pygame.display.set_mode((window_width, window_height))

# Set window titles
pygame.display.set_caption("Window 1")
pygame.display.set_caption("Window 2")

# Set up colors
white = (255, 255, 255)

# Main game loop
while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

    # Fill the windows with white color
    window1.fill(white)
    window2.fill(white)

    # Update the display for each window
    pygame.display.update()

# Note: This code will still not behave exactly as expected. 
# Pygame allows only one window at a time to have focus, 
# so you may need to handle user input differently for each window.
