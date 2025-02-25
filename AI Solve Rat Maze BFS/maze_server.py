from flask import Flask, jsonify
from flask_cors import CORS
import subprocess
import json

app = Flask(__name__)
CORS(app)

@app.route('/solve-maze', methods=['GET'])
def solve_maze():
    # Run the C++ executable
    subprocess.run(["solve_maze_bfs.exe"], check=True)

    # Read the generated path.json file
    try:
        with open("path.json", "r") as file:
            path_data = json.load(file)
        return jsonify({"status": "success", "path": path_data})
    except FileNotFoundError:
        return jsonify({"status": "error", "message": "path.json not found"}), 500

@app.route('/generate-maze', methods=['GET'])
def generate_maze():
    # Run the Python script without the visualization
    subprocess.run(["python", "generate_maze.py"], check=True)
    return jsonify({"status": "success", "message": "New maze generated successfully."})


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
