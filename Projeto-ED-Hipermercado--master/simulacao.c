#include "simulacao.h"

CLIENTE *EntradaCliente (SUPERMERCADO *S, Relogio *R)
{
    if (!S || !S->clientes || S->clientes->NEL == 0 || !S->produtos || S->produtos->NEL == 0) return;

    //printf ("\n\n    %s  ",  __FUNCTION__);
    CLIENTE *C = (CLIENTE *)malloc (sizeof (CLIENTE));

    C = EscolherCliente (S); //guarda o cliente  que foi escolhido
    while (C == NULL)
    {
        C = EscolherCliente (S);
    }

    C->produtos_comprados = EscolherProdutosComprados (S->produtos); //coloca na estrutra cliente a lista de produtos que este compra
    while (C->produtos_comprados == NULL || C->produtos_comprados->NEL == 0)
    {
        C->produtos_comprados = EscolherProdutosComprados (S->produtos);
    }

    C = AtualizaHorasCliente (C, S);
    InserirInicioClienteOrdem (S->clientes_dentro, C);

    //MostrarListaProdutos(C->produtos_comprados);
    //printf (" // %s ",  __FUNCTION__);
    return C;
}

void ProcessarCorredor (SUPERMERCADO *S)
{
    if (!S || !S->clientes_dentro || S->clientes_dentro->NEL == 0) return;

    time_t tempo_atual = VerTempoRelogio (S->relogio_supermercado);
    printf ("\n    %s",  __FUNCTION__);

    CAIXA *CX;
    ELEMENTO_CLIENTE *NO_cliente = S->clientes_dentro->Inicio;
    ELEMENTO_CLIENTE *NO_cliente_menor = S->clientes_dentro->Inicio;
    ELEMENTO_CAIXA *NO_caixa = S->caixas->Inicio;

    while (NO_cliente != NULL)
    {
        if (NO_cliente->info->hora_caixa < tempo_atual)
        {
            CX = EscolherCaixa (S->caixas);
            AtualizaDadosCaixa (CX);
            AddFila (CX, NO_cliente->info);
            RemoverClienteCodigo (S->clientes_dentro, NO_cliente->info->numero_cliente);
            AtualizaDadosCaixa (CX);
            printf ("\n %d -> caixa %d", NO_cliente->info->numero_cliente, CX->numero_caixa);
            AtualizaTempoEspera (NO_cliente->info, NO_caixa->info);
            CX = NULL;
        }
        NO_cliente = NO_cliente->Prox;
    }
    TempoEsperaCliente (S);

    //Gravar (__FUNCTION__, S->relogio_supermercado);
    printf ("\n // %s ",  __FUNCTION__);
}

void ProcessarCaixas (SUPERMERCADO *S)
{
    if (!S) return;

    printf ("\n    %s ",  __FUNCTION__);

    ELEMENTO_CAIXA *NO_caixa = S->caixas->Inicio->Prox;

    if (GerirAberturaCaixa (S) == 1)
    {
        RedistribuirClientes (EscolherCaixa (S->caixas), S);
    }
    TempoEsperaCliente (S);

    //Gravar (__FUNCTION__, S->relogio_supermercado);
    printf ("\n // %s ",  __FUNCTION__);
}

void SairCliente (SUPERMERCADO *S)
{
    if (!S) return;

    time_t tempo_atual = VerTempoRelogio (S->relogio_supermercado);
    printf ("\n    %s",  __FUNCTION__);

    ELEMENTO_CAIXA *NO_caixa = S->caixas->Inicio;
    ELEMENTO_CLIENTE *NO_cliente = NO_caixa->info->fila->Inicio;

    while (NO_caixa != NULL)
    {
        float valor = 0;
        NO_cliente = NO_caixa->info->fila->Inicio;

        while (NO_cliente != NULL && NO_cliente->info->hora_saida <= tempo_atual)
        {
            printf ("\n %d -> saiu", NO_cliente->info->numero_cliente);
            OferecerProduto (NO_cliente->info, S);                                      //verifica se o cliente esperou muito e oferece um produto se for verdade
            ValorCompra (NO_cliente->info, NO_caixa->info);
            InserirInicioCliente (NO_caixa->info->atendidos, NO_cliente->info);
            RemoverFila (NO_caixa->info);

            NO_cliente = NO_cliente->Prox;
        }
        NO_caixa->info->funcionario->clientes_atendidos++;
        AtualizaDadosCaixa (NO_caixa->info);

        NO_caixa = NO_caixa->Prox;
    }
    if (S->caixas->NEL > 2) GerirFechoCaixa (S);
    TempoEsperaCliente (S);

    Gravar (__FUNCTION__, S->relogio_supermercado);
    printf ("\n // %s ",  __FUNCTION__);
}







