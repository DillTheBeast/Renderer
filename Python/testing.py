import pygame
import sys
import math

# Initialize Pygame
pygame.init()

# Constants
WIDTH, HEIGHT = 800, 600
FPS = 60

# Colors
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)

# Create the screen
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Simple 3D Renderer")
clock = pygame.time.Clock()

scale = 0.5

# Define a cube with three coordinates for each vertex
vertices = [
    [-1 * scale, -1 * scale, -1 * scale],
    [-1 * scale, 1 * scale, -1 * scale],
    [1 * scale, 1 * scale, -1 * scale],
    [1 * scale, -1 * scale, -1 * scale],
    [-1 * scale, -1 * scale, 1 * scale],
    [-1 * scale, 1 * scale, 1 * scale],
    [1 * scale, 1 * scale, 1 * scale],
    [1 * scale, -1 * scale, 1 * scale]
]

# Define edges of the cube
edges = [
    (0, 1), (1, 2), (2, 3), (3, 0),
    (4, 5), (5, 6), (6, 7), (7, 4),
    (0, 4), (1, 5), (2, 6), (3, 7)
]

# Camera parameters
camera_pos = [0, 0, -5]  # Initial camera position
yaw = 0  # Yaw angle (rotation around the y-axis)
pitch = 0  # Pitch angle (rotation around the x-axis)

# Main game loop
while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

    # Handle camera movement and rotation
    keys = pygame.key.get_pressed()
    speed = 0.1
    rotation_speed = 0.02

    if keys[pygame.K_w]:
        camera_pos[2] -= speed * math.cos(yaw)
        camera_pos[0] += speed * math.sin(yaw)
        camera_pos[1] += speed * math.sin(pitch)

    if keys[pygame.K_s]:
        camera_pos[2] += speed * math.cos(yaw)
        camera_pos[0] -= speed * math.sin(yaw)
        camera_pos[1] -= speed * math.sin(pitch)

    if keys[pygame.K_a]:
        yaw += rotation_speed

    if keys[pygame.K_d]:
        yaw -= rotation_speed

    if keys[pygame.K_q]:
        camera_pos[1] -= speed

    if keys[pygame.K_e]:
        camera_pos[1] += speed

    if keys[pygame.K_UP]:
        pitch += rotation_speed

    if keys[pygame.K_DOWN]:
        pitch -= rotation_speed

    pitch = max(-math.pi / 2, min(math.pi / 2, pitch))

    # Clear the screen
    screen.fill(BLACK)

    # Define the view matrix (camera transformation)
    view_matrix = [
        [math.cos(yaw), math.sin(pitch) * math.sin(yaw), -math.cos(pitch) * math.sin(yaw), camera_pos[0]],
        [0, math.cos(pitch), math.sin(pitch), camera_pos[1]],
        [math.sin(yaw), -math.sin(pitch) * math.cos(yaw), math.cos(pitch) * math.cos(yaw), camera_pos[2]],
        [0, 0, 0, 1]
    ]

    # Define the perspective projection matrix
    fov = 500
    projection_matrix = [
        [2 * fov / WIDTH, 0, 0, 0],
        [0, -2 * fov / HEIGHT, 0, 0],
        [0, 0, -1, 0]
    ]

    # Project and draw the cube
    for edge in edges:
        start = vertices[edge[0]]
        end = vertices[edge[1]]

        # Apply view matrix
        start_view = [
            start[0] * view_matrix[0][0] + start[1] * view_matrix[0][1] + start[2] * view_matrix[0][2] + view_matrix[0][3],
            start[0] * view_matrix[1][0] + start[1] * view_matrix[1][1] + start[2] * view_matrix[1][2] + view_matrix[1][3],
            start[0] * view_matrix[2][0] + start[1] * view_matrix[2][1] + start[2] * view_matrix[2][2] + view_matrix[2][3]
        ]
        end_view = [
            end[0] * view_matrix[0][0] + end[1] * view_matrix[0][1] + end[2] * view_matrix[0][2] + view_matrix[0][3],
            end[0] * view_matrix[1][0] + end[1] * view_matrix[1][1] + end[2] * view_matrix[1][2] + view_matrix[1][3],
            end[0] * view_matrix[2][0] + end[1] * view_matrix[2][1] + end[2] * view_matrix[2][2] + view_matrix[2][3]
        ]

        # Apply perspective projection
        start_projected = [
            start_view[0] * projection_matrix[0][0] + start_view[1] * projection_matrix[0][1] + start_view[2] * projection_matrix[0][2] + projection_matrix[0][3],
            start_view[0] * projection_matrix[1][0] + start_view[1] * projection_matrix[1][1] + start_view[2] * projection_matrix[1][2] + projection_matrix[1][3],
            start_view[0] * projection_matrix[2][0] + start_view[1] * projection_matrix[2][1] + start_view[2] * projection_matrix[2][2] + projection_matrix[2][3]
        ]
        end_projected = [
            end_view[0] * projection_matrix[0][0] + end_view[1] * projection_matrix[0][1] + end_view[2] * projection_matrix[0][2] + projection_matrix[0][3],
            end_view[0] * projection_matrix[1][0] + end_view[1] * projection_matrix[1][1] + end_view[2] * projection_matrix[1][2] + projection_matrix[1][3],
            end_view[0] * projection_matrix[2][0] + end_view[1] * projection_matrix[2][1] + end_view[2] * projection_matrix[2][2] + projection_matrix[2][3]
        ]

        # Perspective divide
        start_projected = [start_projected[0] / start_projected[2], start_projected[1] / start_projected[2]]
        end_projected = [end_projected[0] / end_projected[2], end_projected[1] / end_projected[2]]

        # Scale and translate
        start_screen = (
            int(start_projected[0] * WIDTH / 2 + WIDTH / 2),
            int(-start_projected[1] * HEIGHT / 2 + HEIGHT / 2)
        )
        end_screen = (
            int(end_projected[0] * WIDTH / 2 + WIDTH / 2),
            int(-end_projected[1] * HEIGHT / 2 + HEIGHT / 2)
        )
        # Print camera position for debugging
        print("Camera Position:", camera_pos)

        # Draw the line
        pygame.draw.line(screen, WHITE, start_screen, end_screen, 2)

    # Update the display
    pygame.display.flip()

    # Cap the frame rate
    clock.tick(FPS)
