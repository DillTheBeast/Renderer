import turtle

screen = turtle.Screen()
t = turtle.Turtle()
t.speed(100)

# Move the turtle to the desired position
x_position = 100
y_position = 100
t.hideturtle()
t.penup()  # Lift the pen
t.goto(x_position, y_position)  # Move to the desired position
t.pendown()  # Put the pen down

# Draw a dot
dot_size = 10  # Adjust the size of the dot as needed
t.dot(dot_size)

# Keep the window open until it's closed by the user
turtle.done()