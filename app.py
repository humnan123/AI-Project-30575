import os
import subprocess
import json
from flask import Flask, request, jsonify, send_from_directory

app = Flask(__name__, static_folder='.')

# ─────────────────────────────────────────────
# STATIC FILE SERVING
# ─────────────────────────────────────────────

@app.route('/')
def index():
    return send_from_directory('.', 'surgical_planner_complete.html')

@app.route('/<path:filename>')
def static_files(filename):
    allowed = ['.html', '.css', '.js', '.json', '.csv', '.png', '.ico', '.py']
    if any(filename.endswith(ext) for ext in allowed):
        return send_from_directory('.', filename)
    return jsonify({'error': 'Not allowed'}), 403

# ─────────────────────────────────────────────
# RUN PLANNER ENDPOINT
# ─────────────────────────────────────────────

@app.route('/run', methods=['POST'])
def run_planner():
    try:
        data = request.get_json(force=True)
        if not data or 'grid' not in data:
            return jsonify({'error': 'No grid data provided'}), 400

        grid_csv = data['grid']

        # validate basic structure
        rows = grid_csv.strip().split('\n')
        if len(rows) < 2:
            return jsonify({'error': 'Grid too small'}), 400

        # check entry (8) and tumour (9) exist
        flat = grid_csv.replace('\n', ',').split(',')
        values = [v.strip() for v in flat if v.strip()]
        if '8' not in values:
            return jsonify({'error': 'No entry point (S) found'}), 400
        if '9' not in values:
            return jsonify({'error': 'No tumour (T) found'}), 400

        # write grid to file
        with open('grid.csv', 'w') as f:
            f.write(grid_csv)

        # find planner executable
        planner = './planner'
        if not os.path.exists(planner):
            return jsonify({'error': 'Planner not found — build may have failed'}), 500

        # run planner
        result = subprocess.run(
            [planner],
            capture_output=True,
            text=True,
            timeout=60
        )

        if result.returncode != 0:
            return jsonify({
                'error': 'Planner failed',
                'details': result.stderr or result.stdout
            }), 500

        # read and return result.json
        if not os.path.exists('result.json'):
            return jsonify({'error': 'result.json not created'}), 500

        with open('result.json', 'r') as f:
            return jsonify(json.load(f))

    except subprocess.TimeoutExpired:
        return jsonify({'error': 'Timed out — try a smaller grid'}), 500
    except json.JSONDecodeError:
        return jsonify({'error': 'result.json is not valid JSON'}), 500
    except Exception as e:
        return jsonify({'error': str(e)}), 500

# ─────────────────────────────────────────────
# HEALTH CHECK
# ─────────────────────────────────────────────

@app.route('/health')
def health():
    return jsonify({
        'status': 'ok',
        'planner': 'ready' if os.path.exists('./planner') else 'missing'
    })

if __name__ == '__main__':
    port = int(os.environ.get('PORT', 10000))
    app.run(host='0.0.0.0', port=port)
