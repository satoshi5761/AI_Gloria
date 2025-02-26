#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <sstream>
#include <cmath>
#include <unordered_map>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

struct Point {
    int x, y;

    // Comparison operator for priority queue (tie ensures unique ordering)
    bool operator<(const Point& other) const { 
        return tie(x, y) < tie(other.x, other.y); 
    }

    // Equality operator for unordered_map
    bool operator==(const Point& other) const { 
        return x == other.x && y == other.y; 
    }
};

// Hash function for using Point in unordered_map
struct PointHash {
    size_t operator()(const Point& p) const { 
        return hash<int>()(p.x) ^ (hash<int>()(p.y) << 1);
    }
};

// Manhattan distance heuristic function for A* search
int heuristic(Point a, Point b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// Function to load the maze from a file
vector<vector<int>> loadMaze(const string& filePath) {
    ifstream file(filePath);
    if (!file) {
        cerr << "Error: Could not open " << filePath << endl;
        exit(1);
    }

    vector<vector<int>> maze;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        vector<int> row;
        int val;
        while (ss >> val) {
            row.push_back(val);
        }
        maze.push_back(row);
    }
    return maze;
}

// Function to find the start and end points of the maze
pair<Point, Point> findStartEnd(const vector<vector<int>>& maze) {
    int cols = maze[0].size();
    Point startPoint = {0, -1}, endPoint = {(int)maze.size() - 1, -1};

    for (int j = 0; j < cols; ++j) {
        if (maze[0][j] == 0) {
            startPoint = {0, j};
            break;
        }
    }

    for (int j = cols - 1; j >= 0; --j) {
        if (maze[maze.size() - 1][j] == 0) {
            endPoint = {(int)maze.size() - 1, j};
            break;
        }
    }

    return {startPoint, endPoint};
}

// A* Algorithm to solve the maze
vector<Point> aStarSolveMaze(const vector<vector<int>>& maze) {
    int rows = maze.size(), cols = maze[0].size();
    Point startPoint, endPoint;
    tie(startPoint, endPoint) = findStartEnd(maze);

    priority_queue<pair<int, Point>, vector<pair<int, Point>>, greater<>> openSet;
    unordered_map<Point, Point, PointHash> cameFrom;
    unordered_map<Point, int, PointHash> gScore;
    unordered_map<Point, int, PointHash> fScore;

    vector<Point> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    gScore[startPoint] = 0;
    fScore[startPoint] = heuristic(startPoint, endPoint);
    openSet.push({fScore[startPoint], startPoint});

    while (!openSet.empty()) {
        Point current = openSet.top().second;
        openSet.pop();

        if (current == endPoint) {
            vector<Point> path;
            for (Point at = endPoint; cameFrom.find(at) != cameFrom.end(); at = cameFrom[at]) {
                path.push_back(at);
            }
            path.push_back(startPoint);
            reverse(path.begin(), path.end());
            return path;
        }

        for (const auto& dir : directions) {
            Point neighbor = {current.x + dir.x, current.y + dir.y};

            if (neighbor.x >= 0 && neighbor.x < rows &&
                neighbor.y >= 0 && neighbor.y < cols &&
                maze[neighbor.x][neighbor.y] == 0) {
                
                int tentative_gScore = gScore[current] + 1;

                if (gScore.find(neighbor) == gScore.end() || tentative_gScore < gScore[neighbor]) {
                    cameFrom[neighbor] = current;
                    gScore[neighbor] = tentative_gScore;
                    fScore[neighbor] = tentative_gScore + heuristic(neighbor, endPoint);
                    openSet.push({fScore[neighbor], neighbor});
                }
            }
        }
    }
    return {};
}

// Function to save the solution path to a JSON file
void savePathToJson(const vector<Point>& path, const string& filePath) {
    json jsonPath = json::array();
    for (const auto& p : path) {
        jsonPath.push_back({p.x, p.y});
    }

    ofstream file(filePath);
    if (!file) {
        cerr << "Error: Could not open " << filePath << " for writing." << endl;
        exit(1);
    }
    
    file << jsonPath.dump(4); // Pretty-print with indentation
}

int main() {
    string mazeFile = "maze.txt";
    string pathFile = "path.json";

    vector<vector<int>> maze = loadMaze(mazeFile);
    vector<Point> path = aStarSolveMaze(maze);
    
    if (!path.empty()) {
        savePathToJson(path, pathFile);
        cout << "AI Path saved to " << pathFile << " ✅" << endl;
    } else {
        cout << "No path found! ❌" << endl;
    }

    return 0;
}
