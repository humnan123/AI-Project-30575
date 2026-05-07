#!/bin/bash
set -e # Exit immediately if a command fails

g++ -std=c++17 -O3 main.cpp -o path_planner
pip install -r requirements.txt