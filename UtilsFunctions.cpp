#include "UtilsFunctions.h"

std::vector <std::string> SplitStr(const std::string& str, char delimiter){
    std::vector <std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)){
        tokens.push_back(token);
    }
    return tokens;
}

int mesAInt(const std::string& mes){
    std::map <std::string, int> meses = {
        {"Ene", 1}, {"Feb", 2}, {"Mar", 3}, {"Apr", 4},{"May", 5}, {"Jun", 6},
        {"Jul", 7}, {"Aug", 8}, {"Sep", 9}, {"Oct", 10},{"Nov", 11}, {"Dec", 12}
    };
    auto it = meses.find(mes);
    if (it != meses.end()){
        return it->second;
    } else {
        return -1;
    }
}

std::vector <int>& ExtraerEstrato(const std::string& str, std::vector<int>& estratos){
    char estratoChar = str[1];
    int estratoInt = estratoChar - '0';
    estratos[estratoInt] +=1;
    return estratos;
}

int CalcularEdad(const std::string& str){
    time_t timestamp;
    time(&timestamp);
    std::vector <std::string> fechaHoraVect = SplitStr(str, 'T');
    std::string fecha = fechaHoraVect[0];
    std::string hora = fechaHoraVect[1];
    std::vector <std::string> fechaVect = SplitStr(fecha, '-');
    int yearBirth = stoi(fechaVect[0].substr(1,4));
    int monthBirth = stoi(fechaVect[1]);
    int dayBirth = stoi(fechaVect[2]);

    std::vector <std::string> fechaHoraHoy = SplitStr(ctime(&timestamp), ' '); //Sat Jun 21 01:31:49 2025
    int yearToday = stoi(fechaHoraHoy[4]);
    int monthToday = mesAInt(fechaHoraHoy[1]);
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
    return 0;
}