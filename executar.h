#ifndef EXECUTAR_H
#define EXECUTAR_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include "entrada.h"
#include "escrever.h"
#include "heuristica.h"

class Executar {
private:
    unsigned int larguraFolha, alturaFolha;
    int totalItens(std::vector<std::pair<int, std::vector<Item>>> conjuntos);
    void resultados(long long int lucroTotal, int totalFilas,
        int totalItensAlocados, long long int areaOcupada,
        long double perdaCorte, double taxaOcupacao, int tempoTotal);

public:
    Executar(unsigned int larguraFolha, unsigned int alturaFolha);
    DadosExecucao instancia(std::string tipoLucro, std::string tipoMochila,
        std::string instancia, int numArquivo);
};

#endif
