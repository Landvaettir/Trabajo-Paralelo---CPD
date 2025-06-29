#include "UtilsFunctions.h"

using namespace std;

vector <string> SplitStr(const string& str, char delimiter = ' '){
    vector <string> tokens;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delimiter)){
        tokens.push_back(token);
    }
    return tokens;
}

int MesAInt(const string& mes){
    map <string, int> meses = {
        {"Jan", 1}, {"Feb", 2}, {"Mar", 3}, {"Apr", 4},{"May", 5}, {"Jun", 6},
        {"Jul", 7}, {"Aug", 8}, {"Sep", 9}, {"Oct", 10},{"Nov", 11}, {"Dec", 12},
        {"Ene", 1}, {"Abr", 4}, {"Ago", 8}, {"Dic", 12}
    };
    auto it = meses.find(mes);
    if (it != meses.end()){
        return it->second;
    } else {
        return -1;
    }
}

vector <int>& ExtraerEstrato(const string& cpOrigen, vector<int>& estratos){
    char estratoChar = cpOrigen[1];
    int estratoInt = estratoChar - '0';
    estratos[estratoInt] +=1;
    return estratos;
}

int CalcularEdad(const string& birthDay){
    string fecha = birthDay.substr(1,9);
    int yearBirth = stoi(fecha.substr(0,4));
    int monthBirth = stoi(fecha.substr(5,2));
    int dayBirth = stoi(fecha.substr(8,2));

    if (monthBirth<7){
        return 2025-yearBirth;
    } else if (monthBirth>7){
        return 2025-yearBirth-1;
    } else {
        if (dayBirth<=2){
            return 2025-yearBirth;
        } else {
            return 2025-yearBirth-1;
        }
    }
    return -1;
}

float EdadPromedio(std::vector <int>& edades, int& count){
    unsigned long countLong = count;
    float suma = 0.0;
    for (int edad : edades){
        suma += edad;
    }
    if (countLong == edades.size()){
        return suma/count;
    } else {
        std::cerr << "ERROR: Incongruencia en los datos" << std::endl;
        return -1.0;
    }
}

float EdadMediana(vector <int>& edades){
    if (edades.size()%2 == 0){
        int mid = edades.size()/2;
        return (edades[mid-1]+edades[mid])/2.0f;
    } else {
        int mid = edades.size()/2;
        return edades[mid];
    }
}

map <string, int>& SegmentarEdad(vector <int>& edades, map <string, int>& edadesSegm){
    edadesSegm = {{"0-17", 0}, {"18-35", 0}, {"36-60", 0}, {"60<", 0}};
    int size = edades.size();
    #pragma omp parallel for
    for (int i=0; i<size; i++){
        #pragma omp critical
        {
            if (edades[i] < 18){
                edadesSegm["0-17"] += 1;
            } else if ((18 < edades[i]) && (edades[i] < 36)){
                edadesSegm["18-35"] += 1;
            } else if ((35 < edades[i]) && (edades[i] < 61)){
                edadesSegm["36-60"] += 1;
            } else if (60 < edades[i]){
                edadesSegm["60<"] += 1;
            }
        }
    }
    return edadesSegm;
}

map <string, int>& VisitasCiudad(const string& cpDestino, map <string, int>& visitas){
    string ciudad = cpDestino.substr(2,6);
    visitas[ciudad] += 1;
    return visitas;
}

vector<pair<string, int>> ExtraerTop10000(map <string, int>& visitasPoblados){
    auto comp = [](const auto& a,  const auto& b){return a.second > b.second;};

    priority_queue <
        pair <string, int>, //{frecuencia, ciudad}
        vector <pair<string, int>>,
        decltype(comp) //Menor frecuencia al tope
    > minHeap(comp);

    for(const auto& [ciudad, visitas] : visitasPoblados){
        minHeap.push({ciudad, visitas});
        if (minHeap.size() > 10000){
            minHeap.pop(); //Se elimina la ciudad con menos visitas del heap
        }
    }

    vector<pair<string, int>> topPoblados;
    while(!minHeap.empty()){
        topPoblados.push_back(minHeap.top());
        minHeap.pop();
    }

    reverse(topPoblados.begin(), topPoblados.end());
    return topPoblados;
}