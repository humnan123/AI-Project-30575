""" Humna Naveed 30575
    python mri_to_grid.py scan.dcm
    python mri_to_grid.py scan.dcm --entry 10,5 --tumour 80,120
    python mri_to_grid.py scan.dcm --scale 0.5 --entry 10,5 --tumour 80,120 --preview
"""

import sys
import os
import argparse
import numpy as np

try:
    import pydicom
except ImportError:
    print("ERROR: pydicom not installed. Run: pip install pydicom")
    sys.exit(1)

try:
    from PIL import Image
    PIL_AVAILABLE = True
except ImportError:
    PIL_AVAILABLE = False


# CT Hounsfield Unit ranges (approximate):
#   Air:          -1000 to -500 HU
#   Fat:           -500 to  -50 HU
#   Soft tissue:    -50 to  +50 HU  → code 1 (safe)
#   Organs/blood:   +50 to +150 HU  → code 2/3 (minor/major organ)
#   Bone:          +300 to +3000 HU → code 4 (bone)

CT_THRESHOLDS = [
    (-2000, -500, 1),   # air and background → safe tissue 
    (-500,  -50,  1),   # fat → safe tissue
    (-50,    50,  1),   # soft tissue → safe
    (50,    150,  2),   # minor organs, blood → minor organ
    (150,   300,  3),   # major organs, dense tissue → major organ
    (300,  5000,  4),   # bone → bone
]


MRI_THRESHOLDS_PCT = [
    (0,   5,   1),   # air and background → safe tissue 
    (5,   25,  1),   # low signal (fat, fluid) → safe
    (25,  55,  2),   # moderate signal (soft tissue) → minor organ
    (55,  80,  3),   # high signal (organs) → major organ
    (80,  100, 4),   # very high signal (bone marrow, dense tissue) → bone
]


def classify_ct(pixel_value, intercept, slope):
    hu = pixel_value * slope + intercept
    for low, high, code in CT_THRESHOLDS:
        if low <= hu < high:
            return code
    return 1 


def classify_mri(pixel_value, max_val):
    pct = (pixel_value / max_val) * 100 if max_val > 0 else 0
    for low, high, code in MRI_THRESHOLDS_PCT:
        if low <= pct < high:
            return code
    return 1


def load_dicom(filepath):
    print(f"Loading: {filepath}")
    ds = pydicom.dcmread(filepath)

    pixels = ds.pixel_array.astype(np.float32)

    modality = getattr(ds, 'Modality', 'MR').upper()
    print(f"Modality: {modality}")
    print(f"Image size: {pixels.shape[1]} x {pixels.shape[0]} pixels")

    intercept = float(getattr(ds, 'RescaleIntercept', 0))
    slope = float(getattr(ds, 'RescaleSlope', 1))

    return pixels, modality, intercept, slope


def pixels_to_grid(pixels, modality, intercept, slope):
    rows, cols = pixels.shape
    grid = np.ones((rows, cols), dtype=np.int32)

    if modality == 'CT':
        print("Using CT Hounsfield Unit classification...")
        for r in range(rows):
            for c in range(cols):
                grid[r, c] = classify_ct(pixels[r, c], intercept, slope)
    else:
        print("Using MRI normalised intensity classification...")
        max_val = pixels.max()
        for r in range(rows):
            for c in range(cols):
                grid[r, c] = classify_mri(pixels[r, c], max_val)

    return grid


def downsample(grid, scale):
    if scale >= 1.0:
        return grid

    rows, cols = grid.shape
    new_rows = max(1, int(rows * scale))
    new_cols = max(1, int(cols * scale))

    block_r = rows // new_rows
    block_c = cols // new_cols

    new_grid = np.ones((new_rows, new_cols), dtype=np.int32)
    for r in range(new_rows):
        for c in range(new_cols):
            block = grid[r*block_r:(r+1)*block_r, c*block_c:(c+1)*block_c]
            new_grid[r, c] = int(block.max())

    print(f"Downsampled to: {new_cols} x {new_rows} cells")
    return new_grid


def place_marker(grid, coord_str, code, label):
    if not coord_str:
        return grid
    try:
        r, c = [int(x.strip()) for x in coord_str.split(',')]
        if 0 <= r < grid.shape[0] and 0 <= c < grid.shape[1]:
            grid[r, c] = code
            print(f"{label} placed at row={r}, col={c}")
        else:
            print(f"WARNING: {label} coordinates ({r},{c}) out of bounds — skipped")
    except ValueError:
        print(f"WARNING: invalid {label} coordinates '{coord_str}' — use format row,col")
    return grid


def save_csv(grid, output_path):
    np.savetxt(output_path, grid, delimiter=',', fmt='%d')
    print(f"Grid saved to: {output_path}")
    print(f"Grid dimensions: {grid.shape[1]} cols x {grid.shape[0]} rows")


def save_preview(grid, output_path):
    if not PIL_AVAILABLE:
        print("Pillow not installed — skipping preview image")
        return

    color_map = {
        1: (40, 90, 60),    # safe tissue — dark green
        2: (90, 90, 30),    # minor organ — dark yellow
        3: (90, 30, 30),    # major organ — dark red
        4: (50, 60, 80),    # bone — dark blue-grey
        8: (30, 120, 180),  # entry — blue
        9: (120, 50, 150),  # tumour — purple
    }

    rows, cols = grid.shape
    img = Image.new('RGB', (cols, rows))
    pixels_out = []
    for r in range(rows):
        for c in range(cols):
            pixels_out.append(color_map.get(grid[r, c], (40, 90, 60)))
    img.putdata(pixels_out)

    scale_up = max(1, min(8, 800 // max(rows, cols)))
    if scale_up > 1:
        img = img.resize((cols * scale_up, rows * scale_up), Image.NEAREST)

    img.save(output_path)
    print(f"Preview image saved to: {output_path}")


def print_stats(grid):
    total = grid.size
    codes = {1: 'Safe tissue', 2: 'Minor organ', 3: 'Major organ', 4: 'Bone', 8: 'Entry', 9: 'Tumour'}
    print("\nGrid composition:")
    for code, label in codes.items():
        count = int(np.sum(grid == code))
        if count > 0:
            pct = count / total * 100
            print(f"  {label:15s} ({code}): {count:6d} cells  ({pct:.1f}%)")
    print(f"  {'Total':15s}   : {total:6d} cells")


def main():
    parser = argparse.ArgumentParser(
        description='Convert DICOM MRI/CT slice to surgical planner CSV grid'
    )
    parser.add_argument('dicom', help='Path to DICOM file (.dcm)')
    parser.add_argument('--output', default='grid.csv', help='Output CSV path (default: grid.csv)')
    parser.add_argument('--scale', type=float, default=0.25,
                        help='Downsample scale factor 0.0-1.0 (default: 0.25 = 25%% of original size)')
    parser.add_argument('--entry',  default=None, help='Entry point coordinates as row,col  e.g. 10,5')
    parser.add_argument('--tumour', default=None, help='Tumour coordinates as row,col  e.g. 80,120')
    parser.add_argument('--preview', action='store_true', help='Save a colour-coded preview image')
    parser.add_argument('--no-downsample', action='store_true', help='Keep original resolution (may be very large)')

    args = parser.parse_args()

    if not os.path.exists(args.dicom):
        print(f"ERROR: file not found: {args.dicom}")
        sys.exit(1)

    pixels, modality, intercept, slope = load_dicom(args.dicom)

    print("Classifying pixels...")
    grid = pixels_to_grid(pixels, modality, intercept, slope)

    if not args.no_downsample:
        scale = args.scale
        print(f"Downsampling to {int(scale*100)}% of original size...")
        grid = downsample(grid, scale)
    else:
        print(f"Keeping original size: {grid.shape[1]}x{grid.shape[0]}")

    grid = place_marker(grid, args.entry,  8, 'Entry point')
    grid = place_marker(grid, args.tumour, 9, 'Tumour')

    if args.entry is None:
        print("NOTE: No entry point set. Edit grid.csv and change one cell to 8, or use --entry row,col")
    if args.tumour is None:
        print("NOTE: No tumour set. Edit grid.csv and change target cell to 9, or use --tumour row,col")

    print_stats(grid)

    save_csv(grid, args.output)

    if args.preview:
        preview_path = args.output.replace('.csv', '_preview.png')
        save_preview(grid, preview_path)

    print("\nDone. Next steps:")
    print("  1. Run:  .\\planner  (C++ backend reads grid.csv)")
    print("  2. Open: http://127.0.0.1:8000/surgical_planner_complete.html")
    print("  3. Click Load result.json in the Results Viewer tab")


if __name__ == '__main__':
    main()
