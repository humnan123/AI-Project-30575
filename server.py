"""
AI Surgical Path Planner — Flask Server
Wraps the C++ planner executable for cloud deployment on Render.
"""

from flask import Flask, request, jsonify, send_from_directory
import subprocess
import json
import os
import tempfile
import shutil

app = Flask(__name__, static_folder='.')

# ─────────────────────────────────────────────
# STATIC FILE SERVING
# ─────────────────────────────────────────────

@app.route('/')
def index():
    return send_from_directory('.', 'surgical_planner_complete.html')

@app.route('/<path:filename>')
def static_files(filename):
    # only allow safe file types
    allowed = ['.html', '.css', '.js', '.json', '.csv', '.png', '.ico']
    if any(filename.endswith(ext) for ext in allowed):
        return send_from_directory('.', filename)
    return jsonify({'error': 'Not allowed'}), 403

# ─────────────────────────────────────────────
# RUN PLANNER ENDPOINT
# ─────────────────────────────────────────────

@app.route('/run', methods=['POST'])
def run_planner():
    """
    Accepts grid CSV as JSON body, runs C++ planner, returns result.json.
    Body: { "grid": "8,1,1,...\n1,1,1,..." }
    """
    try:
        data = request.get_json(force=True)
        if not data or 'grid' not in data:
            return jsonify({'error': 'No grid data provided'}), 400

        grid_csv = data['grid']

        # validate basic structure
        rows = grid_csv.strip().split('\n')
        if len(rows) < 2:
            return jsonify({'error': 'Grid too small — need at least 2 rows'}), 400

        # check entry (8) and tumour (9) exist
        flat = grid_csv.replace('\n', ',').split(',')
        values = [v.strip() for v in flat if v.strip()]
        if '8' not in values:
            return jsonify({'error': 'No entry point (S) found — place an entry point in the editor'}), 400
        if '9' not in values:
            return jsonify({'error': 'No tumour (T) found — place a tumour in the editor'}), 400

        # write grid to file
        with open('grid.csv', 'w') as f:
            f.write(grid_csv)

        # determine planner executable name
        planner = './planner' if os.path.exists('./planner') else './planner.exe'
        if not os.path.exists(planner):
            return jsonify({'error': 'Planner executable not found — build may have failed'}), 500

        # run planner with timeout
        result = subprocess.run(
            [planner],
            capture_output=True,
            text=True,
            timeout=30  # 30 second timeout
        )

        if result.returncode != 0:
            return jsonify({
                'error': 'Planner failed',
                'details': result.stderr or result.stdout
            }), 500

        # read and return result.json
        if not os.path.exists('result.json'):
            return jsonify({'error': 'result.json was not created — planner may have crashed'}), 500

        with open('result.json', 'r') as f:
            result_data = json.load(f)

        return jsonify(result_data)

    except subprocess.TimeoutExpired:
        return jsonify({'error': 'Planner timed out after 30 seconds — try a smaller grid'}), 500
    except json.JSONDecodeError:
        return jsonify({'error': 'result.json is not valid JSON — check C++ output'}), 500
    except Exception as e:
        return jsonify({'error': str(e)}), 500


# ─────────────────────────────────────────────
# HEALTH CHECK
# ─────────────────────────────────────────────

@app.route('/health')
def health():
    planner_exists = os.path.exists('./planner') or os.path.exists('./planner.exe')
    return jsonify({
        'status': 'ok',
        'planner': 'ready' if planner_exists else 'missing'
    })


if __name__ == '__main__':
    port = int(os.environ.get('PORT', 8000))
    debug = os.environ.get('FLASK_ENV') == 'development'
    print(f"Starting server on port {port}")
    app.run(host='0.0.0.0', port=port, debug=debug)
