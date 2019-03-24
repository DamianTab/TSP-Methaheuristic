//
// Created by Damian IT on 08.12.2018.
//

#include "Methaheuristic.h"
#include "Population.h"
#include "Instance.h"
#include <iostream>
#include <algorithm>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>

using namespace std;


Methaheuristic::Methaheuristic(Instance* init){
    BaseInstance=init;
    All_Populations.clear();
    Population* temp_population;

    for (int i = 1; i < BaseInstance->Cooridante_Table[0][0]+1 ; ++i) { // od i=1 do size+1 - dla kazdego wierzchołka
        temp_population = new Population(BaseInstance);
        temp_population->greedy_algorithm(i);
        All_Populations.insert(std::pair<double,Population*>(temp_population->getLast_length_of_cycle(),temp_population));
//        All_Populations[temp_population->getLast_length_of_cycle()]=temp_population; // TO jest to samo co linijke wyżej
    }
}

void Methaheuristic::show_Generation(int generation_number,bool show_all=false, bool show_vector=false) {
    /*
     * @param
     * show_all - pokazuje wszystkie populacje wraz z dlugościami
     * show_vector - pokazuje same wektory populacji
     *
     */
    cout<<endl<<"++++++++++++++++++++++++++++++++++++++++++"<<endl;
    cout<<"Oto obecne pokolenie: "<<generation_number<<endl;
    if(show_all){

        int counter=0;
        for (auto it=All_Populations.begin(); it != All_Populations.end(); it++) {
            cout<<counter<<". "<<"Dlugosc drogi wynosi: "<<it->first<<endl;
            if(show_vector) it->second->show_List_of_vertices();
            counter++;
        }
    }
    cout<<"Najlepsza sciezka dla tej generacji wynosi: "<<All_Populations.begin()->first<<endl;
    cout<<endl<<"++++++++++++++++++++++++++++++++++++++++++"<<endl;
}


void Methaheuristic::show_Length_vector() {
    for (int i = 0; i < Length_of_Population.size(); ++i) {
        cout << i << ". " << "Dlugosc drogi wynosi: " << Length_of_Population[i] << endl;
    }
}



void Methaheuristic::make_Length_vector() {
    Length_of_Population.clear();
    Length_of_Population.reserve(BaseInstance->Cooridante_Table[0][0]*2);
    for (auto it=All_Populations.begin(); it != All_Populations.end(); it++) {
        Length_of_Population.push_back(it->first);
    }
}


//Główne ciało algorytmy
void Methaheuristic::run(int time) {
    clock_t koniec = clock() + time * CLOCKS_PER_SEC;
    clock_t start_mutation = clock()+20*CLOCKS_PER_SEC;
    Population *Child1, *Child2;
    int counter=0, start, end ,which_crossover_operator; // start - poczatkowy wierzchołek krzyżowania OX, end - końcowy wierzchołek krzyżowania OX, counter - ilość iteracji

    while(clock()<koniec){     //petla ogólna która działą przez określoną ilość sekund
//    while(counter<0){

        while (All_Populations.size()>50){
            auto it = All_Populations.end();
            it--;
            All_Populations.erase(it);
        }
        make_Length_vector();


        //tutaj krzyżowanie
        for (int j = 0; j < 4; ++j) {
            for (int i = (Length_of_Population.size()*1/4*j); i < (Length_of_Population.size()*1/4*(j+1)-1); i+=1+j){
                which_crossover_operator=rand()%2;
                switch (which_crossover_operator){

                    //krzyzowanie OX
                    case 0:
                        do{
                            start=rand()%BaseInstance->Cooridante_Table[0][0]+1;
                            end=rand()%BaseInstance->Cooridante_Table[0][0]+1;
                        }while(start==end);

                        Child1 = All_Populations[Length_of_Population[i]]->OX_crossover(*All_Populations[Length_of_Population[i+1]],start, end);
//                        Child2 = All_Populations[Length_of_Population[i+1]]->OX_crossover(*All_Populations[Length_of_Population[i]],start, end);
                        Child1->algorithm_2opt_optimized(); //OPTYMALIZACJA LOKALNA
//                        Child2->algorithm_2opt_optimized(); //OPTYMALIZACJA LOKALNA
                        All_Populations[Child1->getLast_length_of_cycle()] = Child1; // dodawanie do mapy
//                        All_Populations[Child2->getLast_length_of_cycle()] = Child2; // dodawanie do mapy
                        break;



                        //krzyzowanie AEX
                    case 1:
                        Child1 = All_Populations[Length_of_Population[i]]->AEX_crossover(*All_Populations[Length_of_Population[i+1]]);
                        Child1->algorithm_2opt_optimized(); //OPTYMALIZACJA LOKALNA
                        All_Populations[Child1->getLast_length_of_cycle()] = Child1;
                        break;
                }
            }
        }

        //tutaj jest mutacja | co 50 wykonań mutuje zaczynając od 200 iteracji
//        if (counter>200 && counter%50==0){

        //tutaj mutacja zaczynająca się po 20sekundach co 10 sekund

        if (start_mutation<clock()){
            start_mutation = clock() + 10*CLOCKS_PER_SEC;
            int how_many_mutations = 7;
            for (int i = 0; i < how_many_mutations; ++i) {
                int x=rand()%Length_of_Population.size();

                Population* temp_population;
                temp_population = All_Populations[Length_of_Population[x]]; // potrzebna tablica długości cyklu
                auto it = All_Populations.find(Length_of_Population[x]); // potrzebna tablica długości cyklu
                All_Populations.erase(it);

                //tutaj wariant zeby nie zmienialo najlepszego wyniku
//                if (x==0) {
//                    Population* stay_the_best = new Population(BaseInstance);
//                    stay_the_best->List_of_vertices=temp_population->List_of_vertices;
//                    stay_the_best->length_of_Cycle();
//                    All_Populations[stay_the_best->getLast_length_of_cycle()]=stay_the_best;
//                    system("pause");
//                }
                temp_population->mutation();
                All_Populations[temp_population->getLast_length_of_cycle()]=temp_population;
                make_Length_vector();
            }
        }
        show_Generation(counter);
        counter++;
    }
}

void Methaheuristic::save_result_to_hall_of_fame(string path) {
    string name, best_value, worst_value, how_many_all_attempts, average_value_of_cycle;
    string is_cycle_valid,cycle;
    string entry_line;

    int best_result, worst_result;
    double number_of_all, number_of_average;

    ifstream file;
    ofstream file1;
    file.open("Best.txt");
    file1.open("temp.txt");

    //Hall of fame jest tak zapisany:
    //nazwa_instancji   najlepszy_wynik  ile_jest_wynikow_oddalonych_o5%    ilosc_wszystkich_wynikow    czy_poprawny    cykl

    if (!file.good())
    {
        cout << "Cannot open the file Best.txt :("<<endl;
        return ;
    }
    getline(file,entry_line);
    file1<<entry_line<<endl;

    while(!file.eof()){
        file>>name;
//        cout<<name<<endl;
        if (file.eof())break;
        file>>best_value>>worst_value>>how_many_all_attempts>>average_value_of_cycle>>is_cycle_valid>>cycle;
//        cout<<how_many_all_attempts<<endl;
        if(name==path){

            best_result = stoi(best_value);
            worst_result = stoi(worst_value);

            number_of_all = stod(how_many_all_attempts);
            number_of_average = stod(average_value_of_cycle);
            number_of_average *= number_of_all;
            number_of_all++;
            number_of_average += All_Populations.begin()->first;
            number_of_average /= number_of_all;

            if(All_Populations.begin()->first< best_result){
//                cout<<"1"<<endl;
                if (All_Populations.begin()->first > worst_result){

                    file1<<name<<"\t"<<All_Populations.begin()->first<<"\t"<<All_Populations.begin()->first<<"\t"<<number_of_all<<"\t"
                         <<number_of_average<<"\t"
                         <<All_Populations.begin()->second->is_valid_cycle()<<"\t"<<All_Populations.begin()->second->give_string_List_of_vertices()<<endl;
//                    cout<<"2"<<endl;

                }else {
                    file1 << name << "\t" << All_Populations.begin()->first << "\t" << worst_value << "\t"
                          << number_of_all << "\t"
                          << number_of_average << "\t"
                          << All_Populations.begin()->second->is_valid_cycle() << "\t"
                          << All_Populations.begin()->second->give_string_List_of_vertices() << endl;
//                    cout << "3" << endl;
                }


            } else if(All_Populations.begin()->first > worst_result){
                file1<<name<<"\t"<<best_value<<"\t"<<All_Populations.begin()->first<<"\t"<<number_of_all<<"\t"
                     <<number_of_average<<"\t"<<is_cycle_valid<<"\t"<<cycle<<endl;
//                cout<<"4"<<endl;
            }
            else{
                file1<<name<<"\t"<<best_value<<"\t"<<worst_value<<"\t"<<number_of_all<<"\t"
                     <<number_of_average<<"\t"<<is_cycle_valid<<"\t"<<cycle<<endl;
//                cout<<"4.5"<<endl;
            }
        } else {
            file1 << name << "\t" << best_value << "\t" << worst_value << "\t" << how_many_all_attempts << "\t"
                  << average_value_of_cycle << "\t" << is_cycle_valid << "\t" << cycle << endl;
//            cout << "5" << endl;
        }
        }
    file.close();
    file1.close();
    remove("Best.txt");
    rename("temp.txt","Best.txt");
}






