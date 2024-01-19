#ifndef UTEIS_H_INCLUDED
#define UTEIS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <windows.h>
#include <malloc.h>
#include <ctype.h>
#include <locale.h>
#include <math.h>

#include "clientes.h"
#include "caixas.h"
#include "produtos.h"
#include "funcionarios.h"
#include "simulacao.h"
#include "text.h"
#include "fila.h"
#include "calculos.h"
#include "supermercado.h"

typedef struct estatisticas
{
    int clientes_atendidos;
    int caixas_abertas;
    int produtos_vendidos; //numero de produtos vendidos
    int produtos_oferecidos; //numero de produtos oferecidos
    int tempo_medio_espera; //tempo medio de espera
    float faturacao_total;//faturacao total
    float faturacao_media;//faturacao media por cliente
    float perdas; //total de perdas em produtos oferecidos
    int tempo_operacao; //tempo que o supermercado esteve operacional

} ESTATISTICAS;

typedef struct relogio
{
    time_t START;
    int VELOCIDADE; //velocidade de passagem de tempo
    time_t Hora_Inicio; //inicio do relogio
    time_t Hora_Fim; //final do relogio
} Relogio;


//----------------------------------------------------------------->
/*
int Aleatorio (int min, int max);
ESTATISTICAS* CriarEstatisticas();
struct tm *ImprimeDataHoraAtual();
float ConverterTempoMiliS (float valor);
float SomarTempo (time_t tempo, float valor);
void pausar();
void StartRelogio (Relogio *R, int Vel, const char *H_Inicio);
void StopRelogio (Relogio *R);
time_t LerHoraChar (const char *hora_fim_char, SUPERMERCADO *S);
time_t VerTempoRelogio (Relogio *R);
time_t TempoOperacao (Relogio *R);
void Wait (int s);
void WaitSegundos (int s);
char ConverterTimeToChar (time_t tempo_virtual);

void GerarEstatisticas (SUPERMERCADO *S);
void MostrarEstatisticas (SUPERMERCADO *S);
*/

#endif // UTEIS_H_INCLUDED
