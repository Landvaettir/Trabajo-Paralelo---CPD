#include "UtilsFunctions.h"

std::vector <std::string> SplitStr(const std::string& str, char delimiter = ' '){
    std::vector <std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)){
        tokens.push_back(token);
    }
    return tokens;
}

int MesAInt(const std::string& mes){
    std::map <std::string, int> meses = {
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

std::vector <int>& ExtraerEstrato(const std::string& cpOrigen, std::vector<int>& estratos){
    char estratoChar = cpOrigen[1];
    int estratoInt = estratoChar - '0';
    estratos[estratoInt] +=1;
    return estratos;
}

int CalcularEdad(const std::string& birthDay){
    time_t timestamp;
    time(&timestamp);
    std::vector <std::string> fechaHoraVect = SplitStr(birthDay, 'T');
    std::string fecha = fechaHoraVect[0];
    std::string hora = fechaHoraVect[1];
    std::vector <std::string> fechaVect = SplitStr(fecha, '-');
    int yearBirth = stoi(fechaVect[0].substr(1,4));
    int monthBirth = stoi(fechaVect[1]);
    int dayBirth = stoi(fechaVect[2]);

    std::vector <std::string> fechaHoraHoy = SplitStr(ctime(&timestamp), ' '); //Sat Jun 21 01:31:49 2025
    int yearToday = stoi(fechaHoraHoy[4]);
    int monthToday = MesAInt(fechaHoraHoy[1]);
    int dayToday = stoi(fechaHoraHoy[2]);

    if (monthBirth<monthToday){
        return yearToday-yearBirth;
    } else if (monthBirth>monthToday){
        return yearToday-yearBirth-1;
    } else {
        if (dayBirth<=dayToday){
            return yearToday-yearBirth;
        } else {
            return yearToday-yearBirth-1;
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

float EdadMediana(std::vector <int>& edades){
    if (edades.size()%2 == 0){
        int mid = edades.size()/2;
        return (edades[mid-1]+edades[mid])/2.0f;
    } else {
        int mid = edades.size()/2;
        return edades[mid];
    }
}

std::map <std::string, int>& SegmentarEdad(std::vector <int>& edades, std::map <std::string, int>& edadesSegm){
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

std::map <std::string, int>& VisitasCiudad(const std::string& cpDestino, std::map <std::string, int>& visitas){
    std::string ciudad = cpDestino.substr(2,6);
    visitas[ciudad] += 1;
    return visitas;
}

std::vector<std::pair<std::string, int>> ExtraerTop10000(std::map <std::string, int>& visitasPoblados){
    auto comp = [](const auto& a,  const auto& b){return a.second > b.second;};

    std::priority_queue <
        std::pair <std::string, int>, //{frecuencia, ciudad}
        std::vector <std::pair<std::string, int>>,
        decltype(comp) //Menor frecuencia al tope
    > minHeap(comp);

    for(const auto& [ciudad, visitas] : visitasPoblados){
        minHeap.push({ciudad, visitas});
        if (minHeap.size() > 10000){
            minHeap.pop(); //Se elimina la ciudad con menos visitas del heap
        }
    }

    std::vector<std::pair<std::string, int>> topPoblados;
    while(!minHeap.empty()){
        topPoblados.push_back(minHeap.top());
        minHeap.pop();
    }

    std::reverse(topPoblados.begin(), topPoblados.end());
    return topPoblados;
}