//Humna Naveed 30575
#include "grid.cpp"
#include "output.cpp"
#include <string>
#include "search.cpp"
#include "metrics.cpp"
#include <chrono>
using namespace std;

int main(int argc, char* argv[]) {
    string filename = "grid.csv";
    if (argc > 1) filename = argv[1];    
            

    Grid g(filename);
    g.print();

    std::pair<int,int> start  = g.find(8);  
    std::pair<int,int> target = g.find(9);  

    vector<pair<int,int>> exp1, exp2,expa1,expa2, expa3, expg1, expg2, expg3;
    int n1=0, n2=0, na1=0,na2=0,na3=0, ng1=0, ng2=0, ng3=0;
    vector<Metrics> results;

    auto t1 = std::chrono::high_resolution_clock::now();
    auto path1 = bfs(g, start, target, n1, exp1);
    auto t2 = std::chrono::high_resolution_clock::now();
    float ms1 = std::chrono::duration<float, std::milli>(t2 - t1).count();
    auto smoothpath1 = smoothpath(g, path1);

    t1 = std::chrono::high_resolution_clock::now();
    auto path2 = UCS(g, start, target, n2, exp2);
    t2 = std::chrono::high_resolution_clock::now();
    float ms2= std::chrono::duration<float, std::milli>(t2 - t1).count();
    auto smoothpath2 = smoothpath(g, path2);

    t1 = std::chrono::high_resolution_clock::now();
    auto pathg1 = greedy(g, start, target, ng1, expg1,MANHATTAN);
    t2 = std::chrono::high_resolution_clock::now();
    float msg1 = std::chrono::duration<float, std::milli>(t2 - t1).count();
    t1 = std::chrono::high_resolution_clock::now();
    auto smoothg1 = smoothpath(g, pathg1);

    t2 = std::chrono::high_resolution_clock::now();
    auto pathg2 = greedy(g, start, target, ng2, expg2,EUCLIDEAN);
    float msg2 = std::chrono::duration<float, std::milli>(t2 - t1).count();
    t1 = std::chrono::high_resolution_clock::now();
    auto smoothg2 = smoothpath(g, pathg2);

    auto pathg3 = greedy(g, start, target, ng3, expg3,CHEBYSHEV);
    t2 = std::chrono::high_resolution_clock::now();
    float msg3 = std::chrono::duration<float, std::milli>(t2 - t1).count();
    auto smoothg3 = smoothpath(g, pathg3);

    t1 = std::chrono::high_resolution_clock::now();
    auto patha1 = Astar(g, start, target, na1, expa1, MANHATTAN);
    t2 = std::chrono::high_resolution_clock::now();
    float msa1 = std::chrono::duration<float, std::milli>(t2 - t1).count();
    auto smootha1 = smoothpath(g, patha1);
    
    t1 = std::chrono::high_resolution_clock::now();
    auto patha2 = Astar(g, start, target, na2, expa2, EUCLIDEAN);
    t2 = std::chrono::high_resolution_clock::now();
    float msa2= std::chrono::duration<float, std::milli>(t2 - t1).count();
    auto smootha2= smoothpath(g, patha2);

    t1 = std::chrono::high_resolution_clock::now();
    auto patha3 = Astar(g, start, target, na3, expa3, CHEBYSHEV);
    t2 = std::chrono::high_resolution_clock::now();
    float msa3= std::chrono::duration<float, std::milli>(t2 - t1).count();
    auto smootha3 = smoothpath(g, patha3);
    


    results.push_back(calcmetrics(g, path1,smoothpath1, n1, exp1));
    results.push_back(calcmetrics(g, path2,smoothpath2, n2, exp2));
    results.push_back(calcmetrics(g, pathg1,smoothg1, ng1, expg1));
    results.push_back(calcmetrics(g, pathg2,smoothg2, ng2, expg2));
    results.push_back(calcmetrics(g, pathg3,smoothg3, ng3, expg3));
    results.push_back(calcmetrics(g, patha1,smootha1, na1, expa1));
    results.push_back(calcmetrics(g, patha2,smootha2, na2, expa2));
    results.push_back(calcmetrics(g, patha3,smootha3, na3, expa3));

    results[0].timems=ms1;
    results[1].timems=ms2;
    results[2].timems=msg1;
    results[3].timems=msg2;
    results[4].timems=msg3;
    results[5].timems=msa1;
    results[6].timems=msa2;
    results[7].timems=msa3;
    results[0].name = "BFS";
    results[1].name = "UCS";
    results[2].name = "Greedy Manhattan";
    results[3].name = "Greedy Euclidean";
    results[4].name = "Greedy Chebyshev";
    results[5].name = "A* Manhattan";
    results[6].name = "A* Euclidean";
    results[7].name = "A* Chebyshev";


    
    writeJSON(g, start, target, results);
    return 0;
}
