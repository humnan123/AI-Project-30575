#!/bin/bash
# Render build script — runs once before server starts
set -e

echo "Installing Python dependencies..."
pip install -r requirements.txt

echo "Compiling C++ planner..."
g++ main.cpp -o planner -std=c++17 -O2

echo "Build complete. Planner compiled successfully."
ls -la planner
