import pyglet
from pyglet.gl import *
import numpy as np
import ctypes

model_path = "bowl.obj"

# Load OBJ file manually
vertices = []
faces = []

with open(model_path, 'r', encoding='latin-1') as obj_file:
    for line in obj_file:
        if line.startswith('v '):
            vertices.append(list(map(float, line.split()[1:])))
        elif line.startswith('f '):
            faces.append([int(vertex.split('/')[0]) - 1 for vertex in line.split()[1:]])

# Flatten the faces list
flat_faces = [vertex for face in faces for vertex in face]

# Convert to NumPy arrays
vertices_np = np.array(vertices, dtype=np.float32)
faces_np = np.array(flat_faces, dtype=np.uint32)

# Set up Pyglet window
window = pyglet.window.Window()

# Enable depth testing
glEnable(GL_DEPTH_TEST)

# Create VBO (Vertex Buffer Object)
vbo = GLuint()
glGenBuffers(1, ctypes.byref(vbo))
glBindBuffer(GL_ARRAY_BUFFER, vbo)
glBufferData(GL_ARRAY_BUFFER, vertices_np.nbytes, vertices_np.ctypes.data_as(ctypes.POINTER(GLfloat)), GL_STATIC_DRAW)

# Create IBO (Index Buffer Object)
ibo = GLuint()
glGenBuffers(1, ctypes.byref(ibo))
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo)
glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces_np.nbytes, faces_np.ctypes.data_as(ctypes.POINTER(GLuint)), GL_STATIC_DRAW)

# Create VAO (Vertex Array Object)
vao = GLuint()
glGenVertexArrays(1, ctypes.byref(vao))
glBindVertexArray(vao)
glEnableVertexAttribArray(0)
glBindBuffer(GL_ARRAY_BUFFER, vbo)
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, ctypes.c_void_p(0))
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo)
glBindVertexArray(0)

@window.event
def on_draw():
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

    glBindVertexArray(vao)
    glDrawElements(GL_TRIANGLES, len(flat_faces), GL_UNSIGNED_INT, ctypes.c_void_p(0))
    glBindVertexArray(0)

pyglet.app.run()
