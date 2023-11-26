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

    def changeColors(self):
        print(self.x, self.y)
        if 20 <= self.x <= 100 and self.y < 845 / 2:
            category = "Background Color"
        elif 20 <= self.x <= 100 and self.y > 845 / 2:
            category = "Shape Color"
        else:
            return  # Ignore clicks outside color categories

        color_index = (self.y - 60) // 35  # Calculate color index based on y-coordinate

        if category == "Background Color":
            if 0 <= color_index < len(self.texts):
                self.backgroundColor = self.getColorByIndex(color_index)
        elif category == "Shape Color":
            if 0 <= color_index < len(self.texts):
                self.shapeColor = self.getColorByIndex(color_index)

    def getColorByIndex(self, index):
        colors = [self.BLACK, self.WHITE, self.BLUE, self.RED, self.GREEN, self.GREY, self.PINK, self.CYAN, self.ORANGE, self.PURPLE]
        return colors[index]