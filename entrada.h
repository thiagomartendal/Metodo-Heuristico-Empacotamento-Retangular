#ifndef ENTRADA_H
#define ENTRADA_H

#include <fstream>
#include <sstream>
#include <vector>

class Entrada {
private:
    std::vector<int> itens, larguras, alturas;
    std::vector<int> quebraString(std::string s);
    void lerInstancias(int arq, std::string instancia);

public:
    Entrada(int arquivo = 0, std::string instancia = "");
    std::vector<int> getItens();
    std::vector<int> getLarguras();
    std::vector<int> getAlturas();
};

#endif