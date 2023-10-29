# lst = [0, 1, 2, 3, 4, 5]

# for i in range(5):  # Since last element (5) doesn't need to be the starting point of a pair
#     if i % 5 == 0:  # Equivalent to checking if i == 0
#         print(lst[i], lst[i+1])
#         print(lst[i], lst[i+2])
#     else:
#         print(lst[i], lst[i+2])


for i in range(5):  # Since last element (5) doesn't need to be the starting point of a pair
    if i % 5 == 0:  # Equivalent to checking if i == 0
        print(i, i+1)
    print(i, i+2)
