#include "entrada.h"

Entrada::Entrada(int arquivo, std::string instancia) {
    Entrada::lerInstancias(arquivo, instancia);
}

// O método quebraString separa uma linha em três elementos (índice, largura, altura)
std::vector<int> Entrada::quebraString(std::string s) {
    std::vector<int> inteiros;
    std::stringstream stm(s);
    std::string palavra;
    while (stm >> palavra)
        inteiros.push_back(std::stoi(palavra));
    return inteiros;
}

// O método lerInstancias varre os arquivos de instâncias para obter os itens e suas dimensões
void Entrada::lerInstancias(int arq, std::string instancia) {
    std::string pasta = "./Instancias/" + instancia + "/" + instancia
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

// Retorna os índices dos itens
std::vector<int> Entrada::getItens() {
    return itens;
}

// Retorna as larguras dos itens
std::vector<int> Entrada::getLarguras() {
    return larguras;
}

// Retorna as alturas dos itens
std::vector<int> Entrada::getAlturas() {
    return alturas;
}
