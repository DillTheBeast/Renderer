from random import randint
from star import Star
import turtle

num_stars = 100
stars = [Star() for _ in range(num_stars)]


# for star in stars:
#     print(f"Star coordinates: x={star.x}, y={star.y}, z={star.z}")

def setup():
    screen = turtle.Screen()
    screen.setup(400, 400)
    screen.bgcolor("black")

    for i in range(len(stars)):
        stars[i].add(Star(), i)

    circle = turtle.Turtle()
    circle.shape("circle")
    circle.color("dark red")


    screen.mainloop()


setup()