<<<<<<< HEAD
//Humna Naveed 30575
#pragma once
#include <vector>
#include <queue>
#include <algorithm>
#include <cstddef>   
#include <vector>     
#include <initializer_list> 
#include <iostream>
#include <cmath>
#include <map>
#include <limits>
#include "grid.cpp"
using namespace std;

enum heuristictype { MANHATTAN, EUCLIDEAN, CHEBYSHEV };

float calcheuristic(int r, int c, int gr, int gc, heuristictype type) {

    float dist;
    int dr = abs(r - gr), dc = abs(c - gc);

    if (type == MANHATTAN)
        dist = dr + dc;
    else if (type == EUCLIDEAN)
        dist = sqrt(dr*dr + dc*dc);
    else  
        dist = max(dr, dc);

    return dist;
}

struct node{
        int type;
        int c;
        int r;
        int z;
        float f;
        float g;
        float h;
        float sortby;
    node(Grid grid,pair<int,int> p,pair<int,int> x, heuristictype ht=EUCLIDEAN){
            r=p.first;
            c=p.second;
            type=grid.getcell(r,c);
            g=0;
            h=calcheuristic(r,c,x.first,x.second,ht);
            f=h+g;
            sortby=f;
    }
    void setsort(int sort){
            if(sort ==1)sortby=g;
            else if(sort == 2)sortby=h;
            else sortby=f;
    }
    bool operator<(const node& that)const{
        return sortby>that.sortby;
    }
};

vector<pair<int,int>> bfs(Grid& grid, pair<int,int> start, pair<int,int> target, int& nodes, vector<pair<int,int>>& explored){
    int rows = grid.rows, cols = grid.cols;

    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<vector<pair<int,int>>> parent(rows, vector<pair<int,int>>(cols, {-1,-1}));
    queue<pair<int,int>> q;

    q.push(start);
    visited[start.first][start.second] = true;
    nodes = 0;

    int dr[] = {-1, 1, 0, 0, -1, -1, 1,  1};
    int dc[] = {0, 0, -1, 1, -1,  1, -1, 1};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        explored.push_back({r, c});
        nodes++;

        if (r == target.first && c == target.second) {
            vector<pair<int,int>> path;
            int cr = r, cc = c;
            while (cr != -1) {
                path.push_back({cr, cc});
                auto [pr, pc] = parent[cr][cc];
                cr = pr; cc = pc;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        for (int d = 0; d < 8; d++) {
            int nr = r + dr[d];
            int nc = c + dc[d];

            if (!grid.inbounds(nr, nc)) continue;
            if (visited[nr][nc]) continue;

            visited[nr][nc] = true;
            parent[nr][nc] = {r, c};
            q.push({nr, nc});
        }
    }

    return {};
}

vector<pair<int,int>> Astar(Grid& grid, pair<int,int> start, pair<int,int> target, int& nodes,vector<pair<int,int>>& explored,heuristictype htype = EUCLIDEAN){
    std::priority_queue<node> open;
    vector<vector<double>> score(grid.getl(), vector<double>(grid.getb(), numeric_limits<double>::infinity()));
    vector<vector<bool>> visited(grid.getl(), vector<bool>(grid.getb(), false));
    vector<vector<pair<int,int>>> parent(grid.getl(), vector<pair<int,int>>(grid.getb(), {-1,-1}));
    node s(grid,start,target,htype);
    s.setsort(0);
    bool f =0;
    open.push(s);
    int dr[] = {-1, 1, 0, 0, -1, -1, 1,  1};
    int dc[] = {0, 0, -1, 1, -1,  1, -1, 1};
    while(!open.empty()&&!f){
        node q = open.top();
        open.pop();
        
        if (visited[q.r][q.c]) continue; 
        visited[q.r][q.c] = true; 
        explored.push_back({q.r, q.c}); 
        nodes++;
        if(score[q.r][q.c]<q.f)continue;
        for (int d = 0; d < 8; d++) {
            int nr = q.r + dr[d];
            int nc = q.c + dc[d];
            float movecost = (d < 4) ? 1.0f : 1.414f;
            if(!grid.inbounds(nr,nc))continue;
            if (visited[nr][nc]) continue; 
            node q1(grid,make_pair(nr, nc),target, htype); q1.g =q.g+ grid.getcost(q1.r,q1.c)* movecost; q1.f= q1.h +q1.g; q1.setsort(0);
            if (q1.f < score[q1.r][q1.c]) {score[q1.r][q1.c] = q1.f;parent[q1.r][q1.c] = {q.r, q.c};open.push(q1);}
            if(target.first==q1.r&&target.second==q1.c){f=1;break;}}
    }
    vector<pair<int,int>> path;
    int cr = target.first, cc = target.second;
    while (cr != -1) {
        path.push_back({cr, cc});
        auto [pr, pc] = parent[cr][cc];
        cr = pr; cc = pc;
    }
    reverse(path.begin(), path.end());
    return path;
}
vector<pair<int,int>> UCS(Grid& grid, pair<int,int> start, pair<int,int> target, int& nodes, vector<pair<int,int>>& explored){
    std::priority_queue<node> open;
    vector<vector<double>> score(grid.getl(), vector<double>(grid.getb(), numeric_limits<double>::infinity()));
    vector<vector<bool>> visited(grid.getl(), vector<bool>(grid.getb(), false));
    vector<vector<pair<int,int>>> parent(grid.getl(), vector<pair<int,int>>(grid.getb(), {-1,-1}));
    node s(grid, start,target);
    s.setsort(1);
    s.g =0;
    score[start.first][start.second] = 0;  
    bool f =0;
    open.push(s);
    int dr[] = {-1, 1, 0, 0, -1, -1, 1,  1};
    int dc[] = {0, 0, -1, 1, -1,  1, -1, 1};
    while(!open.empty()&&!f){
        node q = open.top();
        open.pop();
        if (visited[q.r][q.c]) continue; 
        visited[q.r][q.c] = true;
        explored.push_back({q.r, q.c});   
        nodes++;
        if (q.g > score[q.r][q.c]) continue;
        for (int d = 0; d < 8; d++) {
            int nr = q.r + dr[d];
            int nc = q.c + dc[d];
            float movecost = (d < 4) ? 1.0f : 1.414f;
            if(!grid.inbounds(nr,nc))continue;
            if (visited[nr][nc]) continue; 
            node q1(grid,make_pair(nr, nc),target); q1.g = q.g + grid.getcost(q1.r, q1.c) * movecost; q1.setsort(1);
            if (q1.g < score[q1.r][q1.c]) {score[q1.r][q1.c] = q1.g;parent[q1.r][q1.c] = {q.r, q.c};open.push(q1);}
            if(target.first==q1.r&&target.second==q1.c){f=1;break;}}
    }
    vector<pair<int,int>> path;
    int cr = target.first, cc = target.second;
    while (cr != -1) {
        path.push_back({cr, cc});
        auto [pr, pc] = parent[cr][cc];
        cr = pr; cc = pc;
    }
    reverse(path.begin(), path.end());
    return path;
}
vector<pair<int,int>> greedy(Grid& grid, pair<int,int> start, pair<int,int> target, int& nodes,vector<pair<int,int>>& explored, heuristictype ht=EUCLIDEAN){
    std::priority_queue<node> open;
    vector<vector<double>> score(grid.getl(), vector<double>(grid.getb(), numeric_limits<double>::infinity()));
    vector<vector<bool>> visited(grid.getl(), vector<bool>(grid.getb(), false));
    vector<vector<pair<int,int>>> parent(grid.getl(), vector<pair<int,int>>(grid.getb(), {-1,-1}));
    node s(grid, start,target,ht);
    s.setsort(2);
    bool f =0;
    open.push(s);
    int dr[] = {-1, 1, 0, 0, -1, -1, 1,  1};
    int dc[] = {0, 0, -1, 1, -1,  1, -1, 1};
    while(!open.empty()&&!f){
        node q = open.top();
        open.pop();
        if (visited[q.r][q.c]) continue; 
        visited[q.r][q.c] = true; 
        explored.push_back({q.r, q.c}); 
        nodes++;
        if(score[q.r][q.c]<q.h)continue;
        for (int d = 0; d < 8; d++) {
            int nr = q.r + dr[d];
            int nc = q.c + dc[d];
            if(!grid.inbounds(nr,nc))continue;
            if (visited[nr][nc]) continue; 
            node q1(grid,make_pair(nr, nc),target,ht); q1.setsort(2);
            if (q1.h < score[q1.r][q1.c]) {score[q1.r][q1.c] = q1.h;parent[q1.r][q1.c] = {q.r, q.c};open.push(q1);}
            if(target.first==q1.r&&target.second==q1.c){f=1;break;}}
    }
    vector<pair<int,int>> path;
    int cr = target.first, cc = target.second;
    while (cr != -1) {
        path.push_back({cr, cc});
        auto [pr, pc] = parent[cr][cc];
        cr = pr; cc = pc;
    }
    reverse(path.begin(), path.end());
    return path;
}
=======
//Humna Naveed 30575
#pragma once
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <map>
#include <limits>
#include "grid.cpp"
using namespace std;

enum heuristictype { MANHATTAN, EUCLIDEAN, CHEBYSHEV };

float calcheuristic(int r, int c, int gr, int gc, heuristictype type) {

    float dist;
    int dr = abs(r - gr), dc = abs(c - gc);

    if (type == MANHATTAN)
        dist = dr + dc;
    else if (type == EUCLIDEAN)
        dist = sqrt(dr*dr + dc*dc);
    else  
        dist = max(dr, dc);

    return dist;
}

struct node{
        int type;
        int c;
        int r;
        int z;
        float f;
        float g;
        float h;
        float sortby;
    node(Grid grid,pair<int,int> p,pair<int,int> x, heuristictype ht=EUCLIDEAN){
            r=p.first;
            c=p.second;
            type=grid.getcell(r,c);
            g=0;
            h=calcheuristic(r,c,x.first,x.second,ht);
            f=h+g;
            sortby=f;
    }
    void setsort(int sort){
            if(sort ==1)sortby=g;
            else if(sort == 2)sortby=h;
            else sortby=f;
    }
    bool operator<(const node& that)const{
        return sortby>that.sortby;
    }
};

vector<pair<int,int>> bfs(Grid& grid, pair<int,int> start, pair<int,int> target, int& nodes, vector<pair<int,int>>& explored){
    int rows = grid.rows, cols = grid.cols;

    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<vector<pair<int,int>>> parent(rows, vector<pair<int,int>>(cols, {-1,-1}));
    queue<pair<int,int>> q;

    q.push(start);
    visited[start.first][start.second] = true;
    nodes = 0;

    int dr[] = {-1, 1, 0, 0, -1, -1, 1,  1};
    int dc[] = {0, 0, -1, 1, -1,  1, -1, 1};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        explored.push_back({r, c});
        nodes++;

        if (r == target.first && c == target.second) {
            vector<pair<int,int>> path;
            int cr = r, cc = c;
            while (cr != -1) {
                path.push_back({cr, cc});
                auto [pr, pc] = parent[cr][cc];
                cr = pr; cc = pc;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        for (int d = 0; d < 8; d++) {
            int nr = r + dr[d];
            int nc = c + dc[d];

            if (!grid.inbounds(nr, nc)) continue;
            if (visited[nr][nc]) continue;

            visited[nr][nc] = true;
            parent[nr][nc] = {r, c};
            q.push({nr, nc});
        }
    }

    return {};
}

vector<pair<int,int>> Astar(Grid& grid, pair<int,int> start, pair<int,int> target, int& nodes,vector<pair<int,int>>& explored,heuristictype htype = EUCLIDEAN){
    std::priority_queue<node> open;
    vector<vector<double>> score(grid.getl(), vector<double>(grid.getb(), numeric_limits<double>::infinity()));
    vector<vector<bool>> visited(grid.getl(), vector<bool>(grid.getb(), false));
    vector<vector<pair<int,int>>> parent(grid.getl(), vector<pair<int,int>>(grid.getb(), {-1,-1}));
    node s(grid,start,target,htype);
    s.setsort(0);
    bool f =0;
    open.push(s);
    int dr[] = {-1, 1, 0, 0, -1, -1, 1,  1};
    int dc[] = {0, 0, -1, 1, -1,  1, -1, 1};
    while(!open.empty()&&!f){
        node q = open.top();
        open.pop();
        
        if (visited[q.r][q.c]) continue; 
        visited[q.r][q.c] = true; 
        explored.push_back({q.r, q.c}); 
        nodes++;
        if(score[q.r][q.c]<q.f)continue;
        for (int d = 0; d < 8; d++) {
            int nr = q.r + dr[d];
            int nc = q.c + dc[d];
            float movecost = (d < 4) ? 1.0f : 1.414f;
            if(!grid.inbounds(nr,nc))continue;
            if (visited[nr][nc]) continue; 
            node q1(grid,make_pair(nr, nc),target, htype); q1.g =q.g+ grid.getcost(q1.r,q1.c)* movecost; q1.f= q1.h +q1.g; q1.setsort(0);
            if (q1.f < score[q1.r][q1.c]) {score[q1.r][q1.c] = q1.f;parent[q1.r][q1.c] = {q.r, q.c};open.push(q1);}
            if(target.first==q1.r&&target.second==q1.c){f=1;break;}}
    }
    vector<pair<int,int>> path;
    int cr = target.first, cc = target.second;
    while (cr != -1) {
        path.push_back({cr, cc});
        auto [pr, pc] = parent[cr][cc];
        cr = pr; cc = pc;
    }
    reverse(path.begin(), path.end());
    return path;
}
vector<pair<int,int>> UCS(Grid& grid, pair<int,int> start, pair<int,int> target, int& nodes, vector<pair<int,int>>& explored){
    std::priority_queue<node> open;
    vector<vector<double>> score(grid.getl(), vector<double>(grid.getb(), numeric_limits<double>::infinity()));
    vector<vector<bool>> visited(grid.getl(), vector<bool>(grid.getb(), false));
    vector<vector<pair<int,int>>> parent(grid.getl(), vector<pair<int,int>>(grid.getb(), {-1,-1}));
    node s(grid, start,target);
    s.setsort(1);
    s.g =0;
    score[start.first][start.second] = 0;  
    bool f =0;
    open.push(s);
    int dr[] = {-1, 1, 0, 0, -1, -1, 1,  1};
    int dc[] = {0, 0, -1, 1, -1,  1, -1, 1};
    while(!open.empty()&&!f){
        node q = open.top();
        open.pop();
        if (visited[q.r][q.c]) continue; 
        visited[q.r][q.c] = true;
        explored.push_back({q.r, q.c});   
        nodes++;
        if (q.g > score[q.r][q.c]) continue;
        for (int d = 0; d < 8; d++) {
            int nr = q.r + dr[d];
            int nc = q.c + dc[d];
            float movecost = (d < 4) ? 1.0f : 1.414f;
            if(!grid.inbounds(nr,nc))continue;
            if (visited[nr][nc]) continue; 
            node q1(grid,make_pair(nr, nc),target); q1.g = q.g + grid.getcost(q1.r, q1.c) * movecost; q1.setsort(1);
            if (q1.g < score[q1.r][q1.c]) {score[q1.r][q1.c] = q1.g;parent[q1.r][q1.c] = {q.r, q.c};open.push(q1);}
            if(target.first==q1.r&&target.second==q1.c){f=1;break;}}
    }
    vector<pair<int,int>> path;
    int cr = target.first, cc = target.second;
    while (cr != -1) {
        path.push_back({cr, cc});
        auto [pr, pc] = parent[cr][cc];
        cr = pr; cc = pc;
    }
    reverse(path.begin(), path.end());
    return path;
}
vector<pair<int,int>> greedy(Grid& grid, pair<int,int> start, pair<int,int> target, int& nodes,vector<pair<int,int>>& explored, heuristictype ht=EUCLIDEAN){
    std::priority_queue<node> open;
    vector<vector<double>> score(grid.getl(), vector<double>(grid.getb(), numeric_limits<double>::infinity()));
    vector<vector<bool>> visited(grid.getl(), vector<bool>(grid.getb(), false));
    vector<vector<pair<int,int>>> parent(grid.getl(), vector<pair<int,int>>(grid.getb(), {-1,-1}));
    node s(grid, start,target,ht);
    s.setsort(2);
    bool f =0;
    open.push(s);
    int dr[] = {-1, 1, 0, 0, -1, -1, 1,  1};
    int dc[] = {0, 0, -1, 1, -1,  1, -1, 1};
    while(!open.empty()&&!f){
        node q = open.top();
        open.pop();
        if (visited[q.r][q.c]) continue; 
        visited[q.r][q.c] = true; 
        explored.push_back({q.r, q.c}); 
        nodes++;
        if(score[q.r][q.c]<q.h)continue;
        for (int d = 0; d < 8; d++) {
            int nr = q.r + dr[d];
            int nc = q.c + dc[d];
            if(!grid.inbounds(nr,nc))continue;
            if (visited[nr][nc]) continue; 
            node q1(grid,make_pair(nr, nc),target,ht); q1.setsort(2);
            if (q1.h < score[q1.r][q1.c]) {score[q1.r][q1.c] = q1.h;parent[q1.r][q1.c] = {q.r, q.c};open.push(q1);}
            if(target.first==q1.r&&target.second==q1.c){f=1;break;}}
    }
    vector<pair<int,int>> path;
    int cr = target.first, cc = target.second;
    while (cr != -1) {
        path.push_back({cr, cc});
        auto [pr, pc] = parent[cr][cc];
        cr = pr; cc = pc;
    }
    reverse(path.begin(), path.end());
    return path;
}
>>>>>>> 25db3816c3bdba76abdf6e5701011888b91e726d
