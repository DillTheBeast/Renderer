import pygame
from pygame.locals import *
from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
import glm
import math
import numpy as np

# Load 3D model (replace 'sushi.obj' with the path to your model file)
model_path = 'bowl.obj'
model_vertices = []

# Scaling factor to make the model smaller
scale_factor = 0.5

with open(model_path, 'r', encoding='latin-1') as file:
    for line in file:
        if line.startswith('v '):
            vertices = line.split()[1:]
            model_vertices.append([float(v) for v in vertices])

# Apply scaling to the entire model
scaled_model_vertices = np.array(model_vertices) * scale_factor

# Initialize Pygame
pygame.init()

# Set desired OpenGL attributes
pygame.display.gl_set_attribute(pygame.GL_CONTEXT_MAJOR_VERSION, 3)
pygame.display.gl_set_attribute(pygame.GL_CONTEXT_MINOR_VERSION, 3)
pygame.display.gl_set_attribute(pygame.GL_CONTEXT_PROFILE_MASK, pygame.GL_CONTEXT_PROFILE_CORE)

# Try to create the screen with the OPENGL flag
try:
    screen = pygame.display.set_mode((800, 600), DOUBLEBUF | OPENGL)
except pygame.error as e:
    print(f"Error creating OpenGL context: {e}")
    pygame.quit()
    quit()

# Constants
FPS = 60

# Set up the OpenGL context
glEnable(GL_DEPTH_TEST)
glEnable(GL_CULL_FACE)

# Vertex and fragment shader code
vertex_shader = """
#version 330 core
layout(location = 0) in vec3 in_position;
uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;

void main()
{
    gl_Position = projection_matrix * modelview_matrix * vec4(in_position, 1.0);
}
"""

fragment_shader = """
#version 330 core
out vec4 fragColor;

void main()
{
    fragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
"""

# Compile shaders
shader_program = glCreateProgram()
vertex_shader_obj = glCreateShader(GL_VERTEX_SHADER)
fragment_shader_obj = glCreateShader(GL_FRAGMENT_SHADER)

glShaderSource(vertex_shader_obj, vertex_shader)
glCompileShader(vertex_shader_obj)
glAttachShader(shader_program, vertex_shader_obj)

glShaderSource(fragment_shader_obj, fragment_shader)
glCompileShader(fragment_shader_obj)
glAttachShader(shader_program, fragment_shader_obj)

glLinkProgram(shader_program)
glUseProgram(shader_program)

# Create VBO and VAO
vao = glGenVertexArrays(1)
glBindVertexArray(vao)

vbo = glGenBuffers(1)
glBindBuffer(GL_ARRAY_BUFFER, vbo)
glBufferData(GL_ARRAY_BUFFER, scaled_model_vertices, GL_STATIC_DRAW)

# Get the attribute location, enable it, and set the pointer
attribute_location = glGetAttribLocation(shader_program, "in_position")
glEnableVertexAttribArray(attribute_location)
glVertexAttribPointer(attribute_location, 3, GL_FLOAT, False, 0, None)

# Unbind VAO
glBindVertexArray(0)

# Projection matrix setup
projection_matrix_location = glGetUniformLocation(shader_program, "projection_matrix")
projection_matrix = glm.perspective(glm.radians(45.0), 800 / 600, 0.1, 100.0)
glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE, glm.value_ptr(projection_matrix))

# Modelview matrix setup
modelview_matrix_location = glGetUniformLocation(shader_program, "modelview_matrix")

# Camera parameters for top-down view
camera_pos = glm.vec3(0, 10, 0)
yaw, pitch = 0, -math.pi / 2  # Look straight down

clock = pygame.time.Clock()

# Main game loop
while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            quit()

    # Handle camera movement
    keys = pygame.key.get_pressed()
    speed = 0.1

    if keys[pygame.K_w]:
        camera_pos.z -= speed
    if keys[pygame.K_s]:
        camera_pos.z += speed
    if keys[pygame.K_q]:
        camera_pos.y -= speed
    if keys[pygame.K_e]:
        camera_pos.y += speed

    # Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

    # Modelview matrix setup
    modelview_matrix = glm.lookAt(camera_pos, glm.vec3(0, 0, 0), glm.vec3(0, 0, 1))
    glUniformMatrix4fv(modelview_matrix_location, 1, GL_FALSE, glm.value_ptr(modelview_matrix))

    # Draw the 3D model
    glBindVertexArray(vao)
    glDrawArrays(GL_TRIANGLES, 0, len(scaled_model_vertices))
    glBindVertexArray(0)

    # Update the display
    pygame.display.flip()

    # Cap the frame rate
    clock.tick(FPS)
