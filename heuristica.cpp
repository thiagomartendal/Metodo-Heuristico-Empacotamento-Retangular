#include "heuristica.h"

Heuristica::Heuristica(int larguraCompartimento, int alturaCompartimento, std::vector<int> &itens, std::vector<int> &larguras, std::vector<int> &alturas, std::string tipoLucro) {
    I = itens;
    this->larguraFolha = larguraCompartimento;
    this->alturaFolha = alturaCompartimento;
    this->larguras = larguras;
    this->alturas = alturas;
    Heuristica::iniciarMochila(tipoLucro);
}

// Retorna a soma dos tempos cronometrados de execução da mochila
int Heuristica::getTemposMochila() {
    return somaTemposMochila;
}

// O método iniciarMochila se responsabiliza pela inicialização da classe Mochila, e aciona a definição dos lucros (que é feito em definirLucros)
void Heuristica::iniciarMochila(std::string tipoLucro) {
    std::vector<long long int> lucros = Heuristica::definirLucros(tipoLucro);
    mochila = Mochila(larguraFolha, larguras, lucros);
}

// O método definirLucros é o auxiliar que determina os lucros a serem atribuídos a cada item
std::vector<long long int> Heuristica::definirLucros(std::string tipoLucro) {
    std::vector<long long int> lucros = std::vector<long long int>(I.size());
    long long int maiorLucro = 0;
    for (int i: I) {
        long long int lucro;
        if (tipoLucro == "area")
            lucro = larguras[i]*alturas[i];
        else if (tipoLucro == "largura")
            lucro = larguras[i];
        else
            lucro = alturas[i];
        lucros[i] = lucro;
    }
    return lucros;
}

// O método existeItemMenor verifica se ainda existem itens que caibam nas dimensões informadas
bool Heuristica::existeItemMenor(int larguraLivre, int alturaLivre) {
    for (int i: I)
        if (larguras[i] <= larguraLivre && alturas[i] <= alturaLivre)
            return true;
    return false;
}

// O método procedimentoMochila é um auxiliar que aciona o procedimento de mochila escolhido para selecionar os itens da entrada
std::tuple<long long int, int, std::vector<Item>> Heuristica::procedimentoMochila(int alturaLivre, std::string tipoMochila) {
    long long int lucro;
    int maiorAltura;
    std::vector<Item> itensSelecionados;
    std::chrono::system_clock::time_point inicio, fim;
    if (tipoMochila == "dinamica_") {
        inicio = std::chrono::system_clock::now();
        std::tie(lucro, maiorAltura, itensSelecionados) =
            mochila.dinamica(I, alturas, alturaLivre);
        fim = std::chrono::system_clock::now();
    } else {
        inicio = std::chrono::system_clock::now();
        std::tie(lucro, maiorAltura, itensSelecionados) =
            mochila.fracionaria(I, alturas, alturaLivre);
        fim = std::chrono::system_clock::now();
    }
    int tempoTotal = std::chrono::duration_cast<std::chrono::milliseconds>(fim
        - inicio).count();
    somaTemposMochila += tempoTotal;
    return std::tuple(lucro, maiorAltura, itensSelecionados);
}

/**
 * O método coordenadasValidas verifica se o ponto de coordenadas (x,y) sobrepõe uma região em uso.
 */
bool Heuristica::coordenadasValidas(std::vector<Item> itensAdicionados, int x, int y) {
    /* As coordenadas são avaliadas com os dados dos itens já inseridos no conjunto atual sendo formado. Quando a lista está vazia,
    não são feitas comparações pois ainda não existem itens alocados, então o procedimento retorna verdadeiro neste caso */
    for (Item item: itensAdicionados) {
        int w = larguras[item.id];
        int h = alturas[item.id];
        // Condição que determina se as coordenadas x e y estão dentro de uma área já ocupada por um item
        if ((x >= item.x && x < (item.x + w)) && (y >= item.y
            && y < (item.y + h)))
            return false;
    }
    return true;
}

/**
 * O método procurarEspacosLivres é encarregado de alocar os itens em coordenadas da folha de estoque, e também demarcar os espaços livres que possam ocorrer
 * entre os itens alocados e a altura limite atribuida a cada conjunto e subconjunto sendo formado.
*/
void Heuristica::procurarEspacosLivres(int x, int y, int maiorAltura, long long int &lucro, long long int &areaOcupada, std::vector<Espaco> &espacosLivres, std::vector<Item> itensSelecionados, std::vector<Item> &itensAlocados) {
    // Fazer uma verificação de pontos disponíveis: um modo de verificar se tal ponto está disponível para empacotar um item
    int areaFolha = larguraFolha*alturaFolha;
    int xItem = x;
    bool primeiro = true; // Flag que indica se um espaço produzido é o primeiro espaço produzido na etapa

    // São avaliados todos os itens do conjunto de selecionados
    for (Item &item: itensSelecionados) {
        int areaItem = larguras[item.id]*alturas[item.id];
        // Verifica se um item venha a proporcionar uma área ocupada maior que a área da folha (o que não é permitido)
        if (areaItem+areaOcupada <= areaFolha) { // Impede uma área ocupada maior que o limite de largura * altura
            // Verifica se as coordenadas de alocação não sobrepõe uma região da folha já ocupada por itens
            if (Heuristica::coordenadasValidas(itensAlocados, xItem, y)) {
                item.x = xItem;
                item.y = y;
                itensAlocados.push_back(item);
                areaOcupada += areaItem;

                // Forma o espaço livre
                Espaco espaco = Espaco{xItem, y+alturas[item.id], larguras[item.id], maiorAltura-alturas[item.id]};
                
                // Verifica se o espaço formado é o primeiro da alocação atual
                if (primeiro) {
                    espacosLivres.push_back(espaco);
                    primeiro = false;
                } else
                    /* Caso o espaço formado não seja o primeiro, é checado se este está no mesmo nível de altura que o espaço anterior, e caso esteja,
                    o espaço formado é desconsiderado mas sua largura é adicionada a largura do espaço anterior. Do contrário, o espaço é adicionado a lista
                    de espaços livres. */
                    if (espacosLivres[espacosLivres.size()-1].y == espaco.y)
                        espacosLivres[espacosLivres.size()-1].w += espaco.w;
                    else 
                        espacosLivres.push_back(espaco);
                xItem += larguras[item.id];
                auto it = std::find(I.begin(), I.end(), item.id);
                auto dist = std::distance(I.begin(), it);
                I.erase(I.begin() + dist);
            } else
                lucro -= item.lucro;
        } else
            lucro -= item.lucro;
    }
}

/**
 * O método empacotarEspacosLivres realiza a formação de subconjuntos de itens nos espaços livres formados na etapa de inicialização do conjunto principal
 * (método formarConjuntos). Aqui os itens são selecionados pelo procedimento de mochila considerando as dimensões de um espaço livre em questão.
 */
void Heuristica::empacotarEspacosLivres(long long int &lucroAtual, long long int &areaOcupada, std::vector<Item> &itensAdicionados, std::vector<Espaco> &espacosLivres, std::string tipoMochila) {
    while (espacosLivres.size() > 0) {
        Espaco atual = espacosLivres[0];
        espacosLivres.erase(espacosLivres.begin());
        int alturaLivre = atual.h, y = atual.y;

        // Enquanto houver itens para as dimensões disponíveis, são alocados os sobconjuntos de itens na folha
        while (alturaLivre > 0 && Heuristica::existeItemMenor(atual.w, alturaLivre)) {
            mochila.setPeso(atual.w);
            long long int lucroCarregado;
            int maiorAltura;
            std::vector<Item> novaSelecao;

            // Seleciona os itens a serem alocados com um procedimento de mochila
            std::tie(lucroCarregado, maiorAltura, novaSelecao) = Heuristica::procedimentoMochila(alturaLivre, tipoMochila);
            
            // Alocam-se os itens selecionados no subconjunto. Aqui novos espaços livres podem ser formados, o que origina novos subconjuntos
            Heuristica::procurarEspacosLivres(atual.x, y, maiorAltura, lucroCarregado, areaOcupada, espacosLivres, novaSelecao, itensAdicionados);
            
            lucroAtual += lucroCarregado;
            y += maiorAltura;
            alturaLivre -= maiorAltura;
        }
    }
}

/**
 * O método formarConjuntos é responsável pela inicialização de cada conjunto principal de itens na folha de estoque. A formação é feita sequencialmente
 * em "fila" da esquerda para a direita, com cada item sendo alocado de forma adjacente, um a um.
 */
std::tuple<long long int, long long int, std::vector<std::pair<int, std::vector<Item>>>> Heuristica::formarConjuntos(std::string tipoMochila) {
    if (tipoMochila == "fracionaria_")
        mochila.ordenacaoFracionaria(I); // Faz a ordenação do conjunto de entrada caso seja usada a mochila fracionária para selecionar os itens
    
    std::vector<std::pair<int, std::vector<Item>>> conjuntos;
    int alturaLivre = alturaFolha, x = 0, y = 0;
    long long int lucroTotal = 0, areaOcupada = 0;
 
    // Formam-se conjuntos na folha enquanto houver dimensões disponíveis para se alocar itens
    while (alturaLivre > 0 && Heuristica::existeItemMenor(larguraFolha, alturaLivre)) {
        mochila.setPeso(larguraFolha);
        std::vector<Item> itensAdicionados; // Conjunto atual
        std::vector<Espaco> espacosLivres;
        long long int lucroAtual;
        int maiorAltura;
        std::vector<Item> itensSelecionados;

        // Seleciona os itens a serem alocados com um procedimento de mochila
        std::tie(lucroAtual, maiorAltura, itensSelecionados) = Heuristica::procedimentoMochila(alturaLivre, tipoMochila);

        // Alocam-se os itens selecionados, e ao mesmo tempo, são demarcados os espaços livres para futuras alocações
        Heuristica::procurarEspacosLivres(x, y, maiorAltura, lucroAtual, areaOcupada, espacosLivres, itensSelecionados, itensAdicionados);

        // Faz-se a formação dos subconjuntos de itens nos espaços livres demarcados.
        Heuristica::empacotarEspacosLivres(lucroAtual, areaOcupada, itensAdicionados, espacosLivres, tipoMochila);

        conjuntos.push_back(std::make_pair(lucroAtual, itensAdicionados));
        lucroTotal += lucroAtual;
        y += maiorAltura;
        alturaLivre -= maiorAltura;
    }

    return std::tuple(lucroTotal, areaOcupada, conjuntos);
}