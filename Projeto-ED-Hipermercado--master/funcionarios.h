#ifndef FUNCIONARIOS_H_INCLUDED
#define FUNCIONARIOS_H_INCLUDED

#include "clientes.h"
#include "caixas.h"
#include "produtos.h"
#include "funcionarios.h"
#include "simulacao.h"
#include "text.h"
#include "calculos.h"
#include "supermercado.h"
#include "uteis.h"



//estrutura para representar um funcionario
typedef struct funcionario
{
    int numero_funcionario;
    char *nome_funcionario;
    bool estado; // 0-indisponivel, 1-disponivel
    int clientes_atendidos; //numero de clientes atendidos
    float faturado; //total faturado

} FUNCIONARIO;

//Elemento do funcionario (NO)
typedef struct elemento_funcionario
{
    FUNCIONARIO *info;
    struct elemento_funcionario *Prox;
} ELEMENTO_FUNCIONARIO, *ptElemento_Funcionario;

//Lista funcionarios
typedef struct lista_funcionario
{
    FUNCIONARIO* Inicio;
    FUNCIONARIO* Fim;
    int NEL;
} LISTA_FUNCIONARIO;

//----------------------------------------------------------------->
/*
// FUNCOES BASICAS
LISTA_FUNCIONARIO *CriarListaFuncionario();
FUNCIONARIO *CriarFuncionario (int _nfuncionario, char *_nome);
void MostrarFuncionario (FUNCIONARIO *F);
void MostrarListaFuncionario (LISTA_FUNCIONARIO *LF);
void MostrarListaFuncionarioOcupados (LISTA_FUNCIONARIO *LF, SUPERMERCADO *S)
void DestruirFuncionario (FUNCIONARIO *F);
void DestruirListaFuncionario(LISTA_FUNCIONARIO *LF);

// INSERIR
void InserirInicioFuncionario (LISTA_FUNCIONARIO *LF, FUNCIONARIO *F);

// PESQUISA
FUNCIONARIO* FuncionarioMaisAtendeu (struct supermercado *S);
FUNCIONARIO* FuncionarioMenosAtendeu (struct supermercado *S);

// SIMULACAO
FUNCIONARIO* EscolherFuncionario (LISTA_FUNCIONARIO *LF);
//FUNCIONARIO* EscolherFuncionario (LISTA_FUNCIONARIO* LF);

// CALCULOS
size_t MemoriaOcupadaFuncionarios (LISTA_FUNCIONARIO *LF);
*/

#endif // FUNCIONARIOS_H_INCLUDED
