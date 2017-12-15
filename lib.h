#include <allegro5/allegro.h>                       //inclue as principais funções da allegro 5;
#include <allegro5/allegro_image.h>                 //inclue manipulação e manuseio de imagens;
#include <allegro5/allegro_primitives.h>            //inclue manipulação e manuseio de primitivas;
#include <allegro5/allegro_font.h>                  //inclue manipulação e manuseio de fontes;
#include <allegro5/allegro_ttf.h>                   //inclue manipulação e manuseio do formato .ttf;
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/display.h>
#include <math.h>
#include <stdio.h>

//valores para a parte gráfica
#define FPS 450.0                                   //necessário a presença do ".0" ||450||;
#define largura 1300
#define altura 820

//valores para a simulação
#define max_pop 40
#define num_max_movimento 300
#define taxa_muta 50
#define quanto_muta 1
#define nota_pela_vitoria 20
#define nota_pela_derrota 10
#define nota_pelo_empate 13
#define numero_bala 3
#define tempo_para_apagar_bala FPS*5
#define geracao_preda 10
#define valor_inicial 999999
#define coef 200

//valores maximos player
#define maximo_prot 6
#define max_atk_spd 10
#define max_dano 10
#define max_range_corpo 50
#define max_range_range 400
#define crit_max 0

//minimo player
#define min_prot 0
#define min_atk_spd 2
#define min_atk_spd_rang 2
#define min_dano 1
#define min_range_corpo 20
#define min_range_range 150

//valores fixos para o player
#define vel_corpo 8
#define vel_range 8
#define maior_hp 10
#define turno_b  20
#define tamanho_blinky 0
#define cte_critico 3000




typedef struct
{
    short movx;
    short movy;
    short tempo;
    int rangex;
    int rangey;
    bool viva;
    float xvel;
    float yvel;
    int x;
    int y;

}bullet;

typedef struct
{
    int dano;
    int range;
    int tempo_atk;
    int qual_arma;              //0 = espada(corpo_a_corpo); 1 = arco e flecha(ranged)
    bullet bala[10];
    int alvox;
    int alvoy;
    int cool;
    int quanto_atk;

}arma;

typedef struct                  //struct para a sprite;
{
    bool ativo;

    float x;                    //coordenada X da sprite;
    float y;                    //coordenada Y da sprite;
    float xant[2];
    float yant[2];
    float velx;                 //velocidade X da sprite;
    float vely;                 //velocidade Y da sprite;
    short movx;                 //sentido X da sprite;
    short movy;                 //sentido Y da sprite;

    int framemax;               //numero maximo de frames da sprite;
    int frameatual;             //valor do frame atual da animação;
    int framecount;             //contador para atrazo de animação;
    int frameatrazo;            //valor de atrazo de animação;
    int framelargura;           //lagura da sprite;
    int framealtura;            //altura da sprite;
    int colunaanima;            //quantas colunas possui a animação;
    int movanima;               //define em qual sentido a sprite ira mover sua animaçao;

    ALLEGRO_BITMAP *imagem;     //sprite;
    ALLEGRO_BITMAP *anima[10];

}fanta;

typedef struct
{
    arma weap;
    int hp;
    int maxhp;
    int protecao;
    int crit;
    float nota;
    int blink;
    int tamanho_blink;
    fanta imagem;
    bool ja_participou;

}popula;

enum {corpo_a_corpo,ranged};

int simulacao(int extra_fps,int geracao_predador,int numero_populacao);

void ia(popula &one,popula&two);

popula inicia_populacao(bool ja_iniciou);

popula torneio(int i, popula vet[],int taxa_mutacao,int valor_mutacao,int tam_vet,int melhor_i);

void fronteira(popula &one);

void atira_arco(popula &one);

void move_projetil(popula &one);

bool avalia_populacao(popula &one,popula &two,bool empate, int conta);

void anima_player(popula &one);

void calibra_fanta(popula &one);


