//
// Created by Damian IT on 08.12.2018.
//

#ifndef TSP_METAHEURISTIC_METHAHEURISTIC_H
#define TSP_METAHEURISTIC_METHAHEURISTIC_H

#include "Population.h"
#include <map>



class Methaheuristic {
private:
    map <int , Population*> All_Populations; //mapa wskaźników populacji - kluczem jest długość cyklu danej populacji - jest od razu posortowana - nie zawiera takich samych kluczy czyli populacje o tym samym cyklu nie istnieją, zwieksza to szybkość algorytmu
    // UWAGA DOUBLE POWODUJE TWORZENIE TAKICH SAMYCH KLUCZY DLATEGO INT
    Instance* BaseInstance; // wskaźnik na instancje gdzie jest tablica współrzędnych punktów (wierzchołków)
    vector<double> Length_of_Population; // wektor długości populacji potrzebny do identyfikacji elementów w mapie
public:
    Methaheuristic(Instance* init);
    void show_Generation(int generation_number,bool show_all ,bool show_vector);
    void run(int time); // GŁÓWNE CIAŁO PROGRAMU
    void make_Length_vector();
    void show_Length_vector();

    void save_result_to_hall_of_fame(string path);
public:
};


#endif //TSP_METAHEURISTIC_METHAHEURISTIC_H
