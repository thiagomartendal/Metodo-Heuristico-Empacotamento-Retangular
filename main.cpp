#include <iostream>
#include "executar.h"

// Conjunto representa um arquivo de instância de dados com suas dimensões de folha de estoque associadas para os experimentos
struct Conjunto {
    int arquivo;
    std::pair<int, int> dimensao;
};

// A função csvMaster unifica os dados das 10 etapas de execução realizadas em cada instância, onde os valores dos tempos são unificados através das médias
void csvMaster(std::vector<DadosExecucao> &execucoes, std::string tipoLucro, std::string tipoMochila, std::string instancia) {
    std::string pasta = "Resultados_"+instancia, pastaCSV = pasta+"/Resultado/",
    arquivo = tipoMochila+tipoLucro+"_master";

    std::ofstream csv(pastaCSV+'/'+arquivo+".csv");
    csv << "Instância" << ',' << "Total de Itens Alocados" << ','
        << "Lucro Total";
    
    if (tipoLucro != "area")
        csv << ',' << "Área Ocupada" << ',';
    else
        csv << ',';
    
    csv << "Perda de Corte" << ',' << "Taxa de Ocupação" << ',' << "Média do Tempo Total"
        << ',' << "Média da Soma dos Tempos de Chamada da Mochila" << '\n';

    for (DadosExecucao exec: execucoes) {
        std::string inst = exec.instancia;
        
        if (instancia == "c") inst[0] = 'C';

        csv << inst << ',' << exec.totalItensAlocados << ',' << exec.lucroTotal;
        
        if (tipoLucro != "area")
            csv << ',' << exec.areaOcupada << ',';
        else
            csv << ',';
        
        csv << exec.perdaDeCorte << ',' << exec.taxaDeOcupacao << ','
            << exec.tempoTotal << ',' << exec.tempoMochila << '\n';
    }
}

// A função experimentos realiza a execução das instâncias de entrada junto as dimensões de folha de estoque associadas nos conjuntos C, gcut e zdf
void experimentos(std::string tipoLucro, std::string tipoMochila, std::string instancia) {
    // {índice do arquivo, {largura da folha, altura da folha}}
    std::vector<Conjunto> C = {
        {11, {20, 20}}, // C11
        {12, {20, 20}}, // C12
        {13, {20, 20}}, // C13
        {21, {40, 15}}, // C21
        {22, {40, 15}}, // C22
        {23, {40, 15}}, // C23
        {31, {60, 30}}, // C31
        {32, {60, 30}}, // C32
        {33, {60, 30}}, // C33
        {41, {60, 60}}, // C41
        {42, {60, 60}}, // C42
        {43, {60, 60}}, // C43
        {51, {60, 90}}, // C51
        {52, {60, 90}}, // C52
        {53, {60, 90}}, // C53
        {61, {80, 120}}, // C61
        {62, {80, 120}}, // C62
        {63, {80, 120}}, // C63
        {71, {160, 240}}, // C71
        {72, {160, 240}}, // C72
        {73, {160, 240}}, // C73
    };

    std::vector<Conjunto> gcut = {
        {1, {250, 1016}}, // gcut1
        {2, {250, 1133}}, // gcut2
        {3, {250, 1803}}, // gcut3
        {4, {250, 2934}}, // gcut4
        {5, {500, 1172}}, // gcut5
        {6, {500, 2514}}, // gcut6
        {7, {500, 4641}}, // gcut7
        {8, {500, 5703}}, // gcut8
        {9, {1000, 2022}}, // gcut9
        {10, {1000, 5356}}, // gcut10
        {11, {1000, 6537}}, // gcut11
        {12, {1000, 12522}}, // gcut12
        {13, {3000, 4772}} // gcut13
    };

    std::vector<Conjunto> zdf = {
        {1, {100, 330}}, // zdf1
        {2, {100, 357}}, // zdf2
        {3, {100, 384}}, // zdf3
        {4, {100, 407}}, // zdf4
        {5, {100, 434}}, // zdf5
        {6, {3000, 4872}}, // zdf6
        {7, {3000, 4852}}, // zdf7
        {8, {3000, 5172}}, // zdf8
        {9, {3000, 5172}}, // zdf9
        {10, {6000, 5172}}, // zdf10
        {11, {6000, 5172}}, // zdf11
        {12, {6000, 5172}}, // zdf12
        {13, {9000, 5172}}, // zdf13
        {14, {3000, 5172}}, // zdf14
        {15, {3000, 5172}} // zdf15
    };
    if (instancia == "zdf" && tipoMochila == "fracionaria_")
        zdf.push_back({16, {3000, 5172}}); // zdf16

    std::vector<Conjunto> conjunto = (
        (instancia == "c") ? C : (instancia == "gcut") ? gcut : zdf
    );

    std::vector<DadosExecucao> execucoes;
    for (int i = 0; i < conjunto.size(); i++) {
        Conjunto atual = conjunto[i];
        Executar executar = Executar(atual.dimensao.first, atual.dimensao.second);
        DadosExecucao execucao = executar.instancia(tipoLucro, tipoMochila, instancia, atual.arquivo);
        execucoes.push_back(execucao);
    }

    csvMaster(execucoes, tipoLucro, tipoMochila, instancia);

    /* Mochila 0-1 não executa com as instâncias:
    * zdf16 para 3000x5172
    */
}

int main(int argc, char **argv) {
    std::string instancia = argv[1]; // c - gcut - zdf
    
    std::string argv2 = argv[2];
    std::string tipoLucro;
    if (argv2 == "0")
        tipoLucro = "area";
    else if (argv2 == "1")
        tipoLucro = "largura";
    else if (argv2 == "2")
        tipoLucro = "altura";

    std::string argv3 = argv[3];
    std::string tipoMochila = ((argv3 == "d") ? "dinamica_" : "fracionaria_");

    experimentos(tipoLucro, tipoMochila, instancia);

    return 0;
}

/**
 * Exemplo de execução:
 * ./Main c 0 d
 * Serão feitas execuções considerando o conjunto C de instância, utilizando a modalidade de área dos itens como lucro associado, e utilizando a mochila 0-1
 * para selecionar os itens em cada etapa de alocação.
 */