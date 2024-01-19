#include "supermercado.h"

SUPERMERCADO  *CriarSupermercado (char *_nome, Relogio R, LISTA_CLIENTE *LC, LISTA_FUNCIONARIO *LF, LISTA_CAIXA *LCX, LISTA_PRODUTO *LP, time_t *_hora_fim, int tempo_max, int n_clientes_max)
{
    SUPERMERCADO *S = (SUPERMERCADO*)malloc (sizeof (SUPERMERCADO));
    strcpy (S->nome_supermercado, _nome);
    S->clientes = LC;
    S->funcionario = LF;
    S->caixas = LCX;
    S->produtos_oferecidos = CriarListaProduto();
    S->produtos = LP;
    S->clientes_dentro = CriarListaCliente();
    time_t _hora_inicio = VerTempoRelogio (R);
    S->hora_inicio = _hora_inicio;
    S->hora_fim = _hora_fim;
    S->relogio_supermercado = &R;
    S->tempo_max_espera = tempo_max;
    S->caixas_permanentes = 0;
    S->dados_estatisticas = CriarEstatisticas();
    S->n_max_clientes = n_clientes_max;

    return S;
}

void MostrarSupermercado (SUPERMERCADO *S)
{
    if (!S) return;

    printf ("\nSUPERMERCADO %s : %s\n", S->nome_supermercado, ConverterTimeToChar (S->hora_inicio));
    printf ("CLIENTES: %d\n", S->clientes->NEL);
    printf ("FUNCIONARIOS: %d\n", S->funcionario->NEL);
    printf ("PRODUTOS: %d\n", S->produtos->NEL);
    printf ("CAIXAS: %d\n", S->caixas->NEL);
}

//Executa todas as operacoes para abrir o supermercado, criterios do utlizador para as caixas tambem
SUPERMERCADO *AbrirSupermercado (SUPERMERCADO *S, Relogio *R)
{
    if (!S || !R) return;

    char hora_fechar[51];
    int qnt_caixas = 100;

    while (qnt_caixas > 25 || qnt_caixas < 0)
    {
        printf ("\nDeseja abrir o Supermercado com quantas caixas?\n >");
        scanf ("%d", &qnt_caixas);
        if (qnt_caixas > 25 || qnt_caixas < 0) printf ("\n\t* Quantidade invalida! [0 - 90] *");
    }
    for (int i = 0; i < qnt_caixas; i++)
    {
        char descricao[21];
        sprintf (descricao, "CAIXA N_%d", i + 1);

        CAIXA *C = CriarCaixa (i + 1, descricao);
        C = AtivarCaixa (C, S->funcionario);
        InserirFimCaixa (S->caixas, C);
    }
    qnt_caixas = 100;
    while (qnt_caixas > 25 || qnt_caixas < 0)
    {
        printf ("\nQuantas devem permanecer sempre abertas?\n >");
        scanf ("%d", &qnt_caixas);
        if (qnt_caixas > 25 || qnt_caixas < 0) printf ("\n\t* Quantidade invalida! [0 - 90] *");
    }
    S->caixas_permanentes = qnt_caixas;
    Gravar (__FUNCTION__, S->relogio_supermercado);

    return S;
}

void DestruirSupermercado (SUPERMERCADO *S)
{
    if (S) return;

    // Liberar memoria da lista de produtos
    DestruirListaProduto (S->produtos);
    DestruirListaProduto (S->produtos_oferecidos);

    // Liberar memoria da lista de funcionarios
    DestruirListaFuncionario (S->funcionario);

    // Liberar memoria da lista de clientes
    DestruirListaCliente (S->clientes);

    //DestruirListaCliente(S->clientes_dentro);

    // Liberar memoria da lista de caixas
    DestruirListaCaixa (S->caixas);

    // Liberar memoria da estrutura de estat�sticas
    free (S->dados_estatisticas);

    // Liberar mem�ria do objeto SUPERMERCADO
    free (S);
}

size_t MemoriaOcupadaSupermercado (SUPERMERCADO *S)
{
    if (!S || sizeof (S) == 0)
    {
        return 0;
    }

    size_t memoria_total = sizeof (SUPERMERCADO);

    // Mem�ria ocupada pelos nomes do supermercado e outras informa��es est�ticas
    memoria_total += strlen (S->nome_supermercado) + 1;

    // Mem�ria ocupada pelos produtos
    memoria_total += MemoriaOcupadaProdutos (S->produtos);
    memoria_total += MemoriaOcupadaProdutos (S->produtos_oferecidos);

    // Mem�ria ocupada pelos funcion�rios
    memoria_total += MemoriaOcupadaFuncionarios (S->funcionario);

    // Mem�ria ocupada pelos clientes
    memoria_total += MemoriaOcupadaClientes (S->clientes);
    memoria_total += MemoriaOcupadaClientes (S->clientes_dentro);

    // Mem�ria ocupada pelas caixas
    memoria_total += MemoriaOcupadaCaixas (S->caixas);

    // Mem�ria ocupada pelas estat�sticas
    memoria_total += sizeof (ESTATISTICAS);

    // Mem�ria ocupada pelo rel�gio
    memoria_total += sizeof (Relogio);

    return memoria_total;
}





