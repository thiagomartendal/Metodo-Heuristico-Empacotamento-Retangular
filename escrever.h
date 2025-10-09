#ifndef ESCREVER_H
#define ESCREVER_H

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include "item.h"
#include "entrada.h"

struct DadosExecucao {
    std::string instancia;
    int numExecucao;
    int totalItens;
    int totalItensAlocados;
    int conjuntosFormados;
    long long int lucroTotal;
    long long int areaOcupada;
    long double perdaDeCorte;
    double taxaDeOcupacao;
    double tempoTotal;
    double tempoMochila;
};

class Escrever {
private:
    std::vector<DadosExecucao> execucoes;
    int larguraFolha, alturaFolha;
    void abrirPasta(std::string pasta);

public:
    Escrever(std::vector<DadosExecucao> execucoes, int larguraFolha,
        int alturaFolha);
    void CSV(std::string pasta, std::string arquivo, long long int areaTotal,
        std::string tipoLucro, std::string instancia, double mediaTempoTotal,
        double mediaTempoMochila);
    void TXT(std::string pasta, std::string arquivo, std::string tipoMochila,
        std::string tipoLucro, std::string instancia,
        std::vector<std::pair<int, std::vector<Item>>> conjuntos, Entrada entrada);
    void SVG(std::string pasta, std::string arquivo, std::string tipoMochila,
        std::string tipoLucro, std::string instancia,
        std::vector<std::pair<int, std::vector<Item>>> conjuntos, Entrada entrada);
};

#endif
