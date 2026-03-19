#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define V 7

// City indices
enum Cities { DELHI, MUMBAI, CHENNAI, KOLKATA, HYDERABAD, BANGALORE, PUNE };
const char* cityNames[] = {"Delhi", "Mumbai", "Chennai", "Kolkata", "Hyderabad", "Bangalore", "Pune"};

// Find the vertex with minimum distance value
int minDistance(int dist[], bool sptSet[]) {
    int min = INT_MAX, min_index;
    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;
    return min_index;
}

// Function to print the shortest path
void printPath(int parent[], int j) {
    if (parent[j] == -1) return;
    printPath(parent, parent[j]);
    printf(" -> %s", cityNames[j]);
}

// Dijkstra's Algorithm
void dijkstra(int graph[V][V], int src, int dest) {
    int dist[V];     
    bool sptSet[V];  
    int parent[V];   

    for (int i = 0; i < V; i++) {
        parent[0] = -1;
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }

    dist[src] = 0;
    parent[src] = -1;

    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, sptSet);
        sptSet[u] = true;

        for (int v = 0; v < V; v++)
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]) {
                parent[v] = u;
                dist[v] = dist[u] + graph[u][v];
            }
    }

    printf("Shortest Path from %s to %s:\n", cityNames[src], cityNames[dest]);
    printf("Distance: %d km\n", dist[dest]);
    printf("Path: %s", cityNames[src]);
    printPath(parent, dest);
    printf("\n");
}

int main() {
    int graph[V][V] = {
        /* DEL, MUM, CHE, KOL, HYD, BLR, PUN */
        { 0,   1400, 2200, 1500, 1500, 2100, 1450 }, // DELHI
        { 1400, 0,   1330, 1900, 710,  980,  150  }, // MUMBAI
        { 2200, 1330, 0,   1670, 630,  350,  1180 }, // CHENNAI
        { 1500, 1900, 1670, 0,   1500, 1860, 1750 }, // KOLKATA
        { 1500, 710,  630,  1500, 0,   570,  590  }, // HYDERABAD
        { 2100, 980,  350,  1860, 570,  0,   840  }, // BANGALORE
        { 1450, 150,  1180, 1750, 590,  840,  0   }  // PUNE
    };

    dijkstra(graph, DELHI, BANGALORE);
    return 0;
}
