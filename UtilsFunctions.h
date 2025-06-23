#pragma once
#ifndef UTILSFUNCTION_H
#define UTILSFUNCTION_H

#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <map>

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
std::vector <std::string> SplitStr(const std::string& str, char delimiter);

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
int MesAInt(const std::string& mes);

/*
* @brief Extrae el estrato social al que corresponde una persona en base al CP origen.
*
* @param cpOrigen (std::string&): Referencia del string correspondiente al código postal de origen.
* @param estratos (std::vector<int>&): Referencia del vector que contiene la cantidad de personas
* por estrato.
*
* @return Retorna el vector estratos modificado.
*/
std::vector <int>& ExtraerEstrato(const std::string& cpOrigen, std::vector<int>& estratos);

/*
* @brief Calcula la edad de la persona en base a la fecha actual del equipo.
*
* @param birthDay (std::string&): Referencia del string correspondiente a la fecha de nacimiento
* de una persona.
*
* @return Retorna la edad correspondiente.
*/
int CalcularEdad(const std::string& birthDay);

/*
* @brief Suma en uno la cantidad de persona en una especie correspondiente.
*
* @param especie (std::string&): Referencia del string correspondiente a la especie de una persona.
* @param personasPorEspecie (std::map <std::string, int>&): Referencia del map que contiene la
* cantidad de personas por especie.
*
* @return Retorna el map personasPorEspecie modificado.
*/
std::map <std::string, int>& SumarPersonaEspecie(const std::string& especie, std::map <std::string, int>& personasPorEspecie);

std::map <std::string, int>& SumarPersonaGenero(const std::string& genero, std::map <std::string, int>& personasPorGenero);
#endif