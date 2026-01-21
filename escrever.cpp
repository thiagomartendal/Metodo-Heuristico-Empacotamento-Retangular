#include "escrever.h"

Escrever::Escrever(std::vector<DadosExecucao> execucoes, int larguraFolha,
    int alturaFolha) {
    this->execucoes = execucoes;
    this->larguraFolha = larguraFolha;
    this->alturaFolha = alturaFolha;
}

// O método abrirPasta inicia os diretórios para armazenar os arquivos com os dados de execução (como imagens e arquivos de texto)
void Escrever::abrirPasta(std::string pasta) {
    if (!std::filesystem::exists(pasta))
        std::filesystem::create_directories(pasta);
}

// O método CSV escreve os dados obtidos na execução da heurística em um arquivo CSV
void Escrever::CSV(std::string pasta, std::string arquivo, long long int areaTotal, std::string tipoLucro, std::string instancia, double mediaTempoTotal, double mediaTempoMochila) {
    std::string pastaCSV = pasta+"/Resultado/"+std::to_string(larguraFolha)
        +'x'+std::to_string(alturaFolha)+'/'+instancia;
    Escrever::abrirPasta(pastaCSV);
    std::ofstream csv(pastaCSV+'/'+arquivo+".csv");
    
    csv << "Largura da Folha" << ',' << "Altura da Folha" << ','
        << "Área da Folha" << ',' << "Tipo de Lucro" << '\n';
    csv << larguraFolha << ',' << alturaFolha << ',' << areaTotal
        << ',' << tipoLucro << '\n';
    csv << "Instância" << ',' << "Execução" << ','
        << "Total de Itens da Instância" << ',' << "Total de Itens Alocados"
        << ',' << "Conjuntos Formados" << ',' << "Lucro Total" << ','
        << "Área Ocupada" << ',' << "Perda de Corte" << ','
        << "Taxa de Ocupação" << ',' << "Tempo Total" << ','
        << "Soma dos Tempos de Chamada da Mochila" << ',' << "Unidade de Tempo"
        << '\n';
    
    for (DadosExecucao exec: execucoes) {
        csv << exec.instancia << ',' << exec.numExecucao << ','
            << exec.totalItens << ',' << exec.totalItensAlocados << ','
            << exec.conjuntosFormados << ',' << exec.lucroTotal << ','
            << exec.areaOcupada << ',' << exec.perdaDeCorte << ','
            << exec.taxaDeOcupacao << ',' << exec.tempoTotal << ','
            << exec.tempoMochila << ',' << "ms\n";
    }

    csv << "Média do tempo de execução" << ',' << mediaTempoTotal << ',' <<
    "Média da soma dos tempos de mochila" << ',' << mediaTempoMochila << '\n';
    csv.close();
}

// O método TXT escreve os itens e suas coordenadas de alocação pada cada item alocado na folha
void Escrever::TXT(std::string pasta, std::string arquivo, std::string tipoMochila, std::string tipoLucro, std::string instancia, std::vector<std::pair<int, std::vector<Item>>> conjuntos, Entrada entrada) {
    std::string pastaTXT = pasta+"/Itens_empacotados/"
        +std::to_string(larguraFolha)+'x'+std::to_string(alturaFolha)+'/'
        +tipoMochila+tipoLucro+'/'+instancia;
    Escrever::abrirPasta(pastaTXT);
    std::string mochila = tipoMochila;
    mochila.erase(mochila.size()-1, 1);
    std::ofstream txt(pastaTXT+'/'+arquivo+".txt");
    
    txt << "Mochila utilizada: " << mochila << '\n';
    txt << "Lucro atribuído: " << tipoLucro << '\n';
    txt << "Dimensão da folha: " << larguraFolha << 'x'
        << alturaFolha << '\n';
    txt << "{Item, Largura, Altura, Ponto inferior esquerdo (x,y)}\n";
    
    for (std::pair<int, std::vector<Item>> conjunto: conjuntos)
        for (Item item: conjunto.second)
            txt << '{' << item.id << ", " << entrada.getLarguras()[item.id]
                << ", " << entrada.getAlturas()[item.id] << ", (" << item.x
                << ',' << item.y << ")}\n";
    
    txt.close();
}

// O método SVG produz figuras vetoriais do empacotamento obtido em uma folha
void Escrever::SVG(std::string pasta, std::string arquivo, std::string tipoMochila, std::string tipoLucro, std::string instancia, std::vector<std::pair<int, std::vector<Item>>> conjuntos, Entrada entrada) {
    std::string pastaSVG = pasta+"/Imagens_empacotamentos/"
        +std::to_string(larguraFolha)+'x'+std::to_string(alturaFolha)+'/'
        +tipoMochila+tipoLucro+'/'+instancia;
    Escrever::abrirPasta(pastaSVG);
    std::ofstream svg(pastaSVG+'/'+arquivo+".svg");
    
    svg << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n\n";
    svg << "<svg width=\"" << larguraFolha << "cm\" height=\""
        << alturaFolha << "cm\" viewBox=\"0 0 " << larguraFolha
        << " " << alturaFolha << "\">\n";
    svg << "\t<rect id=\"fundo\" style=\"fill:#cccccc;\" x=\"0\" y=\"0\" width=\""
        << larguraFolha << "\" height=\""
        << alturaFolha << "\" />\n";

    for (std::pair<int, std::vector<Item>> conjunto: conjuntos)
        for (Item item: conjunto.second) {
            int largura = entrada.getLarguras()[item.id],
                altura = entrada.getAlturas()[item.id];
            int y = item.y;
            svg << "\t<rect id=\"item" << item.id
                << "\" style=\"fill:#ffffff;stroke:#000000;stroke-width:0.1\" x=\""
                << item.x << "\" y=\"" << y << "\" width=\"" << largura
                << "\" height=\"" << altura
                << "\" transform=\"matrix(1, 0, 0, -1, 0, "
                << alturaFolha << ")\" />\n";
        }
    svg << "</svg>";
    svg.close();
}