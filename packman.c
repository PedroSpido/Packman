#include <stdio.h>
#include <stdlib.h>
#include "packman.h"
#include <string.h>
#include <time.h>

MAPA m;
POSICAO p;
int tempilula = 0;

int ehparede(MAPA* m, int x, int y){
    return
    m->matriz[x][y] == '|' || 
    m->matriz[x][y] == '-';
}

int ehpersonagem(MAPA* m, char personagem, int x, int y){
    return m->matriz[x][y] == personagem;
}

int podeandar(MAPA* m, char personagem, int x, int y){
    return ehvalida(m, x, y) && 
            !ehparede(m, x, y) &&                           
            !ehpersonagem(m, personagem, x, y);
}

int direcaofantasma(int xorigem, int yorigem, 
    int* destinox, int* destinoy){

    int origem[4][2] = {
        { xorigem, yorigem+1}, //direita
        { xorigem, yorigem-1}, //esquerda
        { xorigem+1, yorigem}, //baixo
        { xorigem-1, yorigem}, //cima
    };

    srand(time(0));

    for(int i = 0; i < 10; i++){

        int posicao = rand() % 4;

        if(podeandar(&m, 'F', origem[posicao][0], origem[posicao][1])){
        *destinox = origem[posicao][0];
        *destinoy = origem[posicao][1];
        return 1;
        }
    }
    return 0; 
}  

void fantasma(){
    int i;
    int j;

    MAPA copia;

    copiamapa(&m, &copia);

    for(i = 0; i < m.linhas; i++){
        for(j = 0; j < m.colunas; j++){
            
            if(copia.matriz[i][j] ==  'F'){

                int destinox;
                int destinoy;

                int encontrou = direcaofantasma(i, j, &destinox, &destinoy);

                if(encontrou){
                    andanomapa(&m, i, j, destinox, destinoy);
                    
                }
            }
        }
    }

    liberamapa(&copia);
}

void copiamapa(MAPA* origem, MAPA* destino){
    destino->linhas = origem->linhas;
    destino->colunas = origem->colunas;

    alocamapa(destino);

    for(int i = 0; i < origem->linhas; i++){
        strcpy(destino->matriz[i], origem->matriz[i]);
    }
}

void liberamapa(MAPA* m){
    for(int i=0; i < m->linhas; i++){
        free(m->matriz[i]);
    }
    free(m->matriz);
}

void alocamapa(MAPA* m){
    m->matriz = malloc(sizeof(char*) * m->linhas);

    for(int i=0; i < m->linhas; i++){
        m->matriz[i] = malloc(sizeof(char) * m->colunas+1);
    }  
}

void lemapa(MAPA* m){
    FILE* f;

    f = fopen("mapa.txt", "r"); 
    if(f==0){
        printf("Erro de leitura");
        exit(1);
    }

    fscanf(f, "%d %d", &(m->linhas), &(m->colunas));

    alocamapa(m);

    for(int i = 0; i < m->linhas; i++){
        fscanf(f, "%s", m->matriz[i]);
    }

    fclose(f);
}

char desenhoparede[4][7] = {
    {"......" },
    {"......" },
    {"......" },
    {"......" }
};

char desenhoheroi[4][7] = {
    {" .--. "  },
    {"/ _.-'"  },
    {"\\  '-." },
    {" '--' "  }
};

char desenhopilula[4][7] = {
    {"      "},
    {" .-.  "},
    {" '-'  "},
    {"      "}
};

char desenhovazio[4][7] = {
    {"      "},
    {"      "},
    {"      "},
    {"      "}
};

char desenhofantasma[4][7] = {
    {" .-.  " },
    {"| OO| " },
    {"|   | " },
    {"'^^^' " }
};

void imprimeparte(char desenho[4][7], int parte){
        printf("%s", desenho[parte]);
}

void imprimemapa(MAPA* m){
    for(int i = 0; i < m->linhas; i++){
        for(int parte = 0; parte < 4; parte++){
            for(int j = 0; j < m->colunas; j++){

                switch (m->matriz[i][j]){
                case 'F':
                    imprimeparte(desenhofantasma, parte);     
                    break;
                case '.':
                    imprimeparte(desenhovazio, parte);
                    break;
                case '-':
                case '|':
                    imprimeparte(desenhoparede, parte);
                    break;
                case '@':
                    imprimeparte(desenhoheroi, parte);
                    break;
                case 'P':
                    imprimeparte(desenhopilula, parte);
                    break;
                }
            }
            printf("\n");
        }
    }
}

int encontraheroi(MAPA* m, POSICAO* p, char c){
    int i;
    int j;
    for(i = 0; i < m->linhas; i++){
        for(j = 0; j < m->colunas; j++){
            if(m->matriz[i][j] ==  c){
                p->x = i;
                p->y = j;
                return 1;
            }
        }
    }
    return 0;
}

int acabou(){
    POSICAO pos;
    int encontrouheroi = encontraheroi(&m, &pos, '@');
    return !encontrouheroi;
}

int ehdirecao(char direcao){
        return(direcao =='a' || direcao =='s' || direcao =='d' || direcao =='w');
}

int ehvalida(MAPA* m, int x, int y){
    if(y >= (*m).colunas){
        return 0;
    }
    if(x >= (*m).linhas){
        return 0;
    }
    return 1;
}

int ehvazio(MAPA* m, int x, int y){
    return((*m).matriz[x][y] == '.');   
}

void andanomapa(MAPA* m, int xorigem, int yorigem, 
int xdestino, int ydestino){

    char personagem = m->matriz[xorigem][yorigem];
    m->matriz[xdestino][ydestino] = personagem;
    m->matriz[xorigem][yorigem] = '.';

}

void move(char direcao){

    if(!ehdirecao(direcao)){
        return;
    }

    int proximox = p.x;
    int proximoy = p.y;

    switch (direcao)
    {
    case 'a':
        proximoy--;
        break;
    case 's':
        proximox++;
        break;
    case 'd':
        proximoy++;
        break;
    case 'w':
        proximox--;
        break;
    }

    if(!podeandar(&m, '@', proximox, proximoy)){
        return;
    }

    if(ehpersonagem(&m, 'P', proximox, proximoy)){
        tempilula = 1;
    }

    andanomapa(&m, p.x, p.y, proximox, proximoy);

    p.x = proximox;
    p.y = proximoy;

}

void explodepilula(){

    if(!tempilula) return;

    explodepilula2(p.x, p.y, 3, 0, 1);  //direita
    explodepilula2(p.x, p.y, 3, 1, 0);  //baixo
    explodepilula2(p.x, p.y, 3, -1, 0); //cima
    explodepilula2(p.x, p.y, 3, 0, -1); //esquerda

    tempilula = 0;
}

void explodepilula2(int x, int y, int qnt, int somax, int somay){

    int novox = x + somax; 
    int novoy = y + somay;

    if (qnt == 0) return;
    if(!ehvalida(&m, novox, novoy)) return;
    if(ehparede(&m, novox, novoy)) return;
    
    m.matriz[novox][novoy] = '.';

    explodepilula2(novox, novoy, qnt-1, somax, somay);
}

int main(){
    
    lemapa(&m);
    
    encontraheroi(&m, &p, '@');

    do{
        printf("Tem pilula? %s\n", (tempilula ? "SIM" : "NAO"));
        imprimemapa(&m);

        char comando;
        scanf(" %c", &comando);

        move(comando);

        if(comando == 'b') explodepilula();
        
        fantasma();

    }while(!acabou());
    
    liberamapa(&m);
}