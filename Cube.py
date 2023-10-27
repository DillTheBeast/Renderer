import cv2
import numpy as np

# Window dimensions
WIDTH, HEIGHT = 800, 600

# Create a white image (3 channels for RGB with all values 255 for white)
img = np.ones((HEIGHT, WIDTH, 3), dtype=np.uint8) * 255

#Makes square
# Set those pixel positions to black
#for x in range(10, 500):
#    for y in range(10, 500):
#        img[y, x] = [0, 0, 0]  # [B, G, R] because OpenCV uses BGR format

#Makes Cube
for y in range(50, 500):
    for x in range(10, 500):
        if (y > 480 or y < 70) or (x > 480 or x < 30):
            img[y, x] = [0, 0, 0]

#Makes top left angled lines
x = 10
count = 10
for i in range(20):
    for y in range(50, 1, -1):
        img[y, x] = [0,0,0]
        x += 1
    count += 1
    x = count

#Makes top right angled lines
x = 500
count = 500-20
for i in range(20):
    for y in range(50, 1, -1):
        img[y, x] = [0,0,0]
        x += 1
    count += 1
    x = count

#Makes top connecting lines
for i in range(2, 20):
    for x in range(70-(10+i), 500+31-i):
        img[i, x] = [0, 0, 0]

#Makes side connecting lines
for i in range(500+30, 500+50):
    for y in range(2, 451):
        img[y, i] = [0, 0, 0]

#Makes bottom right angled lines
x = 500
count = 500-20
for i in range(20):
    for y in range(500, 450, -1):
        img[y, x] = [0,0,0]
        x += 1
    count += 1
    x = count

# Keep the window open until a key press
cv2.imshow('Window', img)
cv2.waitKey(1)
# Removes Square
# for x in range(499, 9, -1):
#     for y in range(499, 9, -1):
#         img[y, x] = [255, 255, 255]  # [B, G, R] because OpenCV uses BGR format
#     cv2.imshow('Window', img)
#     cv2.waitKey(1)
cv2.waitKey(0)
cv2.destroyAllWindows()
