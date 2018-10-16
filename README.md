# Graph Visualization
A force-directed layout algorithm to more easily and intuitively see complex nodal relationships in a graph. It reads in data about nodes and edges from a file, initializes their position in a circle in a graphical window, calculates attractive and repulsive forces according to the Fruchterman-Reingold algorithm, and updates nodal positions according to each node's net forces.

## Background
Graphs are defined as having nodes (or vertices) and edges that connect them. They can be used to show simple and complex relationships between objects. For example, Facebook uses graphs to represent people and their innterrelationships as a graph and the Google search engine is powered by the PageRank algorithm, which treats webpages as nodes and their links to each other as edges. These relationships can be incredibly sophisticated and hard to visualize. Thus we seek a way to intuitively visualize these graphs so that we can make more informed decisions in industry.

![Bad Graph](/images/logo.png)
![Good Graph](/images/logo.png)

## Implementation
Though there are many ways of approaching this problem, there are two ad hoc rules that work reasonably well:
1. Place connected nodes near one another.
2. Place disconnected nodes far from one another.

### Force Calculations
We tackle this problem by calculating attractive and repulsive forces between the nodes in the graph and using the net forces to update each node's position. This achieves both heuristic goals of putting connected nodes as close as possible and putting disconnected nodes as far away as possible. We use the **Fruchterman-Reingold algorithm** to calculate these forces where the attractive force is inversely related to the square of the nodal distance and the repulsive forces is inversely related to the nodal distance.

Note: There exists a repulsive force between every node and every other node in the graph but there only exists attractive forces between nodes connected with an edge.

### Component Dissection
Upon calculating the magnitude of the force between two nodes, it's necessary to dissect the force into its x and y components and update the nodal positions accordingly. This is done by finding the angle—theta—between the force vector and the horizontal with the *atan2* C++ STL library function. Then the force vector is multiplied by *cos(theta)* and *sin(theta)* to update the x and y node positions, respectively.

## Possible Extensions
1. Possible adding masses and velocities to all the nodes, so there would be a concept of "momentum" for the nodes and it would be less likely to be stuck at a local minimum for this optimization problem.
2. Adding random perturbations to the nodes at ever reevaluation of the forces to prevent getting stuck at a local optimum.
3. Penalize crossing edges because these make the graph relationships hard to decipher.

## Comments
This project was extremely insightful for me because I got to work with some graphics visualization for the first time and to think about what goes behind the scenes in these sorts of programs. It was also vaguely reminiscent of some machine learning gradient descent techniques where the goal is to optimize some sort of relationship and prevent getting stuck at local optima. Definitely looking forward to learning more about momentum gradient descent in future classes!

## Credits
This project was part of CS 106L (Standard C++ Programming) taught by Ali Malik in 2018 at Stanford University.
