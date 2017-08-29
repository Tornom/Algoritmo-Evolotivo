#include <math.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
#define maxpop 10000
#define mutamuta 4          //valor que incrementa a taxa de muta��o para esta abranger maior area de busca;
#include <allegro5/allegro.h>
#include <allegro5/bitmap.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#define largura 1000    //largura da tela;
#define altura 700      //altura da tela;
#define FPS  60.0      // 200.0 original (importante colocar ".0" ao final do n�mero);
#define maxprocura 450  // limita��o do tamanho m�ximo de procura da muta��o incrementada;
#define quantomata 500  //quantidade de elemtentos que s�o eliminados da popula��o;


using namespace std;

typedef struct {

float gene[2]; //(X,Y);
float nota;

}um;                    //struct para guardar informa��es da popula��o;

um populacao[maxpop];   //vetor popula��o;

um melhor;              //o melhor;

um ultimelhor;          //o ultimo melhor;

int c[quantomata];      //vetor para eliminar parte da popula��o;

int gera;               //qual gera��o o programa est�;

int conta;              //contador que controla quando o programa iniciar� a busca expandida;

float taxamutacao;      //taxa com que os valores do cromossomo dos membros ser� alterado;

void avalia();          //fun��o de elitismo, seleciona o pixel com maior nota;

void crozz();           //fun��o que faz a media entre todos os filhos gerados com o melhor;

void mutacao ();        //altera X e Y em fun��o da taxa de muta��o;

void inicia();          //inicia a popula��o aleat�riamente;

void comecome();        //elimina uma quantia de membros da popula��o, substituindo por novos totalmente aleat�rios;

ALLEGRO_MOUSE_STATE rato;   //variavel que mantem controle da posi��o do mouse na tela;

int main()
{

    /*Variaveis allegro*/
    ALLEGRO_DISPLAY *tela;   //warning diz que n�o estou usando tela para nada, por�m ele � usado na linha 82;
    ALLEGRO_EVENT_QUEUE *fila;
    ALLEGRO_TIMER *tempo;
    ALLEGRO_FONT *fonte;
    ALLEGRO_SAMPLE *muziic;


    //inicializa��o dos m�dulos da allegro;
    al_init();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();
    al_install_mouse();
    al_init_acodec_addon();
    al_install_audio();
    al_reserve_samples(1);

    tempo = al_create_timer(1/FPS); //cria o timer do programa para fun��es que dependem do tempo;

    tela = al_create_display(largura,altura); //cria o display com os tamanhos passados para a fun��o;

    /*carrega o audio para o ponteiro muziic (para utilizar o audio
    � necess�rio existir o arquivo de audio na mesma pasta que o executavel)*/
    muziic = al_load_sample("uuuu.wav");
    fonte = al_load_font("VLADIMIR.TTF",30,0);      //carregar a fonte para a variavel, com tamanho definido;
    fila = al_create_event_queue();    //fila de eventos, seleciona em qual ordem os eventos ser�o atendidos

    /*registra o teclado e o mouse como entradas de eventos*/
    al_register_event_source(fila,al_get_keyboard_event_source());
    al_register_event_source(fila,al_get_timer_event_source(tempo));

    al_clear_to_color(al_map_rgb(0,0,0)); //limpa a tela para a cor mapeada;

    bool primeira = true;

    srand((unsigned)time(NULL));        //semente randomica;
    inicia();                           //inicia a popula��o;

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
                primeira = false;       //espera o pressionar do bot�o enter para iniciar o programa;
            }
        }



        if(al_event_queue_is_empty(fila))
        {
            al_flip_display();          //fun��o que atualiza os desenhos feitos na ultima rodada;
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
    /*gera uma popula��o totalmente randomica*/
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
    al_get_mouse_state(&rato); //pega a posi��o atual do mouse;
    u = rato.x - melhor.gene[0];
    v = rato.y - melhor.gene[1];
    if(u < 0) u = -u;
    if(v < 0) v = -v;
    if(u != 0 && v != 0) melhor.nota = 1/u + 1/v - u*u*u - v*v*v;  //fun��o de fitness (tenho )
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
    /*se a nota do maior n�o aumentar em no m�nimo 10%, incrementa o
    o contador para a expans�o da �rea de busca*/
    {
        conta++;
    }
    else // caso contr�rio resete o contador e a �rea de busca;
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
        /*esta fun��o � a respons�vel pelo quadrado que aparece na tela quando a nota do melhor
        permanece constante por um determinado n�mero de gera��es(altere as opera��es para resultados
        inesperados)*/;
        roll  = rand()%10;          //seleciona aleat�riamente uma dessas dez possibilidades;
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

void crozz()        //nada muito diferente, somente a media entre o gene do melhor com o gene da popula��o;
{
    for(int i = 0; i < maxpop; i++)
    {
        populacao[i].gene[0] = (melhor.gene[0] + populacao[i].gene[0])/2.0;
        populacao[i].gene[1] = (melhor.gene[1] + populacao[i].gene[1])/2.0;
    }
}

void comecome()
{
/*fun��o (acredito eu) complicadamente desnecess�ria (sugest�es para altera��es s�o bem-vindas)
a ideia da fun��o � criar um vetor com os piores elementos*/
    float pior = populacao[0].nota;
    int aux[2];
    int l = 0;
    for(int i = 0; i < maxpop; i++)
    {
        if(pior > populacao[i].nota)
        {
            for(int p = 0; p < quantomata; p++)
            {
                /*esta troca de posi��o � a parte que precisa de melhorias(tenho consciecia
                de que ele n�o seleciona todos os piores mas n�o alterei esta pois os
                resultados obtidos foram satisfat�rios)*/
                aux[l] = c[p];
                if(p == 0)
                {
                    /* "l(L)" � indice do vetor auxiliar, assumindo os valores 0 e este � o motivo
                    pelo qual eu complementei o valor de "l", para alternar entre a primeira e a
                    segunda posi��o;*/
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
