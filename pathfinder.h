/*******************
**Caroline Kistler**
**Semester Project**
*****Pathfinder*****
********************
********************/


#pragma once
#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <set>
#include <stack>
using namespace std;

//for each point of the maze
struct Node {
	int x, y;
	char character;
	int distance;
	int manhattanDistance;
	vector<Node> neighbors;
	Node* parent;
	int priority;

	//initalizing node to be empty
	Node() {
		x = 0;
		y = 0;
		character = NULL;
		distance = 0;
		manhattanDistance = 0;
		parent = NULL;
	}

	//making it  a pointer to ensure values are held true
	Node(Node* temp) {
		x = temp->x;
		y = temp->y;
		character = temp->character;
		distance = temp->distance;
		manhattanDistance = temp->manhattanDistance;
		neighbors = temp->neighbors;
		parent = temp->parent;
	}

	//to compare distances of the Nodes
	bool operator<(const Node& rhs) const {
		if (distance != rhs.distance) {
			return distance < rhs.distance;
		} else if (y != rhs.y) {
			return y < rhs.y;
		} else {
			return x < rhs.x;
		}
	}

	bool operator()(const Node& lhs) {
		return ((x == lhs.x) && (y == lhs.y));
	}
};


struct nodeEqual {
	int x;
	int y;

	nodeEqual(int x1, int y1) : x(x1), y(y1) {}

	bool operator () (const Node& n) const {
		return n.x == x && n.y == y;
	}
};


class pathfind {
private:
	int r, c;
	Node start;
	Node end;
	vector<vector<Node>> gridOfNodes;
	set<Node> visited;
	set<Node> unvisited;

public:
	template<std::size_t rows, std::size_t cols>
	vector<char> pathfinder(char(&map)[rows][cols]);

	void getManhattanDistance();
	vector<Node> getNeighbors(Node current);
	vector<char> getPath(bool);
	char getDirectionCharacters(Node, Node);
	int manhattanDistance(Node);
	int getRows();
	int getColumns();
};


//actual function called
template<std::size_t rows, std::size_t cols>
vector<char> pathfinder(char(&map)[rows][cols]) {
	pathfind p;
	vector<char> directions;
	directions = p.pathfinder(map);
	for (auto &it : directions) {
		cout << it;
	}
	return directions;
}

template<std::size_t rows, std::size_t cols>
vector<char> pathfind::pathfinder(char(&map)[rows][cols]) {
	r = rows;
	c = cols;
	Node node;
	vector<Node> point;
	//goes through every point in grid
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			node.x = j;
			node.y = i;
			node.character = map[i][j];
			//initializes each distance depending on what the character is
			if (map[i][j] == 's') {
				node.distance = 0;
				//initialize the starting node location
				start = node;
			} else if (map[i][j] == 'w') {
				node.distance = numeric_limits<int>::max();
			} else if (map[i][j] == 'e') {
				node.distance = numeric_limits<int>::max();
				//go to empty space before a bomb but after a gold
				node.priority = 1;
			} else if (map[i][j] == 'g') {
				node.distance = numeric_limits<int>::max();
				//go to a gold space before any other
				node.priority = 2;
			} else if (map[i][j] == 'b') {
				node.distance = numeric_limits<int>::max();
				//go to a bomb if only have to
				node.priority = 0;
			} else if (map[i][j] == 'f') {
				node.distance = numeric_limits<int>::max();
				//initialize the ending node location
				end = node;
			}
			//puts all the nodes into a vector<vector<Nodes>> to create maze
			point.push_back(node);
			if (j + 1 >= cols) {
				gridOfNodes.push_back(point);
			}
		}
		//creates a new rows of Nodes
		point.clear();
	}
	vector<char> path = getPath(false);
	if (path.at(0) == '\0') {
		path = getPath(true);
	}
	return path;
}

//heuristic to help find quickest distance to finish
//gets the distance for each node
void pathfind::getManhattanDistance() {
	for (int i = 0; i < getRows(); i++) {
		for (int j = 0; j < getColumns(); j++) {
			int distance = manhattanDistance(gridOfNodes[i][j]);
			gridOfNodes[i][j].manhattanDistance = distance;
		}
	}
}

//gets the all neighbors for the node passed in
//returns all neighbors in a vector
vector<Node> pathfind::getNeighbors(Node current) {
	vector<Node> neighbors;
	Node neighbor;
	int left = current.x - 1;
	int right = current.x + 1;
	int up = current.y + 1;
	int down = current.y - 1;
	//deals with bounds checking
	if (left >= 0 && left < getColumns()) {
		neighbor = gridOfNodes[current.y][left];
		neighbors.push_back(neighbor);
	}
	if (right >= 0 && right < getColumns()) {
		neighbor = gridOfNodes[current.y][right];;
		neighbors.push_back(neighbor);
	}
	if (up >= 0 && up < getRows()) {
		neighbor = gridOfNodes[up][current.x];
		neighbors.push_back(neighbor);
	}
	if (down >= 0 && down < getRows()) {
		neighbor = gridOfNodes[down][current.x];
		neighbors.push_back(neighbor);
	}
	return neighbors;
}

//deals with getting the actual path from start to end
vector<char> pathfind::getPath(bool goThroughBomb) {
	getManhattanDistance();
	Node start = this->start;
	unvisited.insert(start);
	while (!unvisited.empty()) {
		//starts at the beginning
		Node temp = *unvisited.begin();
		Node *current = new Node(temp);
		vector<Node> neighbors = getNeighbors(current);
		//will check each neighbor of current node
		for (int i = 0; i < neighbors.size(); i++) {
			//ignores neighbors that are walls
			if (neighbors.at(i).character != 'w') {
				//if the neighbor found is the end
				if (neighbors.at(i).x == this->end.x && neighbors.at(i).y == this->end.y) {
					neighbors.at(i).parent = current;
					visited.insert(neighbors.at(i));
					break;
				}
				//only go to node if it has not been visited yet
				if (find_if(visited.begin(), visited.end(), nodeEqual(neighbors.at(i).x, neighbors.at(i).y)) == visited.end()) {
					//only go in if the neighbor is in unvisited
					auto it = find_if(unvisited.begin(), unvisited.end(), nodeEqual(neighbors.at(i).x, neighbors.at(i).y));
					if (it != unvisited.end()) {
						int oldDistance = abs(neighbors.at(i).distance);
						int newDistance = abs(current->distance + current->manhattanDistance);
						//only will updatate distance if the new distance is less than the old distance
						if (newDistance < oldDistance) {
							neighbors.at(i).distance = newDistance;
							neighbors.at(i).parent = current;
							unvisited.erase(it);
							unvisited.insert(neighbors.at(i));
						}
					} else {
						int distance = 0;
						//if path not found that does not avoid bombs, go through bombs
						if (neighbors.at(i).character == 'b' && goThroughBomb) {
							neighbors.at(i).manhattanDistance += 20;
							distance += 20;
						} else if (neighbors.at(i).character == 'b' && !goThroughBomb) {
							continue;
						}
						//if it has not been visited then put it in unvisited 
						//updates its parents and distance
						distance = current->distance + current->manhattanDistance;
						neighbors.at(i).distance = distance;
						neighbors.at(i).parent = current;
						unvisited.insert(neighbors.at(i));
					}
				}
			}
		}
		//add node just visited and erase it from unvisted
		visited.insert(current);
		unvisited.erase(unvisited.find(current));
	}
	//if no path is found
	auto findEnd = visited.find(end);
	if (findEnd == visited.end()) {
		vector<char> vectorOfDirections;
		vectorOfDirections.push_back('\0');
		return vectorOfDirections;
	}
	const Node last = *findEnd;
	//getting the path from the end to each parent (backtracking)
	char direction = getDirectionCharacters(last.parent, last);
	stack<char> stackOfDirections;
	stackOfDirections.push(direction);
	//put directions in stack so that they can be popped off in the right order
	Node* iterator = findEnd->parent;
	while (iterator->parent != NULL) {
		direction = getDirectionCharacters(iterator->parent, iterator);
		stackOfDirections.push(direction);
		iterator = iterator->parent;
	}
	//a vector of chars that contains every connection
	vector<char> vectorOfDirections;
	int sizeOfStack = stackOfDirections.size();
	for (int i = 0; i < sizeOfStack; i++) {
		char poppedChar = stackOfDirections.top();
		stackOfDirections.pop();
		vectorOfDirections.push_back(poppedChar);
	}
	return vectorOfDirections;
}

//prints out the directions that the maze is taking
char pathfind::getDirectionCharacters(Node start, Node end) {
	int startXVal = start.x;
	int endXVal = end.x;
	int startYVal = start.y;
	int endYVal = end.y;
	//check horizontal directions
	if (startXVal != endXVal) {
		if (startXVal == (endXVal - 1)) {
			return 'R';
		} else if (startXVal == (endXVal + 1)) {
			return 'L';
		}
		//check vertical directions
	} else {
		if (startYVal == (endYVal - 1)) {
			return 'D';
		} else if (startYVal == (endYVal + 1)) {
			return 'U';
		}
	}
}

//equation to get the manhattan distance from current node to end
//does not include diagonals (heuristic)
int pathfind::manhattanDistance(Node current) {
	int distance = abs(current.x - this->end.x) + abs(current.y - this->end.y);
	return distance;
}

//helper function
int pathfind::getRows() {
	return r;
}

//helper function
int pathfind::getColumns() {
	return c;
}

//helps print out the maze depending on what character is in array
char getCharacter(char charOfMap) {
	char output;
	switch (charOfMap) {
	case 's':
		output = 's';
		break;
	case 'e':
		output = ' ';
		break;
	case 'w':
		output = '|';
		break;
	case 'f':
		output = 'f';
		break;
	case 'g':
		output = 'g';
		break;
	case 'b':
		output = 'b';
		break;
	default:
		output = ' ';
		break;
	}
	return output;
}

//just to help visualize map
//will print out the map with correct characters
template<std::size_t rows, std::size_t cols>
void printMap(char(&map)[rows][cols]) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			char character = getCharacter(map[i][j]);
			cout << character;
		}
		cout << endl;
	}
}

