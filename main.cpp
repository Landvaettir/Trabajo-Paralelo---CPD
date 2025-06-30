#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <ctime>
#include <omp.h>

#include "UtilsFunctions.h"

using namespace std;

int main() {
    omp_set_num_threads(omp_get_max_threads()); //Configuración para usar el máximo de hilos disponibles.
    
    int count = 0;
    const int ChunkSize = 1000000;
    
    vector <int> personasPorEstratoGlobal(10, 0);
    vector <float> percentPorEstratoGlobal(10, 0);
    
    map <string, int> personasPorEspecieGlobal = {
        {"Humana", 0}, {"Elfica", 0}, {"Enana", 0}, {"Hombre Bestia", 0}
    };
    map <string, int> personasPorGeneroGlobal = {
        {"MACHO", 0}, {"HEMBRA", 0}, {"OTRO", 0}
    };

    vector <int> edadesHumanGlobal, edadesElfGlobal, edadesEnanaGlobal, edadesHBestiaGlobal; //Vectores de edades por especie.
    vector <int> edadesMachoGlobal, edadesHembraGlobal, edadesOtroGlobal; //Vectores de edades por género.

    int dependientesGlobal = 0;
    int independientesGlobal = 0;

    map <string, int> visitasPobladosGlobal;
    vector<pair<string, int>> top10000PobladosGlobal;

    ifstream file("eldoria.csv");
    if (!file.is_open()){
        cerr << "Error: No se puede abrir el archivo" << endl;
        return 1;
    };

    string line;
    getline(file, line); //Extraemos los encabezados.

    while (file.good()){ //Procesamiento por bloques.
        vector <string> lineChunk;
        for (int i=0; i<ChunkSize && getline(file, line); i++){
            lineChunk.push_back(line);
            count++;
        }
        if (lineChunk.empty()){
            break;
        }

        #pragma omp parallel 
        {   
            vector <int> personasPorEstratoLocal(10, 0);

            map <string, int> personasPorEspecieLocal = {
                {"Humana", 0}, {"Elfica", 0}, {"Enana", 0}, {"Hombre Bestia", 0}
            };
            map <string, int> personasPorGeneroLocal = {
                {"MACHO", 0}, {"HEMBRA", 0}, {"OTRO", 0}
            };
            
            vector <int> edadesHumanLocal, edadesElfLocal, edadesEnanaLocal, edadesHBestiaLocal;
            vector <int> edadesMachoLocal, edadesHembraLocal, edadesOtroLocal;

            int dependientesLocal = 0;
            int independientesLocal = 0;

            map <string, int> visitasPobladosLocal;

            int sizeLineChunk = lineChunk.size();

            #pragma omp for
            for (int i=0; i<sizeLineChunk; i++){
                vector <string> tokens = SplitStr(lineChunk[i], ';');
                personasPorEstratoLocal = ExtraerEstrato(tokens[6], personasPorEstratoLocal); //P.1-2

                int edad = CalcularEdad(tokens[5]); //P3
                string especie = tokens[1];
                string genero = tokens[2];
                especie.erase(0,1); //Eliminamos la comilla doble del inicio.
                especie.pop_back(); //Eliminamos la comilla doble del final.
                genero.erase(0,1);
                genero.pop_back();
                
                if (especie == "Humana" || especie == "HUMANA"){
                    edadesHumanLocal.push_back(edad);
                    personasPorEspecieLocal["Humana"] += 1;
                } else if (especie == "Elfica" || especie == "ELFICA"){
                    edadesElfLocal.push_back(edad);
                    personasPorEspecieLocal["Elfica"] += 1;
                } else if (especie == "Enana" || especie == "ENANA"){
                    edadesEnanaLocal.push_back(edad);
                    personasPorEspecieLocal["Enana"] += 1;
                } else if (especie == "Hombre Bestia" || especie == "HOMBRE BESTIA"){
                    edadesHBestiaLocal.push_back(edad);
                    personasPorEspecieLocal["Hombre Bestia"] += 1;
                }
                if (genero == "MACHO" || genero == "Macho"){
                    edadesMachoLocal.push_back(edad);
                    personasPorGeneroLocal["MACHO"] += 1;
                } else if (genero == "HEMBRA" || genero == "Hembra"){
                    edadesHembraLocal.push_back(edad);
                    personasPorGeneroLocal["HEMBRA"] += 1;
                } else if (genero == "OTRO" || genero == "Otro"){
                    edadesOtroLocal.push_back(edad);
                    personasPorGeneroLocal["OTRO"] += 1;
                }
                
                if ((edad<15) || (edad>64)){ //P.7
                    dependientesLocal += 1;
                } else {
                    independientesLocal += 1;
                }

                VisitasCiudad(tokens[7], visitasPobladosLocal); //P.8
            }

            #pragma omp critical //Se actualizan las variables globales, agregando la información de las variables locales.
            {
                for(int i=0; i<10; i++){
                    personasPorEstratoGlobal[i] += personasPorEstratoLocal[i];
                }
                
                for (auto& par : personasPorEspecieLocal){
                    personasPorEspecieGlobal[par.first] += par.second;
                }
                
                for (auto& par : personasPorGeneroLocal){
                    personasPorGeneroGlobal[par.first] += par.second;
                }
                
                edadesHumanGlobal.insert(edadesHumanGlobal.end(), edadesHumanLocal.begin(), edadesHumanLocal.end());
                edadesElfGlobal.insert(edadesElfGlobal.end(), edadesElfLocal.begin(), edadesElfLocal.end());
                edadesEnanaGlobal.insert(edadesEnanaGlobal.end(), edadesEnanaLocal.begin(), edadesEnanaLocal.end());
                edadesHBestiaGlobal.insert(edadesHBestiaGlobal.end(), edadesHBestiaLocal.begin(), edadesHBestiaLocal.end());
                edadesMachoGlobal.insert(edadesMachoGlobal.end(), edadesMachoLocal.begin(), edadesMachoLocal.end());
                edadesHembraGlobal.insert(edadesHembraGlobal.end(), edadesHembraLocal.begin(), edadesHembraLocal.end());
                edadesOtroGlobal.insert(edadesOtroGlobal.end(), edadesOtroLocal.begin(), edadesOtroLocal.end());
                
                dependientesGlobal += dependientesLocal;
                independientesGlobal += independientesLocal;
                
                for (const auto& par : visitasPobladosLocal){
                    visitasPobladosGlobal[par.first] += par.second;
                }
            }
        }
    }
    file.close();

    //Ordenamiento de los vectores de edad - P.4
    sort(edadesHumanGlobal.begin(), edadesHumanGlobal.end());
    sort(edadesElfGlobal.begin(), edadesElfGlobal.end());
    sort(edadesEnanaGlobal.begin(), edadesEnanaGlobal.end());
    sort(edadesHBestiaGlobal.begin(), edadesHBestiaGlobal.end());

    sort(edadesMachoGlobal.begin(), edadesMachoGlobal.end());
    sort(edadesHembraGlobal.begin(), edadesHembraGlobal.end());
    sort(edadesOtroGlobal.begin(), edadesOtroGlobal.end());

    //Segmentación de edades - P.5-6
    std::map <std::string, int> edadesHumanSegm, edadesElfSegm, edadesEnanaSegm, edadesHBestiaSegm;
    std::map <std::string, int> edadesMachoSegm, edadesHembraSegm, edadesOtroSegm;

    edadesHumanSegm = SegmentarEdad(edadesHumanGlobal, edadesHumanSegm);
    edadesElfSegm = SegmentarEdad(edadesElfGlobal, edadesElfSegm);
    edadesEnanaSegm = SegmentarEdad(edadesEnanaGlobal, edadesEnanaSegm);
    edadesHBestiaSegm = SegmentarEdad(edadesHBestiaGlobal, edadesHBestiaSegm);

    edadesMachoSegm = SegmentarEdad(edadesMachoGlobal, edadesMachoSegm);
    edadesHembraSegm = SegmentarEdad(edadesHembraGlobal, edadesHembraSegm);
    edadesOtroSegm = SegmentarEdad(edadesOtroGlobal, edadesOtroSegm);

    //Extraer top 10000 poblados más visitados
    top10000PobladosGlobal = ExtraerTop10000(visitasPobladosGlobal); 

    cout << "Total de datos: " << count << endl;

    //RESPUESTAS----------
    cout << "-----\nRespuestas:" << endl;

    //1. Personas por estrato
    cout << "1. Personas por estrato:" << endl;
    for(int i=0; i<10; i++){
        cout << "   Personas en estrato " << i << ": " << personasPorEstratoGlobal[i] << endl;
    }
    cout << "-----" << endl;

    //2.Porcentaje por estrato
    float countDb = count;
    cout << "2. Porcentaje por estrato:" << endl;
    for (int i=0; i<10; i++){
        percentPorEstratoGlobal[i] = (personasPorEstratoGlobal[i]/countDb)*100;
        cout << "   Porcentaje estrato "<< i << ": " << percentPorEstratoGlobal[i] << "%" << endl;
    }
    cout << "-----" << endl;

    //3. Edad promedio por especie y genero
    cout << "3. Edades promedio por especie y género:" << endl;
    cout << "   Edad promedio especie Humana: " << EdadPromedio(edadesHumanGlobal, personasPorEspecieGlobal["Humana"]) << endl;
    cout << "   Edad promedio especie Elfica: " << EdadPromedio(edadesElfGlobal, personasPorEspecieGlobal["Elfica"]) << endl;
    cout << "   Edad promedio especie Enana: " << EdadPromedio(edadesEnanaGlobal, personasPorEspecieGlobal["Enana"]) << endl;
    cout << "   Edad promedio especie Hombre Bestia: " << EdadPromedio(edadesHBestiaGlobal, personasPorEspecieGlobal["Hombre Bestia"]) << endl;
    cout << endl;
    cout << "   Edad promedio género Macho: " << EdadPromedio(edadesMachoGlobal, personasPorGeneroGlobal["MACHO"]) << endl;
    cout << "   Edad promedio género Hembra: " << EdadPromedio(edadesHembraGlobal, personasPorGeneroGlobal["HEMBRA"]) << endl;
    cout << "   Edad promedio género Otro: " << EdadPromedio(edadesOtroGlobal, personasPorGeneroGlobal["OTRO"]) << endl;
    cout << "-----" << endl;

    //4. Edad mediana por especie y genero
    cout << "4. Edades mediana por especie y género:" << endl;
    cout << "   Edad mediana especie Humana: " << EdadMediana(edadesHumanGlobal) << endl;
    cout << "   Edad mediana especie Elfica: " << EdadMediana(edadesElfGlobal) << endl;
    cout << "   Edad mediana especie Enana: " << EdadMediana(edadesEnanaGlobal) << endl;
    cout << "   Edad mediana especie Hombre Bestia: " << EdadMediana(edadesHBestiaGlobal) << endl;
    cout << endl;
    cout << "   Edad mediana género Macho: " << EdadMediana(edadesMachoGlobal) << endl;
    cout << "   Edad mediana género Hembra: " << EdadMediana(edadesHembraGlobal) << endl;
    cout << "   Edad mediana género Otro: " << EdadMediana(edadesOtroGlobal) << endl;
    cout << "-----" << endl;

    //5. Proporción segmentada de la población
    cout << "5. Proporción segmentada de la población según especie y género:" << endl;
    cout << "   Especie Humana: " << endl << "    ";
    float size = edadesHumanGlobal.size();
    for(auto& pair:edadesHumanSegm){
        cout << pair.first << ":" << (pair.second/size)*100 << "%" << " | ";
    }
    cout << endl;
    cout << "   Especie Elfica: " << endl << "    ";
    size = edadesElfGlobal.size();
    for(auto& pair:edadesElfSegm){
        cout << pair.first << ":" << (pair.second/size)*100 << "%" << " | ";
    }
    cout << endl;
    cout << "   Especie Enana: " << endl << "    ";
    size = edadesEnanaGlobal.size();
    for(auto& pair:edadesEnanaSegm){
        cout << pair.first << ":" << (pair.second/size)*100 << "%" << " | ";
    }
    cout << endl;
    cout << "   Especie Hombre Bestia: " << endl << "    ";
    size = edadesHBestiaGlobal.size();
    for(auto& pair:edadesHBestiaSegm){
        cout << pair.first << ":" << (pair.second/size)*100 << "%" << " | ";
    }
    cout << endl;
    cout << endl;
    cout << "   Género Macho: " << endl << "    ";
    size = edadesMachoGlobal.size();
    for(auto& pair:edadesMachoSegm){
        cout << pair.first << ":" << (pair.second/size)*100 << "%" << " | ";
    }
    cout << endl;
    cout << "   Género Hembra: " << endl << "    ";
    size = edadesHembraGlobal.size();
    for(auto& pair:edadesHembraSegm){
        cout << pair.first << ":" << (pair.second/size)*100 << "%" << " | ";
    }
    cout << endl;
    cout << "   Género Otro: " << endl << "    ";
    size = edadesOtroGlobal.size();
    for(auto& pair:edadesOtroSegm){
        cout << pair.first << ":" << (pair.second/size)*100 << "%" << " | ";
    }
    cout << endl;
    cout << "-----" << endl;

    //6. Pirámide de edades
    cout << "6. Pirámide de edades:" << endl;
    cout << "   Especie Humana" <<endl;
    for (auto& segm : edadesHumanSegm){
        cout << "     " << segm.first << ":" << segm.second << " personas" << endl;
    }
    cout << "   Especie Elfica" <<endl;
    for (auto& segm : edadesElfSegm){
        cout << "     " << segm.first << ":" << segm.second << " personas" << endl;
    }
    cout << "   Especie Enana" <<endl;
    for (auto& segm : edadesEnanaSegm){
        cout << "     " << segm.first << ":" << segm.second << " personas" << endl;
    }
    cout << "   Especie Hombre Bestia" <<endl;
    for (auto& segm : edadesHBestiaSegm){
        cout << "     " << segm.first << ":" << segm.second << " personas" << endl;
    }
    cout << endl;
    cout << "   Género Macho" <<endl;
    for (auto& segm : edadesMachoSegm){
        cout << "     " << segm.first << ":" << segm.second << " personas" << endl;
    }
    cout << "   Género Hembra" <<endl;
    for (auto& segm : edadesHembraSegm){
        cout << "     " << segm.first << ":" << segm.second << " personas" << endl;
    }
    cout << "   Género Otro" <<endl;
    for (auto& segm : edadesOtroSegm){
        cout << "     " << segm.first << ":" << segm.second << " personas" << endl;
    }
    cout << "-----" << endl;

    //7. Índice de dependencia
    cout << "7. Índice de dependencia: " << (dependientesGlobal*1.0f)/(independientesGlobal*1.0f) << endl;
    cout << "-----" << endl;

    //8. 10000 poblados más visitados
    cout << "8. 10000 poblados más visitados" << endl;

    ofstream archivo("Top 10000 poblados.txt");
    if (!archivo){
        cout << "No se pudo cre4ear el archivo" << endl;
        return 1;
    }

    archivo << "Top 10000 poblados.txt\n";

    for (std::pair par:top10000PobladosGlobal){
        archivo << "Ciudad " << par.first << ": " << par.second << "visitas\n";
    }
    archivo.close();
    return 0;
}