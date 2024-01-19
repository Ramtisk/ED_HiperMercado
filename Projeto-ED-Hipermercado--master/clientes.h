#ifndef CLIENTES_H_INCLUDED
#define CLIENTES_H_INCLUDED

#include "uteis.h"

// Estrutura para representar um produto comprado por um <<cliente>>
typedef struct cliente
{
    int numero_cliente;
    char *nome_cliente;
    struct lista_produto *produtos_comprados; //lista de produtos comprados
    time_t hora_saida; //hora para sair
    time_t hora_caixa; //hora para ser colocado em caixa
    time_t hora_entrada; //hora que fez a entrada no supermercado
    int tempo_espera; //tempo que tera de esperar para ser atendido
    int tempo_espera_total; // tempo maximo que esperou
} CLIENTE;

//Elemento do cliente (NO)
typedef struct elemento_cliente
{
    CLIENTE *info;
    struct elemento_cliente *Prox;
} ELEMENTO_CLIENTE;

//Lista de clientes
typedef struct lista_cliente
{
    ELEMENTO_CLIENTE* Inicio;
    ELEMENTO_CLIENTE* Fim;
    int NEL;
} LISTA_CLIENTE;

/*
// FUNCOES BASICAS
LISTA_CLIENTE *CriarListaCliente();
CLIENTE *CriarCliente (int _ncliente, char *_nome);
void DestruirCliente (CLIENTE *C);
void DestruirListaCliente (LISTA_CLIENTE *LC);
void MostrarCliente (CLIENTE *C);
void MostrarListaCliente (LISTA_CLIENTE *LC);
void MostrarClienteSimples (CLIENTE *C);

// INSERIR E REMOVER
void InserirInicioCliente (LISTA_CLIENTE *LC, CLIENTE *cliente);
void InserirInicioClienteOrdem (LISTA_CLIENTE* LC, CLIENTE* C);
void InserirFimCliente (LISTA_CLIENTE *LC, CLIENTE *cliente);;
void RemoverClienteCodigo (LISTA_CLIENTE* LC, int numero_cliente);

// PESQUISA
void PesquisarCliente (CAIXA *CX, int _numero_cliente);
int EncontrarClienteDentro (struct supermercado *S, CLIENTE *C);

//SIMULACOES
CLIENTE *EscolherCliente (struct supermercado *S);
CLIENTE *AtualizaHorasCliente (CLIENTE *C, struct supermercado *S);
void TempoEsperaCliente (struct supermercado *S);
void AtualizaTempoEspera (CLIENTE *C, struct caixa *CX);
void TransferirCliente (struct caixa *CX_origem, struct caixa *CX_destino, int _numero_cliente, struct supermercado *S);

// CALCULOS
size_t MemoriaOcupadaClientes (LISTA_CLIENTE *LC);
*/
#endif // CLIENTES_H_INCLUDED
