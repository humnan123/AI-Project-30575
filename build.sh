#!/bin/bash
set -e

g++ -std=c++17 -O3 main.cpp -o planner
pip install -r requirements.txt