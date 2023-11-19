import pygame
from pygame.locals import *
from OpenGL.GL import *
from OpenGL.GLUT import *
import os

# Function to load a texture from a file
def load_texture(file_path):
    texture_surface = pygame.image.load(file_path)
    texture_data = pygame.image.tostring(texture_surface, "RGBA", 1)
    width, height = texture_surface.get_size()

    texture_id = glGenTextures(1)
    glBindTexture(GL_TEXTURE_2D, texture_id)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data)

    return texture_id

# Define the vertices and indices for a cube
vertices = (
    (1, -1, -1),
    (1, 1, -1),
    (-1, 1, -1),
    (-1, -1, -1),
    (1, -1, 1),
    (1, 1, 1),
    (-1, -1, 1),
    (-1, 1, 1)
)

edges = (
    (0, 1),
    (1, 2),
    (2, 3),
    (3, 0),
    (4, 5),
    (5, 6),
    (6, 7),
    (7, 4),
    (0, 4),
    (1, 5),
    (2, 6),
    (3, 7)
)

# Define the texture file paths for each side of the cube
texture_files = [
    "front.png",
    "back.png",
    "right.png",
    "left.png",
    "top.png",
    "bottom.png"
]

# Initialize Pygame
pygame.init()

# Set the window size
window_size = (800, 600)
pygame.display.set_mode(window_size, DOUBLEBUF | OPENGL)

# Load textures
textures = [load_texture(os.path.join("path/to/your/textures", file)) for file in texture_files]

# Set up the perspective
gluPerspective(45, (window_size[0] / window_size[1]), 0.1, 50.0)
glTranslatef(0.0, 0.0, -5)

# Main loop
rotation_angle = 0
while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            quit()

    # Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

    # Rotate the cube
    glRotatef(rotation_angle, 1, 1, 0)
    rotation_angle += 1

    # Draw the cube with textures
    for i in range(6):
        glBindTexture(GL_TEXTURE_2D, textures[i])
        glBegin(GL_QUADS)
        for vertex in edges:
            for vertex_index in vertex:
                glVertex3fv(vertices[vertex_index])
        glEnd()

    # Update the display
    pygame.display.flip()
    pygame.time.wait(10)
