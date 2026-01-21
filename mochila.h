#ifndef MOCHILA_H
#define MOCHILA_H

#include <algorithm>
#include <vector>
#include <cmath>
#include <tuple>
#include "item.h"

class Mochila {
private:
    unsigned int peso;
    std::vector<int> w; // Pesos
    std::vector<long long int> v; // Lucros
    std::tuple<int, std::vector<Item>> procurarItensMochilaDinamica(std::vector<std::vector<Item>> &M, std::vector<int> &alturas);

public:
    Mochila() = default;
    Mochila(unsigned int peso, std::vector<int> &pesos, std::vector<long long int> &lucros);
    void setPeso(unsigned int peso);
    void ordenacaoFracionaria(std::vector<int> &itensAtuais);
    std::tuple<long long int, int, std::vector<Item>> dinamica(std::vector<int> &I, std::vector<int> &alturas, int alturaLimite);
    std::tuple<long long int, int, std::vector<Item>> fracionaria(std::vector<int> &I, std::vector<int> &alturas, int alturaLimite);
};

#endif