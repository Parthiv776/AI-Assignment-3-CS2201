#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define GRID_SIZE 70

// Re-using the Node struct and A* logic from 2)
typedef struct { int f, g, parent_r, parent_c; } Node;
int grid[GRID_SIZE][GRID_SIZE]; 
Node nodes[GRID_SIZE][GRID_SIZE];
bool closedList[GRID_SIZE][GRID_SIZE];
bool openListGrid[GRID_SIZE][GRID_SIZE];

int dr[] = {-1, 1, 0, 0, -1, -1, 1, 1};
int dc[] = {0, 0, -1, 1, -1, 1, -1, 1};

int calcH(int r, int c, int dr, int dc_g) { return fmax(abs(r - dr), abs(c - dc_g)) * 10; }
bool isValid(int r, int c) { return (r >= 0) && (r < GRID_SIZE) && (c >= 0) && (c < GRID_SIZE); }

// Modified A* that returns a boolean if a path exists
bool findPath(int start_r, int start_c, int goal_r, int goal_c) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            nodes[i][j].f = 1e9; nodes[i][j].g = 1e9;
            closedList[i][j] = false; openListGrid[i][j] = false;
        }
    }
    nodes[start_r][start_c].f = 0; nodes[start_r][start_c].g = 0;
    openListGrid[start_r][start_c] = true;

    while (true) {
        int min_f = 1e9, cr = -1, cc = -1;
        for (int i = 0; i < GRID_SIZE; i++)
            for (int j = 0; j < GRID_SIZE; j++)
                if (openListGrid[i][j] && nodes[i][j].f < min_f) { min_f = nodes[i][j].f; cr = i; cc = j; }

        if (cr == -1) return false; 
        openListGrid[cr][cc] = false; closedList[cr][cc] = true;

        if (cr == goal_r && cc == goal_c) return true;

        for (int i = 0; i < 8; i++) {
            int nr = cr + dr[i], nc = cc + dc[i];
            if (isValid(nr, nc) && grid[nr][nc] == 0 && !closedList[nr][nc]) {
                int gNew = nodes[cr][cc].g + ((i < 4) ? 10 : 14);
                int fNew = gNew + calcH(nr, nc, goal_r, goal_c);
                if (nodes[nr][nc].f > fNew) {
                    openListGrid[nr][nc] = true;
                    nodes[nr][nc].f = fNew; nodes[nr][nc].g = gNew;
                    nodes[nr][nc].parent_r = cr; nodes[nr][nc].parent_c = cc;
                }
            }
        }
    }
}

// Extract the next step to take from the planned path
void getNextStep(int start_r, int start_c, int goal_r, int goal_c, int *next_r, int *next_c) {
    int curr_r = goal_r, curr_c = goal_c;
    while (nodes[curr_r][curr_c].parent_r != start_r || nodes[curr_r][curr_c].parent_c != start_c) {
        int pr = nodes[curr_r][curr_c].parent_r;
        int pc = nodes[curr_r][curr_c].parent_c;
        curr_r = pr; curr_c = pc;
    }
    *next_r = curr_r; *next_c = curr_c;
}

int main() {
    srand(time(NULL));
    int u_r = 0, u_c = 0;       // UGV Current Position
    int goal_r = 69, goal_c = 69; // Goal
    int replans = 0;

    // Start with empty grid, obstacles will be discovered dynamically
    for(int i=0; i<GRID_SIZE; i++) for(int j=0; j<GRID_SIZE; j++) grid[i][j] = 0;

    printf("UGV Starting Dynamic Navigation...\n");

    while (u_r != goal_r || u_c != goal_c) {
        if (!findPath(u_r, u_c, goal_r, goal_c)) {
            printf("Target unreachable due to dynamic obstacles.\n");
            return 0;
        }

        int next_r, next_c;
        getNextStep(u_r, u_c, goal_r, goal_c, &next_r, &next_c);

        // Simulate Dynamic Obstacle occurring in front of the UGV (5% chance per step)
        if (rand() % 100 < 5 && !(next_r == goal_r && next_c == goal_c)) {
            printf("Dynamic obstacle detected at (%d, %d)! Re-planning...\n", next_r, next_c);
            grid[next_r][next_c] = 1; // Mark block
            replans++;
            continue; // Skip moving
        }

        // Move UGV
        u_r = next_r;
        u_c = next_c;
    }

    printf("Dynamic Navigation Complete!\n");
    printf("Total Re-calculations Required: %d\n", replans);
    return 0;
}
