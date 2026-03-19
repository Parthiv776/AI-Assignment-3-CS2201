#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define GRID_SIZE 70

typedef struct {
    int r, c;
    int f, g, h;
    int parent_r, parent_c;
} Node;

int grid[GRID_SIZE][GRID_SIZE]; // 0 = Free, 1 = Obstacle
Node nodes[GRID_SIZE][GRID_SIZE];
bool closedList[GRID_SIZE][GRID_SIZE];
bool openListGrid[GRID_SIZE][GRID_SIZE];

// Directions: 8-way movement
int dr[] = {-1, 1, 0, 0, -1, -1, 1, 1};
int dc[] = {0, 0, -1, 1, -1, 1, -1, 1};

int calculateHValue(int row, int col, int dest_r, int dest_c) {
    // Chebyshev distance for 8-way grid
    return fmax(abs(row - dest_r), abs(col - dest_c)) * 10;
}

bool isValid(int r, int c) {
    return (r >= 0) && (r < GRID_SIZE) && (c >= 0) && (c < GRID_SIZE);
}

void generateObstacles(int density_percent) {
    int total_cells = GRID_SIZE * GRID_SIZE;
    int obstacles_to_place = (total_cells * density_percent) / 100;
    
    for (int i = 0; i < obstacles_to_place; i++) {
        int r = rand() % GRID_SIZE;
        int c = rand() % GRID_SIZE;
        grid[r][c] = 1;
    }
}

void aStarSearch(int start_r, int start_c, int goal_r, int goal_c) {
    if (!isValid(start_r, start_c) || !isValid(goal_r, goal_c) || grid[start_r][start_c] == 1 || grid[goal_r][goal_c] == 1) {
        printf("Invalid start or goal.\n");
        return;
    }

    // Initialize nodes
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            nodes[i][j].f = 1e9;
            nodes[i][j].g = 1e9;
            nodes[i][j].parent_r = -1;
            nodes[i][j].parent_c = -1;
            closedList[i][j] = false;
            openListGrid[i][j] = false;
        }
    }

    nodes[start_r][start_c].f = 0;
    nodes[start_r][start_c].g = 0;
    openListGrid[start_r][start_c] = true;

    int nodesExpanded = 0;
    bool foundDest = false;

    while (true) {
        int min_f = 1e9;
        int current_r = -1, current_c = -1;

        // Find node with min f in open list (Simple array implementation)
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                if (openListGrid[i][j] && nodes[i][j].f < min_f) {
                    min_f = nodes[i][j].f;
                    current_r = i; current_c = j;
                }
            }
        }

        if (current_r == -1) break; // Open list empty

        openListGrid[current_r][current_c] = false;
        closedList[current_r][current_c] = true;
        nodesExpanded++;

        if (current_r == goal_r && current_c == goal_c) {
            foundDest = true;
            break;
        }

        // Explore neighbors
        for (int i = 0; i < 8; i++) {
            int new_r = current_r + dr[i];
            int new_c = current_c + dc[i];

            if (isValid(new_r, new_c) && grid[new_r][new_c] == 0 && !closedList[new_r][new_c]) {
                int gNew = nodes[current_r][current_c].g + ((i < 4) ? 10 : 14); // 10 for straight, 14 for diagonal
                int hNew = calculateHValue(new_r, new_c, goal_r, goal_c);
                int fNew = gNew + hNew;

                if (nodes[new_r][new_c].f == 1e9 || nodes[new_r][new_c].f > fNew) {
                    openListGrid[new_r][new_c] = true;
                    nodes[new_r][new_c].f = fNew;
                    nodes[new_r][new_c].g = gNew;
                    nodes[new_r][new_c].parent_r = current_r;
                    nodes[new_r][new_c].parent_c = current_c;
                }
            }
        }
    }

    if (foundDest) {
        printf("Goal Reached!\n");
        printf("--- Measures of Effectiveness ---\n");
        printf("Total Path Cost: %d\n", nodes[goal_r][goal_c].g);
        printf("Nodes Expanded: %d out of %d\n", nodesExpanded, GRID_SIZE*GRID_SIZE);
    } else {
        printf("Failed to find a path.\n");
    }
}

int main() {
    srand(time(NULL));
    int start_r = 0, start_c = 0;
    int goal_r = 69, goal_c = 69;

    printf("Executing Static Environment (Medium Density 25%% Obstacles)...\n");
    generateObstacles(25); 
    grid[start_r][start_c] = 0; // Ensure start is free
    grid[goal_r][goal_c] = 0;   // Ensure goal is free

    clock_t begin = clock();
    aStarSearch(start_r, start_c, goal_r, goal_c);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Execution Time: %f seconds\n", time_spent);

    return 0;
}
