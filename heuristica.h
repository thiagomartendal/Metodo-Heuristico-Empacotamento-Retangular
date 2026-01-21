#ifndef HEURISTICA_H
#define HEURISTICA_H

#include <vector>
#include <string>
#include <chrono>
#include <cmath>
#include "mochila.h"

struct Espaco {
    int x, y; // Canto inferior esquerdo
    int w, h; // Largura (w) e altura (h) do espaço
};

class Heuristica {
private:
    int larguraFolha, alturaFolha;
    int somaTemposMochila{0};
    std::vector<int> I, larguras, alturas;
    Mochila mochila;
    void iniciarMochila(std::string tipoLucro);
    std::vector<long long int> definirLucros(std::string tipoLucro);
    bool existeItemMenor(int larguraLivre, int alturaLivre);
    bool coordenadasValidas(std::vector<Item> itensAdicionados, int x, int y);
    std::tuple<long long int, int, std::vector<Item>> procedimentoMochila(int alturaLivre, std::string tipoMochila);
    void procurarEspacosLivres(int x, int y, int maiorAltura, long long int &lucro, long long int &areaOcupada, std::vector<Espaco> &espacosLivres, std::vector<Item> itensSelecionados, std::vector<Item> &itensAlocados);
    void empacotarEspacosLivres(long long int &lucroAtual, long long int &areaOcupada, std::vector<Item> &itens, std::vector<Espaco> &espacosLivres, std::string tipoMochila);

public:
    Heuristica(int larguraFolha, int alturaFolha, std::vector<int> &itens, std::vector<int> &larguras, std::vector<int> &alturas, std::string tipoLucro);
    std::tuple<long long int, long long int, std::vector<std::pair<int, std::vector<Item>>>> formarConjuntos(std::string tipoMochila);
    int getTemposMochila();
};

#endif