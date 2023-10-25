from PIL import Image, ImageDraw
import time

# Create a blank white image
img = Image.new('RGB', (800, 600), color='white')

# Use the ImageDraw module to fill the image
draw = ImageDraw.Draw(img)

#Square
for x in range(100, 500):
    for y in range(100, 500):
        draw.point((x,y), fill='black')
img.show()
time.sleep(3)
for x in range(100, 500):
    for y in range(100, 500):
        draw.point((x,y), fill='white')
        time.sleep(1)
#Right Triangle
# for x in range(100, 500):
#     for y in range(100, x):
#         draw.point((x, y), fill='black')

