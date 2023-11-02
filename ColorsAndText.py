import pygame

pygame.init()

class ColorsAndText:
    
    def __init__(self):
        self.texts = []
        self.buttonText = (255, 87, 51)
        self.buttonShell = (1, 50, 32)
        self.smallfont = pygame.font.SysFont('Corbel', 35)
        self.BLACK = (0, 0, 0)
        self.WHITE = (255, 255, 255)
        self.RED = (255, 0, 0)
        self.GREEN = (0, 255, 0)
        self.BLUE = (0, 0, 255)
        self.GREY = (192,192,192)
        self.PINK = (255,182,193)
        self.CYAN = (0,255,255)
        self.ORANGE = (255,165,0)
        self.PURPLE = (186,85,211)
        self.x = 0
        self.y = 0
        self.backgroundColor = self.BLACK
        self.shapeColor = self.WHITE
        self.runButton = runButton

    def addColors(self):
        # Rendering headers + button texts
        self.runButton = self.smallfont.render('Run', True, self.buttonShell)
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
        if 20 <= self.x <= 100:
            if 60 <= self.y <= 80:
                #Black
                self.backgroundColor = self.BLACK
            elif 100 <= self.y <= 115:
                #White
                self.backgroundColor = self.WHITE
            elif 135 <= self.y <= 150:
                #Blue
                self.backgroundColor = self.BLUE
            elif 170 <= self.y <= 185:
                #Red
                self.backgroundColor = self.RED
            elif 205 <= self.y <= 220:
                #Green
                self.backgroundColor = self.GREEN
            elif 240 <= self.y <= 255:
                #Grey
                self.backgroundColor = self.GREY
            elif 275 <= self.y <= 290:
                #Pink
                self.backgroundColor = self.PINK
            elif 310 <= self.y <= 325:
                #Cyan
                self.backgroundColor = self.CYAN
            elif 345 <= self.y <= 360:
                #Orange
                self.backgroundColor = self.ORANGE
            elif 380 <= self.y <= 395:
                #Purple
                self.backgroundColor = self.PURPLE

            #Now onto shape colors
            elif 490 <= self.y <= 505:
                #Black2
                self.shapeColor = self.BLACK
            elif 525 <= self.y <= 540:
                #White2
                self.shapeColor = self.WHITE
            elif 560 <= self.y <= 575:
                #Blue2
                self.shapeColor = self.BLUE
            elif 595 <= self.y <= 610:
                #Red2
                self.shapeColor = self.RED
            elif 630 <= self.y <= 645:
                #Green2
                self.shapeColor = self.GREEN
            elif 665 <= self.y <= 680:
                #Grey2
                self.shapeColor = self.GREY
            elif 700 <= self.y <= 715:
                #Pink2
                self.shapeColor = self.PINK
            elif 735 <= self.y <= 750:
                #Cyan2
                self.shapeColor = self.CYAN
            elif 770 <= self.y <= 785:
                #Orange2
                self.shapeColor = self.ORANGE
            elif 805 <= self.y <= 820:
                #Purple2
                self.shapeColor = self.PURPLE
