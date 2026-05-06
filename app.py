from flask import Flask, request, jsonify, send_from_directory
import subprocess
import json
import os

app = Flask(__name__, static_folder='.')

# serve the HTML frontend
@app.route('/')
def index():
    return send_from_directory('.', 'surgical_planner_complete.html')

# serve any static file
@app.route('/<path:filename>')
def static_files(filename):
    return send_from_directory('.', filename)

# run the planner
@app.route('/run', methods=['POST'])
def run_planner():
    # save uploaded grid.csv
    data = request.get_json()
    grid_csv = data.get('grid', '')
    
    with open('grid.csv', 'w') as f:
        f.write(grid_csv)
    
    # run compiled planner
    result = subprocess.run(['./planner'], capture_output=True, text=True)
    
    if result.returncode != 0:
        return jsonify({'error': result.stderr}), 500
    
    # return result.json
    with open('result.json', 'r') as f:
        return jsonify(json.load(f))

if __name__ == '__main__':
    port = int(os.environ.get('PORT', 8000))
    app.run(host='0.0.0.0', port=port)