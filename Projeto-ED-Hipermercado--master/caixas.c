#include "clientes.h"
#include "caixas.h"
#include "produtos.h"
#include "funcionarios.h"
#include "simulacao.h"
#include "uteis.h"

// FUNCOES BASICAS
LISTA_CAIXA *CriarListaCaixa()
{
    LISTA_CAIXA *L = (LISTA_CAIXA *)malloc (sizeof (LISTA_CAIXA));
    L->Inicio = NULL;
    L->Fim = NULL;
    L->NEL = 0;
    return L;
}

CAIXA *CriarCaixa (int _numero_caixa, char *_desc)
{
    CAIXA *CX = (CAIXA *)malloc (sizeof (CAIXA));
    CX->numero_caixa = _numero_caixa;
    CX->descricao = (char *)malloc ((strlen (_desc) + 1) * sizeof (char));
    CX->tempo_atendimento = 0;
    LISTA_CLIENTE *LC = CriarListaCliente();
    CX->fila = LC;
    LISTA_CLIENTE *LCA = CriarListaCliente();
    CX->atendidos = LCA;
    strcpy (CX->descricao, _desc);
    CX->funcionario = NULL;
    CX->valor_acumulado = 0;
    return CX;
}

void MostrarCaixa (CAIXA *CX)
{
    printf ("\nCAIXA: [%d] : [%s] : clientes [%d] : antendeu=[%d] : TEsp %d : ", CX->numero_caixa, CX->descricao, CX->fila->NEL, CX->atendidos->NEL, CX->tempo_atendimento);
    //printf ("\n\%d", CX->funcionario->numero_funcionario);
    if (CX->estado == 0) printf ("ENCERRADA");
    else if (CX->estado == 1) printf ("ABERTA");
    //MostrarListaCliente (CX->fila);
}

void MostrarCaixaSimples (CAIXA *CX)
{
    printf ("\nCAIXA: [%d] : CLIENTES [%d] : ATENDEU: [%d] : FATUROU: [%.2f E] : ", CX->numero_caixa, CX->fila->NEL, CX->atendidos->NEL, CX->valor_acumulado);
    //printf ("\n\%d", CX->funcionario->numero_funcionario);
    if (CX->estado == 0) printf ("ENCERRADA");
    else if (CX->estado == 1) printf ("ABERTA");
    //MostrarListaClienteSimples (CX->fila);
}

void MostrarListaCaixas (LISTA_CAIXA *LCX)
{
    if (!LCX) return;
    //int contador = 0;
    ELEMENTO_CAIXA *NO = LCX->Inicio;

    while (NO != NULL)
    {
        MostrarCaixaSimples (NO->info);
        //MostrarCaixa (NO->info);
        NO = NO->Prox;
        //contador++;
    }
}

void MostrarListaCaixasAbertas (LISTA_CAIXA *LCX)
{
    if (!LCX) return;
    //int contador = 0;
    ELEMENTO_CAIXA *NO = LCX->Inicio;

    while (NO != NULL)
    {
        if (NO->info->estado == 1)
            MostrarCaixaSimples (NO->info);
        NO = NO->Prox;
        //contador++;
    }
}

void MostrarListaCaixasFechadas (LISTA_CAIXA *LCX)
{
    if (!LCX) return;
    //int contador = 0;
    ELEMENTO_CAIXA *NO = LCX->Inicio;

    while (NO != NULL)
    {
        if (NO->info->estado == 0)
            MostrarCaixaSimples (NO->info);
        NO = NO->Prox;
        //contador++;
    }
}

//Apagar caixa
void DestruirCaixa (CAIXA *CX)
{
    free (CX->descricao);
    DestruirListaCliente (CX->fila);
    //DestruirListaCliente(CX->atendidos);
    free (CX);
}

//Apagar Lista Caixa
void DestruirListaCaixa (LISTA_CAIXA *LC)
{
    if (!LC || LC->NEL == 0) return;

    ELEMENTO_CAIXA *atual = LC->Inicio;
    ELEMENTO_CAIXA *proximo = NULL;

    while (atual != NULL)
    {
        proximo = atual->Prox;
        DestruirCaixa (atual->info);
        free (atual);
        atual = proximo;
    }
    free (LC);
}

//INSERIR
//Inserir no inicio uma caixa
void InserirInicioCaixa (LISTA_CAIXA *LC, CAIXA *caixa)
{
    if (!LC || !caixa) return;
    //gravar(__FUNCTION__);
    ELEMENTO_CAIXA *NO = (ELEMENTO_CAIXA*)malloc (sizeof (ELEMENTO_CAIXA));
    NO->info = caixa;

    if (LC->Inicio == NULL)
    {
        NO->Prox = NULL;
        LC->Fim = NO;
    }
    else
    {
        NO->Prox = LC->Inicio;
    }

    LC->Inicio = NO;
    LC->NEL++;
}

//Inserir no fim uma caixa
void InserirFimCaixa (LISTA_CAIXA* LC, CAIXA* caixa)
{
    if (!LC || !caixa) return;

    ELEMENTO_CAIXA* novoElemento = (ELEMENTO_CAIXA*)malloc (sizeof (ELEMENTO_CAIXA));
    novoElemento->info = caixa;
    novoElemento->Prox = NULL;

    if (LC->Inicio == NULL)
    {
        LC->Inicio = novoElemento;
    }
    else
    {
        LC->Fim->Prox = novoElemento;
    }

    LC->Fim = novoElemento;
    LC->NEL++;
}

//GERIR ABERTURAS E FECHOS DE CAIXA
//Executa todas as atualizacoes necessarias para a caixa se dar como aberta
CAIXA *AtivarCaixa (CAIXA *CX, LISTA_FUNCIONARIO *LF)
{
    if (!CX) return;

    CX->tempo_atendimento = 0;
    CX->estado = 1;
    CX->funcionario = EscolherFuncionario (LF);
    if (CX->funcionario == NULL) printf ("\n * IMPOSSIVEL ABRIR CAIXA * funcionarios = NULL");//nao existem funcionarios disponiveis
    return CX;
}

//Executa todas as atualizacoes necessarias para a caixa se dar como fechada
CAIXA *DesativarCaixa (CAIXA *CX)
{
    if (!CX) return;

    CX->funcionario->estado = 1;
    CX->estado = 0;

    return CX;
}

//Fecha uma caixa com um dado numero
void FecharCaixaNum (int numero_caixa, SUPERMERCADO *S)
{
    if (!S) return;

    ELEMENTO_CAIXA *NO_caixa = S->caixas->Inicio;

    while (NO_caixa != NULL)
    {
        if (NO_caixa->info->numero_caixa == numero_caixa)
        {
            DesativarCaixa (NO_caixa->info);
            printf ("\n - - - - FECHAR CX %d - - - - ", NO_caixa->info->numero_caixa);
            Gravar ("Fechar Caixa", S->relogio_supermercado);
            return;
        }
        NO_caixa = NO_caixa->Prox;
    }
    printf ("\n * CAIXA NAO EXISTE * ");
}

//Abre uma nova caixa, pois na lista de caixas nao existe nenhuma aberta ou com a possibilidade de retomar o trabalho
CAIXA *AbrirNovaCaixa (SUPERMERCADO *S)
{
    if (!S) return;

    int numero_caixa = S->caixas->NEL + 1;
    char descricao[21];

    sprintf (descricao, "CAIXA N_%d", numero_caixa);

    CAIXA *CX = CriarCaixa (numero_caixa, descricao);
    AtivarCaixa (CX, S->funcionario);
    InserirFimCaixa (S->caixas, CX);
    printf ("\n - - - - ABRIR CX %d - - - - ", CX->numero_caixa);
    Gravar ("Abrir Caixa", S->relogio_supermercado);
    return CX;
}

//Abre uma caixa, verifica se e necessaria abrir uma caixa nova ou se existe alguma ja na lista que possa retomar o trabalho
CAIXA *AbrirCaixa (SUPERMERCADO *S)
{
    if (!S) return;
    if (TotalCaixasAbertas (S) >= 25 && S->caixas->NEL >= 25)
    {
        printf ("\n * QNT MAXIMA ATINGIDA - 25 CAIXAS");
        return;
    }
    ELEMENTO_CAIXA *NO_caixa = S->caixas->Inicio;

    while (NO_caixa != NULL)
    {
        if (NO_caixa->info->estado == 0 && NO_caixa->info->fila->NEL == 0)
        {
            CAIXA *CX = AtivarCaixa (NO_caixa->info, S->funcionario);
            printf ("\n - - - - ABRIR CX %d - - - - ", CX->numero_caixa);
            Gravar ("Abrir Caixa", S->relogio_supermercado);
            return NO_caixa->info;
        }
        NO_caixa = NO_caixa->Prox;
    }
    return AbrirNovaCaixa (S);
}

//Verifica a necessidade de abrir uma nova caixa mediante o a media de clientes por fila
int GerirAberturaCaixa (SUPERMERCADO *S)
{
    if (!S) return 0;
    //printf ("\n    %s ",  __FUNCTION__);

    int media_clientes = MediaClientesCaixa (S);

    if (media_clientes > S->n_max_clientes)
    {
        CAIXA *CX;
        CX = AbrirCaixa (S);
        if (CX != NULL)
        {
            return 1;
        }
    }
    //printf ("\n  //  %s ",  __FUNCTION__);
    return 0;
}

//Verifica a necessidade de fechar uma caixa mediante o a media de clientes por fila
void GerirFechoCaixa (SUPERMERCADO *S)
{
    if (!S) return;
    //printf ("\n    %s ",  __FUNCTION__);

    int media_clientes = MediaClientesCaixa (S);
    int caixas_abertas = TotalCaixasAbertas (S);

    ELEMENTO_CAIXA *NO_caixa = S->caixas->Inicio;

    //compara se a media e inferior ao numero maximo permitido
    if (media_clientes < S->n_max_clientes && caixas_abertas > S->caixas_permanentes)//tambem so e encerrada alguma caixa se o numero de caixas abertas e superior
    {
        //ao numero de caixas que o utilizador quer sempre abertas
        while (NO_caixa != NULL)
        {
            if (NO_caixa->info->numero_caixa != 1 && NO_caixa->info->estado == 1)
            {
                printf ("\n - - - - FECHAR CX %d - - - - ",  NO_caixa->info->numero_caixa);
                Gravar ("FECHAR CAIXA", S->relogio_supermercado);
                DesativarCaixa (NO_caixa->info);
                return;
            }
            NO_caixa = NO_caixa->Prox;
        }
    }
    //printf ("\n  //  %s ",  __FUNCTION__);
}

//PESQUISA
//Pesquisa uma caixa pelo o numero e retorna um ponteiro para essa mesma caixa
CAIXA *PesquisarCaixaCod (SUPERMERCADO *S, int numero_caixa)
{
    if (!S || numero_caixa < 0) return NULL;

    ELEMENTO_CAIXA *NO_caixa = S->caixas->Inicio;

    while (NO_caixa != NULL)
    {
        if (NO_caixa->info->numero_caixa == numero_caixa) return NO_caixa->info;
        NO_caixa = NO_caixa->Prox;
    }
    return NULL;
}

//Retorna a caixa que mais numero de clientes atendeu
CAIXA* CaixaMaisAtendeu (SUPERMERCADO* S)
{
    if (!S || !S->caixas) return NULL;

    printf ("\n%s", __FUNCTION__);
    ELEMENTO_CAIXA* NO_caixa = S->caixas->Inicio;
    CAIXA* caixaMaisAtendeu = NO_caixa->info;

    while (NO_caixa != NULL)
    {
        if (NO_caixa->info->atendidos->NEL > caixaMaisAtendeu->atendidos->NEL)
        {
            caixaMaisAtendeu = NO_caixa->info;
        }
        NO_caixa = NO_caixa->Prox;
    }
    return caixaMaisAtendeu;
}

//Retorna a caixa que mais numero de clientes atendeu
CAIXA* CaixaMenosAtendeu (SUPERMERCADO* S)
{
    if (!S || !S->caixas) return NULL;
    printf ("\n%s", __FUNCTION__);
    ELEMENTO_CAIXA* NO_caixa = S->caixas->Inicio;
    CAIXA* caixaMenosAtendeu = NO_caixa->info;

    while (NO_caixa != NULL)
    {
        if (NO_caixa->info->atendidos->NEL < caixaMenosAtendeu->atendidos->NEL)
        {
            caixaMenosAtendeu = NO_caixa->info;
        }
        NO_caixa = NO_caixa->Prox;
    }
    return caixaMenosAtendeu;
}


//______________________________________________________________________________
//SIMULACOES
//Escolhe a melhor caixa para colocar um cliente mediante o tempo de espera
CAIXA *EscolherCaixa (LISTA_CAIXA *LCX)
{
    if (!LCX) return;

    ELEMENTO_CAIXA *NO_menor_tempo = LCX->Inicio;
    ELEMENTO_CAIXA *NO_atual = LCX->Inicio->Prox;

    while (NO_atual != NULL)//percorre toda a lista de caixas
    {
        if (NO_atual->info->estado == 1 && NO_atual->info->tempo_atendimento < NO_menor_tempo->info->tempo_atendimento)//comparar tempo no_atual com o no que tem o menor tempo
        {
            //e temos de estar a falar de uma caixa aberta (estado=1)
            // Se o tempo de atendimento da caixa atual for menor, atualiza a caixa de menor tempo
            NO_menor_tempo = NO_atual;
        }
        NO_atual = NO_atual->Prox;//avanca na lista
    }
    return NO_menor_tempo->info;
}

//Atualiza o tempo de atendimento de uma dada caixa
void AtualizaDadosCaixa (CAIXA *CX)
{
    if (!CX || !CX->fila) return;

    ELEMENTO_CLIENTE *NO_cliente = CX->fila->Inicio;
    CX->tempo_atendimento = 0;

    if (CX->fila->NEL == 0)
    {
        return;
    }
    while (NO_cliente != NULL)
    {
        CX->tempo_atendimento += TempoGastoCaixaCliente (NO_cliente->info);

        NO_cliente = NO_cliente->Prox;
    }

    return;
}


//GESTAO DE CLIENTES NAS CAIXAS
//Redistribui os todos clientes de uma caixa, por ex, uma caixa que fechou
void RedistribuirClientesCaixa (int numero_caixa, SUPERMERCADO* S)
{
    if (!S) return;

    CAIXA *CX = PesquisarCaixaCod (S, numero_caixa);
    ELEMENTO_CAIXA* NO_caixa = S->caixas->Inicio;
    ELEMENTO_CLIENTE *NO_cliente = CX->fila->Inicio;

    if (CX->fila->NEL == 0) return;
    while (NO_cliente != NULL)
    {
        AddFila (EscolherCaixa (S->caixas), NO_cliente->info);
        RemoverClienteCodigo (CX->fila, NO_cliente->info->numero_cliente);

        NO_cliente = NO_cliente->Prox;
    }
    TempoEsperaCliente (S);
    return;
}

//Redistribui os clientes que estao em caixas para esta caixa caso assim compense, por ex, para quando uma caixa abre
CAIXA* RedistribuirClientes (CAIXA* CX, SUPERMERCADO* S)
{
    if (!S) return NULL;

    ELEMENTO_CAIXA *NO_caixa = S->caixas->Inicio;
    ELEMENTO_CLIENTE *NO_cliente_novo = CX->fila->Inicio;
    ELEMENTO_CLIENTE *NO_cliente = NO_caixa->info->fila->Inicio;

    int total_clientes = TotalClientesCaixas (S);

    while (NO_caixa != NULL)
    {
        NO_cliente = NO_caixa->info->fila->Inicio;
        while (NO_cliente != NULL)
        {
            if (NO_cliente->info->tempo_espera > CX->tempo_atendimento)
            {
                // Transferir cliente para a nova caixa
                printf ("\n* * tranferir %d  CX %d -> CX %d \n", NO_cliente->info->numero_cliente, NO_caixa->info->numero_caixa, CX->numero_caixa);
                RemoverClienteCodigo (NO_caixa->info->fila, NO_cliente->info->numero_cliente);
                AddFila (CX, NO_cliente->info);
                NO_cliente->info->hora_caixa = VerTempoRelogio (S->relogio_supermercado);
                NO_cliente->info->hora_saida += TempoGastoCaixaCliente (NO_cliente->info);
                AtualizaDadosCaixa (CX);
                TempoEsperaCliente (S);
            }
            NO_cliente = NO_cliente->Prox;
        }
        AtualizaDadosCaixa (CX);
        NO_caixa = NO_caixa->Prox;
    }

    return NULL;
}

//CALCULOS DAS CAIXAS
size_t MemoriaOcupadaCaixas (LISTA_CAIXA *LCX)
{
    ELEMENTO_CAIXA *NO = LCX->Inicio;
    size_t memoria = 0;

    while (NO != NULL)
    {
        // Memória ocupada pela estrutura CAIXA
        memoria += sizeof (CAIXA);
        // Memória ocupada pela descrição da caixa
        memoria += strlen (NO->info->descricao) + 1;
        // Memória ocupada pela fila de clientes
        memoria += MemoriaOcupadaClientes (NO->info->fila);
        // Memória ocupada pela lista de clientes atendidos
        memoria += MemoriaOcupadaClientes (NO->info->atendidos);
        // Memória ocupada pelo ponteiro para o funcionário
        memoria += sizeof (FUNCIONARIO *);
        // Memória ocupada pelos nós da lista de caixas
        memoria += sizeof (ELEMENTO_CAIXA);

        NO = NO->Prox;
    }
    return memoria;
}























