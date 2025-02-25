import numpy as np
import random
import matplotlib.pyplot as plt
import sys

def generate_maze(size):
    maze = -1 * np.ones((size, size), dtype=int)
    directions = [(0, 2), (0, -2), (2, 0), (-2, 0)]
    
    def carve_path(x, y):
        maze[x, y] = 0
        random.shuffle(directions)
        for dx, dy in directions:
            nx, ny = x + dx, y + dy
            if 0 < nx < size - 1 and 0 < ny < size - 1 and maze[nx, ny] == -1:
                maze[x + dx // 2, y + dy // 2] = 0
                carve_path(nx, ny)
    
    start_x, start_y = 1, 1
    maze[start_x, start_y] = 0
    carve_path(start_x, start_y)
    
    maze[0, 1] = 0
    maze[size - 1, size - 2] = 0
    return maze

size = random.choice(range(21, 52, 2))  # Generates only odd numbers
maze = generate_maze(size)
np.savetxt("maze.txt", maze, fmt="%d")

# Only display the maze if "--show" argument is provided
if "--show" in sys.argv:
    plt.figure(figsize=(6,6))
    plt.imshow(maze, cmap='gray', interpolation='nearest')
    plt.title(f"Generated {size}x{size} Maze")
    plt.axis("off")
    plt.show()
