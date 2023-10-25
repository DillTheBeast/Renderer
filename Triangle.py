from PIL import Image, ImageDraw

# Create a blank white image
img = Image.new('RGB', (800, 600), color='white')

# Use the ImageDraw module to fill the image
draw = ImageDraw.Draw(img)
for x in range(-200, 200):
    for y in range(-200, 200):
        draw.point((x + 400, y + 300), fill='red')  # Adjusting for center

img.show()
