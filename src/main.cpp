#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
#include "SimpleGraph.h"

using std::cout;	using std::endl;
using std::string;  using std::getline;
using std::cin;     using std::istringstream;
using std::ifstream;
using std::cos;     using std::sin;
using std::vector;  using std::sqrt;
using std::pow;     using std::atan;
using std::atan2;   using std::cerr;

// Function and Struct Prototypes
struct force;
void welcome();
string getFileName();
void waitForEnter();
double getRunDuration();
SimpleGraph readGraphFile(string file_name);
void calculateRepulsion(vector<force>& net_forces, SimpleGraph& graph);
void calculateAttraction(vector<force>& net_forces, SimpleGraph& graph);
vector<force> initializeForceVector(SimpleGraph graph);
void moveNodes(vector<force>& net_forces, SimpleGraph& graph);
void repeat(bool& response);

// Global Constants
const double PI = 3.14159265358979323;

// Main method
int main() {
    welcome();

    while (true) {
        string file_name = getFileName();
        double duration = getRunDuration();
        SimpleGraph graph = readGraphFile(file_name);

        InitGraphVisualizer(graph);
        DrawGraph(graph);

        // iterate until user-defined time is reached
        time_t start_time = time(nullptr);
        while (difftime(time(nullptr), start_time) < duration) {
            vector<force> net_forces = initializeForceVector(graph);
            calculateRepulsion(net_forces, graph);
            calculateAttraction(net_forces, graph);
            moveNodes(net_forces, graph);
            DrawGraph(graph);
        }

        bool userInput;
        repeat(userInput);
        if (!userInput) {
            cout << "Thank you! Have a nice day!" << endl;
            break;
        }
    }
    return 0;
}

/* Prints a message to the console welcoming the user and
 * describing the program. */
void welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
}

/* Prompts user for a graph filename and reads it as a string.
 * Possible addition: data validation to make sure file name
 * exists in project directory. Reprompts of file doesn't exist. */
string getFileName() {
    while (true) {
        cout << "Please enter the graph file name you'd like to visualize." << endl;
        string file_name;
        getline(cin, file_name);
        ifstream input;
        input.open(file_name);
        if (input.is_open()) {
            return file_name;
        }
        cerr << "The file name does not exist. Please try again.\n" << endl;
    }
}

/* Prompts user for a runtime duration in seconds. */
double getRunDuration() {
    cout << "How many seconds would you like the algorithm to run for?" << endl;
    string duration_string;
    getline(cin, duration_string);
    istringstream input(duration_string);
    double duration;
    input >> duration;
    return duration;
}

/* Force class with x and y components. */
struct force {
    double x = 0;
    double y = 0;
};

/* Reads graph file into a SimpleGraph object. */
SimpleGraph readGraphFile(string file_name) {
    ifstream input;
    input.open(file_name);
    SimpleGraph graph;

    // intialize SimpleGraph with node positions
    size_t num_nodes;
    input >> num_nodes;
    for (size_t i = 0; i < num_nodes; i++) {
        graph.nodes.push_back({
            cos((2*PI*i)/num_nodes),
            sin((2*PI*i)/num_nodes)
        });
    }

    // initialize SimpleGraph struct with edges
    size_t node1;
    size_t node2;
    while (input >> node1 >> node2) {
        Edge edge;
        edge.start = node1;
        edge.end = node2;
        graph.edges.push_back(edge);
    }

    return graph;
}

/* Creates an empty compound vector that stores the forces
 * in the x and y directions for all the nodes. */
vector<force> initializeForceVector(SimpleGraph graph) {
    vector<force> net_forces;
    for (size_t i = 0; i < graph.nodes.size(); i++) {
        force new_force;
        net_forces.push_back(new_force);
    }
    return net_forces;
}

/* Calculates repulsive forces between all nodes and updates
 * net forces vector with repulsive forces. */
void calculateRepulsion(vector<force>& net_forces, SimpleGraph& graph) {
    const double k = 0.001;
    vector<Node> nodes = graph.nodes;
    for (size_t i = 0; i <= nodes.size() - 2; i++) {
        for (size_t j = i + 1; j <= nodes.size() - 1; j++) {
            double force = k / sqrt(pow(nodes[i].x - nodes[j].x, 2) +
                                    pow(nodes[i].y - nodes[j].y, 2));
            double theta = atan2((nodes[j].y - nodes[i].y),
                                (nodes[j].x - nodes[i].x));
            net_forces[i].x -= force * cos(theta);
            net_forces[i].y -= force * sin(theta);
            net_forces[j].x += force * cos(theta);
            net_forces[j].y += force * sin(theta);
        }
    }
}

/* Calculates attractive forces between nodes connected by
 * edges and updates net forces vector accordingly. */
void calculateAttraction(vector<force>& net_forces, SimpleGraph& graph) {
    const double k = 0.001;
    vector<Edge> edges = graph.edges;
    vector<Node> nodes = graph.nodes;

    // iterate through the edges
    for (size_t i = 0; i < edges.size(); i++) {
        size_t node1 = edges[i].start;
        size_t node2 = edges[i].end;

        // calculate force and angle between nodes
        double force = k * (pow(nodes[node1].x - nodes[node2].x, 2)
                            + pow(nodes[node1].y - nodes[node2].y, 2));
        double theta = atan2((nodes[node2].y - nodes[node1].y),
                            (nodes[node2].x - nodes[node1].x));

        // update net forces on nodes
        net_forces[node1].x += force * cos(theta);
        net_forces[node1].y += force * sin(theta);
        net_forces[node2].x -= force * cos(theta);
        net_forces[node2].y -= force * sin(theta);
    }
}

/* Moves the node positions of the graph struct by an
 * amount specified by the net force on the node */
void moveNodes(vector<force>& net_forces, SimpleGraph& graph) {
    for (size_t i = 0; i < graph.nodes.size(); i++) {
        graph.nodes[i].x += net_forces[i].x;
        graph.nodes[i].y += net_forces[i].y;
    }
}

/* Prompts user to see if (s)he wants to visualize another graph.
 * Reprompts if the user enters an illegal response. */
void repeat(bool& response) {
    string userInput;
    while(true) {
        cout << "Would you like to visualize another graph? (Y/N)" << endl;
        getline(cin, userInput);
        if(userInput.compare("Y") == 0 || userInput.compare("y") == 0) {
            response = true;
            break;
        } else if(userInput.compare("N") == 0 || userInput.compare("n") == 0) {
            response = false;
            break;
        } else {
            cerr << "Please type a valid response." << endl;
        }
    }
}
