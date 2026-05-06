<<<<<<< HEAD
//Humna Naveed 30575
#pragma once
#include "grid.cpp"
#include "metrics.cpp"
#include <vector>
#include <cstddef>   
#include <vector>     
#include <initializer_list> 
#include <iostream>
#include <string>
#include <fstream>
#include <iostream>


void writeJSON(const Grid& grid, std::pair<int,int> start, std::pair<int,int> target, std::vector<Metrics> results) {

    std::ofstream f("result.json");
    if (!f.is_open()) {
        std::cout << "ERROR: could not write result.json\n";
        return;
    }

    f << "{\n  \"grid\": [\n";
    for (int r = 0; r < grid.rows; r++) {
        f << "    [";
        for (int c = 0; c < grid.cols; c++) {
            f << grid.getcell(r, c);
            if (c < grid.cols - 1) f << ",";
        }
        f << "]";
        if (r < grid.rows - 1) f << ",";
        f << "\n";
    }
    f << "  ],\n";

    f << "  \"start\": [" << start.first << "," << start.second << "],\n";
    f << "  \"target\": [" << target.first << "," << target.second << "],\n";

    f << "  \"results\": [\n";
    for (int i = 0; i < (int)results.size(); i++) {
        Metrics& a = results[i];
        f<<"    {\n";
        f<<"      \"name\": \""<< a.name<< "\",\n";
        f<<"      \"pathLength\": "<< a.pathlength<< ",\n";
        f<<"      \"dangerCost\": "<< a.dangercost<< ",\n";
        f<<"      \"nodesExplored\": "<< a.nodesexplored<< ",\n";
        f<<"      \"organCrossings\": "<< a.organcrossings<< ",\n";
        f<<"      \"safetyScore\": "<< a.safetyscore<< ",\n";
        f<<"      \"timeMs\": "<< a.timems<< ",\n";

        f << "      \"path\": [";
        for (int j = 0; j < (int)a.path.size(); j++) {
            f << "[" << a.path[j].first << "," << a.path[j].second << "]";
            if (j < (int)a.path.size() - 1) f << ",";
        }
        f << "],\n";

        f << "      \"pathRaw\": [";
        for (int j = 0; j < (int)a.pathraw.size(); j++) {
            f << "[" << a.pathraw[j].first << "," << a.pathraw[j].second << "]";
            if (j < (int)a.pathraw.size() - 1) f << ",";
        }
        f << "],\n";
        f << "\"rawPathLength\": " << a.pathraw.size() << ",\n";


        f << "      \"explored\": [";
        for (int j = 0; j < (int)a.explored.size(); j++) {
            f << "[" << a.explored[j].first << "," << a.explored[j].second << "]";
            if (j < (int)a.explored.size() - 1) f << ",";
        }
        f << "]\n";        

        f << "    }";       
        if (i < (int)results.size() - 1) f << ",";
        f << "\n";
    }

    f<<"  ]\n";        
    f<<"}\n";            

    f.close();             
    std::cout << "result.json written successfully\n";
=======
//Humna Naveed 30575
#pragma once
#include "grid.cpp"
#include "metrics.cpp"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>


void writeJSON(const Grid& grid, std::pair<int,int> start, std::pair<int,int> target, std::vector<Metrics> results) {

    std::ofstream f("result.json");
    if (!f.is_open()) {
        std::cout << "ERROR: could not write result.json\n";
        return;
    }

    f << "{\n  \"grid\": [\n";
    for (int r = 0; r < grid.rows; r++) {
        f << "    [";
        for (int c = 0; c < grid.cols; c++) {
            f << grid.getcell(r, c);
            if (c < grid.cols - 1) f << ",";
        }
        f << "]";
        if (r < grid.rows - 1) f << ",";
        f << "\n";
    }
    f << "  ],\n";

    f << "  \"start\": [" << start.first << "," << start.second << "],\n";
    f << "  \"target\": [" << target.first << "," << target.second << "],\n";

    f << "  \"results\": [\n";
    for (int i = 0; i < (int)results.size(); i++) {
        Metrics& a = results[i];
        f<<"    {\n";
        f<<"      \"name\": \""<< a.name<< "\",\n";
        f<<"      \"pathLength\": "<< a.pathlength<< ",\n";
        f<<"      \"dangerCost\": "<< a.dangercost<< ",\n";
        f<<"      \"nodesExplored\": "<< a.nodesexplored<< ",\n";
        f<<"      \"organCrossings\": "<< a.organcrossings<< ",\n";
        f<<"      \"safetyScore\": "<< a.safetyscore<< ",\n";
        f<<"      \"timeMs\": "<< a.timems<< ",\n";

        f << "      \"path\": [";
        for (int j = 0; j < (int)a.path.size(); j++) {
            f << "[" << a.path[j].first << "," << a.path[j].second << "]";
            if (j < (int)a.path.size() - 1) f << ",";
        }
        f << "],\n";

        f << "      \"pathRaw\": [";
        for (int j = 0; j < (int)a.pathraw.size(); j++) {
            f << "[" << a.pathraw[j].first << "," << a.pathraw[j].second << "]";
            if (j < (int)a.pathraw.size() - 1) f << ",";
        }
        f << "],\n";
        f << "\"rawPathLength\": " << a.pathraw.size() << ",\n";


        f << "      \"explored\": [";
        for (int j = 0; j < (int)a.explored.size(); j++) {
            f << "[" << a.explored[j].first << "," << a.explored[j].second << "]";
            if (j < (int)a.explored.size() - 1) f << ",";
        }
        f << "]\n";        

        f << "    }";       
        if (i < (int)results.size() - 1) f << ",";
        f << "\n";
    }

    f<<"  ]\n";        
    f<<"}\n";            

    f.close();             
    std::cout << "result.json written successfully\n";
>>>>>>> 25db3816c3bdba76abdf6e5701011888b91e726d
}