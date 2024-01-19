#include "fila.h"

// Função para enfileirar um cliente na fila
void AddFila (CAIXA *CX, CLIENTE *C)
{
    if (!CX || !C) return;

    ELEMENTO_CLIENTE* novoElemento = (ELEMENTO_CLIENTE*)malloc (sizeof (ELEMENTO_CLIENTE));
    novoElemento->info = C;
    novoElemento->Prox = NULL;

    if (CX->fila->Inicio == NULL)//esta vazia a fila
    {
        CX->fila->Inicio = novoElemento;
        CX->fila->Fim = novoElemento;
    }
    else//se nao adiciona no fim
    {
        CX->fila->Fim->Prox = novoElemento;
        CX->fila->Fim = novoElemento;
    }

    CX->fila->NEL++;
}

void AddFilaOrdem (CAIXA* CX, CLIENTE* C)
{
    if (!CX || !C) return;

    ELEMENTO_CLIENTE* novoElemento = (ELEMENTO_CLIENTE*)malloc (sizeof (ELEMENTO_CLIENTE));
    novoElemento->info = C;
    novoElemento->Prox = NULL;

    if (CX->fila->Inicio == NULL)   // A fila está vazia
    {
        CX->fila->Inicio = novoElemento;
        CX->fila->Fim = novoElemento;
    }
    else
    {
        ELEMENTO_CLIENTE* anterior = NULL;
        ELEMENTO_CLIENTE* atual = CX->fila->Inicio;

        while (atual != NULL && atual->info->hora_caixa <= C->hora_caixa)
        {
            anterior = atual;
            atual = atual->Prox;
        }

        if (anterior == NULL)   // O novo elemento deve ser o primeiro da fila
        {
            novoElemento->Prox = CX->fila->Inicio;
            CX->fila->Inicio = novoElemento;
        }
        else     // O novo elemento deve ser inserido após o elemento anterior
        {
            novoElemento->Prox = atual;
            anterior->Prox = novoElemento;
        }

        if (atual == NULL)   // O novo elemento deve ser o último da fila
        {
            CX->fila->Fim = novoElemento;
        }
    }

    CX->fila->NEL++;
}
/*void AddFila(CAIXA *CX, CLIENTE* C)
{
    if(!CX || !C) return;


    ELEMENTO_CLIENTE* novoElemento = (ELEMENTO_CLIENTE*)malloc (sizeof (ELEMENTO_CLIENTE));
    novoElemento->info = C;
    novoElemento->Prox = NULL;

    ELEMENTO_CLIENTE *NO_cliente = CX->fila->Inicio;
    if (CX->fila->Inicio == NULL)//esta vazia a fila
    {
        CX->fila->Inicio = novoElemento;
        CX->fila->Fim = novoElemento;
    }
    else//se nao adiciona no fim
    {
        while(C->hora_caixa < NO_cliente->info->hora_caixa)
        CX->fila->Fim->Prox = novoElemento;
        CX->fila->Fim = novoElemento;
    }

    CX->fila->NEL++;
}*/

// Função para desenfileirar um cliente da fila
CLIENTE *RemoverFila (CAIXA *CX)
{
    if (!CX) return;
    if (CX->fila->Inicio == NULL)
    {
        printf ("Fila vazia. Impossível retirar cliente.\n");
        return NULL;
    }

    ELEMENTO_CLIENTE *NO_cliente = CX->fila->Inicio;
    CLIENTE *C = NO_cliente->info;

    CX->fila->Inicio = NO_cliente->Prox;
    InserirInicioCliente (CX->atendidos, NO_cliente->info);
    free (NO_cliente);

    CX->fila->NEL--;
    return C;
}

// Função para mostrar a fila por ordem
void MostrarFila (CAIXA *CX)
{
    if (!CX) return;

    ELEMENTO_CAIXA *atual = CX->fila->Inicio;
    int ordem = 1;

    if (CX->fila->Inicio == NULL)
    {
        printf ("Fila vazia.\n");
        return;
    }

    printf ("Ordem de atendimento:\n");
    while (atual != NULL)
    {
        printf ("Cliente %d\n", ordem);
        MostrarCliente (atual->info);
        atual = atual->Prox;
        ordem++;
    }
}
