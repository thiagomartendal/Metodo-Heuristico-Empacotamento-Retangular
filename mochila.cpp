#include "mochila.h"

Mochila::Mochila(unsigned int peso, std::vector<int> &pesos,
    std::vector<long long int> &lucros) {
    
    this->peso = peso;
    w = pesos;
    v = lucros;
}

void Mochila::setPeso(unsigned int peso) {
    this->peso = peso;
}

std::tuple<long long int, int, std::vector<Item>> Mochila::dinamica(
    std::vector<int> &I, std::vector<int> &alturas, int alturaLimite) {

    int n = I.size();
    std::vector<std::vector<Item>> M(n+1, std::vector<Item>(peso+1));
    for (int i = 1; i <= n; i++) {
        int idItem = I[i-1];
        for (int j = 1; j <= peso; j++) {
            if (j < w[idItem])
                M[i][j] = M[i-1][j];
            else {
                long long int valor1 = M[i-1][j].valor;
                long long int valor2 = v[idItem]+M[i-1][j-w[idItem]].valor;
                if (valor1 < valor2 && alturas[idItem] <= alturaLimite) {
                    M[i][j] = Item{idItem, valor2, v[idItem], 0, 0};
                } else
                    M[i][j] = M[i-1][j];
            }
        }
    }
    long long int lucro = M[n][peso].valor;
    int maiorAltura;
    std::vector<Item> itensSelecionados;
    std::tie(maiorAltura, itensSelecionados) =
        Mochila::procurarItensMochilaDinamica(M, alturas);

    return std::tuple(lucro, maiorAltura, itensSelecionados);
}

// Passar a referência da matriz M é necessário, pois passa apenas a referência do endereço de memória onde a matriz começa, e não o seu conteúdo.
// Isso impede que a execução seja quebrada quando a matriz fica muito grande.
std::tuple<int, std::vector<Item>> Mochila::procurarItensMochilaDinamica(
    std::vector<std::vector<Item>> &M, std::vector<int> &alturas) {

    std::vector<Item> itensSelecionados = std::vector<Item>();
    int maiorAltura = 0;
    int p = M[0].size()-1;
    for (int i = M.size()-1; i > 0 && p > 0; i--)
        if (M[i][p].valor != M[i - 1][p].valor) { // Item i foi escolhido
            Item item = M[i][p];
            maiorAltura = ((maiorAltura < alturas[item.id]) ? alturas[item.id] : maiorAltura);
            itensSelecionados.push_back(item);
            p -= w[item.id];
        }

    return std::tuple(maiorAltura, itensSelecionados);
}

void Mochila::ordenacaoFracionada(std::vector<int> &itensAtuais) {
    std::sort(itensAtuais.begin(), itensAtuais.end(), [this](int item1, int item2) {
        if (w[item1] == 0 || w[item2] == 0)
            return false;
        double razao1 = (double)v[item1] / (double)w[item1];
        double razao2 = (double)v[item2] / (double)w[item2];
        return razao1 > razao2; // Condição para ordem decrescente
    });
}

std::tuple<long long int, int, std::vector<Item>> Mochila::fracionada(
    std::vector<int> &I, std::vector<int> &alturas, int alturaLimite) {

    long long int lucro = 0;
    int maiorAltura = 0;
    std::vector<Item> selecao;
    int pesoAtual = peso;
    for (int i: I) {
        if ((w[i] <= pesoAtual) && (alturas[i] <= alturaLimite)) {
            maiorAltura = ((maiorAltura < alturas[i]) ? alturas[i] : maiorAltura);
            selecao.push_back(Item{i, 0, v[i], 0, 0});
            pesoAtual -= w[i];
            lucro += v[i];
        }
    }

    return std::tuple(lucro, maiorAltura, selecao);
}
