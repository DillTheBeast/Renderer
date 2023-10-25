import turtle

screen = turtle.Screen()
t = turtle.Turtle()
t.speed(100)

t.hideturtle()
t.penup()  # Lift the pen
t.goto(300, 300)  # Move to the desired position
t.pendown()  # Put the pen down

# Draw a dot
dot_size = 5  # Adjust the size of the dot as needed
t.dot(dot_size)


t.goto(-300, 300)  # Move to the desired position

# Draw a dot
dot_size = 5  # Adjust the size of the dot as needed
t.dot(dot_size)

t.goto(0, 0)

# Draw a dot
dot_size = 5  # Adjust the size of the dot as needed
t.dot(dot_size)

t.goto(300, 300)

# Keep the window open until it's closed by the user
turtle.done()