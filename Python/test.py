import pygame
import sys

pygame.init()

WIDTH, HEIGHT = 1400, 800
pygame.display.set_caption("Graph")
screen = pygame.display.set_mode((WIDTH, HEIGHT))
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
BLUE = (0, 0, 255)

# Font and text size
font = pygame.font.Font(None, 36)

class TextBox:
    def __init__(self, x, y, width, height):
        self.rect = pygame.Rect(x, y, width, height)
        self.text = ''
        self.color = WHITE

    def handle_event(self, event):
        if event.type == pygame.MOUSEBUTTONDOWN:
            if self.rect.collidepoint(event.pos):
                self.active = not self.active
            else:
                self.active = False
            self.color = WHITE if not self.active else (200, 200, 200)
        if event.type == pygame.KEYDOWN and self.active:
            if event.key == pygame.K_RETURN:
                self.active = False
            elif event.key == pygame.K_BACKSPACE:
                self.text = self.text[:-1]
            else:
                self.text += event.unicode

    def draw(self, screen):
        pygame.draw.rect(screen, self.color, self.rect, 2)
        text_surface = font.render(self.text, True, WHITE)
        width = max(200, text_surface.get_width()+10)
        self.rect.w = width
        screen.blit(text_surface, (self.rect.x+5, self.rect.y+5))

# Create textboxes
textbox_m_numerator = TextBox(50, HEIGHT - 80, 100, 30)
textbox_m_denominator = TextBox(200, HEIGHT - 80, 100, 30)
textbox_b = TextBox(350, HEIGHT - 80, 100, 30)

textboxes = [textbox_m_numerator, textbox_m_denominator, textbox_b]

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

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        for textbox in textboxes:
            textbox.handle_event(event)

    screen.fill(BLACK)
    drawGrid()

    for textbox in textboxes:
        textbox.draw(screen)

    pygame.display.flip()

pygame.quit()
