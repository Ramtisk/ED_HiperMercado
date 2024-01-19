#ifndef CAIXAS_H_INCLUDED
#define CAIXAS_H_INCLUDED

#include "clientes.h"
#include "caixas.h"
#include "produtos.h"
#include "funcionarios.h"
#include "simulacao.h"
#include "text.h"
#include "calculos.h"
#include "supermercado.h"
#include "uteis.h"


// Estrutura para representar uma caixa
typedef struct caixa
{
    int numero_caixa;
    char *descricao;
    time_t tempo_atendimento; //tempo maximo de espera na caixa
    struct lista_cliente *fila; // fila de clientes na caixa
    struct lista_cliente *atendidos; //lista de clientes que foram atendidos na caixa
    FUNCIONARIO *funcionario; //funcionario que esta a trabalhar na caixa
    float valor_acumulado; // total faturado na pela caiza
    bool estado; //encerrada = 0 , aberta = 1
} CAIXA;

//Elemento do Caixa (NO)
typedef struct elemento_caixa
{
    CAIXA *info;
    struct elemento_caixa *Prox;
} ELEMENTO_CAIXA, *ptElemento_Caixa;

//Lista de caixas
typedef struct lista_caixa
{
    ELEMENTO_CAIXA *Inicio;
    ELEMENTO_CAIXA *Fim;
    int NEL;
} LISTA_CAIXA;

//----------------------------------------------------------------->
/*
// FUNCOES BASICAS
LISTA_CAIXA *CriarListaCaixa();
CAIXA *CriarCaixa (int _numero_caixa, char *_desc);
void MostrarCaixa (CAIXA *CX);
void MostrarCaixaSimples (CAIXA *CX);
void MostrarListaCaixas (LISTA_CAIXA *LCX);
void MostrarListaCaixasAbertas (LISTA_CAIXA *LCX);
void MostrarListaCaixasFechadas (LISTA_CAIXA *LCX);
void DestruirCaixa (CAIXA *C);
void DestruirListaCaixa (LISTA_CAIXA *LC);

// INSERIR
void InserirInicioCaixa (LISTA_CAIXA *LC, CAIXA *caixa);
void InserirFimCaixa (LISTA_CAIXA* LC, CAIXA* caixa);

/*
//GERIR ABERTURAS E FECHOS DE CAIXA
CAIXA *AtivarCaixa (CAIXA *CX, LISTA_FUNCIONARIO *LF);
CAIXA *DesativarCaixa (CAIXA *CX);
void FecharCaixaNum (int numero_caixa, struct supermercado *S);
CAIXA *AbrirNovaCaixa (struct supermercado *S);
CAIXA *AbrirCaixa (struct supermercado *S);
int GerirAberturaCaixa (struct supermercado *S);
void GerirFechoCaixa (struct supermercado *S);

// PESQUISA
CAIXA *PesquisarCaixaCod (struct supermercado *S, int numero_caixa);
CAIXA* CaixaMaisAtendeu (struct supermercado* S);
CAIXA* CaixaMenosAtendeu (struct supermercado* S);

//SIMULACOES
CAIXA *EscolherCaixa (LISTA_CAIXA *LCX);
void AtualizaDadosCaixa (CAIXA *CX);

//GESTAO DE CLIENTES NAS CAIXAS
void RedistribuirClientesCaixa (int numero_caixa, struct supermercado* S);
CAIXA* RedistribuirClientes (CAIXA* CX, struct supermercado* S);

//CALCULOS DAS CAIXAS
size_t MemoriaOcupadaCaixas (LISTA_CAIXA *LCX);
*/
#endif // CAIXAS_H_INCLUDED
