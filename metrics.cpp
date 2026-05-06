//Humna Naveed 30575
#pragma once
#include <cstddef>  
#include <initializer_list>      
#include <iostream>
#include "grid.cpp"
#include <vector>
#include<cmath>

struct Metrics {
    std::string name;
    std::vector<std::pair<int,int>> pathraw;
    std::vector<std::pair<int,int>> path;
    int pathlength;
    int dangercost;
    int nodesexplored;
    int organcrossings;   
    float safetyscore;   
    float timems;
    std::vector<std::pair<int,int>> explored;
};

Metrics calcmetrics(const Grid& grid, const vector<pair<int,int>>& pr,const vector<pair<int,int>>& ps,int nodes,vector<pair<int,int>>& exp) {

    Metrics m;
    m.pathraw = pr; 
    m.path = ps;
    m.pathlength = ps.size();
    m.nodesexplored = nodes;
    m.dangercost = 0;
    m.organcrossings = 0;

    for (auto [r, c] : ps) {
        int cost = grid.getcost(r, c);
        m.dangercost += cost;
        if (cost >= 20) m.organcrossings++;
    }

    int worstpossible = m.pathlength * 50;
    m.safetyscore = 100.0f - ((float)m.dangercost / worstpossible * 100.0f);
    m.explored =exp;

    return m;
}

bool canskip(const Grid& grid, int r1, int c1,int r2, int c2, int maxcost) {
    int steps = max(abs(r2-r1), abs(c2-c1));
    for (int i = 0; i <= steps; i++) {
        float t = steps == 0 ? 0 : (float)i / steps;
        int r = round(r1 + t * (r2 - r1));
        int c = round(c1 + t * (c2 - c1));
        if (grid.getcost(r, c) > maxcost) return false;
    }
    return true;
}


vector<pair<int,int>> linecells(int r1, int c1, int r2, int c2) {
    vector<pair<int,int>> cells;
    int steps = max(abs(r2-r1), abs(c2-c1));
    if (steps == 0) { cells.push_back({r1,c1}); return cells; }
    for (int i = 0; i <= steps; i++) {
        float t = (float)i / steps;
        int r = round(r1 + t * (r2-r1));
        int c = round(c1 + t * (c2-c1));
        if (cells.empty() || cells.back() != make_pair(r,c))
            cells.push_back({r,c});
    }
    return cells;
}

bool linesafe(const Grid& grid, int r1, int c1,int r2, int c2, int maxCost = 5) {
    for (auto [r,c] : linecells(r1,c1,r2,c2))
        if (grid.getcost(r,c) > maxCost) return false;
    return true;
}

vector<pair<int,int>> smoothpath(const Grid& grid,const vector<pair<int,int>>& path, int maxcost = 5) {

    if (path.size() < 3) return path;

    vector<pair<int,int>> smooth;
    int i = 0;
    smooth.push_back(path[0]);

    while (i < (int)path.size() - 1) {
        int j = path.size() - 1;
        while (j > i + 1) {
            if (linesafe(grid, path[i].first, path[i].second,
                               path[j].first, path[j].second, maxcost))
                break;
            j--;
        }
        auto line = linecells(path[i].first, path[i].second,
                              path[j].first, path[j].second);
        for (int k = 1; k < (int)line.size(); k++)
            smooth.push_back(line[k]);
        i = j;
    }
    return smooth;
=======
//Humna Naveed 30575
#pragma once
#include "grid.cpp"
#include <vector>
#include<cmath>

struct Metrics {
    std::string name;
    std::vector<std::pair<int,int>> pathraw;
    std::vector<std::pair<int,int>> path;
    int pathlength;
    int dangercost;
    int nodesexplored;
    int organcrossings;   
    float safetyscore;   
    float timems;
    std::vector<std::pair<int,int>> explored;
};

Metrics calcmetrics(const Grid& grid, const vector<pair<int,int>>& pr,const vector<pair<int,int>>& ps,int nodes,vector<pair<int,int>>& exp) {

    Metrics m;
    m.pathraw = pr; 
    m.path = ps;
    m.pathlength = ps.size();
    m.nodesexplored = nodes;
    m.dangercost = 0;
    m.organcrossings = 0;

    for (auto [r, c] : ps) {
        int cost = grid.getcost(r, c);
        m.dangercost += cost;
        if (cost >= 20) m.organcrossings++;
    }

    int worstpossible = m.pathlength * 50;
    m.safetyscore = 100.0f - ((float)m.dangercost / worstpossible * 100.0f);
    m.explored =exp;

    return m;
}

bool canskip(const Grid& grid, int r1, int c1,int r2, int c2, int maxcost) {
    int steps = max(abs(r2-r1), abs(c2-c1));
    for (int i = 0; i <= steps; i++) {
        float t = steps == 0 ? 0 : (float)i / steps;
        int r = round(r1 + t * (r2 - r1));
        int c = round(c1 + t * (c2 - c1));
        if (grid.getcost(r, c) > maxcost) return false;
    }
    return true;
}


vector<pair<int,int>> linecells(int r1, int c1, int r2, int c2) {
    vector<pair<int,int>> cells;
    int steps = max(abs(r2-r1), abs(c2-c1));
    if (steps == 0) { cells.push_back({r1,c1}); return cells; }
    for (int i = 0; i <= steps; i++) {
        float t = (float)i / steps;
        int r = round(r1 + t * (r2-r1));
        int c = round(c1 + t * (c2-c1));
        if (cells.empty() || cells.back() != make_pair(r,c))
            cells.push_back({r,c});
    }
    return cells;
}

bool linesafe(const Grid& grid, int r1, int c1,int r2, int c2, int maxCost = 5) {
    for (auto [r,c] : linecells(r1,c1,r2,c2))
        if (grid.getcost(r,c) > maxCost) return false;
    return true;
}

vector<pair<int,int>> smoothpath(const Grid& grid,const vector<pair<int,int>>& path, int maxcost = 5) {

    if (path.size() < 3) return path;

    vector<pair<int,int>> smooth;
    int i = 0;
    smooth.push_back(path[0]);

    while (i < (int)path.size() - 1) {
        int j = path.size() - 1;
        while (j > i + 1) {
            if (linesafe(grid, path[i].first, path[i].second,
                               path[j].first, path[j].second, maxcost))
                break;
            j--;
        }
        auto line = linecells(path[i].first, path[i].second,
                              path[j].first, path[j].second);
        for (int k = 1; k < (int)line.size(); k++)
            smooth.push_back(line[k]);
        i = j;
    }
    return smooth;
}