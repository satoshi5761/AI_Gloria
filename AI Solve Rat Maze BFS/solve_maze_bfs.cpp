#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <sstream>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

struct Point {
    int x, y;
};

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

// Function to solve the maze using BFS
vector<Point> bfsSolveMaze(const vector<vector<int>>& maze) {
    int rows = maze.size(), cols = maze[0].size();
    
    // Fix: Explicitly declare startPoint and endPoint
    Point startPoint, endPoint;
    tie(startPoint, endPoint) = findStartEnd(maze);

    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<vector<Point>> parent(rows, vector<Point>(cols, {-1, -1}));

    queue<Point> q;
    vector<Point> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    q.push(startPoint);
    visited[startPoint.x][startPoint.y] = true;

    while (!q.empty()) {
        Point curr = q.front();
        q.pop();

        if (curr.x == endPoint.x && curr.y == endPoint.y) {
            vector<Point> path;
            for (Point at = endPoint; at.x != -1; at = parent[at.x][at.y]) {
                path.push_back(at);
            }
            reverse(path.begin(), path.end());
            return path;
        }

        for (const auto& dir : directions) {
            int nx = curr.x + dir.x, ny = curr.y + dir.y;
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && 
                maze[nx][ny] == 0 && !visited[nx][ny]) {
                q.push({nx, ny});
                visited[nx][ny] = true;
                parent[nx][ny] = curr;
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
    vector<Point> path = bfsSolveMaze(maze);
    
    if (!path.empty()) {
        savePathToJson(path, pathFile);
        cout << "Path saved to " << pathFile << endl;
    } else {
        cout << "No path found!" << endl;
    }

    return 0;
}
