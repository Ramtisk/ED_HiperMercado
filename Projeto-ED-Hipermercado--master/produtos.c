#include "clientes.h"
#include "caixas.h"
#include "produtos.h"
#include "funcionarios.h"
#include "simulacao.h"
#include "uteis.h"

// FUNCOES BASICAS
LISTA_PRODUTO* CriarListaProduto()
{
    LISTA_PRODUTO* L = (LISTA_PRODUTO*)malloc (sizeof (LISTA_PRODUTO));
    L->Inicio = NULL;
    L->Fim = NULL;
    L->NEL = 0;

    return L;
}

PRODUTO *CriarProduto (int _codigo, char *_nome, float _preco, float _tcompra, float _tcaixa)
{
    PRODUTO *P = (PRODUTO *)malloc (sizeof (PRODUTO));
    P->codigo_produto = _codigo;
    P->nome_produto = (char *)malloc ((strlen (_nome) + 1) * sizeof (char));
    strcpy (P->nome_produto, _nome);
    P->preco = _preco;
    P->tcompra = _tcompra;
    P->tcaixa = _tcaixa;
    return P;
}

void MostrarProduto (PRODUTO *P)
{
    printf ("\nCodigo: %d", P->codigo_produto);
    printf ("\nNome: %s", P->nome_produto);
    printf ("\nPreco: %.2f EUR", P->preco);
    printf ("\nTcompra: %.2f", P->tcompra);
    printf ("\nTcaixa: %.2f\n____________\n", P->tcaixa);
}

void MostrarListaProdutos (LISTA_PRODUTO *LP)
{
    ELEMENTO_PRODUTO *NO = LP->Inicio;
    printf ("NEL : %d", LP->NEL);
    while (NO != NULL)
    {
        MostrarProduto (NO->info);
        NO = NO->Prox;
    }
}

void DestruirProduto (PRODUTO *P)
{
    free (P->nome_produto);
    free (P);
}

void DestruirListaProduto (LISTA_PRODUTO *LP)
{
    if (!LP) return;

    ELEMENTO_PRODUTO* atual = LP->Inicio;
    ELEMENTO_PRODUTO* proximo = NULL;

    while (atual != NULL)
    {
        proximo = atual->Prox;
        DestruirProduto (atual->info);
        free (atual);
        atual = proximo;
    }
    free (LP);
}

// INSERIR E REMOVER
// Insere um produto na lista de produtos
void InserirProduto (LISTA_PRODUTO *LP, PRODUTO *produto)
{
    ELEMENTO_PRODUTO *NO = (ELEMENTO_PRODUTO*)malloc (sizeof (ELEMENTO_PRODUTO));
    NO->info = produto;

    if (LP->Inicio == NULL)
    {
        NO->Prox = NULL;
        LP->Fim = NO;
    }
    else
    {
        NO->Prox = LP->Inicio;
    }

    LP->Inicio = NO;
    LP->NEL++;
}

// Remove um produto especifico da lista de produtos dado um codigo de produto
void RemoverProduto (LISTA_PRODUTO *LP, PRODUTO *cod_prod)
{
    ELEMENTO_PRODUTO *atual = LP->Inicio;
    ELEMENTO_PRODUTO *anterior = NULL;

    while (atual != NULL)
    {
        if (atual->info == cod_prod)
        {
            if (anterior == NULL)
            {
                LP->Inicio = atual->Prox;
            }
            else
            {
                anterior->Prox = atual->Prox;
            }

            if (atual->Prox == NULL)
            {
                LP->Fim = anterior;
            }

            LP->NEL--;
            free (atual);
            return;
        }

        anterior = atual;
        atual = atual->Prox;
    }
}

// PESQUISAR
//Procura o produto de menor custo/valor de uma dada lista
PRODUTO *ProcurarProdutoMenorValor (LISTA_PRODUTO *LP)
{
    if (!LP) return NULL;

    ELEMENTO_PRODUTO *NO_produto = LP->Inicio;
    PRODUTO *MenorValor = NO_produto->info;

    while (NO_produto != NULL)
    {
        if (NO_produto->info->preco < MenorValor->preco)
        {
            MenorValor = NO_produto->info;
        }
        NO_produto = NO_produto->Prox;
    }
    return MenorValor;
}

//______________________________________________________________________________
//SIMULACOES
LISTA_PRODUTO *EscolherProdutosComprados (LISTA_PRODUTO *LP)
{
    if (!LP) return NULL;

    int qnt_produtos = Aleatorio (0, 10); //gera a quantidade de produtos a comprar

    LISTA_PRODUTO *LPC = CriarListaProduto();

    for (int i = 0; i < qnt_produtos; i++) //havendo a quantidade de produtos agora gera uma posicao para selecionar CADA produto comprado
    {
        int posicao = Aleatorio (1, LP->NEL - 1);
        ELEMENTO_PRODUTO *NO_PRODUTO = LP->Inicio;

        for (int j = 0; j < posicao; j++) //percorre a lista até ao numero gerado
        {
            NO_PRODUTO = NO_PRODUTO->Prox;
        }
        InserirProduto (LPC, NO_PRODUTO->info); //insere numa lista de produtos apenas os produtos comprados
    }
    return LPC;
}

//Verifica se e necessario oferecer o produto de menor valor ao cliente, mediante o tempo de espera
void OferecerProduto (CLIENTE *C, SUPERMERCADO *S)
{
    if (!C) return;
    PRODUTO *MenorValor;

    if (C->tempo_espera_total > S->tempo_max_espera)
    {
        MenorValor = ProcurarProdutoMenorValor (C->produtos_comprados);
        InserirProduto (S->produtos_oferecidos, MenorValor);
        //RemoverProduto(C->produtos_comprados, MenorValor->codigo_produto);
    }
}

//CALCULOS
size_t MemoriaOcupadaProdutos (LISTA_PRODUTO *LP)
{
    if (!LP) return 0;

    ELEMENTO_PRODUTO *NO = LP->Inicio;
    size_t memoria = 0;

    while (NO != NULL)
    {
        //Memoria ocupada pelos nomes dos produtos
        memoria += strlen (NO->info->nome_produto) + 1;
        //Memoria ocupada pelos produtos
        memoria += sizeof (PRODUTO);
        //Memoria ocupada pelos NOS da lista de produtos
        memoria += sizeof (ELEMENTO_PRODUTO);

        NO = NO->Prox;
    }
    return memoria;
}

