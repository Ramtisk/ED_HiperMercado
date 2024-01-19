#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "uteis.h"

int Aleatorio (int min, int max)
{
    return min + rand() * rand() % (max - min + 1);
}

ESTATISTICAS* CriarEstatisticas()
{
    ESTATISTICAS* estatisticas = (ESTATISTICAS*)malloc (sizeof (ESTATISTICAS));

    estatisticas->clientes_atendidos = 0;
    estatisticas->caixas_abertas = 0;
    estatisticas->produtos_vendidos = 0;
    estatisticas->produtos_oferecidos = 0;
    estatisticas->tempo_medio_espera = 0;
    estatisticas->faturacao_total = 0.0;
    estatisticas->faturacao_media = 0.0;
    estatisticas->perdas = 0.0;
    estatisticas->tempo_operacao = 0;

    return estatisticas;
}

void DestruirEstatisticas (ESTATISTICAS *E)
{
    free (E);
}

// RELOGIO
void StartRelogio (Relogio *R, int Vel, const char *H_Inicio)
{
    R->START = time (0);
    R->VELOCIDADE = Vel;
    struct tm *tmp = localtime (&R->START);
    sscanf (H_Inicio, "%d:%d:%d", &tmp->tm_hour, &tmp->tm_min, &tmp->tm_sec);
    R->Hora_Inicio = mktime (tmp);
}
void StopRelogio (Relogio *R)
{
    time_t NOW = time (0);
    struct tm *tmp = localtime (&NOW);
    R->Hora_Fim = mktime (tmp);
}

void DestruirRelogio (Relogio *R)
{
    free (R);
}
// TEMPO
time_t VerTempoRelogio (Relogio *R)
{
    time_t Dif = difftime (time (0), R->START);
    time_t tempo_virtual = R->Hora_Inicio + Dif * R->VELOCIDADE;
    return tempo_virtual;
}

void Wait (int s)
{
    clock_t T0 = clock();
    clock_t T1 = T0 + s;
    while (clock() < T1);
}

void WaitSegundos (int s)
{
    Wait (s * CLOCKS_PER_SEC);
}

char ConverterTimeToChar (time_t tempo_virtual)
{
    if (!tempo_virtual) return '\0';
    char *tempo_string = asctime (localtime (&tempo_virtual));
    RemoverCaracterNull (tempo_string);
    return tempo_string;
}

time_t ConverterIntToTime (int tempo_inteiro)
{
    if (!tempo_inteiro) return 0;
    return mktime (localtime (&tempo_inteiro));
}

// TEXTO
void RemoverCaracterNull (char *str)
{
    size_t length = strlen (str);
    if (length > 0 && str[length - 1] == '\n')
    {
        str[length - 1] = '\0';
    }
}

void GerarEstatisticas (SUPERMERCADO *S)
{
    if (!S) return;

    //printf ("\n    %s ",  __FUNCTION__);
    ESTATISTICAS *estatisticas = S->dados_estatisticas;

    estatisticas->clientes_atendidos = TotalClientesAtendidos (S);
    estatisticas->caixas_abertas = S->caixas->NEL;
    estatisticas->produtos_vendidos = TotalProdutosVendidos (S);
    MediaTempoEspera (S);
    estatisticas->produtos_oferecidos = TotalProdutosOferecidos (S);
    TotalFaturacao (S);
    MediaFaturacao (S);
    TotalPerdas (S);
    TempoTotalOperacao (S);

    Gravar (__FUNCTION__, S->relogio_supermercado);
}

void MostrarEstatisticas (SUPERMERCADO *S)
{
    if (!S) return;

    ESTATISTICAS *estatisticas = S->dados_estatisticas;

    printf ("\n #---------------------------------------------------------#\n");
    printf (" | Estatisticas do Supermercado:                           | \n");
    printf (" #---------------------------------------------------------#\n");
    printf (" | - Clientes atendidos: %d                                | \n", estatisticas->clientes_atendidos);
    printf (" | - Caixas abertas: %d                                     | \n", estatisticas->caixas_abertas);
    printf (" | - Produtos vendidos: %dun                                | \n", estatisticas->produtos_vendidos);
    printf (" | - Produtos oferecidos: %dun                              | \n", estatisticas->produtos_oferecidos);
    printf (" | - Tempo medio de espera: %ds                             | \n", estatisticas->tempo_medio_espera);
    printf (" | - Faturacao total: %.2fEUR                              | \n", estatisticas->faturacao_total);
    printf (" | - Faturacao media por cliente: %.2fEUR                  | \n", estatisticas->faturacao_media);
    printf (" | - Perdas: %.2fEUR                                       | \n", estatisticas->perdas);
    printf (" | - Tempo de operacao: %d                                  | \n", estatisticas->tempo_operacao);
    printf (" #---------------------------------------------------------#\n");


    MostrarCaixaSimples (CaixaMaisAtendeu (S));
    MostrarCaixaSimples (CaixaMenosAtendeu (S));
    MostrarFuncionario (FuncionarioMaisAtendeu (S));
    MostrarFuncionario (FuncionarioMenosAtendeu (S));
}
