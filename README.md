# AI-Assignment-3-CS2201
1)Dijkstras implementation for major indian cities 
For this problem, we represent the cities as nodes in a graph and the road distances as weighted edges in an Adjacency Matrix. Selected 7 major Indian cities with approximate driving distances in kilometers.
2)UGV Navigation
For a grid search space, Uniform-Cost Search expands in all directions equally (like a ripple). To make our UGV efficient, we upgrade this to A* (A-Star) Search. A* is a guided variant of Dijkstra's .We will use 3 density levels for obstacles: LOW (10%), MEDIUM (25%), and HIGH (40%).
3) UGV Navigation (Dynamic Obstacles)
The UGV calculates an optimal path, moves along it, and "senses" its immediate surroundings. If an unknown dynamic obstacle pops up on its planned route, it halts, marks the new obstacle on its internal map, and runs the A* search again from its current coordinates to the goal.
