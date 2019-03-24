#include <iostream>
#include "Population.cpp"
#include "Instance.cpp"
#include "Methaheuristic.cpp"
#include <cstdlib>
#include <ctime>

using namespace std;



int main(int argc, char *argv[]){
    string path;
    srand( time( NULL ) );
    Instance in;
//    cout<<"TO JEST ARG: "<<argv[1]<<endl;
    path=in.load();
    Methaheuristic meta(&in);
    meta.run(30);
    meta.show_Generation(-1);


    meta.save_result_to_hall_of_fame(path);
    system("pause");

    return 0;
}