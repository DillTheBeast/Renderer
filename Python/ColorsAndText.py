import pygame

class ColorsAndText:
    def __init__(self):
        self.falling = False
        self.texts = []
        self.buttonText = (255, 87, 51)
        self.smallfont = pygame.font.SysFont('Corbel', 35)
        self.BLACK = (0, 0, 0)
        self.WHITE = (255, 255, 255)
        self.RED = (255, 0, 0)
        self.GREEN = (0, 255, 0)
        self.BLUE = (0, 0, 255)
        self.GREY = (192, 192, 192)
        self.PINK = (255, 182, 193)
        self.CYAN = (0, 255, 255)
        self.ORANGE = (255, 165, 0)
        self.PURPLE = (186, 85, 211)
        self.x = 0
        self.y = 0
        self.backgroundColor = self.BLACK
        self.shapeColor = self.WHITE
        self.runButton = self.smallfont.render('Run', True, self.buttonText)

    def addColors(self):
        # Rendering headers + button texts
        self.header1 = self.smallfont.render('Background Color', True, self.buttonText)
        self.texts.append(self.header1)
        color1 = self.smallfont.render('Black', True, self.buttonText)
        self.texts.append(color1)
        color2 = self.smallfont.render('White', True, self.buttonText)
        self.texts.append(color2)
        color3 = self.smallfont.render('Blue', True, self.buttonText)
        self.texts.append(color3)
        color4 = self.smallfont.render('Red', True, self.buttonText)
        self.texts.append(color4)
        color5 = self.smallfont.render('Green', True, self.buttonText)
        self.texts.append(color5)
        color6 = self.smallfont.render('Grey', True, self.buttonText)
        self.texts.append(color6)
        color7 = self.smallfont.render('Pink', True, self.buttonText)
        self.texts.append(color7)
        color8 = self.smallfont.render('Cyan', True, self.buttonText)
        self.texts.append(color8)
        color9 = self.smallfont.render('Orange', True, self.buttonText)
        self.texts.append(color9)
        color10 = self.smallfont.render('Purple', True, self.buttonText)
        self.texts.append(color10)

        self.header2 = self.smallfont.render('Shape Color', True, self.buttonText)
        backgroundColor = self.BLACK
        shapeColor = self.WHITE

    def changeColors(self):
        WIDTH, HEIGHT = 1440, 845
        if 20 <= self.x <= 100:
            if 60 <= self.y <= 80:
                # Black
                self.backgroundColor = self.BLACK
            elif 100 <= self.y <= 115:
                # White
                self.backgroundColor = self.WHITE
            # ... (continue for other colors)
            elif 805 <= self.y <= 820:
                # Purple
                self.backgroundColor = self.PURPLE
        elif 1440 - 55 <= self.x <= 1440:
            if 10 <= self.y <= 30:
                self.falling = not self.falling
                if self.falling:
                    print("Falling")
                else:
                    print("No")
