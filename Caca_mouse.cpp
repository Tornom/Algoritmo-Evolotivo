#include <math.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
#define maxpop 10000
#define mutamuta 4          //valor que incrementa a taxa de mutação para esta abranger maior area de busca;
#include <allegro5/allegro.h>
#include <allegro5/bitmap.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#define largura 1000    //largura da tela;
#define altura 700      //altura da tela;
#define FPS  60.0      // 200.0 original (importante colocar ".0" ao final do número);
#define maxprocura 450  // limitação do tamanho máximo de procura da mutação incrementada;
#define quantomata 500  //quantidade de elemtentos que são eliminados da população;


using namespace std;

typedef struct {

float gene[2]; //(X,Y);
float nota;

}um;                    //struct para guardar informações da população;

um populacao[maxpop];   //vetor população;

um melhor;              //o melhor;

um ultimelhor;          //o ultimo melhor;

int c[quantomata];      //vetor para eliminar parte da população;

int gera;               //qual geração o programa está;

int conta;              //contador que controla quando o programa iniciará a busca expandida;

float taxamutacao;      //taxa com que os valores do cromossomo dos membros será alterado;

void avalia();          //função de elitismo, seleciona o pixel com maior nota;

void crozz();           //função que faz a media entre todos os filhos gerados com o melhor;

void mutacao ();        //altera X e Y em função da taxa de mutação;

void inicia();          //inicia a população aleatóriamente;

void comecome();        //elimina uma quantia de membros da população, substituindo por novos totalmente aleatórios;

ALLEGRO_MOUSE_STATE rato;   //variavel que mantem controle da posição do mouse na tela;

int main()
{

    /*Variaveis allegro*/
    ALLEGRO_DISPLAY *tela;   //warning diz que não estou usando tela para nada, porém ele é usado na linha 82;
    ALLEGRO_EVENT_QUEUE *fila;
    ALLEGRO_TIMER *tempo;
    ALLEGRO_FONT *fonte;
    ALLEGRO_SAMPLE *muziic;


    //inicialização dos módulos da allegro;
    al_init();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();
    al_install_mouse();
    al_init_acodec_addon();
    al_install_audio();
    al_reserve_samples(1);

    tempo = al_create_timer(1/FPS); //cria o timer do programa para funções que dependem do tempo;

    tela = al_create_display(largura,altura); //cria o display com os tamanhos passados para a função;

    /*carrega o audio para o ponteiro muziic (para utilizar o audio
    é necessário existir o arquivo de audio na mesma pasta que o executavel)*/
    muziic = al_load_sample("uuuu.wav");
    fonte = al_load_font("VLADIMIR.TTF",30,0);      //carregar a fonte para a variavel, com tamanho definido;
    fila = al_create_event_queue();    //fila de eventos, seleciona em qual ordem os eventos serão atendidos

    /*registra o teclado e o mouse como entradas de eventos*/
    al_register_event_source(fila,al_get_keyboard_event_source());
    al_register_event_source(fila,al_get_timer_event_source(tempo));

    al_clear_to_color(al_map_rgb(0,0,0)); //limpa a tela para a cor mapeada;

    bool primeira = true;

    srand((unsigned)time(NULL));        //semente randomica;
    inicia();                           //inicia a população;

    bool terminou = false;

    gera = 0;

    conta = 0;

    ultimelhor.nota = 0;

    melhor.nota = 0;

    taxamutacao = 0.05;

    al_start_timer(tempo); //inicia o temporizador para o evento "Timer";
    while(!terminou)
    {
        ALLEGRO_EVENT evento;

        al_wait_for_event(fila,&evento);


        if(evento.type == ALLEGRO_EVENT_TIMER && !primeira)
        {
            gera++;

            avalia();

            mutacao();

            crozz();

            if(gera%10 == 0) comecome();

            if(conta > 200 && taxamutacao < maxprocura)
            {
                al_play_sample(muziic,0.5,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                taxamutacao += mutamuta;
            }
        }


        if(evento.type == ALLEGRO_EVENT_KEY_CHAR)
        {
            if(evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) terminou = true;

            if(evento.keyboard.keycode == ALLEGRO_KEY_P) al_clear_to_color(al_map_rgb(0,0,0));

            if(evento.keyboard.keycode == ALLEGRO_KEY_ENTER)
            {
                primeira = false;       //espera o pressionar do botão enter para iniciar o programa;
            }
        }



        if(al_event_queue_is_empty(fila))
        {
            al_flip_display();          //função que atualiza os desenhos feitos na ultima rodada;
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_textf(fonte,al_map_rgb(255,255,255),150,190,0,"Melhor nota: %0.2f",melhor.nota);
            al_draw_textf(fonte,al_map_rgb(255,255,255),150,220,0,"Geracao: %i, X:%0.2f, Y:%0.2f",gera,melhor.gene[0],melhor.gene[1]);

            for(int j = 0; j < maxpop; j++)
            {
                al_draw_pixel(populacao[j].gene[0], populacao[j].gene[1],al_map_rgb(255,255,255)); // gerar pixel para cada elemento;
            }

        }
    }

    return 0;
}

void inicia()
{
    /*gera uma população totalmente randomica*/
    srand((unsigned)time(NULL));
    for(int i = 0; i < maxpop; i++)
    {
        populacao[i].gene[0] = (float) (rand()%( largura * 10)/10.0);
        populacao[i].gene[1] = (float) (rand()%( altura * 10)/10.0);
    }

}

void avalia()
{
    float x,y,u,v;
    //avalia o melhor
    al_get_mouse_state(&rato); //pega a posição atual do mouse;
    u = rato.x - melhor.gene[0];
    v = rato.y - melhor.gene[1];
    if(u < 0) u = -u;
    if(v < 0) v = -v;
    if(u != 0 && v != 0) melhor.nota = 1/u + 1/v - u*u*u - v*v*v;  //função de fitness (tenho )
    else
    {
        //melhor.nota = 0;
        if(u == v)melhor.nota = 100000000000000;

    }

    for(int i = 0; i < maxpop; i++)
    {
        x = populacao[i].gene[0];
        y = populacao[i].gene[1];

        u = rato.x - x;
        v = rato.y - y;
        if(u < 0) u = - u;
        if(v < 0) v = - v;
        if(u != 0 && v != 0)
        {
            populacao[i].nota = 1/u + 1/v - u*u*u - v*v*v;
        }
        else
        {
            populacao[i].nota = 0;
            if(u == v) populacao[i].nota = 100000000000000;
        }
    }

    for(int j = 0;j < maxpop; j++ )
    {
        if(populacao[j].nota >= melhor.nota)
        {
            ultimelhor.gene[0] = melhor.gene[0];
            ultimelhor.gene[1] = melhor.gene[1];
            ultimelhor.nota = melhor.nota;

            melhor.nota = populacao[j].nota;
            melhor.gene[0] = populacao[j].gene[0];
            melhor.gene[1] = populacao[j].gene[1];
        }

    }

    if(melhor.nota / ultimelhor.nota > 0.9)
    /*se a nota do maior não aumentar em no mínimo 10%, incrementa o
    o contador para a expansão da área de busca*/
    {
        conta++;
    }
    else // caso contrário resete o contador e a área de busca;
    {
        conta = 0;
        taxamutacao = 0.05;
    }
}

void mutacao ()
{
    int roll;

    for(int i = 0; i < maxpop; i++)
    {
        /*esta função é a responsável pelo quadrado que aparece na tela quando a nota do melhor
        permanece constante por um determinado número de gerações(altere as operações para resultados
        inesperados)*/;
        roll  = rand()%10;          //seleciona aleatóriamente uma dessas dez possibilidades;
        if(roll == 0) populacao[i].gene[0] -= taxamutacao;
        if(roll == 1) populacao[i].gene[1] -= taxamutacao;
        if(roll == 2) populacao[i].gene[0] += taxamutacao;
        if(roll == 3) populacao[i].gene[1] += taxamutacao;
        if(roll == 4) populacao[i].gene[0] -= taxamutacao;
        if(roll == 5) populacao[i].gene[1] -= taxamutacao;
        if(roll == 6) populacao[i].gene[0] += taxamutacao;
        if(roll == 7) populacao[i].gene[1] += taxamutacao;
        if(roll == 8) populacao[i].gene[0] -= taxamutacao;
        if(roll == 9) populacao[i].gene[1] -= taxamutacao;
    }

}

void crozz()        //nada muito diferente, somente a media entre o gene do melhor com o gene da população;
{
    for(int i = 0; i < maxpop; i++)
    {
        populacao[i].gene[0] = (melhor.gene[0] + populacao[i].gene[0])/2.0;
        populacao[i].gene[1] = (melhor.gene[1] + populacao[i].gene[1])/2.0;
    }
}

void comecome()
{
/*função (acredito eu) complicadamente desnecessária (sugestões para alterações são bem-vindas)
a ideia da função é criar um vetor com os piores elementos*/
    float pior = populacao[0].nota;
    int aux[2];
    int l = 0;
    for(int i = 0; i < maxpop; i++)
    {
        if(pior > populacao[i].nota)
        {
            for(int p = 0; p < quantomata; p++)
            {
                /*esta troca de posição é a parte que precisa de melhorias(tenho consciecia
                de que ele não seleciona todos os piores mas não alterei esta pois os
                resultados obtidos foram satisfatórios)*/
                aux[l] = c[p];
                if(p == 0)
                {
                    /* "l(L)" é indice do vetor auxiliar, assumindo os valores 0 e este é o motivo
                    pelo qual eu complementei o valor de "l", para alternar entre a primeira e a
                    segunda posição;*/
                    l = !l;
                    c[p] = i;
                }
                else
                {
                    l = !l;
                    c[p] = aux[l];
                }
            }
                pior = populacao[i].nota;
        }
    }

    for(int j = 0; j < quantomata; j++)
    {
        populacao[c[j]].gene[0] = (rand()%(largura * 10)/10.0);
        populacao[c[j]].gene[1] = (rand()%(altura * 10)/10.0);
        populacao[c[j]].nota = 0;

    }
}
