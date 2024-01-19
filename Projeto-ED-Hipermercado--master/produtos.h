#ifndef PRODUTOS_H_INCLUDED
#define PRODUTOS_H_INCLUDED

#include "clientes.h"
#include "caixas.h"
#include "produtos.h"
#include "funcionarios.h"
#include "simulacao.h"
#include "text.h"
#include "calculos.h"
#include "supermercado.h"
#include "uteis.h"


// Estrutura para representar um <<Produto>>
typedef struct produto
{
    int codigo_produto;
    char *nome_produto;
    float preco;
    float tcompra; //tempo de compra
    float tcaixa; //tenpo de caixa

} PRODUTO;

//Elemento do produto (NO)
typedef struct elemento_produto
{
    PRODUTO *info;
    struct elemento_produto *Prox;
} ELEMENTO_PRODUTO;


//Lista de produtos
typedef struct lista_produto
{
    PRODUTO* Inicio;
    PRODUTO* Fim;
    int NEL;
} LISTA_PRODUTO;


LISTA_PRODUTO* CriarListaProduto();
PRODUTO *CriarProduto (int _codigo, char *_nome, float _preco, float _tcompra, float _tcaixa);
void InserirProduto (LISTA_PRODUTO *LP, PRODUTO *produto);
void RemoverProduto (LISTA_PRODUTO *LP, PRODUTO *cod_prod);
void MostrarProduto (PRODUTO *P);
void DestruirProduto (PRODUTO *P);
void DestruirListaProduto (LISTA_PRODUTO *LP);

//----------------------------------------------------------------->
/*
// FUNCOES BASICAS
LISTA_PRODUTO* CriarListaProduto();
PRODUTO *CriarProduto(int _codigo, char *_nome, float _preco, float _tcompra, float _tcaixa);
void MostrarProduto(PRODUTO *P);
void MostrarListaProdutos(LISTA_PRODUTO *LP);
void DestruirProduto(PRODUTO *P);
void DestruirListaProduto(LISTA_PRODUTO *LP);

// INSERIR E REMOVER
void InserirProduto(LISTA_PRODUTO *LP, PRODUTO *produto);
void RemoverProduto(LISTA_PRODUTO *LP, PRODUTO *cod_prod);

//SIMULACOES
LISTA_PRODUTO *EscolherProdutosComprados(LISTA_PRODUTO *LP);
PRODUTO *ProcurarProdutoMenorValor(LISTA_PRODUTO *LP);
void OferecerProduto(CLIENTE *C, SUPERMERCADO *S);

// CALCULOS
size_t MemoriaOcupadaProdutos (LISTA_PRODUTO *LP);

*/
#endif // PRODUTOS_H_INCLUDED
