import pygame
from pygame.locals import *
from OpenGL.GL import *
from OpenGL.GLU import *
import glm
import math
import numpy
import ctypes

# Load 3D model (replace 'your_model.obj' with the path to your model file)
model_path = 'sushi.obj'
model_vertices = []
model_faces = []

# Scaling factor to make the model smaller
scale_factor = 0.5  # You can adjust this value based on how much you want to scale the model

with open(model_path, 'r', encoding='latin-1') as file:
    for line in file:
        if line.startswith('v '):
            vertices = line.split()[1:]
            model_vertices.append([float(v) for v in vertices])
        elif line.startswith('f '):
            faces = line.split()[1:]
            model_faces.append([int(f.split('/')[0]) - 1 for f in faces])

# Apply scaling to the entire model
scaled_model_vertices = [[v[0] * scale_factor, v[1] * scale_factor, v[2] * scale_factor] for v in model_vertices]

WIDTH, HEIGHT = 800, 600

pygame.init()
pygame.display.set_mode((WIDTH, HEIGHT), DOUBLEBUF | OPENGL | pygame.OPENGL_COMPATBLT)


# Constants
FPS = 60

# Colors
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)

# Create the screen
screen = pygame.display.set_mode((WIDTH, HEIGHT), DOUBLEBUF | OPENGL)
pygame.display.set_caption("Simple 3D Model Viewer")
clock = pygame.time.Clock()

# Camera parameters
camera_pos = glm.vec3(0, 0, 5)  # Initial camera position
yaw = 0  # Yaw angle (rotation around the y-axis)
pitch = 0  # Pitch angle (rotation around the x-axis)

# OpenGL setup
glEnable(GL_DEPTH_TEST)
glEnable(GL_CULL_FACE)

# Perspective setup
glMatrixMode(GL_PROJECTION)
gluPerspective(45, (WIDTH / HEIGHT), 0.1, 100.0)
glMatrixMode(GL_MODELVIEW)
glTranslatef(0.0, 0.0, -5)

# After loading the model, create VBO and VAO
vbo = glGenBuffers(1)
glBindBuffer(GL_ARRAY_BUFFER, vbo)
glBufferData(GL_ARRAY_BUFFER, numpy.array(scaled_model_vertices, dtype='float32'), GL_STATIC_DRAW)

vao = glGenVertexArrays(1)
glBindVertexArray(vao)
glEnableVertexAttribArray(0)
glVertexAttribPointer(0, 3, GL_FLOAT, False, 0, ctypes.c_void_p(0))

# Main game loop
while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            quit()

    # Handle camera movement and rotation
    keys = pygame.key.get_pressed()
    speed = 0.1
    rotation_speed = 0.02

    if keys[pygame.K_w]:
        camera_pos.z -= speed * glm.cos(yaw)
        camera_pos.x += speed * glm.sin(yaw)
        camera_pos.y += speed * glm.sin(pitch)

    if keys[pygame.K_s]:
        camera_pos.z += speed * glm.cos(yaw)
        camera_pos.x -= speed * glm.sin(yaw)
        camera_pos.y -= speed * glm.sin(pitch)

    if keys[pygame.K_a]:
        yaw += rotation_speed

    if keys[pygame.K_d]:
        yaw -= rotation_speed

    if keys[pygame.K_q]:
        camera_pos.y -= speed

    if keys[pygame.K_e]:
        camera_pos.y += speed

    if keys[pygame.K_UP]:
        pitch += rotation_speed

    if keys[pygame.K_DOWN]:
        pitch -= rotation_speed

    pitch = max(-math.pi / 2, min(math.pi / 2, pitch))

    # Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

    # Load the identity matrix to reset transformations
    glLoadIdentity()

    # Apply camera transformations
    gluLookAt(camera_pos.x, camera_pos.y, camera_pos.z,
              camera_pos.x + glm.sin(yaw), camera_pos.y + glm.sin(pitch), camera_pos.z - glm.cos(yaw),
              0, 1, 0)

    # Draw the 3D model
    glBindVertexArray(vao)
    glDrawArrays(GL_TRIANGLES, 0, len(scaled_model_vertices))
    glBindVertexArray(0)

    # Update the display
    pygame.display.flip()

    # Cap the frame rate
    clock.tick(FPS)
