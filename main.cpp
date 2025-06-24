#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <omp.h>
#include <ctime>

//Libreria funciones auxiliares
#include "UtilsFunctions.h"

unsigned t0, t1; //BORRAR

int main(){
    std::ifstream file("eldoria.csv");
    t0 = clock(); //BORRAR
    if (!file.is_open()){
        std::cerr << "Error: No se puede abrir el archivo" << std::endl;
        return 1;
    };

    int count = 0;
    std::vector <int> personasPorEstrato = {0,0,0,0,0,0,0,0,0,0};
    std::vector <float> percentPorEstrato = {0,0,0,0,0,0,0,0,0,0};

    std::map <std::string, int> personasPorEspecie = { //AUX
        {"Humana", 0}, {"Elfica", 0}, {"Enana", 0}, {"Hombre Bestia", 0}
    };
    std::map <std::string, int> personasPorGenero = { //AUX
        {"MACHO", 0}, {"HEMBRA", 0}, {"OTRO", 0}
    };
    std::vector <int> edadesHuman, edadesElf, edadesEnana, edadesHBestia;
    std::vector <int> edadesMacho, edadesHembra, edadesOtro;

    std::string headers;
    std::getline(file, headers);

    #pragma omp parallel
    {
        std::string linea;
        while (count<10000000){ //Change condition -> std::getline(file, linea)
            #pragma omp critical
            {
                if (count<10000000 && std::getline(file, linea)){ //Delete count condition count<5 && 
                    std::vector <std::string> tokens = SplitStr(linea, ';');
                    personasPorEstrato = ExtraerEstrato(tokens[6], personasPorEstrato); //P.1-2
                    
                    int edad = CalcularEdad(tokens[5]); //P3
                    std::string especie = tokens[1];
                    std::string genero = tokens[2];
                    especie.erase(0,1); //Eliminamos la comilla doble del inicio.
                    especie.pop_back(); //Eliminamos la comilla doble del final.
                    genero.erase(0,1);
                    genero.pop_back();
                    if (especie == "Humana"){
                        edadesHuman.push_back(edad);
                        personasPorEspecie["Humana"] += 1;
                    } else if (especie == "Elfica"){
                        edadesElf.push_back(edad);
                        personasPorEspecie["Elfica"] += 1;
                    } else if (especie == "Enana"){
                        edadesEnana.push_back(edad);
                        personasPorEspecie["Enana"] += 1;
                    } else if (especie == "Hombre Bestia"){
                        edadesHBestia.push_back(edad);
                        personasPorEspecie["Hombre Bestia"] += 1;
                    }
                    if (genero == "MACHO"){
                        edadesMacho.push_back(edad);
                        personasPorGenero["MACHO"] +=1;
                    } else if (genero == "HEMBRA"){
                        edadesHembra.push_back(edad);
                        personasPorGenero["HEMBRA"] +=1;
                    } else if (genero == "OTRO"){
                        edadesOtro.push_back(edad);
                        personasPorGenero["OTRO"] +=1;
                    }
                    count++;
                } else {
                    linea = "";
                }
            }
        }  
    }
    t1 = clock(); //BORRAR
    double time = (double(t1-t0)/CLOCKS_PER_SEC); //BORRAR
    std::cout << "Tiempo Ejecución:" << time << std::endl; //BORRAR
    std::cout << "Total de datos:" << count << std::endl; //BORRAR

    //RESPUESTAS----------
    std::cout << "-----\nRespuestas:" << std::endl;

    //1. Personas por estrato
    std::cout << "1. Personas por estrato:" << std::endl;
    for(int i=0; i<10; i++){
        std::cout << "   Personas en estrato " << i << ": " << personasPorEstrato[i] << std::endl;
    }
    std::cout << "-----" << std::endl;

    //2.Porcentaje por estrato
    float countDb = count;
    std::cout << "2. Porcentaje por estrato:" << std::endl;
    for (int i=0; i<10; i++){
        percentPorEstrato[i] = (personasPorEstrato[i]/countDb)*100;
        std::cout << "   Porcentaje estrato "<< i << ": " << percentPorEstrato[i] << "%" << std::endl;
    }
    std::cout << "-----" << std::endl;

    //3. Edad promedio por especie y genero
    std::cout << "3. Edades promedio por especie y género:" << std::endl;
    std::cout << "   Edad promedio especie Humana: " << EdadPromedio(edadesHuman, personasPorEspecie["Humana"]) << std::endl;
    std::cout << "   Edad promedio especie Elfica: " << EdadPromedio(edadesElf, personasPorEspecie["Elfica"]) << std::endl;
    std::cout << "   Edad promedio especie Enana: " << EdadPromedio(edadesEnana, personasPorEspecie["Enana"]) << std::endl;
    std::cout << "   Edad promedio especie Hombre Bestia: " << EdadPromedio(edadesHBestia, personasPorEspecie["Hombre Bestia"]) << std::endl;
    std::cout << std::endl;
    std::cout << "   Edad promedio género Macho: " << EdadPromedio(edadesMacho, personasPorGenero["MACHO"]) << std::endl;
    std::cout << "   Edad promedio género Hembra: " << EdadPromedio(edadesHembra, personasPorGenero["HEMBRA"]) << std::endl;
    std::cout << "   Edad promedio género Otro: " << EdadPromedio(edadesOtro, personasPorGenero["OTRO"]) << std::endl;


    file.close();
    return 0;
}