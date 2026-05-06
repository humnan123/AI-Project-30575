#!/bin/bash
set -e # Exit immediately if a command fails

# Compile the C++ logic (must name it 'path_planner' to match app.py)
g++ -std=c++17 -O3 main.cpp grid.cpp search.cpp metrics.cpp output.cpp -o path_planner

# Install Python dependencies
pip install -r requirements.txt