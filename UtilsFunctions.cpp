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

std::vector <int>& ExtraerEstrato(const std::string& str, std::vector<int>& estratos){
    char estratoChar = str[1];
    int estratoInt = estratoChar - '0';
    estratos[estratoInt] +=1;
    return estratos;
}