#include "executar.h"

Executar::Executar(unsigned int larguraFolha, unsigned int alturaFolha) {
    this->larguraFolha = larguraFolha;
    this->alturaFolha = alturaFolha;
}

int Executar::totalItens(
    std::vector<std::pair<int, std::vector<Item>>> conjuntos) {

    int totalItensAlocados = 0;
    for (auto par: conjuntos)
        totalItensAlocados += par.second.size();

    return totalItensAlocados;
}

void Executar::resultados(long long int lucroTotal, int totalFilas,
    int totalItensAlocados, long long int areaOcupada, long double perdaCorte,
    double taxaOcupacao, int tempoTotal) {
        
    std::cout << "Área Ocupada: " << areaOcupada << '\n';
    std::cout << "Conjuntos formados: " << totalFilas << '\n';
    std::cout << "Total de itens alocados: " << totalItensAlocados << '\n';
    std::cout << "Lucro total: " << lucroTotal << '\n';
    std::cout << "Perda de corte: " << perdaCorte << '\n';
    std::cout << "Taxa de ocupação: " << taxaOcupacao << '\n';
    std::cout << "Tempo total: " << tempoTotal << "ms\n";
    std::cout << "----------------------------\n";
}

/*
    A instância zdf15 leva mais de 300000 milisegundos para executar com a mochila 0-1, o que gera uma execução em torno de 6 minutos.
    Uma execução que dura em torno de 6 minutos feita 100 vezes dura um total de 600 minutos, que são 10 horas.
*/

DadosExecucao Executar::instancia(std::string tipoLucro, std::string tipoMochila,
    std::string instancia, int numArquivo) {

    Entrada entrada = Entrada(numArquivo, instancia);
    long long int areaTotal = (long int)larguraFolha*(long int)alturaFolha;
    std::vector<DadosExecucao> resExecucoes;
    std::vector<std::pair<int, std::vector<Item>>> conjuntos;
    double mediaTempoTotal = 0.0, mediaTempoMochila = 0.0;

    for (int j = 0; j < 10; j++) {
        std::cout << "Conjunto de itens " << instancia << numArquivo << '\n';
        std::vector<int> I = entrada.getItens(), larguras = entrada.getLarguras(), alturas = entrada.getAlturas();
        Heuristica heuristica = Heuristica(larguraFolha, alturaFolha,
            I, larguras, alturas,
            tipoLucro);

        int totalItens = entrada.getItens().size();
        int totalItensAlocados = 0, tempoTotal = 0, totalConjuntos = 0;
        int tempoMochila;
        long long int lucroTotal;
        long long int areaOcupada;

        auto inicio = std::chrono::system_clock::now();
        std::tie(lucroTotal, areaOcupada, conjuntos) =
            heuristica.formarConjuntos(tipoMochila);
        auto fim = std::chrono::system_clock::now();
        tempoTotal = std::chrono::duration_cast<std::chrono::milliseconds>(fim
            - inicio).count();
        tempoMochila = heuristica.getTemposMochila(); // A soma dos tempos de mochila devem ser obtidos após a formação dos conjuntos (para não se retornar 0)

        long double perdaCorte = ((double)areaTotal - (double)areaOcupada) / (double)areaTotal;
        double taxaOcupacao = ((double)areaOcupada / (double)areaTotal) * 100.0;
        taxaOcupacao = std::ceil(taxaOcupacao * 100.0) / 100.0; // Arredonda para cima em duas casas decimais
        totalConjuntos = conjuntos.size();
        totalItensAlocados = Executar::totalItens(conjuntos);
        resultados(lucroTotal, totalConjuntos, totalItensAlocados, areaOcupada,
            perdaCorte, taxaOcupacao, tempoTotal);

        resExecucoes.push_back(
            DadosExecucao{
                instancia+std::to_string(numArquivo),
                j+1,
                totalItens,
                totalItensAlocados,
                totalConjuntos,
                lucroTotal,
                areaOcupada,
                perdaCorte,
                taxaOcupacao,
                (double)tempoTotal,
                (double)tempoMochila
            }
        );

        mediaTempoTotal += (double)tempoTotal;
        mediaTempoMochila += (double)tempoMochila;
    }

    mediaTempoTotal /= 10.0;
    mediaTempoMochila /= 10.0;

    std::string mochila = ((tipoMochila == "dinamica_") ? "mochila 0-1"
        : "mochila fracionária");
    std::string pasta = "Resultados_"+instancia,
        arquivo = tipoMochila+tipoLucro;

    Escrever escrever = Escrever(resExecucoes, larguraFolha, alturaFolha);
    escrever.CSV(pasta, arquivo, areaTotal, tipoLucro,
        instancia+std::to_string(numArquivo), mediaTempoTotal, mediaTempoMochila);
    escrever.TXT(pasta, arquivo, tipoMochila, tipoLucro,
        instancia+std::to_string(numArquivo), conjuntos, entrada);
    escrever.SVG(pasta, arquivo, tipoMochila, tipoLucro,
        instancia+std::to_string(numArquivo), conjuntos, entrada);

    DadosExecucao execucao = resExecucoes[0];
    execucao.tempoTotal = mediaTempoTotal;
    execucao.tempoMochila = mediaTempoMochila;

    return execucao;
}
