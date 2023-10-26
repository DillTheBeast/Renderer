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
        img[y, x] = [0, 0, 0]

x = 11
count = 11
for i in range(20):
    for y in range(49, 1, -1):
        img[y, x] = [0,0,0]
        x += 1
    count += 1
    x = count

x = 11
count = 11
for i in range(20):
    for y in range(49, 1, -1):
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
