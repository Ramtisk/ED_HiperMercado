#include "calculos.h"

//TEMPOS CLIENTES
time_t TempoGastoCompraCliente (CLIENTE *C)
{
    if (!C) return -1;

    float tempo_float = 0;
    ELEMENTO_PRODUTO *NO = C->produtos_comprados->Inicio;

    while (NO != NULL)
    {
        tempo_float = tempo_float + NO->info->tcompra;
        NO = NO->Prox;
    }
    int tempo_int = ceil (tempo_float);

    printf ("", tempo_int);
    return ConverterIntToTime (tempo_int);
}


time_t TempoGastoCaixaCliente (CLIENTE *C)
{
    if (!C) return -1;

    float tempo_float = 0;
    ELEMENTO_PRODUTO *NO = C->produtos_comprados->Inicio;

    while (NO != NULL)
    {

        tempo_float = tempo_float + NO->info->tcaixa;
        NO = NO->Prox;
    }
    int tempo_int = ceil (tempo_float);
    printf ("", tempo_int);
    return ConverterIntToTime (tempo_int);
}

float TempoTotalGastoCliente (CLIENTE *C)
{
    if (!C) return -1;

    float tempo = 0;
    ELEMENTO_PRODUTO *NO = C->produtos_comprados->Inicio;

    while (NO != NULL)
    {
        tempo = tempo + NO->info->tcaixa + NO->info->tcompra;
        NO = NO->Prox;
    }

    return tempo;
}

float ValorCompra (CLIENTE *C, CAIXA *CX)
{
    if (!C) return -1;

    float soma = 0;
    ELEMENTO_PRODUTO *NO = C->produtos_comprados->Inicio;

    while (NO != NULL)
    {
        soma = soma + NO->info->preco;
        NO = NO->Prox;
    }
    printf (" - gastou %.2f", soma);
    CX->valor_acumulado += soma;
    CX->funcionario->faturado += soma;
    return soma;
}


//TOTAL
int TotalClientesCaixas (SUPERMERCADO *S)
{
    if (!S) return 0;

    int total = 0;
    ELEMENTO_CAIXA *NO_caixa = S->caixas->Inicio;

    while (NO_caixa != NULL)
    {
        total += NO_caixa->info->fila->NEL;
        NO_caixa = NO_caixa->Prox;
    }
    return total;
}
int TotalClientes (SUPERMERCADO *S)
{
    if (!S) return 0;

    int total = TotalClientesCaixas (S);
    total += S->clientes_dentro->NEL;
    return total;
}
int TotalClientesAtendidos (SUPERMERCADO *S)
{
    if (!S) return 0;

    ELEMENTO_CAIXA *NO_caixa = S->caixas->Inicio;

    int total = 0;
    while (NO_caixa != NULL)
    {
        total += NO_caixa->info->atendidos->NEL;
        NO_caixa = NO_caixa->Prox;
    }
    return total;
}

int TotalCaixasAbertas (SUPERMERCADO *S)
{
    if (!S) return 0;

    ELEMENTO_CAIXA *NO_caixa = S->caixas->Inicio;

    int total = 0;
    while (NO_caixa != NULL)
    {
        if (NO_caixa->info->estado == 1) total++;
        NO_caixa = NO_caixa->Prox;
    }
    return total;
}

int TotalProdutosVendidos (SUPERMERCADO *S)
{
    if (!S) return -1;

    ELEMENTO_CAIXA *NO_caixa = S->caixas->Inicio;
    ELEMENTO_CLIENTE *NO_cliente = NO_caixa->info->atendidos->Inicio;

    int total = 0;

    while (NO_caixa != NULL)
    {
        NO_cliente = NO_caixa->info->atendidos->Inicio;
        while (NO_cliente != NULL)
        {
            total = total + NO_cliente->info->produtos_comprados->NEL;
            NO_cliente = NO_cliente->Prox;
        }
        NO_caixa = NO_caixa->Prox;
    }
    return total;
}

int TotalProdutosOferecidos (SUPERMERCADO *S)
{
    if (!S) return -1;

    return S->produtos_oferecidos->NEL;
}

float TotalPerdas (SUPERMERCADO *S)
{
    if (!S) return -1;

    float total = 0;
    ELEMENTO_PRODUTO *NO_produto = S->produtos_oferecidos->Inicio;

    while (NO_produto != NULL)
    {
        total = total + NO_produto->info->preco;
        NO_produto = NO_produto->Prox;
    }
    S->dados_estatisticas->perdas = total;
    return total;
}

float TotalFaturacao (SUPERMERCADO *S)
{
    if (!S) return -1;

    float total = 0;
    float perda = TotalPerdas (S);

    ELEMENTO_CAIXA *NO_caixa = S->caixas->Inicio;
    if (NO_caixa->info->atendidos->NEL == 0 || !NO_caixa->info->atendidos) return 0;

    ELEMENTO_CLIENTE *NO_cliente = NO_caixa->info->atendidos->Inicio;
    ELEMENTO_PRODUTO *NO_produto = NO_cliente->info->produtos_comprados->Inicio;

    while (NO_caixa != NULL)
    {
        total += NO_caixa->info->valor_acumulado;
        NO_caixa = NO_caixa->Prox;
    }

    S->dados_estatisticas->faturacao_total = total;
    return total;
}

float TotalFaturacaoCaixa (CAIXA *CX)
{
    if (!CX)
    {
        return -1;
        printf ("\n\t\t ERRO ");
    }

    float total = 0;

    ELEMENTO_CLIENTE *NO_cliente = CX->atendidos->Inicio;
    ELEMENTO_PRODUTO *NO_produto = NO_cliente->info->produtos_comprados->Inicio;

    while (NO_cliente != NULL)
    {
        while (NO_produto != NULL)
        {
            total += NO_produto->info->preco;
            NO_produto = NO_produto->Prox;
        }
        NO_cliente = NO_cliente->Prox;
    }
    printf ("\n\t\t FATURACAO %.2f ", total);
    return total;
}

int TempoTotalOperacao (SUPERMERCADO *S)
{
    if (!S) return 0;

    time_t tempo = VerTempoRelogio (S->relogio_supermercado);
    printf ("\n TEMPO AGORA : %s", ConverterTimeToChar (tempo));
    //printf("\n TEMPO AGORA : %d",  S->relogio_supermercado->Hora_Inicio);
    tempo = tempo - S->relogio_supermercado->Hora_Inicio;
    //printf("\n TEMPO AGORA : %d", tempo);
    S->dados_estatisticas->tempo_operacao = tempo;
    return tempo;
}

//MEDIAS
int MediaTempoEsperaAtual (SUPERMERCADO *S)
{
    if (!S) return 0;
    int soma = 0;
    int media = 0;
    int caixas = 0;

    ELEMENTO_CAIXA *NO_caixa = S->caixas->Inicio;

    while (NO_caixa != NULL)
    {
        soma += NO_caixa->info->tempo_atendimento;
        if (NO_caixa->info->estado == 1) caixas++;
        NO_caixa = NO_caixa->Prox;
    }
    return media = soma / caixas;
}

int MediaClientesCaixa (SUPERMERCADO *S)
{
    if (!S) return 0;
    int soma = 0;
    int media = 0;
    int caixas = 0;

    ELEMENTO_CAIXA *NO_caixa = S->caixas->Inicio;

    while (NO_caixa != NULL)
    {
        soma += NO_caixa->info->fila->NEL;
        if (NO_caixa->info->estado == 1) caixas++;
        NO_caixa = NO_caixa->Prox;
    }
    if (caixas < 1) caixas = 1;
    return media = soma / caixas;
}

float MediaTempoEspera (SUPERMERCADO *S)
{
    if (!S) return 0;

    ELEMENTO_CAIXA *NO_caixa = S->caixas->Inicio;
    ELEMENTO_CLIENTE *NO_cliente = NO_caixa->info->atendidos->Inicio;

    int soma = 0;
    int clientes = 0;
    float media = 0;
    while (NO_caixa != NULL)
    {
        NO_cliente = NO_caixa->info->atendidos->Inicio;
        while (NO_cliente != NULL)
        {
            soma += NO_cliente->info->tempo_espera_total;
            clientes ++;
            NO_cliente = NO_cliente->Prox;
        }
        NO_caixa = NO_caixa->Prox;
    }
    if (clientes < 1) clientes = 1;
    media = soma / clientes;
    S->dados_estatisticas->tempo_medio_espera = media;
    return media;
}

float MediaFaturacao (SUPERMERCADO *S)
{
    float soma = TotalFaturacao (S);
    int clientes = 0;
    float media = 0;

    ELEMENTO_CAIXA *NO_caixa = S->caixas->Inicio;

    while (NO_caixa != NULL)
    {
        clientes += NO_caixa->info->atendidos->NEL;
        NO_caixa = NO_caixa->Prox;
    }
    if (clientes < 1) clientes = 1;
    media = soma / clientes;
    S->dados_estatisticas->faturacao_media = media;
    return media;

}

