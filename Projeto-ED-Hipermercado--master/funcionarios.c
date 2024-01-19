#include "clientes.h"
#include "caixas.h"
#include "produtos.h"
#include "funcionarios.h"
#include "simulacao.h"
#include "uteis.h"

// FUNCOES BASICAS
LISTA_FUNCIONARIO *CriarListaFuncionario()
{
    LISTA_FUNCIONARIO *L = (LISTA_FUNCIONARIO *)malloc (sizeof (LISTA_FUNCIONARIO));
    L->Inicio = NULL;
    L->Fim = NULL;
    L->NEL = 0;

    return L;
}

FUNCIONARIO *CriarFuncionario (int _nfuncionario, char *_nome)
{
    FUNCIONARIO *F = (FUNCIONARIO *)malloc (sizeof (FUNCIONARIO));
    F->numero_funcionario = _nfuncionario;
    F->nome_funcionario = (char *)malloc ((strlen (_nome) + 1) * sizeof (char));
    strcpy (F->nome_funcionario, _nome);
    F->estado = 1;
    F->faturado = 0;
    F->clientes_atendidos = 0;

    return F;
}

void MostrarFuncionario (FUNCIONARIO *F)
{
    if (!F) return;
    printf ("\nFUNCIONARIO: [%d] : [%s] : ATENDEU [%d] ", F->numero_funcionario, F->nome_funcionario, F->clientes_atendidos);
    if (F->estado == 1) printf ("DESCANSO");
    else if (F->estado == 0) printf ("TRABALHAR");
    //printf("FUNCIONARIO: [%d] : [%s]\n", F->numero_funcionario, F->nome_funcionario);

}

void MostrarListaFuncionario (LISTA_FUNCIONARIO *LF)
{
    if (!LF) return;
    //int contador = 0;
    ELEMENTO_FUNCIONARIO *NO = LF->Inicio;

    printf ("\n\t FUNCIONARIOS : %d", LF->NEL);
    while (NO != NULL)
    {
        MostrarFuncionario (NO->info);
        NO = NO->Prox;
        //contador++;
    }
}

void MostrarListaFuncionarioOcupados (LISTA_FUNCIONARIO *LF, SUPERMERCADO *S)
{
    if (!LF) return;

    ELEMENTO_FUNCIONARIO *NO = LF->Inicio;

    printf ("\n\t FUNCIONARIOS : %d", LF->NEL);
    while (NO != NULL)
    {
        if (NO->info->estado == 0)
        {
            MostrarFuncionario (NO->info);
        }

        NO = NO->Prox;
    }
    Gravar (__FUNCTION__, S->relogio_supermercado);
}

void DestruirFuncionario (FUNCIONARIO *F)
{
    free (F->nome_funcionario);
    free (F);
}

void DestruirListaFuncionario (LISTA_FUNCIONARIO *LF)
{
    if (!LF) return;

    ELEMENTO_FUNCIONARIO *atual = LF->Inicio;
    ELEMENTO_FUNCIONARIO *proximo = NULL;

    while (atual != NULL)
    {
        proximo = atual->Prox;
        DestruirFuncionario (atual->info);
        free (atual);
        atual = proximo;
    }
    free (LF);
}

void InserirInicioFuncionario (LISTA_FUNCIONARIO *LF, FUNCIONARIO *F)
{
    if (!LF || !F) return;

    ELEMENTO_FUNCIONARIO *NO = (ELEMENTO_FUNCIONARIO*)malloc (sizeof (ELEMENTO_FUNCIONARIO));

    NO->info = F;

    if (LF->Inicio == NULL)
    {
        NO->Prox = NULL;
        LF->Fim = NO;
    }
    else
    {
        NO->Prox = LF->Inicio;
    }

    LF->Inicio = NO;
    LF->NEL++;
    //DestruirCliente(cliente);
}

FUNCIONARIO* EscolherFuncionario (LISTA_FUNCIONARIO* LF)
{
    if (!LF || LF->NEL == 0) return NULL;  // Retornar um valor adequado quando a lista de funcionários for nula

    int contador = 0;
    ELEMENTO_FUNCIONARIO *NO = LF->Inicio;

    //Loop para encontrar um funcionário disponível
    while (contador < LF->NEL)
    {
        if (NO->info->estado == 1)
        {
            NO->info->estado = 0;  // Atualizar o estado para indicar que o funcionário foi selecionado (indisponível)
            return NO->info;  // Retornar o funcionário selecionado
        }
        contador++;
        NO = NO->Prox;
    }

    printf ("\nSem funcionarios disponiveis");
    return NULL;  // Retornar um valor adequado quando não há funcionários disponíveis
}

//Retorna o funcionario que mais numero de clientes atendeu
FUNCIONARIO* FuncionarioMaisAtendeu (SUPERMERCADO* S)
{
    if (!S) return NULL;
    printf ("\n%s", __FUNCTION__);
    ELEMENTO_FUNCIONARIO* NO_funcionario = S->funcionario->Inicio;
    FUNCIONARIO* funcionarioMaisAtendeu = NO_funcionario->info;

    while (NO_funcionario != NULL)
    {
        if (NO_funcionario->info->clientes_atendidos > funcionarioMaisAtendeu->clientes_atendidos)
        {
            funcionarioMaisAtendeu = NO_funcionario->info;
        }
        NO_funcionario = NO_funcionario->Prox;
    }
    return funcionarioMaisAtendeu;
}

//Retorna o funcionario que menos numero de clientes atendeu
FUNCIONARIO* FuncionarioMenosAtendeu (SUPERMERCADO* S)
{
    if (!S) return NULL;
    printf ("\n%s", __FUNCTION__);
    ELEMENTO_FUNCIONARIO* NO_funcionario = S->funcionario->Inicio;
    FUNCIONARIO* funcionarioMenosAtendeu = NO_funcionario->info;

    while (NO_funcionario != NULL)
    {
        if (NO_funcionario->info->clientes_atendidos < funcionarioMenosAtendeu->clientes_atendidos)
        {
            funcionarioMenosAtendeu = NO_funcionario->info;
        }
        NO_funcionario = NO_funcionario->Prox;
    }
    return funcionarioMenosAtendeu;
}

//CALCULOS
size_t MemoriaOcupadaFuncionarios (LISTA_FUNCIONARIO *LF)
{
    ELEMENTO_FUNCIONARIO *NO = LF->Inicio;
    size_t memoria = 0;

    while (NO != NULL)
    {
        //Memoria ocupada pelos nomes dos funcionarios
        memoria += strlen (NO->info->nome_funcionario) + 1;
        //Memoria ocupada pelos funcionarios
        memoria += sizeof (FUNCIONARIO);
        //Memoria ocupada pelos NOS da lista de funcionarios
        memoria += sizeof (ELEMENTO_FUNCIONARIO);

        NO = NO->Prox;
    }
    return memoria;
}
