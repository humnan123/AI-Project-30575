import os
import subprocess
import json
from flask import Flask, render_template, request, jsonify

app = Flask(__name__)

# 1. Configuration
# Render uses port 10000 by default, but we'll be flexible
PORT = int(os.environ.get("PORT", 10000))
EXE_PATH = "./path_planner"

@app.route('/')
def index():
    # This serves your surgical_planner_complete.html file
    return render_template('surgical_planner_complete.html')
@app.route('/result')
def get_result():
    if not os.path.exists('result.json'):
        return jsonify({'error': 'No result yet'}), 404
    with open('result.json', 'r') as f:
        return jsonify(json.load(f))

@app.route('/plan', methods=['POST'])
def plan_path():
    """
    Receives start and end coordinates, runs the C++ A* logic,
    and returns the coordinates of the path.
    """
    try:
        data = request.get_json()
        if not data:
            return jsonify({"error": "No input data provided"}), 400

        start_node = data.get('start')
        end_node = data.get('end')

        # 2. Execution of C++ Binary
        # We assume your C++ code outputs the path as a string of coordinates
        process = subprocess.run(
            [EXE_PATH, str(start_node), str(end_node)],
            capture_output=True,
            text=True,
            timeout=15  # Prevents long-running AI logic from hanging the server
        )

        if process.returncode != 0:
            return jsonify({
                "error": "C++ Planner failed",
                "details": process.stderr
            }), 500

        # 3. Return the result to the frontend
        return jsonify({
            "status": "success",
            "path": process.stdout.strip()
        })

    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == "__main__":
    # Ensure the C++ binary is actually there before starting
    if not os.path.exists(EXE_PATH):
        print(f"WARNING: {EXE_PATH} not found. Did the build.sh fail?")
    
    app.run(host='0.0.0.0', port=PORT)