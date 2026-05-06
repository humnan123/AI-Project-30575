<<<<<<< HEAD
//Humna Naveed 30575
#pragma once
#include <vector>
#include <string>
#include <iostream>
#include<utility>
#include <sstream>
#include <stdexcept>
#include <fstream>
#include <cstddef>   
#include <vector>     
#include <initializer_list> 
#include <iostream>
using namespace std;

enum celltype {
    TISSUE  = 1,   // safe, cost 1
    MINOR   = 2,   // minor organ, cost 5
    MAJOR   = 3,   // major organ, cost 20
    BONE    = 4,   // bone, cost 50
    TUMOUR  = 9,   // target, cost 0
    ENTRY   = 8    // start point, cost 0
};
class Grid {
public:

    int rows, cols;
    vector<vector<int>> cells;

   Grid(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "File could not be loaded!" << std::endl;
        loadpreset(); return;
    }
    string line;
    while (std::getline(file, line)) {
        std::vector<int> row;
        std::stringstream ss(line);
        std::string val;
        while (std::getline(ss, val, ','))
            row.push_back(std::stoi(val));
        cells.push_back(row);
    }

    rows = cells.size();
    cols = cells[0].size();
}
    int getl(){
        return rows;
    }
    int getb(){
        return cols;
    }

    bool inbounds(int r, int c) const {
    return r >= 0 && r < rows && c >= 0 && c < cols;
}
    int getcell(int r, int c)const{
        return cells[r][c];
    }

    void setcell(int r, int c, celltype type){
        cells[r][c]=type;
    }

    int getcost(int r, int c) const {
    switch(cells[r][c]) {
        case TISSUE:return 1;
        case MINOR:return 5;
        case MAJOR:return 20;
        case BONE:return 50;
        case TUMOUR:return 0;
        case ENTRY:return 0;
        default: return 1;
    }
}

    void print()  const {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            switch(cells[r][c]) {
                case TISSUE:cout << ". "; break;
                case MINOR:cout << "m "; break;
                case MAJOR:cout << "M "; break;
                case BONE:cout << "b "; break;
                case TUMOUR:cout << "T "; break;
                case ENTRY:cout << "S "; break;
                default:cout << "? "; break;
            }
        }
        cout << "\n";
    }
}
    void loadpreset(){ 
        cells = {
        {8,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,3,3,3,3,3,1,1,1,1},
        {1,1,1,3,3,3,3,3,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,3,3,3,3,3,1,1,1,1},
        {1,1,1,3,3,3,3,3,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,9,1,1,1,1}
        };
    rows = cells.size();
    cols = cells[0].size();
}
    pair<int,int> find(int t){
        pair<int,int>p;
        for(int i =0; i<rows ; i++){
            for(int j =0; j<cols; j++){
                if(cells[i][j]==t){p.first =i; p.second=j; return p;}
            }
        }
        throw invalid_argument("cell not found");
    }
=======
//Humna Naveed 30575
#pragma once
#include <vector>
#include <string>
#include <iostream>
#include<utility>
#include <sstream>
#include <stdexcept>
#include <fstream>
using namespace std;

enum celltype {
    TISSUE  = 1,   // safe, cost 1
    MINOR   = 2,   // minor organ, cost 5
    MAJOR   = 3,   // major organ, cost 20
    BONE    = 4,   // bone, cost 50
    TUMOUR  = 9,   // target, cost 0
    ENTRY   = 8    // start point, cost 0
};
class Grid {
public:

    int rows, cols;
    vector<vector<int>> cells;

   Grid(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "File could not be loaded!" << std::endl;
        loadpreset(); return;
    }
    string line;
    while (std::getline(file, line)) {
        std::vector<int> row;
        std::stringstream ss(line);
        std::string val;
        while (std::getline(ss, val, ','))
            row.push_back(std::stoi(val));
        cells.push_back(row);
    }

    rows = cells.size();
    cols = cells[0].size();
}
    int getl(){
        return rows;
    }
    int getb(){
        return cols;
    }

    bool inbounds(int r, int c) const {
    return r >= 0 && r < rows && c >= 0 && c < cols;
}
    int getcell(int r, int c)const{
        return cells[r][c];
    }

    void setcell(int r, int c, celltype type){
        cells[r][c]=type;
    }

    int getcost(int r, int c) const {
    switch(cells[r][c]) {
        case TISSUE:return 1;
        case MINOR:return 5;
        case MAJOR:return 20;
        case BONE:return 50;
        case TUMOUR:return 0;
        case ENTRY:return 0;
        default: return 1;
    }
}

    void print()  const {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            switch(cells[r][c]) {
                case TISSUE:cout << ". "; break;
                case MINOR:cout << "m "; break;
                case MAJOR:cout << "M "; break;
                case BONE:cout << "b "; break;
                case TUMOUR:cout << "T "; break;
                case ENTRY:cout << "S "; break;
                default:cout << "? "; break;
            }
        }
        cout << "\n";
    }
}
    void loadpreset(){ 
        cells = {
        {8,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,3,3,3,3,3,1,1,1,1},
        {1,1,1,3,3,3,3,3,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,3,3,3,3,3,1,1,1,1},
        {1,1,1,3,3,3,3,3,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,9,1,1,1,1}
        };
    rows = cells.size();
    cols = cells[0].size();
}
    pair<int,int> find(int t){
        pair<int,int>p;
        for(int i =0; i<rows ; i++){
            for(int j =0; j<cols; j++){
                if(cells[i][j]==t){p.first =i; p.second=j; return p;}
            }
        }
        throw invalid_argument("cell not found");
    }
>>>>>>> 25db3816c3bdba76abdf6e5701011888b91e726d
};