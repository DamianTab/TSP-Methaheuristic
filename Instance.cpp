//
// Created by Damian IT on 08.12.2018.
//

#include "Instance.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;


/*
 * Instancja ładuje się w następującym trybie:
 * size
 * Number X Y
 * .
 * .
 * .
 * Number X Y
 */

string Instance::load(string path="") {
    fstream file;
    if(path=="") {
        cout<<"--- Prosze wpisac sciezke pliku do odczytu: "<<endl;
        cin >> path;
    }
//    path="berlin52.txt";
    file.open(path);
    if (!file.good())
    {
        cout << "Cannot open the file"<<endl;
        return 0;
    }

    int size,trash;
    file >> size;
    Cooridante_Table = new int*[size+1];
    Cooridante_Table[0] = new int[1]; // Pierwszy element w instancji jest wielkością tej instancji.
    Cooridante_Table[0][0] = size;

    for (int i = 1; i < size+1; i++)
    {
        Cooridante_Table[i] = new int[2];
        file >> trash; // W instancji są jeszcze indeksy, żeby nie zniszczyły kodu po prostu je pomijamy.
        file>>Cooridante_Table[i][0]>>Cooridante_Table[i][1];
    }
    file.close();
    return path;
}

void Instance::save() {
    fstream file;
    string path;
    cout<<"--- Prosze wpisac sciezke pliku do zapisu: "<<endl;
    cin >> path;
    file.open(path);
    if (!file.good())
    {
        cout << "Cannot open the file"<<endl;
        return;
    }
    file<<Cooridante_Table[0][0]<<endl;
    for (int i = 1; i < Cooridante_Table[0][0]+1; i++)
    {
        file<<i<<" "<<Cooridante_Table[i][0]<<" "<<Cooridante_Table[i][1]<<endl;
    }
    file.close();
}

void Instance::show_Instance(){
    cout<<"--------------------------------------------"<<endl;
    cout<<Cooridante_Table[0][0]<<endl;
    for (int i = 1; i < Cooridante_Table[0][0]+1; i++)
    {
        cout<<i<<" "<<Cooridante_Table[i][0]<<" "<<Cooridante_Table[i][1]<<endl;
    }
    cout<<"--------------------------------------------"<<endl;
}

void Instance::generate() {
    int size,range;
    cout<<"Wpisz rozmiar instancji: ";
    cin>>size;
    cout<<endl<<"Wpisz do jakiej maksymalnej wartosci moga byc wspolrzedne wierzcholkow: ";
    cin>>range;
    Cooridante_Table = new int*[size+1];
    Cooridante_Table[0] = new int[1]; // Pierwszy element w instancji jest wielkością tej instancji.
    Cooridante_Table[0][0] = size;

    for (int i = 1; i < size+1; i++)
    {
        Cooridante_Table[i] = new int[2];
        Cooridante_Table[i][0]=rand()%range;
        Cooridante_Table[i][1]=rand()%range;
    }
}

