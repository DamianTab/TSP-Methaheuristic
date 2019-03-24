//
// Created by Damian IT on 07.12.2018.
//

#ifndef TSP_METAHEURISTIC_POPULATION_H
#define TSP_METAHEURISTIC_POPULATION_H

#include <vector>
#include "Instance.h"
//#include <string>
using namespace std;

class Population {
private:

    Instance* MyInstance;
    int **Table_of_vertices; // Wierzchołki ponumerowane od 1. Indeks 0 przechowuje ilość krawędzi. | Table_of_vertices[i][0] - poprzednik wierzchołka i  | Table_of_vertices[i][1] - następnik wierzchołka i
    vector <int> List_of_vertices; // LICZYMY OD 0 !!! Po prostu lista wierzchołków jakie po sobie następują.
    double Last_length_of_cycle; // Dlugość cyklu która musi być aktualizowana po każdej zmianie

public:
    Population(Instance *MyInstance);
    void greedy_algorithm(int start);//Algorytm zachłanny od konkretnego wierzchołka - tworzy wektor
    void show_List_of_vertices();//Pokazuje zwykłą liste wierzchołków następujących po sobie.
    double distance_of_2_vertices(int first,int second); //odległość - podajemy numer (liczony od 1) pierwszego i drugiego

    void length_of_Cycle();//uaktualnia pole Last_length_of_cycle
    double getLast_length_of_cycle() const;

    void show_Table();//Pokazuje tablice krawędzi
    void make_Table_from_List(); //tworzy tablice poprzedników i nastepników

    Population* OX_crossover(Population& another_population, int start, int end);
    Population* AEX_crossover(Population& another_popultion);
    void mutation(); // Mutacja tego obiektu
    void algorithm_2opt_not_optimized(); // algorytm lokalnej optymalizacji, służy do ulepszania trasy
    void algorithm_2opt_optimized();

    string is_valid_cycle();
    string give_string_List_of_vertices();

    friend class Methaheuristic;
};


#endif //TSP_METAHEURISTIC_POPULATION_H
