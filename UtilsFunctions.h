#pragma once
#ifndef UTILSFUNCTION_H
#define UTILSFUNCTION_H

#include <string>
#include <vector>
#include <sstream>

std::vector <std::string> SplitStr(const std::string& str, char delimiter);

std::vector <int>& ExtraerEstrato(const std::string& str, std::vector<int>& estratos);

#endif