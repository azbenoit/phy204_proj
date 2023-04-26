import numpy as np
import time

from numba import njit, prange

V = 100
L = 100
eps = 1e-5


def problem_init(grid):
    ly, lx = grid.shape
    grid[0, lx//2-1:] = np.linspace(V, 0, lx//2+1)


def problem_boundary(grid):
    ly, lx = grid.shape
    grid[0, :L-1] = grid[:0:-1, L-1]


def normal_compute_step(grid):
    return .25 * (grid[1:-1, 2:] + grid[1:-1, :-2]
                  + grid[2:, 1:-1] + grid[:-2, 1:-1])

V = 100
L = 100
eps = 1e-5

# @njit("f8[:,:]()", nogil=True, parallel=True, cache=True,fastmath=True)
# def compute_grid():
#     N = 0
#     err = eps+1

#     grid = np.zeros((L,2*L))
    
#     grid[0, L-1:] = np.linspace(V, 0, L+1)

#     while (err > eps):
#         N += 1
#         err = 0
#         for y in prange(1, L-1):
#             for x in prange(1, 2*L-1):
#                 val = grid[y, x]
#                 grid[y, x] = .25 * (grid[y, x+1] + grid[y, x-1]
#                                     + grid[y+1, x] + grid[y-1, x])

#                 err = max(err, abs(val - grid[y, x]))

#         grid[0, :L-1] = grid[:0:-1, L-1]
        

#     return grid

@njit("f8[:,:]()", nogil=True, cache=True, fastmath=True)
def compute_grid():
    L = 100
    V = 100
    eps=1e-5
    err=eps+1

    i = np.array([0,1])

    grids = [np.zeros((L, 2*L)), np.zeros((L, 2*L))]
    grids[i[0]][0, L-1:] = np.linspace(V, 0, L+1)
    # grids[i[0]][0, :L-1] = grids[i[0]][:0:-1, L-1]

    while (err>eps):

        i = i[::-1]

        # N+=1

        grids[i[0]][1:-1,1:-1] = 0.25*(grids[i[1]][2:,2:]+grids[i[1]][2:,:-2]+grids[i[1]][:-2,2:]+grids[i[1]][:-2,:-2])
        # grids[i[0]][0, L-1:] = np.linspace(V, 0, L+1)
        grids[i[0]][0, :L-1] = grids[i[0]][:0:-1, L-1]

        err = (grids[i[0]]-grids[i[1]]).max()

    # print(N)
    return grids[i[0]]

compute_grid()
start = time.time()
grid = compute_grid()
print(time.time() - start)
# print('Iterations: ', n)
