#ifndef SUPERMERCADO_H_INCLUDED
#define SUPERMERCADO_H_INCLUDED

#include "clientes.h"
#include "caixas.h"
#include "produtos.h"
#include "funcionarios.h"
#include "simulacao.h"
#include "text.h"
#include "calculos.h"
#include "supermercado.h"
#include "uteis.h"

// Estrutura para representar o <<Supermercado>>
typedef struct supermercado
{
    char nome_supermercado[50];
    struct lista_produto *produtos; //todos os produtos que sao vendidos
    struct lista_produto *produtos_oferecidos; //produtos que foram oferecidos
    struct lista_funcionario *funcionario; //todos os funcionarios
    struct lista_cliente *clientes; //todos os clientes
    struct lista_cliente *clientes_dentro; //clientes as compras nos corredores
    struct lista_caixa *caixas; //todas as caixas
    int caixas_permanentes;
    struct estatisticas *dados_estatisticas; //estatisticas do supermercado
    int tempo_max_espera; //tempo maximo de espera para ser oferecido um produto
    int n_max_clientes; //numero medio maximo de clientes por caixa
    time_t hora_inicio;
    time_t hora_fim;
    struct relogio *relogio_supermercado; //relogio do sistema do supermercado
} SUPERMERCADO;

//----------------------------------------------------------------->
/*
// FUNCOES BASICAS
SUPERMERCADO  *CriarSupermercado (char *_nome, Relogio R, LISTA_CLIENTE *LC, LISTA_FUNCIONARIO *LF, LISTA_CAIXA *LCX, LISTA_PRODUTO *LP, time_t *_hora_fim, int tempo_max, int n_clientes_max);
void MostrarSupermercado (SUPERMERCADO *S);

// SIMULACAO
SUPERMERCADO *AbrirSupermercado (SUPERMERCADO *S, Relogio *R);

// CALCULOS
size_t MemoriaOcupadaSupermercado (SUPERMERCADO *S);

*/
#endif // SUPERMERCADO_H_INCLUDED
