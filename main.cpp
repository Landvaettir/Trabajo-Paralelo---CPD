#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <omp.h>

//Libreria funciones auxiliares
#include "UtilsFunctions.h"

#include <ctime>
unsigned t0, t1; //BORRAR

int main(){
    std::ifstream file("eldoria.csv");
    t0 = clock(); //BORRAR
    if (!file.is_open()){
        std::cerr << "Error: No se puede abrir el archivo" << std::endl;
        return 1;
    };

    int count = 0; //BORRAR
    std::string headers;
    std::getline(file, headers);

    std::vector <int> personasPorEstrato = {0,0,0,0,0,0,0,0,0,0};

    #pragma omp parallel
    {
        std::string linea;
        while (count<5){ //Change condition -> std::getline(file, linea)
            #pragma omp critical
            {
                if (count<5 && std::getline(file, linea)){ //Delete count condition
                    std::cout << linea << std::endl;
                    std::vector <std::string> tokens = SplitStr(linea, ';');
                    personasPorEstrato = ExtraerEstrato(tokens[6], personasPorEstrato);
                    count++;
                } else {
                    linea = "";
                }
            }
        }  
    }
    t1 = clock();
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    std::cout << time << std::endl; //BORRAR
    std::cout << count << std::endl; //BORRAR

    for(int estrat : personasPorEstrato){
        std::cout << estrat << " ";
    }

    file.close();
    return 0;
}