#include <iostream>
#include "lib.h"


using namespace std;                               //função de iostream que define o local de uso padrão como alvo de suas funções;


int main()
{

    //variaveis;
    bool terminou = false;
    bool repinta = true;

    //variaveis allegro;
    ALLEGRO_EVENT_QUEUE *fila;                      //fila de eventos a serem analizados;
    ALLEGRO_TIMER *tempo;                           //temporizador;
    ALLEGRO_DISPLAY *tela;                          //objeto do display;
    ALLEGRO_DISPLAY_MODE motela;                    //propriedades do display;
    ALLEGRO_FONT* arial = NULL;

    //inicialização allegro;
    al_init();                                      //inicializa as principais funções da biblioteca allegro 5;
    al_init_image_addon();                          //inicializa funções especificas para imagens da biblioteca allegro 5;
    al_init_primitives_addon();                     //inicializa funções especificas para criação de primitivos da biblioteca allegro 5;
                                                    /*inicar fonte antes de ttf*/
    al_init_font_addon();                           //inicializa funções especificas para fontes da biblioteca allegro 5;
    al_init_ttf_addon();                            //inicializa funções especificas para formato .ttf da biblioteca allegro 5;
    al_install_audio();                             //inicializa funções especificas para audios da biblioteca allegro 5;
    al_init_acodec_addon();                         //inicializa funções especificas para audios da biblioteca allegro 5;
    al_install_keyboard();                          //inicializa funções especificas para reconhecimento do teclado;
    al_install_mouse();                             //inicializa funções especificas para reconhecimento do mouse;

    //criação de timer e tela(tamanho fixou ou fullscreen);
    al_get_display_mode(al_get_num_display_modes()-1,&motela);  //retorna valores para configuração do modo tela cheia do display;
    al_set_new_display_flags(ALLEGRO_WINDOWED);               //cria flag para o novo display ser reconhecido como tela cheia;
    tempo = al_create_timer(1/FPS);                             //cria timer para disparar na frequencia FPS;
    tela = al_create_display(motela.width,motela.height);       //MODO FULLSCREEN (tela cheia);

    //registro de fila e eventos;
    fila = al_create_event_queue();                                     //inicializa a fila de eventos;
    al_register_event_source(fila,al_get_keyboard_event_source());      //configura a fila para receber eventos provenientes do teclado;
    al_register_event_source(fila,al_get_mouse_event_source());         //configura a fila para receber eventos provenientes do mouse;
    al_register_event_source(fila,al_get_display_event_source(tela));   //configura a fila para receber eventos provenientes da tela;
    al_register_event_source(fila,al_get_timer_event_source(tempo));    //configura a fila para receber eventos provenientes do timer;

    arial = al_load_font("times.ttf",60,0);

    al_clear_to_color(al_map_rgb(0,0,0));                       //limpa a tela com a cor preta RGB(0,0,0);

    al_draw_textf(arial,al_map_rgb(255,255,255),motela.width/2.0,200,ALLEGRO_ALIGN_CENTER,"bem vindo ao simulador,");
    al_draw_textf(arial,al_map_rgb(255,255,255),motela.width/2.0,270,ALLEGRO_ALIGN_CENTER,"para iniciar o processo aperte ENTER");


    al_start_timer(tempo);                                      //inicia o contador de tempo em sincronia com o loop;
    while(!terminou)
    {
        ALLEGRO_EVENT evento;                                   //cria um objeto evento para reber as entradas;

        al_wait_for_event(fila,&evento);                        //força o loop a esperar até que apareça um evento;

        if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)          //evento que ativa ao pressionar do botão "X" da tela;
        {
            terminou = true;
        }//fim display close;

        if(evento.type == ALLEGRO_EVENT_KEY_DOWN)               //evento que ativa ao pressionar de qualquer botão do teclado;
        {
            if(evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                terminou = true;
            }

            if(evento.keyboard.keycode == ALLEGRO_KEY_ENTER)
            {
                simulacao(0,geracao_preda,max_pop);
                al_flush_event_queue(fila);
                al_set_target_backbuffer(tela);
            }

        }//fim event key down;


        if(evento.type == ALLEGRO_EVENT_KEY_UP)
        {


        }

        if(evento.type == ALLEGRO_EVENT_TIMER)                  //evento que ativa na frequencia estipulada para o timer;
        {

        }//fim event timer;

        if(al_event_queue_is_empty(fila))                       //evento que ativa no momento em que a fila está vazia;
        {

            if(repinta)                                         //variável que mantém o controle de quando é necessário atualizar a tela;
            {

                al_flip_display();
                repinta = false;
            }
        }
    }

    al_destroy_display(tela);
    al_destroy_event_queue(fila);
    al_destroy_timer(tempo);
    al_destroy_font(arial);

    return 0;
}

int simulacao(int extra_fps,int geracao_predador,int numero_populacao)
{
    srand((unsigned)time(NULL));
    ALLEGRO_TIMER* tempo2;
    ALLEGRO_EVENT_QUEUE* fila2;
    ALLEGRO_DISPLAY *tela2;
    ALLEGRO_BITMAP *BACKGROUND;


    tela2 = al_create_display(largura,altura);
    al_set_target_backbuffer(tela2);
    BACKGROUND = al_load_bitmap("new_back.png");

    /*variaveis de controle da simulaçao*/
    int atual_geracao = 0;
    int qual_pop = 0;
    int qual_pop_outro = 0;
    int k,melhor_i,pior_i;
    bool imprime = true;
    bool timer = false;
    bool ativar_grafico = true;
    bool termina_simulacao = false;


    tempo2 = al_create_timer(1/FPS+extra_fps);
    fila2 = al_create_event_queue();
    al_register_event_source(fila2,al_get_keyboard_event_source());         //configura a fila para receber eventos provenientes do teclado;
    al_register_event_source(fila2,al_get_mouse_event_source());            //configura a fila para receber eventos provenientes do mouse;
    al_register_event_source(fila2,al_get_display_event_source(tela2));      //configura a fila para receber eventos provenientes da tela;
    al_register_event_source(fila2,al_get_timer_event_source(tempo2));      //configura a fila para receber eventos provenientes do timer;

    /*inicia a populacao*/
    popula vet_popula[numero_populacao];
    for(int i = 0; i < numero_populacao; i++)
    {
        vet_popula[i] = inicia_populacao(false);
    }

    while(!termina_simulacao)
    {
        atual_geracao++;

        //avaliação;
        for(int o = 0; o < numero_populacao; o++)
        {
            int conta_movimento = 0;
            bool terminou = false;
            int aux;

            //if(vet_popula[o].weap.qual_arma == corpo_a_corpo) cout<<"<<<"<<vet_popula[o].weap.dano<<">>>\n"<<flush;

            if(vet_popula[o].ja_participou) continue;
            for(aux = o + 1; aux < numero_populacao; aux++)
            {
                if(!vet_popula[aux].ja_participou && aux < numero_populacao) break;
            }
            if(aux == numero_populacao)
            {
                break;
            }
            qual_pop = o;
            qual_pop_outro = aux;

            al_draw_scaled_bitmap(BACKGROUND,0,0,1000,800,0,0,largura,altura,0);
            al_start_timer(tempo2);
            while (!terminou)
            {
                ALLEGRO_EVENT evento;

                al_wait_for_event(fila2,&evento);

                if(evento.type == ALLEGRO_EVENT_KEY_DOWN)
                {

                    if(evento.keyboard.keycode == ALLEGRO_KEY_G)
                    {
                        ativar_grafico = !ativar_grafico;
                    }

                    if(evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    {
                        termina_simulacao = true;
                    }

                    if(evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
                    {
                        terminou = true;
                    }
                }

                if(evento.type == ALLEGRO_EVENT_TIMER)
                {
                    timer = true;
                }

                if(al_event_queue_is_empty(fila2))
                {
                    if(termina_simulacao) break;

                    if(timer)
                    {
                        //cout<<conta_movimento<<"|\n"<<flush;
                        if(++conta_movimento >= num_max_movimento)
                        {
                            terminou = avalia_populacao(vet_popula[qual_pop],vet_popula[qual_pop_outro],true,conta_movimento);
                            conta_movimento = 0;
                            break;
                        }
                        timer = false;
                        ia(vet_popula[qual_pop],vet_popula[qual_pop_outro]);
                        ia(vet_popula[qual_pop_outro],vet_popula[qual_pop]);
                        fronteira(vet_popula[qual_pop]);
                        fronteira(vet_popula[qual_pop_outro]);

                        atira_arco(vet_popula[qual_pop]);
                        atira_arco(vet_popula[qual_pop_outro]);

                        move_projetil(vet_popula[qual_pop]);
                        move_projetil(vet_popula[qual_pop_outro]);

                        terminou = avalia_populacao(vet_popula[qual_pop],vet_popula[qual_pop_outro],false,conta_movimento);

                        //cout<<"|"<<vet_popula[qual_pop].weap.bala[0].viva<<"|"<<flush;
                        //cout<<vet_popula[qual_pop].imagem.x<<"<>"<<vet_popula[qual_pop].imagem.y<<"|"<<vet_popula[qual_pop_outro].imagem.x<<"<>"<<vet_popula[qual_pop_outro].imagem.y<<"|\n"<<flush;
                        imprime = true;
                    }

                    if(imprime && ativar_grafico)
                    {
                        al_draw_scaled_bitmap(BACKGROUND,0,0,1000,800,0,0,largura,altura,0);
                        al_draw_tinted_bitmap(vet_popula[qual_pop].imagem.imagem,al_map_rgb(0,255,255),vet_popula[qual_pop].imagem.x,vet_popula[qual_pop].imagem.y,0);
                        al_draw_bitmap(vet_popula[qual_pop_outro].imagem.imagem,vet_popula[qual_pop_outro].imagem.x,vet_popula[qual_pop_outro].imagem.y,0);
                        for(int b = 0; b < numero_bala; b++)
                        {

                            if(vet_popula[qual_pop].weap.bala[b].viva)
                            {
                                al_draw_filled_circle(vet_popula[qual_pop].weap.bala[b].x,vet_popula[qual_pop].weap.bala[b].y,2,al_map_rgb(255,0,0));
                            }

                            if(vet_popula[qual_pop_outro].weap.bala[b].viva)
                            {
                                al_draw_filled_circle(vet_popula[qual_pop_outro].weap.bala[b].x,vet_popula[qual_pop_outro].weap.bala[b].y,2,al_map_rgb(255,0,0));
                            }
                        }


                        al_flip_display();
                        imprime = false;
                    }
                }


            }//fim loop

            if(termina_simulacao) break;

        }//fim for maior;

        //procura melhor i e pior i
        int auxi = 0;
        int auxp = 0;
        int auxmenor = INFINITE;
        int auxnota = 0;
        for(k = 0; k < numero_populacao; k++)
        {
            if(vet_popula[k].nota < auxmenor)
            {
                auxp = k;
                auxmenor = vet_popula[k].nota;
            }

            if(vet_popula[k].nota > auxnota)
            {
                auxi = k;
                auxnota = vet_popula[k].nota;
            }
        }
        melhor_i = auxi;
        pior_i = auxp;

        cout<<"melhor_nota:"<<vet_popula[melhor_i].nota<<"|\n"<<flush;

        //torneio + crossover + mutação
        for(k = 0; k < numero_populacao; k++ )
        {
            vet_popula[k] = torneio(k,vet_popula,taxa_muta,quanto_muta,numero_populacao,melhor_i);
        }

         //predação
        if(atual_geracao % geracao_predador == 0) vet_popula[pior_i] = inicia_populacao(true);


    }

    //encerra simulação
    for(int j = 0; j < numero_populacao; j++)
    {
        al_destroy_bitmap(vet_popula[j].imagem.imagem);
    }

    al_destroy_bitmap(BACKGROUND);
    al_destroy_timer(tempo2);
    al_destroy_event_queue(fila2);
    al_destroy_display(tela2);
    return 1;
}

void ia (popula &one,popula &two)
{
    int blinky = 0;
    int cat1,cat2;
    int refino = 0;
    one.weap.cool++;

    if(one.weap.qual_arma == corpo_a_corpo)
    {
        cat1 = (one.imagem.x + one.imagem.framelargura /2.0) - (two.imagem.x + two.imagem.framelargura /2.0);
        cat2 = (one.imagem.y + one.imagem.framealtura  /2.0) - (two.imagem.y + two.imagem.framealtura  /2.0);

        if(cat1 < 2*one.imagem.velx && cat2 < 2*one.imagem.vely) refino = 5;

        if(one.imagem.x < two.imagem.x) one.imagem.x += one.imagem.velx - refino - one.protecao;
        else if(one.imagem.x > two.imagem.x) one.imagem.x -= one.imagem.velx - refino - one.protecao;

        if(one.imagem.y < two.imagem.y) one.imagem.y += one.imagem.vely - refino - one.protecao;
        else if(one.imagem.y > two.imagem.y) one.imagem.y -= one.imagem.vely - refino - one.protecao;

        //cout<<one.weap.range<<"|"<<cat1<<"|"<<cat2<<"|"<<one.weap.cool<<"|"<<one.weap.tempo_atk<<"|\n"<<flush;

        if(abs(cat1) <= one.weap.range && abs(cat2) <= one.weap.range && one.weap.cool >= one.weap.tempo_atk * FPS)
        {
            one.weap.quanto_atk++;
            if(one.weap.dano - two.protecao > 0) two.hp -= one.weap.dano - two.protecao;
            one.weap.cool = 0;
        }
    }

    else
    {
        one.blink--;
        if(one.blink == 0)
        {
            one.blink = turno_b;
            blinky = one.tamanho_blink;
        }

        one.weap.alvox = two.imagem.x;
        one.weap.alvoy = two.imagem.y;

        for(int j = 0; j < numero_bala; j++)
        {
            if(one.weap.bala[j].viva)
            {
                if(one.weap.bala[j].x > two.imagem.x && one.weap.bala[j].x < two.imagem.x + two.imagem.framelargura
                && one.weap.bala[j].y > two.imagem.y && one.weap.bala[j].y < two.imagem.y + two.imagem.framealtura)
                {
                    one.weap.bala[j].viva = false;
                    if(one.weap.dano - two.protecao > 0) two.hp -= one.weap.dano - two.protecao;
                }
            }
        }

        if(abs(two.imagem.x - one.imagem.x) <= one.weap.range - one.imagem.framelargura && abs(two.imagem.y - one.imagem.y) <= one.weap.range - one.imagem.framealtura)
        {
            if(one.imagem.x <= two.imagem.x)
            {
                one.imagem.movx = -1;
                one.imagem.x -= one.imagem.velx + blinky - one.protecao;
            }
            else if(one.imagem.x > two.imagem.x)
            {
                one.imagem.movx = 1;
                one.imagem.x += one.imagem.velx + blinky - one.protecao;
            }

            if(one.imagem.y <= two.imagem.y)
            {
                one.imagem.movy = -1;
                one.imagem.y -= one.imagem.vely + blinky - one.protecao;
            }
            else if(one.imagem.y > two.imagem.y)
            {
                one.imagem.movy = 1;
                one.imagem.y += one.imagem.vely + blinky - one.protecao;
            }
        }

        else
        {
            if(one.imagem.x < two.imagem.x) one.imagem.x += one.imagem.velx + blinky - one.protecao;
            else if(one.imagem.x > two.imagem.x) one.imagem.x -= one.imagem.velx + blinky - one.protecao;

            if(one.imagem.y < two.imagem.y) one.imagem.y += one.imagem.vely + blinky - one.protecao;
            else if(one.imagem.y > two.imagem.y) one.imagem.y -= one.imagem.vely + blinky - one.protecao;
        }


    }

}

popula inicia_populacao(bool ja_iniciou)
{
    popula vet_popula;
    vet_popula.ja_participou = false;
    vet_popula.hp = maior_hp;
    vet_popula.maxhp = maior_hp;
    vet_popula.nota = 0;
    vet_popula.tamanho_blink = tamanho_blinky;
    vet_popula.blink = turno_b;
    vet_popula.weap.qual_arma = rand()%2;
    vet_popula.weap.tempo_atk = rand()%(max_atk_spd - min_atk_spd + 1)+min_atk_spd;
    vet_popula.protecao = rand()%maximo_prot;
    vet_popula.weap.quanto_atk = 0;
    vet_popula.weap.cool = 0;
    if(vet_popula.weap.qual_arma == corpo_a_corpo)
    {
        vet_popula.weap.range = rand()%(max_range_corpo - min_range_corpo + 1)+min_range_corpo;
        vet_popula.protecao += 1;
        vet_popula.imagem.velx = vel_corpo;
        vet_popula.imagem.vely = vet_popula.imagem.velx;
        vet_popula.weap.dano = rand()%(max_dano - min_dano + 1) + min_dano;
    }
    else if(vet_popula.weap.qual_arma == ranged)
    {
        vet_popula.weap.range = rand()%max_range_range+1;
        vet_popula.imagem.vely = 3;
        vet_popula.imagem.velx = vet_popula.imagem.vely;
        vet_popula.weap.dano = rand()%(max_dano - min_dano + 1) + min_dano;
        vet_popula.weap.tempo_atk += 1;
    }
    if(!ja_iniciou)
    {
        vet_popula.imagem.imagem = al_load_bitmap("magentaquare.png");
        vet_popula.imagem.framealtura = 30;
        vet_popula.imagem.framelargura = 30;
        vet_popula.imagem.frameatual = 0;
        vet_popula.imagem.movx = 1;
        vet_popula.imagem.movy = 1;
    }
    vet_popula.imagem.x = rand()%largura;
    vet_popula.imagem.y = rand()%altura;
    for(int i = 0; i < numero_bala; i++)
    {
        vet_popula.weap.bala[i].rangex = 0;
        vet_popula.weap.bala[i].rangey = 0;
        vet_popula.weap.bala[i].viva = false;
        vet_popula.weap.bala[i].movx = -1;
        vet_popula.weap.bala[i].movy = -1;
        vet_popula.weap.bala[i].tempo = 0;
    }

    return vet_popula;
}

popula torneio(int i, popula vet[],int taxa_mutacao,int valor_mutacao,int tam_vet,int melhor_i)
{
    if(i == melhor_i) return vet[i];                //O melhor não é modificado;

    int t1,t2;
    popula novo_in,p1,p2;

    novo_in = vet[i];;

    //procura o melhor entre 2
    t1 = rand()%tam_vet;
    t2 = rand()%tam_vet;
    if(vet[t1].nota > vet[t2].nota) p1 = vet[t1];
    else p1 = vet[t2];

    //procura o melhor entre 2
    t1 = rand()%tam_vet;
    t2 = rand()%tam_vet;
    if(vet[t1].nota > vet[t2].nota) p2 = vet[t1];
    else p2 = vet[t2];


    //crossover
    novo_in.protecao = floor((p1.protecao + p2.protecao) /2.0);
    int flip = rand()%100;
    if(flip < 50)
    {
        novo_in.weap.qual_arma = p1.weap.qual_arma;
        novo_in.weap.tempo_atk = p1.weap.tempo_atk;
        novo_in.weap.dano = p1.weap.dano;
    }
    else
    {
        novo_in.weap.qual_arma = p2.weap.qual_arma;
        novo_in.weap.tempo_atk = p2.weap.tempo_atk;
        novo_in.weap.dano = p2.weap.dano;
    }


    //mutação
    flip = rand()%100;
    if(flip < taxa_mutacao)
    {
        flip = rand()%4 + 1;
        if(flip == 1)
        {
            flip = rand()%(quanto_muta + 2*quanto_muta);
            flip -= quanto_muta;

            novo_in.protecao += flip;
            if(novo_in.protecao < min_prot) novo_in.protecao = min_prot;
            else if(novo_in.protecao > maximo_prot) novo_in.protecao = maximo_prot;
        }
        else if(flip == 2)
        {
            flip = rand()%(quanto_muta + 2*quanto_muta);
            flip -= quanto_muta;

            novo_in.weap.range += flip*5;
            if(novo_in.weap.qual_arma == corpo_a_corpo)
            {
                if(novo_in.weap.range < min_range_corpo) novo_in.weap.range = min_range_corpo;
                else if(novo_in.weap.range > max_range_corpo) novo_in.weap.range = max_range_corpo;
            }
            else
            {
                if(novo_in.weap.range < min_range_range)novo_in.weap.range = min_range_range;
                else if(novo_in.weap.range > max_range_range) novo_in.weap.range = max_range_range;
            }
        }
        else if(flip == 3)
        {
            flip = rand()%(quanto_muta + 2*quanto_muta);
            flip -= quanto_muta;

            novo_in.weap.dano += flip;

            if(novo_in.weap.dano < min_dano) novo_in.weap.dano = min_dano;
            else if(novo_in.weap.dano > max_dano) novo_in.weap.dano = max_dano;

        }
        else if(flip == 4)
        {
            flip = rand()%(quanto_muta + 2*quanto_muta);
            flip -= quanto_muta;

            novo_in.weap.tempo_atk += flip;

            if(novo_in.weap.qual_arma == corpo_a_corpo)
            {
                if(novo_in.weap.tempo_atk < min_atk_spd) novo_in.weap.tempo_atk = min_atk_spd;
                else if(novo_in.weap.tempo_atk > max_atk_spd) novo_in.weap.tempo_atk = max_atk_spd;
            }
            else
            {
                if(novo_in.weap.tempo_atk < min_atk_spd_rang)novo_in.weap.tempo_atk = min_atk_spd_rang;
                else if(novo_in.weap.tempo_atk > max_atk_spd) novo_in.weap.tempo_atk = max_atk_spd;
            }
        }
    }


    //reinicia
    novo_in.hp = novo_in.maxhp;
    novo_in.weap.quanto_atk = 0;
    novo_in.nota = 0;
    novo_in.imagem.x = rand()%largura;
    novo_in.imagem.y = rand()%altura;
    novo_in.hp = novo_in.maxhp;
    novo_in.ja_participou = false;
    novo_in.blink = 0;
    novo_in.weap.cool = 0;
    for(int j = 0; j < numero_bala; j ++)
    {
        novo_in.weap.bala[j].tempo = 0;
        novo_in.weap.bala[j].rangex = 0;
        novo_in.weap.bala[j].rangey = 0;
        novo_in.weap.bala[j].viva = false;
    }

    return novo_in;
}

void fronteira(popula &one)
{
    if(one.imagem.x + one.imagem.framelargura < 0) one.imagem.x = largura;
    else if(one.imagem.x > largura) one.imagem.x = 0;

    if(one.imagem.y + one.imagem.framealtura < 0) one.imagem.y = altura;
    else if(one.imagem.y > altura) one.imagem.y = 0;

    return;
}

void atira_arco(popula& one)
{

    if(one.weap.qual_arma == corpo_a_corpo) return;
    if(one.weap.tempo_atk * FPS <= one.weap.cool)
    {
        one.weap.cool = 0;
        return;
    }
    int i,cat1,cat2;
    for(i = 0; i < numero_bala; i++)
    {
        if(!one.weap.bala[i].viva)
        {

            break;
        }
    }


    if(i < numero_bala)
    {
        cat1 = one.imagem.x - one.weap.alvox;
        cat2 = one.imagem.y - one.weap.alvoy;
        if(cat1 > one.weap.range || cat2 > one.weap.range) return;
        one.weap.bala[i].xvel = cat1/4.0;
        one.weap.bala[i].yvel = cat2/4.0;
        one.weap.bala[i].x = one.imagem.x + one.imagem.framelargura/2.0;
        one.weap.bala[i].y = one.imagem.y + one.imagem.framealtura/2.0;
        one.weap.bala[i].viva = true;
        if(one.weap.bala[i].xvel == 0 && one.weap.bala[i].yvel == 0) one.weap.bala[i].viva = false;
        one.weap.bala[i].rangex = 0;
        one.weap.bala[i].rangey = 0;
        one.weap.bala[i].tempo = 0;
        one.weap.quanto_atk++;
    }

    return;
}

void move_projetil(popula &one)
{
    if(one.weap.qual_arma == ranged)
    {
        int i;
        for(i = 0; i < numero_bala; i++)
        {
            if(one.weap.bala[i].viva)
            {
                one.weap.bala[i].x += one.weap.bala[i].xvel * one.weap.bala[i].movx;
                one.weap.bala[i].rangex += one.weap.bala[i].xvel;
                one.weap.bala[i].y += one.weap.bala[i].yvel * one.weap.bala[i].movy;
                one.weap.bala[i].rangey += one.weap.bala[i].yvel;
                if(++one.weap.bala[i].tempo >= tempo_para_apagar_bala) one.weap.bala[i].viva = false;
                if(one.weap.bala[i].rangey > one.weap.range || one.weap.bala[i].rangex > one.weap.range)
                {
                    one.weap.bala[i].viva = false;
                }

            }
        }

    }

    return;
}

bool avalia_populacao(popula &one,popula &two,bool empate, int conta)
{
    if(empate)
    {
       // cout<<"empate:"<<two.weap.qual_arma<<"|\n"<<flush;
        //cout<<"empate:"<<one.weap.qual_arma<<"|\n"<<flush;
        one.nota = nota_pelo_empate - one.protecao + (two.maxhp - two.hp) - one.weap.range - one.weap.quanto_atk;
        two.nota = nota_pelo_empate - two.protecao + (one.maxhp - one.hp) - two.weap.range - one.weap.quanto_atk;
        one.ja_participou = true;
        two.ja_participou = true;
        return true;
    }

    if(one.hp <= 0)
    {
        //cout<<"ganhou:"<<two.weap.qual_arma<<"|\n"<<flush;
        two.nota = two.hp - two.protecao + nota_pela_vitoria - two.weap.range - one.weap.quanto_atk;
        one.nota = (two.maxhp - two.hp) - one.protecao + nota_pela_derrota - one.weap.range - one.weap.quanto_atk;
        one.ja_participou = true;
        two.ja_participou = true;
        return true;
    }
    else if(two.hp <= 0)
    {
        //cout<<"ganhou:"<<one.weap.qual_arma<<"|\n"<<flush;
        one.nota = one.hp - one.protecao + nota_pela_vitoria - one.weap.range - one.weap.quanto_atk;
        two.nota = (one.maxhp - one.hp) - two.protecao + nota_pela_derrota - two.weap.range - one.weap.quanto_atk;
        one.ja_participou = true;
        two.ja_participou = true;
        return true;
    }
    else return false;
}
