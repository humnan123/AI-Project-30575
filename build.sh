#!/bin/bash
# Render build script — runs once before server starts
set -e

echo "Installing Python dependencies..."
pip install -r requirements.txt

echo "Compiling C++ planner..."
g++ -std=c++17 -O3 main.cpp grid.cpp search.cpp metrics.cpp output.cpp -o path_planner

pip install -r requirements.txt

echo "Build complete. Planner compiled successfully."
ls -la planner
