#include "heuristica.h"

Heuristica::Heuristica(int larguraCompartimento, int alturaCompartimento,
    std::vector<int> &itens, std::vector<int> &larguras, std::vector<int> &alturas,
    std::string tipoLucro) {

    I = itens;
    this->larguraFolha = larguraCompartimento;
    this->alturaFolha = alturaCompartimento;
    this->larguras = larguras;
    this->alturas = alturas;
    Heuristica::iniciarMochila(tipoLucro);
}

int Heuristica::getTemposMochila() {
    return somaTemposMochila;
}

void Heuristica::iniciarMochila(std::string tipoLucro) {
    std::vector<long long int> lucros = Heuristica::definirLucros(tipoLucro);
    mochila = Mochila(larguraFolha, larguras, lucros);
}

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
        // Outras operações matemáticas envolvendo retângulos
    }
    return lucros;
}

bool Heuristica::existeItemMenor(int larguraLivre, int alturaLivre) {
    for (int i: I)
        if (larguras[i] <= larguraLivre && alturas[i] <= alturaLivre)
            return true;
    return false;
}

std::tuple<long long int, int, std::vector<Item>> Heuristica::procedimentoMochila(
    int alturaLivre, std::string tipoMochila) {

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
            mochila.fracionada(I, alturas, alturaLivre);
        fim = std::chrono::system_clock::now();
    }
    int tempoTotal = std::chrono::duration_cast<std::chrono::milliseconds>(fim
        - inicio).count();
    somaTemposMochila += tempoTotal;
    return std::tuple(lucro, maiorAltura, itensSelecionados);
}

bool Heuristica::coordenadasValidas(std::vector<Item> itensAdicionados, int x, int y) {
    // As coordenadas são avaliadas com os dados dos itens já inseridos no conjunto atual sendo formado
    /* Quando a lista está vazia, não são feitas comparações pois ainda não existem itens alocados,
       então o procedimento retorna verdadeiro neste caso */
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

void Heuristica::procurarEspacosLivres(int x, int y, int maiorAltura,
    long long int &lucro, long long int &areaOcupada,
    std::vector<Espaco> &espacosLivres, std::vector<Item> itensSelecionados,
    std::vector<Item> &itensAlocados) {

    // Fazer uma verificação de pontos disponíveis: um modo de verificar se tal ponto está disponível para empacotar um item
    int areaFolha = larguraFolha*alturaFolha;
    int xItem = x;
    bool primeiro = true; // Flag que indica se um espaço produzido é o primeiro espaço produzido na etapa
    for (Item &item: itensSelecionados) {
        int areaItem = larguras[item.id]*alturas[item.id]; 
        if (areaItem+areaOcupada <= areaFolha) { // Impede uma área ocupada maior que o limite de largura * altura, como ocorre no conjunto zdf8 para uma folha de dimensão 1250x1200
            if (Heuristica::coordenadasValidas(itensAlocados, xItem, y)) {
                item.x = xItem;
                item.y = y;
                itensAlocados.push_back(item);
                areaOcupada += areaItem;
                Espaco espaco = Espaco{xItem, y+alturas[item.id],
                    larguras[item.id], maiorAltura-alturas[item.id]};
                if (primeiro) {
                    espacosLivres.push_back(espaco);
                    primeiro = false;
                } else
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

void Heuristica::empacotarEspacosLivres(long long int &lucroAtual,
    long long int &areaOcupada, std::vector<Item> &itensAdicionados,
    std::vector<Espaco> &espacosLivres, std::string tipoMochila) {

    while (espacosLivres.size() > 0) {
        Espaco atual = espacosLivres[0];
        espacosLivres.erase(espacosLivres.begin());
        int alturaLivre = atual.h, y = atual.y;
        while (alturaLivre > 0 && Heuristica::existeItemMenor(atual.w, alturaLivre)) {
            mochila.setPeso(atual.w);
            long long int lucroCarregado;
            int maiorAltura;
            std::vector<Item> novaSelecao;
            std::tie(lucroCarregado, maiorAltura, novaSelecao) =
                Heuristica::procedimentoMochila(alturaLivre, tipoMochila);
            Heuristica::procurarEspacosLivres(atual.x, y, maiorAltura,
                lucroCarregado, areaOcupada, espacosLivres, novaSelecao,
                itensAdicionados);
            lucroAtual += lucroCarregado;
            y += maiorAltura;
            alturaLivre -= maiorAltura;
        }
    }
}

std::tuple<long long int, long long int,
    std::vector<std::pair<int, std::vector<Item>>>> Heuristica::formarConjuntos(
    std::string tipoMochila) {

    if (tipoMochila == "fracionaria_")
        mochila.ordenacaoFracionada(I);
    
    std::vector<std::pair<int, std::vector<Item>>> conjuntos;
    int alturaLivre = alturaFolha, x = 0, y = 0;
    long long int lucroTotal = 0, areaOcupada = 0;
    while (alturaLivre > 0 && Heuristica::existeItemMenor(larguraFolha, alturaLivre)) {
        mochila.setPeso(larguraFolha);
        std::vector<Item> itensAdicionados; // Conjunto atual
        std::vector<Espaco> espacosLivres;
        long long int lucroAtual;
        int maiorAltura;
        std::vector<Item> itensSelecionados;
        std::tie(lucroAtual, maiorAltura, itensSelecionados) =
            Heuristica::procedimentoMochila(alturaLivre, tipoMochila);
        Heuristica::procurarEspacosLivres(x, y, maiorAltura, lucroAtual,
            areaOcupada, espacosLivres, itensSelecionados, itensAdicionados);
        Heuristica::empacotarEspacosLivres(lucroAtual, areaOcupada,
            itensAdicionados, espacosLivres, tipoMochila);
        conjuntos.push_back(std::make_pair(lucroAtual, itensAdicionados));
        lucroTotal += lucroAtual;
        y += maiorAltura;
        alturaLivre -= maiorAltura;
    }

    return std::tuple(lucroTotal, areaOcupada, conjuntos);
}
