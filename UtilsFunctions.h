#pragma once
#ifndef UTILSFUNCTION_H
#define UTILSFUNCTION_H

#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <map>

std::vector <std::string> SplitStr(const std::string& str, char delimiter);

int mesAInt(const std::string& mes);

std::vector <int>& ExtraerEstrato(const std::string& str, std::vector<int>& estratos);

int CalcularEdad(const std::string& str);

#endif