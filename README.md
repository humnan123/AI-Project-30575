# AI Surgical Path Planner

A C++ and web-based AI system that finds the safest path for a surgical robot navigating through a human body cross-section. Compares BFS, UCS, Greedy, and A* search algorithms on danger-weighted grids.

---

## Requirements

- g++ (C++17)
- Python 3
- Modern browser (Chrome / Edge)
- `pip install pydicom numpy Pillow` *(only needed for MRI/CT input)*

---

## Grid Input — Two Options

### Option A — Draw manually in the editor
Use the built-in web editor to paint your body cross-section and export `grid.csv`.

### Option B — Convert a real MRI or CT scan
```bash
pip install pydicom numpy Pillow
python mri_to_grid.py scan.dcm --scale 0.25 --entry 10,5 --tumour 80,120 --preview
```
This reads a DICOM file, classifies tissue by density, and writes `grid.csv` automatically.  
Free DICOM files: [dicomlibrary.com](https://www.dicomlibrary.com) or [cancerimagingarchive.net](https://cancerimagingarchive.net)

Both options produce the same `grid.csv` — the C++ backend works identically either way.

---

## How to Run

```bash
# 1. Compile
g++ main.cpp -o planner -std=c++17

# 2. Run algorithms (reads grid.csv, writes result.json)
.\planner

# 3. Serve the UI
python -m http.server 8000
```

Then open: **http://127.0.0.1:8000/surgical_planner_complete.html**

> Always use `http://127.0.0.1:8000` — opening the HTML file directly will block JSON loading.

---

## Workflow

**Manual grid:**
1. Open the URL above → Grid Editor tab → paint grid → export `grid.csv`
2. Run `.\planner`
3. Results Viewer tab → click Load result.json

**MRI/CT grid:**
1. Run `python mri_to_grid.py scan.dcm --entry row,col --tumour row,col --preview`
2. Run `.\planner`
3. Results Viewer tab → click Load result.json

---

## Algorithms

| Algorithm | Heuristics |
|---|---|
| BFS | — |
| UCS | — |
| Greedy | Manhattan, Euclidean, Chebyshev |
| A* | Manhattan, Euclidean, Chebyshev |

**Recommended:** A* Euclidean — lowest danger cost, zero organ crossings, best efficiency.

---

## File Structure

```
project/
├── main.cpp
├── grid.cpp
├── search.cpp
├── metrics.cpp
├── output.cpp
├── mri_to_grid.py           <- MRI/CT to grid converter
├── grid.csv                 <- from editor or mri_to_grid.py
├── result.json              <- written by planner
└── surgical_planner_complete.html
```

---

## Cell Types

| Type | Code | Cost |
|---|---|---|
| Safe tissue | 1 | 1 |
| Minor organ | 2 | 5 |
| Major organ | 3 | 20 |
| Bone | 4 | 50 |
| Entry point | 8 | 0 |
| Tumour | 9 | 0 |

---

## Notes

- On Mac/Linux replace `.\planner` with `./planner`
- Re-run `.\planner` and reload result.json every time you change the grid
- Keep the Python server terminal open while using the browser UI
