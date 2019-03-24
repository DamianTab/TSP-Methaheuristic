//
// Created by Damian IT on 07.12.2018.
//

#include "Population.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include "Instance.h"
#include <cstdlib>
#include <ctime>

using namespace std;

Population::Population(Instance *init){
    MyInstance = init;
}


//TUTAJ ALGORYTM ZACHŁANNY
void Population::greedy_algorithm(int start) {

    bool *visited = new bool[MyInstance->Cooridante_Table[0][0]+1];     //    MyInstance->Cooridante_Table[0][0]+1 - inaczej SIZE+1 - indeksujemy od 1
    int counter = 0, current_vertex = start;
    double *dist = new double[MyInstance->Cooridante_Table[0][0]+1];
    double minimum = numeric_limits < double >::max();
    List_of_vertices.clear();
    List_of_vertices.reserve(MyInstance->Cooridante_Table[0][0]);//Tutaj wystrczy size
    List_of_vertices.push_back(current_vertex); // wstawia początowy wierzchołek

    while (counter != MyInstance->Cooridante_Table[0][0]-1)// Trzeba jeszcze wybrac n-1 wierzchołków
    {
        visited[current_vertex] = true;
        for (int j = 1; j < MyInstance->Cooridante_Table[0][0]+1; j++)
        {
            if ((current_vertex == j) || (visited[j] == true))dist[j] = NULL;
            else dist[j] = distance_of_2_vertices(current_vertex,j);
        }

        minimum = numeric_limits < double >::max();

        for (int i = 1; i < MyInstance->Cooridante_Table[0][0]+1; i++)
        {
            if (minimum > dist[i] && dist[i] != NULL)
            {
                minimum = dist[i];
                current_vertex=i;
            }
        }
        List_of_vertices.push_back(current_vertex);
        counter++;
    }
    length_of_Cycle();
}

void Population::show_Table() {
    cout<<"--------------------------------------------"<<endl;
    cout<<"Ilosc krawedzi: "<<Table_of_vertices[0][0]<<endl;
    cout<<"Wierzcholek | Poprzednik | Nastepnik"<<endl;
    cout<<"--------------------------------------------"<<endl;
    for (int i = 1; i < Table_of_vertices[0][0]+1; i++)
    {
        cout<<i<<" | "<<Table_of_vertices[i][0]<<" | "<<Table_of_vertices[i][1]<<endl;
    }
    cout<<"--------------------------------------------"<<endl;
}


void Population::show_List_of_vertices() {
    cout<<"Cykl Hamiltona: ";
    std::copy(List_of_vertices.begin(), List_of_vertices.end(),
              std::ostream_iterator<int>(std::cout, " "));
    cout<<endl<<endl;
}

double Population::distance_of_2_vertices(int first, int second) {
    double value=-999999999999; //jakby coś się nie udało to będzie to od razu widoczne w testach

   // MyInstance->Cooridante_Table[0][1] = 0; ABY TO DZIALALO MUSI BYC FRIEND W KLASIE INSTANCE

    value = sqrt((MyInstance->Cooridante_Table[first][0] - MyInstance->Cooridante_Table[second][0])*
            (MyInstance->Cooridante_Table[first][0] - MyInstance->Cooridante_Table[second][0]) +
            (MyInstance->Cooridante_Table[first][1] - MyInstance->Cooridante_Table[second][1])*
            (MyInstance->Cooridante_Table[first][1] - MyInstance->Cooridante_Table[second][1]));
    return value;
}

void Population::length_of_Cycle() {
    double total_length=0;
    for (int i = 0; i < MyInstance->Cooridante_Table[0][0]-1; ++i) { // przy wektorze liczymy od 0
    total_length+=distance_of_2_vertices(List_of_vertices[i],List_of_vertices[i+1]);
    }
    total_length+=distance_of_2_vertices(List_of_vertices[MyInstance->Cooridante_Table[0][0]-1],List_of_vertices[0]); //Ostatnia krawędź
    Last_length_of_cycle=total_length;
}

void Population::make_Table_from_List() {
    Table_of_vertices=new int* [MyInstance->Cooridante_Table[0][0]+1];
    Table_of_vertices[0]=new int[1];
    Table_of_vertices[0][0]=MyInstance->Cooridante_Table[0][0]; // I TU, I TU PRZECHOWUJEMY SIZE

    for (int i = 1; i < Table_of_vertices[0][0]+1; ++i) {
        Table_of_vertices[i]=new int[2];
    }

    //Nastepniki
    for (int j = 0; j < Table_of_vertices[0][0]-1; ++j) {
        Table_of_vertices[List_of_vertices[j]][1]=List_of_vertices[j+1]; // DLA WIERZCHOŁKA NR 1 (j=1) przyporzodkuje drugi element na liscie wektora - bo wektor liczy od 0 a tablica od 1
    }
    Table_of_vertices[List_of_vertices[Table_of_vertices[0][0]-1]][1]=List_of_vertices[0];  // następnikiem ostatniego jest pierwszy wierzchołek
    //Poprzedniki
    for (int j = Table_of_vertices[0][0]-1; j > 0 ; --j) {
        Table_of_vertices[List_of_vertices[j]][0]=List_of_vertices[j-1]; // DLA WIERZCHOŁKA NR 5 (j=5) przyporzodkuje czwarty element na liscie wektora - bo wektor liczy od 0 a tablica od 1
    }
    Table_of_vertices[List_of_vertices[0]][0]=List_of_vertices[Table_of_vertices[0][0]-1]; // poprzednik pierwszego jest ostatnim elementem w wektorze
}

double Population::getLast_length_of_cycle() const {
    return Last_length_of_cycle;
}


void Population::algorithm_2opt_not_optimized(){

    Population* temp_population;
    int* edges_to_change = new int[2];
    double best_cycle_distant = Last_length_of_cycle;
    vector<int> helper;

//    cout<<"TO jest dlugosc przed: "<<best_cycle_distant<<endl;

    for (int i = 0; i < MyInstance->Cooridante_Table[0][0] -2; ++i) { // TAk na prawde to indeksujemy tutaj od 0
        for (int k = i + 1; k < MyInstance->Cooridante_Table[0][0] -1; ++k) {// Tutaj rowniez
            temp_population = new Population(MyInstance);

            temp_population->List_of_vertices.assign(this->List_of_vertices.begin()+i,this->List_of_vertices.begin()+k+1); // podajemy iterator poczatkowy oraz iterator za interesującym nas elementem
            std::reverse(temp_population->List_of_vertices.begin(),temp_population->List_of_vertices.end());

            temp_population->List_of_vertices.insert (temp_population->List_of_vertices.begin(), this->List_of_vertices.begin(), this->List_of_vertices.begin()+i); // miejsce wstawienia, poczatek ciagu, koniec ciagu

            temp_population->List_of_vertices.insert(temp_population->List_of_vertices.end(),this->List_of_vertices.begin()+k+1,this->List_of_vertices.end());// miejsce wstawienia, poczatek ciagu, koniec ciagu

            temp_population->length_of_Cycle();
            if (best_cycle_distant > temp_population->Last_length_of_cycle){
                best_cycle_distant=temp_population->Last_length_of_cycle;
                edges_to_change[0]=i;
                edges_to_change[1]=k;
            }
            delete temp_population;
        }
    }
    if(best_cycle_distant!=Last_length_of_cycle){
//        cout<<"TAK TO JEST LEPSZA DLUGOSC: "<<best_cycle_distant<<endl;
        int i = edges_to_change[0];
        int k = edges_to_change[1];
        helper.assign(this->List_of_vertices.begin()+i,this->List_of_vertices.begin()+k+1);
        std::reverse(helper.begin(),helper.end());
        helper.insert(helper.begin(), this->List_of_vertices.begin(), this->List_of_vertices.begin()+i);
        helper.insert(helper.end(),this->List_of_vertices.begin()+k+1,this->List_of_vertices.end());
        this->List_of_vertices=helper;
        this->Last_length_of_cycle=best_cycle_distant;
    }
}

void Population::algorithm_2opt_optimized() {

    int* edges_to_change = new int[2];
    double old_distant,new_distant,difference_of_new_and_old_distant, best_difference=0;
//        cout<<"TO jest dlugosc przed: "<<Last_length_of_cycle<<endl;

    //są cztery mozliwości i==0,k==max;   i==0,k-dowolne;    i-dowolne,k==max;    i,k - dowolne różne od reszty przypadków.
    for (int i = 0; i < MyInstance->Cooridante_Table[0][0] -1; ++i) { // TAk na prawde to indeksujemy tutaj od 0
        for (int k = i + 1; k < MyInstance->Cooridante_Table[0][0]; ++k) {// Tutaj rowniez

            if(i==0){
                if(k==MyInstance->Cooridante_Table[0][0]-1) continue; // przypadek i==0,k==0
                else{// przypadek i==0, k - dowolne
                    old_distant = distance_of_2_vertices(List_of_vertices[MyInstance->Cooridante_Table[0][0]-1],List_of_vertices[0]) +
                                  distance_of_2_vertices(List_of_vertices[k],List_of_vertices[k+1]);
                    new_distant = distance_of_2_vertices(List_of_vertices[MyInstance->Cooridante_Table[0][0]-1],List_of_vertices[k]) +
                                  distance_of_2_vertices(List_of_vertices[0],List_of_vertices[k+1]);
                }
            }
            else if(k==MyInstance->Cooridante_Table[0][0]-1){ //przypadek i - dowolne, k==max
                old_distant = distance_of_2_vertices(List_of_vertices[i-1],List_of_vertices[i]) +
                              distance_of_2_vertices(List_of_vertices[MyInstance->Cooridante_Table[0][0]-1],List_of_vertices[0]);
                new_distant = distance_of_2_vertices(List_of_vertices[i-1],List_of_vertices[MyInstance->Cooridante_Table[0][0]-1]) +
                              distance_of_2_vertices(List_of_vertices[i],List_of_vertices[0]);
            }
            else{// przypadek i,k -dowolne
                old_distant = distance_of_2_vertices(List_of_vertices[i-1],List_of_vertices[i]) +
                              distance_of_2_vertices(List_of_vertices[k],List_of_vertices[k+1]);
                new_distant = distance_of_2_vertices(List_of_vertices[i-1],List_of_vertices[k]) +
                              distance_of_2_vertices(List_of_vertices[i],List_of_vertices[k+1]);
            }


            difference_of_new_and_old_distant = new_distant-old_distant; // jeżeli jest na - to oznacza ze odległość się zmniejszyła
            if (difference_of_new_and_old_distant<best_difference){ // jeżeli odlełgość jest mniejsza od najmniejsziej odległości to staję się najmniejszą odległością - czyli najlepszym wynikiem
                best_difference=difference_of_new_and_old_distant;
                edges_to_change[0]=i;
                edges_to_change[1]=k;
            }
        }
    }


    if(best_difference!=0){
        vector<int>helper;
//        cout<<"TAK TO JEST najmniejsza rónica: "<<best_difference<<endl;
        int i = edges_to_change[0];
        int k = edges_to_change[1];
        helper.assign(this->List_of_vertices.begin()+i,this->List_of_vertices.begin()+k+1);
        std::reverse(helper.begin(),helper.end());
        helper.insert(helper.begin(), this->List_of_vertices.begin(), this->List_of_vertices.begin()+i);
        helper.insert(helper.end(),this->List_of_vertices.begin()+k+1,this->List_of_vertices.end());
        this->List_of_vertices=helper;
        this->length_of_Cycle();
//        cout<<"TAK TO JEST długosc: "<<Last_length_of_cycle<<"  i sprawdzenie(roznica): "<<Last_length_of_cycle-best_difference<<endl;
    }
}

void Population::mutation() {

    for (int i = 0; i < 1; ++i) {
        int x = rand()%List_of_vertices.size();
        int y = rand()%List_of_vertices.size();
        swap(List_of_vertices[x],List_of_vertices[y]);
    }
    length_of_Cycle();
}

Population *Population::OX_crossover(Population &another_population,int start, int end) {


    if (start>end) swap(start,end);
    Population* Child = new Population(MyInstance);
    Child->List_of_vertices.clear();
    Child->List_of_vertices.assign(this->List_of_vertices.begin()+start,this->List_of_vertices.begin()+end);

    //wyswietlanie
//    std::copy(Child->List_of_vertices.begin(), Child->List_of_vertices.end(),
//              std::ostream_iterator<int>(std::cout, " "));
//    cout<<endl;

    for (int i = 0; i < another_population.List_of_vertices.size(); ++i) {
        if (std::find(Child->List_of_vertices.begin(), Child->List_of_vertices.end(), another_population.List_of_vertices[i]) == Child->List_of_vertices.end()) // Jeżeli nie ma takiego wierzchołka w dziecku to dodaje
                Child->List_of_vertices.push_back(another_population.List_of_vertices[i]);
    }
    Child->length_of_Cycle();
    return Child;
}

Population *Population::AEX_crossover(Population &another_popultion) {

    Population* Child = new Population(MyInstance);
    vector<int> not_visited;
    Child->List_of_vertices.clear();
    //Przepisuje dwa pierwsze wierzchołki
    Child->List_of_vertices.assign(this->List_of_vertices.begin(),this->List_of_vertices.begin()+2);
    not_visited.assign(this->List_of_vertices.begin()+2,this->List_of_vertices.end());

    this->make_Table_from_List();
    another_popultion.make_Table_from_List();

    int current_value = Child->List_of_vertices[1], next_value ;
    std::vector<int>::iterator it;
    int which_parent_now=2;

    while (!not_visited.empty()){ //jeszcze jakis wierzcholek nie wykorzystany


        if (which_parent_now == 1){
            //szukanie w Parent1
            next_value = this->Table_of_vertices[current_value][1];
            if (std::find(Child->List_of_vertices.begin(), Child->List_of_vertices.end(), next_value) == Child->List_of_vertices.end() ){ //jesli nie znajduje sie w dziecku to:
                it = std::find(not_visited.begin(), not_visited.end(), next_value); //znajduje pozycje
                not_visited.erase(it);
                Child->List_of_vertices.push_back(next_value);
                current_value=next_value;
                which_parent_now=2; //na zmiane
            } else {
                // Jeśli znajduje sie w dziecku to:
                int random_value = rand()%not_visited.size();
                next_value = not_visited[random_value];
                not_visited.erase(not_visited.begin()+random_value);
                Child->List_of_vertices.push_back(next_value);
                current_value=next_value;
                which_parent_now = 1; //na zmiane
            }

        }
        else
        {

            //szukanie w Parent2
            next_value = another_popultion.Table_of_vertices[current_value][1];

            if (std::find(Child->List_of_vertices.begin(), Child->List_of_vertices.end(), next_value) == Child->List_of_vertices.end() ){ //jesli nie znajduje sie w dziecku to:
                it = std::find(not_visited.begin(), not_visited.end(), next_value); //znajduje pozycje
                not_visited.erase(it);
                Child->List_of_vertices.push_back(next_value);
                current_value=next_value;
                which_parent_now=1;//na zmiane
            } else {
                // Jeśli znajduje sie w dziecku to:
                int random_value = rand()%not_visited.size();
                next_value = not_visited[random_value];
                not_visited.erase(not_visited.begin()+random_value);
                Child->List_of_vertices.push_back(next_value);
                current_value=next_value;
                which_parent_now = 1;//na zmiane
            }
        }
    }
    Child->length_of_Cycle();
    return Child;
}

string Population::is_valid_cycle() {
    if(List_of_vertices.size()!=MyInstance->Cooridante_Table[0][0]) return "false";
//    cout<<"ilosc jest ok"<<endl;
    for (int i = 1; i < MyInstance->Cooridante_Table[0][0]+1; ++i) {
        if (find(List_of_vertices.begin(),List_of_vertices.end(),i)==List_of_vertices.end())return "false";
//        cout<<"Jest wierzcholek nr "<<i<<endl;
    }
    return "true";
}


string Population::give_string_List_of_vertices() {
    string result="<";
    for (int i = 0; i < List_of_vertices.size(); ++i) {
        result += to_string(List_of_vertices[i]);
        if (i!=List_of_vertices.size()-1) result +=",";
    }
    result +=">";

    return result;
}
