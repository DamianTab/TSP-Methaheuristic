//
// Created by Damian IT on 08.12.2018.
//

#ifndef TSP_METAHEURISTIC_INSTANCE_H
#define TSP_METAHEURISTIC_INSTANCE_H

#include <string>
using namespace std;

class Instance {
private:
    int **Cooridante_Table; // Wierzchołki ponumerowane od i. Indeks 0 przechowuje rozmiar instancji.
public:
    string load(string path);
    void save();
    void show_Instance();
    void generate();

    friend class Population;
    friend class Methaheuristic;

};


#endif //TSP_METAHEURISTIC_INSTANCE_H
