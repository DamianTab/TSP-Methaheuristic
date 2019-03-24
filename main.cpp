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
    cout<<"TO JEST ARG: "<<argv[1]<<endl;
    path=in.load(argv[1]);
    Methaheuristic meta(&in);
    meta.run(stoi(argv[2]));
    meta.show_Generation(-1);


//    Population lol(&in),lol1(&in);
//    lol.greedy_algorithm(8);
//    lol1.greedy_algorithm(8);
//    lol.show_List_of_vertices();
//    lol.algorithm_2opt_not_optimized();
//    cout<<"LOL: "<<lol.getLast_length_of_cycle()<<endl;
//    lol1.algorithm_2opt_optimized();

    meta.save_result_to_hall_of_fame(path);
//    system("pause");

    return 0;
}