#pragma once
#ifndef UTILS_FUNCTIONS_H
#define UTILS_FUNCTIONS_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <map>
#include <queue>
#include <algorithm>
#include <omp.h>

using namespace std;

/*
* @brief Separa un string segùn el delimitador especificado, si no se especifìca el delimitador,
* su valor por defecto es el espacio (' ').
*
* @param str (std::string&): Referencia del string que se desea separar.
* @param delimiter (char): Caracter que separa el string ingresado.
*
* @return Retorna una vector de strings, que contiene los sub-string separados por el delimitador
* correspondiente.
*/
vector <string> SplitStr(const string& str, char delimiter);

/*
* @brief Transforma un string de 3 carácteres, que representa un mes, en inglés ó español, a su
* nùmero entero correspondiente.
*
* @param mes (std::string&): Referencia del string de 3 carácteres que representa a un mes, en
* inglés o español.
*
* @return Retorna el entero correspondiente al mes ingresado en formato string.
*
* @example Ejemplo:
*   int mes = mesAInt("Jan"); //mes = 1.
*   int mes = mesAInt("Ene"); //mes = 1.
*   int mes = mesAInt("Oct"); //mes = 10.
*/
int MesAInt(const string& mes);

/*
* @brief Extrae el estrato social al que corresponde una persona en base al CP origen.
*
* @param cpOrigen (std::string&): Referencia del string correspondiente al código postal de origen.
* @param estratos (std::vector<int>&): Referencia del vector que contiene la cantidad de personas
* por estrato.
*
* @return Retorna el vector estratos modificado.
*/
vector <int>& ExtraerEstrato(const string& cpOrigen, vector<int>& estratos);

/*
* @brief Calcula la edad de la persona en base a la fecha de entrega (02-07-2025).
*
* @param birthDay (std::string&): Referencia del string correspondiente a la fecha de nacimiento
* de una persona.
*
* @return Retorna la edad correspondiente.
*/
int CalcularEdad(const string& birthDay);

/*
* @brief Calcula la edad promedio de un vector que contiene edades. Y verifica que el tamaño del vector sea igual a la
* cantidad de personas registradas en ese vector.
*
* @param edades (std::vector <int>&): Referencia del vector que contiene las edades.
* @param count (int&): Referencia del entero correspondiente a la cantidad de personas registradas en el vector.
*
* @return Retorna la edad promedio del vector.
*/
float EdadPromedio(std::vector <int>& edades, int& count);

/*
* @brief Calcula la edad mediana de un vector que contiene edades.
*
* @param edades (std::vector <int>&): Referencia del vector que contiene las edades.
*
* @return Retorna la edad mediana del vector.
*/
float EdadMediana(vector <int>& edades);

/*
* @brief Segmenta en 4 rangos las edades de un vector que contiene edades.
* Los rangos son "0-17", "18-35", "36-60" y "60<"
*
* @param edades (std::vector <int>&): Referencia del vector que contiene las edades.
* @param edadesSegm (std::map <std::string, int>&): Referencia del map que contiene la cantidad de edades por segmento.
*
* @return Retorna el map edadesSegm modificado.
*
* @warning FUNCIÓN PARALELA
*/
map <string, int>& SegmentarEdad(vector <int>& edades, map <string, int>& edadesSegm);

/*
* @brief Extrae la provincia y ciudad a la que viaja una persona en base al CP destino.
*
* @param cpDestino (std::string&): Referencia del string correspondiente al código postal de destino.
* @param visitas (std::map <std::string, int>&): Referencia del map que contiene la cantidad de visitas a una ciudad.
*
* @return Retorna el map visitas modificado.
*/
map <string, int>& VisitasCiudad(const string& cpDestino, map <string, int>& visitas);

/*
* @brief Extrae las 10000 ciudades más visitadas.
*
* @param visitasPoblados (std::map <std::string, int>&): Referencia del map que contiene el número de ciudad y su
* cantidad de visitas.
*
* @return Retorna un vector de pares con el número de la ciudad y su cantidad de visitas.
*/
vector<pair<string, int>> ExtraerTop10000(map <string, int>& visitasPoblados);
#endif