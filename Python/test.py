import pygame
import pygame_menu

pygame.init()

WIDTH, HEIGHT = 1400, 800
pygame.display.set_caption("Graph")
screen = pygame.display.set_mode((WIDTH, HEIGHT))
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

def findPoints():
    menu = pygame_menu.Menu('Input Menu', WIDTH, HEIGHT, theme=pygame_menu.themes.THEME_SOLARIZED)

    m_num = menu.add_input('Enter m numerator: ', default='1', input_type=pygame_menu.locals.INPUT_INT)
    m_den = menu.add_input('Enter m denominator: ', default='1', input_type=pygame_menu.locals.INPUT_INT)
    b = menu.add_input('Enter b: ', default='0', input_type=pygame_menu.locals.INPUT_INT)

    menu.add_button('Submit', lambda: points.append((0, int(b.get_value()))))
    menu.mainloop(screen)

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                running = False

    screen.fill(BLACK)
    drawGrid()
    findPoints()
    pygame.display.flip()

pygame.quit()
