#include "entrada.h"

Entrada::Entrada(int arquivo, std::string instancia) {
    Entrada::lerInstancias(arquivo, instancia);
}

std::vector<int> Entrada::quebraString(std::string s) {
    std::vector<int> inteiros;
    std::stringstream stm(s);
    std::string palavra;
    while (stm >> palavra)
        inteiros.push_back(std::stoi(palavra));
    return inteiros;
}

void Entrada::lerInstancias(int arq, std::string instancia) {
    std::string pasta = "../Instancias/" + instancia + "/" + instancia
    + std::to_string(arq) + ".txt";
    std::ifstream arquivo;
    arquivo.open(pasta, std::ifstream::in);
    std::string linha;
    while (std::getline(arquivo, linha, '\n')) {
        std::vector<int> inteiros = quebraString(linha);
        itens.push_back(inteiros[0]);
        larguras.push_back(inteiros[1]);
        alturas.push_back(inteiros[2]);
    }
}

std::vector<int> Entrada::getItens() {
    return itens;
}

std::vector<int> Entrada::getLarguras() {
    return larguras;
}

std::vector<int> Entrada::getAlturas() {
    return alturas;
}
