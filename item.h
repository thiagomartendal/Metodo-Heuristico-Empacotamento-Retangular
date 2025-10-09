#ifndef ITEM_H
#define ITEM_H

struct Item {
    int id;
    long long int valor; // Valor de lucro de uma determinada etapa (utilizado na mochila 0-1)
    long long int lucro; // Lucro atribuído ao item no vetor de lucros
    int x, y; // Canto inferior esquerdo
};

#endif
