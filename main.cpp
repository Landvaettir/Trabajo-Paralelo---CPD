#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <ctime>
#include <omp.h>

//Libreria funciones auxiliares
#include "UtilsFunctions.h"

int main(){
    std::ifstream file("eldoria.csv");
    if (!file.is_open()){
        std::cerr << "Error: No se puede abrir el archivo" << std::endl;
        return 1;
    };

    int count = 0;
    bool flag = true;
    std::vector <int> personasPorEstrato = {0,0,0,0,0,0,0,0,0,0};
    std::vector <float> percentPorEstrato = {0,0,0,0,0,0,0,0,0,0};

    std::map <std::string, int> personasPorEspecie = {
        {"Humana", 0}, {"Elfica", 0}, {"Enana", 0}, {"Hombre Bestia", 0}
    };
    std::map <std::string, int> personasPorGenero = {
        {"MACHO", 0}, {"HEMBRA", 0}, {"OTRO", 0}
    };
    std::vector <int> edadesHuman, edadesElf, edadesEnana, edadesHBestia;
    std::vector <int> edadesMacho, edadesHembra, edadesOtro;

    int dependientes = 0;
    int independientes = 0;

    std::map <std::string, int> visitasPoblados;
    std::vector<std::pair<std::string, int>> top10000Poblados;

    std::string headers;
    std::getline(file, headers);

    #pragma omp parallel
    {
        std::string linea;
        while (flag){ //Change condition -> flag
            #pragma omp critical
            {
                if (std::getline(file, linea)){ 
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
                        personasPorGenero["MACHO"] += 1;
                    } else if (genero == "HEMBRA"){
                        edadesHembra.push_back(edad);
                        personasPorGenero["HEMBRA"] += 1;
                    } else if (genero == "OTRO"){
                        edadesOtro.push_back(edad);
                        personasPorGenero["OTRO"] += 1;
                    }

                    if ((edad<15) || (edad>64)){ //P.7
                        dependientes += 1;
                    } else {
                        independientes += 1;
                    }

                    VisitasCiudad(tokens[7], visitasPoblados); //P.8
                    count++;
                } else {
                    flag = false;
                }
            }
        }
    }
    file.close();
    
    //Ordenamiento de los vectores de edad - P.4
    std::sort(edadesHuman.begin(), edadesHuman.end());
    std::sort(edadesElf.begin(), edadesElf.end());
    std::sort(edadesEnana.begin(), edadesEnana.end());
    std::sort(edadesHBestia.begin(), edadesHBestia.end());

    std::sort(edadesMacho.begin(), edadesMacho.end());
    std::sort(edadesHembra.begin(), edadesHembra.end());
    std::sort(edadesOtro.begin(), edadesOtro.end());

    //Segmentación de edades - P.5-6
    std::map <std::string, int> edadesHumanSegm, edadesElfSegm, edadesEnanaSegm, edadesHBestiaSegm;
    std::map <std::string, int> edadesMachoSegm, edadesHembraSegm, edadesOtroSegm;

    edadesHumanSegm = SegmentarEdad(edadesHuman, edadesHumanSegm);
    edadesElfSegm = SegmentarEdad(edadesElf, edadesElfSegm);
    edadesEnanaSegm = SegmentarEdad(edadesEnana, edadesEnanaSegm);
    edadesHBestiaSegm = SegmentarEdad(edadesHBestia, edadesHBestiaSegm);

    edadesMachoSegm = SegmentarEdad(edadesMacho, edadesMachoSegm);
    edadesHembraSegm = SegmentarEdad(edadesHembra, edadesHembraSegm);
    edadesOtroSegm = SegmentarEdad(edadesOtro, edadesOtroSegm);

    //Extraer top 10000 poblados más visitados
    top10000Poblados = ExtraerTop10000(visitasPoblados);

    std::cout << "Total de datos:" << count << std::endl; //BORRAR
/*
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
    std::cout << "-----" << std::endl;

    //4. Edad mediana por especie y genero
    std::cout << "4. Edades mediana por especie y género:" << std::endl;
    std::cout << "   Edad mediana especie Humana: " << EdadMediana(edadesHuman) << std::endl;
    std::cout << "   Edad mediana especie Elfica: " << EdadMediana(edadesElf) << std::endl;
    std::cout << "   Edad mediana especie Enana: " << EdadMediana(edadesEnana) << std::endl;
    std::cout << "   Edad mediana especie Hombre Bestia: " << EdadMediana(edadesHBestia) << std::endl;
    std::cout << std::endl;
    std::cout << "   Edad mediana género Macho: " << EdadMediana(edadesMacho) << std::endl;
    std::cout << "   Edad mediana género Hembra: " << EdadMediana(edadesHembra) << std::endl;
    std::cout << "   Edad mediana género Otro: " << EdadMediana(edadesOtro) << std::endl;
    std::cout << "-----" << std::endl;

    //5. Proporción segmentada de la población
    std::cout << "5. Proporción segmentada de la población según especie y género:" << std::endl;
    std::cout << "   Especie Humana: " << std::endl << "    ";
    float size = edadesHuman.size();
    for(auto& pair:edadesHumanSegm){
        std::cout << pair.first << ":" << (pair.second/size)*100 << "%" << " | ";
    }
    std::cout << std::endl;
    std::cout << "   Especie Elfica: " << std::endl << "    ";
    size = edadesElf.size();
    for(auto& pair:edadesElfSegm){
        std::cout << pair.first << ":" << (pair.second/size)*100 << "%" << " | ";
    }
    std::cout << std::endl;
    std::cout << "   Especie Enana: " << std::endl << "    ";
    size = edadesEnana.size();
    for(auto& pair:edadesEnanaSegm){
        std::cout << pair.first << ":" << (pair.second/size)*100 << "%" << " | ";
    }
    std::cout << std::endl;
    std::cout << "   Especie Hombre Bestia: " << std::endl << "    ";
    size = edadesHBestia.size();
    for(auto& pair:edadesHBestiaSegm){
        std::cout << pair.first << ":" << (pair.second/size)*100 << "%" << " | ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "   Género Macho: " << std::endl << "    ";
    size = edadesMacho.size();
    for(auto& pair:edadesMachoSegm){
        std::cout << pair.first << ":" << (pair.second/size)*100 << "%" << " | ";
    }
    std::cout << std::endl;
    std::cout << "   Género Hembra: " << std::endl << "    ";
    size = edadesHembra.size();
    for(auto& pair:edadesHembraSegm){
        std::cout << pair.first << ":" << (pair.second/size)*100 << "%" << " | ";
    }
    std::cout << std::endl;
    std::cout << "   Género Otro: " << std::endl << "    ";
    size = edadesOtro.size();
    for(auto& pair:edadesOtroSegm){
        std::cout << pair.first << ":" << (pair.second/size)*100 << "%" << " | ";
    }
    std::cout << std::endl;
    std::cout << "-----" << std::endl;

    //6. Pirámide de edades
    std::cout << "6. Pirámide de edades:" << std::endl;
    std::cout << "   Especie Humana" <<std::endl;
    for (auto& segm : edadesHumanSegm){
        std::cout << "     " << segm.first << ":" << segm.second << " personas" << std::endl;
    }
    std::cout << "   Especie Elfica" <<std::endl;
    for (auto& segm : edadesElfSegm){
        std::cout << "     " << segm.first << ":" << segm.second << " personas" << std::endl;
    }
    std::cout << "   Especie Enana" <<std::endl;
    for (auto& segm : edadesEnanaSegm){
        std::cout << "     " << segm.first << ":" << segm.second << " personas" << std::endl;
    }
    std::cout << "   Especie Hombre Bestia" <<std::endl;
    for (auto& segm : edadesHBestiaSegm){
        std::cout << "     " << segm.first << ":" << segm.second << " personas" << std::endl;
    }
    std::cout << std::endl;
    std::cout << "   Género Macho" <<std::endl;
    for (auto& segm : edadesMachoSegm){
        std::cout << "     " << segm.first << ":" << segm.second << " personas" << std::endl;
    }
    std::cout << "   Género Hembra" <<std::endl;
    for (auto& segm : edadesHembraSegm){
        std::cout << "     " << segm.first << ":" << segm.second << " personas" << std::endl;
    }
    std::cout << "   Género Otro" <<std::endl;
    for (auto& segm : edadesOtroSegm){
        std::cout << "     " << segm.first << ":" << segm.second << " personas" << std::endl;
    }
    std::cout << "-----" << std::endl;

    //7. Índice de dependencia
    std::cout << "7. Índice de dependencia: " << (dependientes*1.0f)/(independientes*1.0f) << std::endl;
    std::cout << "-----" << std::endl;

    //8. 10000 poblados más visitados
    std::cout << "8. 10000 poblados más visitados" << std::endl;
    for (std::pair par:top10000Poblados){
        std::cout << "Ciudad " << par.first << ": " << par.second << "visitas" << std::endl;
    }
*/
    return 0;
}