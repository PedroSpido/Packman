struct mapa{
    char** matriz;
    int linhas;
    int colunas;
};

typedef struct mapa MAPA;

struct posicao{
    int x;
    int y;
};

typedef struct posicao POSICAO;

void liberamapa(MAPA* m);
void lemapa(MAPA* m);
void alocamapa(MAPA* m);
void imprimemapa(MAPA* m);
void move(char direcao);
int acabou();
int encontraheroi(MAPA* m, POSICAO* p, char c);
int ehdirecao(char direcao);
int ehvalida(MAPA* m, int x, int y);
int ehvazio(MAPA* m, int x, int y);
void andanomapa(MAPA* m, int xorigem, int yorigem, int xdestino, int ydestino);
void fantasma();
void copiamapa(MAPA* origem, MAPA* destino);
int direcaofantasma(int xorigem, int yorigem, int* destinox, int* destinoy);
int podeandar(MAPA* m, char personagem, int x, int y);
int ehpersonagem(MAPA* m, char personagem, int x, int y);
int ehparede(MAPA* m, int x, int y);
void explodepilula();
void explodepilula2(int x, int y, int qnt, int somax, int somay);
void imprimeparte(char desenho[4][7], int parte);
