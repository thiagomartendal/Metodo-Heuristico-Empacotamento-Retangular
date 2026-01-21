#include "mochila.h"

Mochila::Mochila(unsigned int peso, std::vector<int> &pesos, std::vector<long long int> &lucros) {
    this->peso = peso;
    w = pesos;
    v = lucros;
}

// O método setPeso altera o peso da mochila no início de cada etapa de alocação.
void Mochila::setPeso(unsigned int peso) {
    this->peso = peso;
}

/**
 * O método da mochila dinamica disponibiliza os itens a serem alocados em determinada etapa através do cálculo da matriz de pesos.
 * Quando a matriz de pesos é efetivamente calculada, um método auxiliar realiza o resgate dos itens considerados no cálculo.
 * Junto aos itens selecionados, é retornado o lucro total calculado.
 */
std::tuple<long long int, int, std::vector<Item>> Mochila::dinamica(std::vector<int> &I, std::vector<int> &alturas, int alturaLimite) {
    int n = I.size();
    std::vector<std::vector<Item>> M(n+1, std::vector<Item>(peso+1));

    // Calcula a matriz de pesos com a estratégia de programação dinâmica
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
    // O método procurarItensMochilaDinamica resgata os itens considerados para o resultado final da matriz M
    std::tie(maiorAltura, itensSelecionados) = Mochila::procurarItensMochilaDinamica(M, alturas);

    return std::tuple(lucro, maiorAltura, itensSelecionados);
}

/**
 * O método procurarItensMochilaDinamica resgata os itens da matriz de calculos M percorrendo-a a partir da ultima célula de trás para frente,
 * verificando os lucros calculados de cada célula entre linhas e colunas. Se uma célula de um item i em uma coluna p tiver um lucro diferente
 * de um item anterior i-1 na mesma coluna p, o item i é selecionado como parte contribuinte para o calculo final, e adicionado a lista
 * de itens selecionados.
 */
std::tuple<int, std::vector<Item>> Mochila::procurarItensMochilaDinamica(std::vector<std::vector<Item>> &M, std::vector<int> &alturas) {
    std::vector<Item> itensSelecionados = std::vector<Item>();
    int maiorAltura = 0;
    int p = M[0].size()-1; // Posição da ultima coluna

    for (int i = M.size()-1; i > 0 && p > 0; i--)
        if (M[i][p].valor != M[i - 1][p].valor) { // Item i foi escolhido
            Item item = M[i][p];
            maiorAltura = ((maiorAltura < alturas[item.id]) ? alturas[item.id] : maiorAltura);
            itensSelecionados.push_back(item);
            p -= w[item.id];
        }

    return std::tuple(maiorAltura, itensSelecionados);
}

// O método ordenacaoFracionaria realiza a ordenação dos itens seguindo o critério de ordenação fracionária para a seleção no método principal.
void Mochila::ordenacaoFracionaria(std::vector<int> &itensAtuais) {
    std::sort(itensAtuais.begin(), itensAtuais.end(), [this](int item1, int item2) {
        if (w[item1] == 0 || w[item2] == 0)
            return false;
        double razao1 = (double)v[item1] / (double)w[item1];
        double razao2 = (double)v[item2] / (double)w[item2];
        return razao1 > razao2; // Condição para ordem decrescente
    });
}

/**
 * O método de mochila fracionaria seleciona os itens que caibam na capacidade de peso da mochila. O conjunto I de itens é previamente
 * ordenado por ordenacaoFracionaria, e então são incluídos conforme o peso remanescente permitir novas inserções.
 */
std::tuple<long long int, int, std::vector<Item>> Mochila::fracionaria(std::vector<int> &I, std::vector<int> &alturas, int alturaLimite) {
    long long int lucro = 0;
    int maiorAltura = 0;
    std::vector<Item> selecao;
    int pesoAtual = peso; // O peso remanescente inicia a partir do peso total da mochila

    for (int i: I) {
        if ((w[i] <= pesoAtual) && (alturas[i] <= alturaLimite)) {
            maiorAltura = ((maiorAltura < alturas[i]) ? alturas[i] : maiorAltura);
            selecao.push_back(Item{i, 0, v[i], 0, 0});
            pesoAtual -= w[i]; // O peso do item selecionado é deduzido do peso remanescente
            lucro += v[i];
        }
    }

    return std::tuple(lucro, maiorAltura, selecao);
}