#include "clientes.h"
#include "caixas.h"
#include "produtos.h"
#include "funcionarios.h"
#include "simulacao.h"
#include "uteis.h"

// FUNCOES BASICAS
LISTA_CLIENTE *CriarListaCliente()
{
    LISTA_CLIENTE *L = (LISTA_CLIENTE *)malloc (sizeof (LISTA_CLIENTE));
    L->Inicio = NULL;
    L->Fim = NULL;
    L->NEL = 0;
    return L;
}

CLIENTE *CriarCliente (int _ncliente, char *_nome)
{
    CLIENTE *C = (CLIENTE *)malloc (sizeof (CLIENTE));
    C->numero_cliente = _ncliente;
    C->nome_cliente = (char *)malloc ((strlen (_nome) + 1) * sizeof (char));
    strcpy (C->nome_cliente, _nome);
    C->produtos_comprados = CriarListaProduto();
    C->hora_entrada = 0;
    C->hora_caixa = 0;
    C->hora_saida = 0;
    C->tempo_espera = 0;
    return C;
}

//Apagar cliente
void DestruirCliente (CLIENTE *C)
{
    free (C->nome_cliente);
    free (C);
}

//Apagar Lista Cliente
void DestruirListaCliente (LISTA_CLIENTE *LC)
{
    if (!LC || LC->NEL == 0) return;

    ELEMENTO_CLIENTE *atual = LC->Inicio;
    ELEMENTO_CLIENTE *proximo;

    while (atual != NULL)
    {
        proximo = atual->Prox;
        DestruirCliente (atual->info);
        free (atual);
        atual = proximo;
    }
    free (LC);
}

void MostrarCliente (CLIENTE *C)
{
    if (!C) return;
    printf ("\nCLIENTE: [%d] : ESPERA %d : prod: [%d] : [%d]", C->numero_cliente, C->tempo_espera, C->produtos_comprados->NEL, TempoGastoCaixaCliente (C));
    //printf ("\nCLIENTE: [%d] : [%s]", C->numero_cliente, C->nome_cliente);

}

void MostrarClienteSimples (CLIENTE *C)
{
    if (!C) return;
    printf ("\nCLIENTE: [%d] : [%s]", C->numero_cliente, C->nome_cliente);

}

void MostrarListaCliente (LISTA_CLIENTE *LC)
{
    if (!LC || LC->NEL == 0) return;

    ELEMENTO_CLIENTE *NO = LC->Inicio;

    while (NO != NULL)
    {
        MostrarCliente (NO->info);
        MostrarClienteSimples (NO->info);
        NO = NO->Prox;
    }
}

// INSERIR E REMOVER
//Inserir no inicio um o cliente
void InserirInicioCliente (LISTA_CLIENTE *LC, CLIENTE *cliente)
{
    if (!LC || !cliente) return;

    ELEMENTO_CLIENTE *NO = (ELEMENTO_CLIENTE*)malloc (sizeof (ELEMENTO_CLIENTE));

    NO->info = cliente;

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

//Inserir no inicio um o cliente por ordem de tempo de chegada
//Caso o programa avance varios segundos com esta funcao os clientes estao na lista de clientes no corredor ja todos por ordem de ir para caixas
void InserirInicioClienteOrdem (LISTA_CLIENTE* LC, CLIENTE* C)
{
    if (!LC || !C) return;

    ELEMENTO_CLIENTE* novoElemento = (ELEMENTO_CLIENTE*)malloc (sizeof (ELEMENTO_CLIENTE));
    novoElemento->info = C;
    novoElemento->Prox = NULL;

    if (LC->Inicio == NULL)   // A lista está vazia
    {
        LC->Inicio = novoElemento;
        LC->Fim = novoElemento;
    }
    else
    {
        ELEMENTO_CLIENTE* anterior = NULL;
        ELEMENTO_CLIENTE* atual = LC->Inicio;

        while (atual != NULL && atual->info->hora_caixa < C->hora_caixa)
        {
            anterior = atual;
            atual = atual->Prox;
        }

        if (anterior == NULL)   // O novo elemento deve ser o primeiro da lista
        {
            novoElemento->Prox = LC->Inicio;
            LC->Inicio = novoElemento;
        }
        else     // O novo elemento deve ser inserido após o elemento anterior
        {
            novoElemento->Prox = atual;
            anterior->Prox = novoElemento;
        }

        if (atual == NULL)   // O novo elemento deve ser o último da lista
        {
            LC->Fim = novoElemento;
        }
    }
    LC->NEL++;
}

//Inserir no fim um cliente
void InserirFimCliente (LISTA_CLIENTE *LC, CLIENTE *cliente)
{
    if (!LC || !cliente) return;
    // Cria um novo elemento da lista
    ELEMENTO_CLIENTE *NO = (ELEMENTO_CLIENTE *) malloc (sizeof (ELEMENTO_CLIENTE));
    NO->info = cliente;
    NO->Prox = NULL;

    // Verifica se a lista está vazia
    if (LC->Inicio == NULL)
    {
        LC->Inicio = NO;
    }
    else
    {
        LC->Fim->Prox = NO;
    }

    // Atualiza o ponteiro Fim da lista
    LC->Fim = NO;

    // Incrementa o contador de elementos NEL da lista
    LC->NEL++;
}

//Remove um cliente ao qual corresponde um dado numero de cliente
void RemoverClienteCodigo (LISTA_CLIENTE* LC, int numero_cliente)
{
    if (!LC || !numero_cliente) return;

    ELEMENTO_CLIENTE* atual = LC->Inicio;
    ELEMENTO_CLIENTE* anterior = NULL;

    while (atual != NULL)
    {
        if (atual->info->numero_cliente == numero_cliente) //verifica se e o cliente com o dado numero
        {
            if (anterior == NULL) // cliente e o primeiro da lista
            {
                LC->Inicio = atual->Prox;
            }
            else // cliente esta apos o elemento anterior
            {
                anterior->Prox = atual->Prox; //o proximo do cliente anterior e o proximo do atual (ou seja, assim salta o elemento atual)
            }

            if (atual == LC->Fim) // cliente esta no fim
            {
                LC->Fim = anterior;
            }

            free (atual);
            LC->NEL--;
            return;
        }
        anterior = atual;
        atual = atual->Prox;
    }
    printf ("Cliente nao encontrado!\n");
}

// PESQUISAR
//Pesquisa o numero de cliente numa dada caixa
void PesquisarCliente (CAIXA *CX, int _numero_cliente)
{
    if (CX == NULL || _numero_cliente < 0 || CX->fila->NEL == 0)
    {
        printf ("Pesquisa inválida.\n");
        return;
    }

    ELEMENTO_CLIENTE *NO_cliente = CX->fila->Inicio;
    while (NO_cliente != NULL)
    {
        if (NO_cliente->info->numero_cliente == _numero_cliente)
        {
            printf ("O cliente de código %d está na fila da caixa %d.\n", _numero_cliente, CX->numero_caixa);
            return;
        }
        NO_cliente = NO_cliente->Prox;
    }
    printf ("O cliente de código %d não está na fila da caixa.\n", _numero_cliente);
}

//Verifica se o cliente esta no supermercado
int EncontrarClienteDentro (SUPERMERCADO *S, CLIENTE *C) //funcao retorna 1 se o cliente esta no supermercado e 0 se nao esta dentro
{
    if (!S || !C) return 1;

    //printf("\n\n\t %s\n", __FUNCTION__);
    ELEMENTO_CLIENTE *NO_cliente = S->clientes_dentro->Inicio; //NO para percorrer a lista de clientes dentro
    ELEMENTO_CAIXA *NO_caixa = S->caixas->Inicio;

    while (NO_cliente != NULL) //verifica se o cliente se encontra nos corredores
    {
        if (C->numero_cliente == NO_cliente->info->numero_cliente)
        {
            return 1;
        }
        NO_cliente = NO_cliente->Prox;
    }

    while (NO_caixa != NULL) //verifica se o cliente se encontra nas filas das caixas
    {
        NO_cliente = NO_caixa->info->fila->Inicio; //NO atualizado para percorrer filas de clientes nas caixas
        while (NO_cliente != NULL)
        {
            if (C->numero_cliente == NO_cliente->info->numero_cliente)
            {
                return 1; //cliente encontrado
            }
            NO_cliente = NO_cliente->Prox;
        }
        NO_caixa = NO_caixa->Prox;
    }
    return 0; //cliente nao encontrado
}

//______________________________________________________________________________
//SIMULACOES

CLIENTE *EscolherCliente (SUPERMERCADO *S)
{
    if (!S || !S->clientes) return NULL;

    //printf("\n\t %s", __FUNCTION__);
    int posicao = Aleatorio (1, S->clientes->NEL - 1); //gera uma posicao para selecionar o cliente

    ELEMENTO_CLIENTE *NO_cliente = S->clientes->Inicio;

    for (int i = 0; i < posicao; i++) //percorre a lista até à posicao gerada
    {
        NO_cliente = NO_cliente->Prox;
    }

    bool repetido = EncontrarClienteDentro (S, NO_cliente->info); //adquiri da funcao EncontrarClienteDentro a verificacao se o cliente ja esta dentro do supermercado
    if (repetido == 1) //o cliente encontra-se dentro do supermercado
    {
        printf ("\nCliente repetido %d\n", NO_cliente->info->numero_cliente);
        return NULL;
    }

    //printf("\n\t  // %s\n", __FUNCTION__);
    return NO_cliente->info; //nao entrou no teste IF, logo nao e repetido e retorna o cliente
}

//Atualiza todos os tempos dos clientes mediante valores disponibilizados junto com os produtos
//Estes dados sao iniciais e posteriormente serao atualizados mediante a quantidade de pessoas nas filas
CLIENTE *AtualizaHorasCliente (CLIENTE *C, SUPERMERCADO *S)
{
    if (!C) return NULL;

    C->hora_entrada = VerTempoRelogio (S->relogio_supermercado);
    C->hora_caixa = C->hora_entrada + TempoGastoCompraCliente (C);
    C->hora_saida = C->hora_caixa + TempoGastoCaixaCliente (C);

    return C;
}

//Atualiza todos os tempos de espera dos clientes mediante toda a gente que esta na fila
void TempoEsperaCliente (SUPERMERCADO *S)
{
    if (!S) return;

    ELEMENTO_CAIXA *NO_caixa = S->caixas->Inicio;
    ELEMENTO_CLIENTE *NO_cliente = NO_caixa->info->fila->Inicio;

    while (NO_caixa != NULL)
    {
        int tempo_acumulado = 0; //ira acumular o tempo deste o primeiro elemento da fila ate ao ultimo
        NO_cliente = NO_caixa->info->fila->Inicio;
        while (NO_cliente != NULL)
        {
            tempo_acumulado += TempoGastoCaixaCliente (NO_cliente->info); //acumula o tempo do proprio cliente
            NO_cliente->info->tempo_espera = tempo_acumulado;
            NO_cliente->info->hora_saida = NO_cliente->info->hora_caixa + tempo_acumulado;//soma todos os tempos dos clientes a sua frente e o seu, obtendo a hora de saida

            NO_cliente = NO_cliente->Prox;
        }
        NO_caixa = NO_caixa->Prox;
    }
}

//Atualiza o tempo de espera maximo, isto e, quando chegou a caixa era este o tempo que tinha pela frente ate ser atendido
void AtualizaTempoEspera (CLIENTE *C, CAIXA *CX)
{
    if (!C || !CX) return;

    C->tempo_espera_total = CX->tempo_atendimento;
}

//Transfere o cliente entre duas dadas caixas
void TransferirCliente (CAIXA *CX_origem, CAIXA *CX_destino, int _numero_cliente, SUPERMERCADO *S)
{
    if (CX_origem == NULL || CX_destino == NULL || _numero_cliente < 0)
    {
        printf ("Transferência inválida.\n");
        return;
    }

    ELEMENTO_CLIENTE *NO_cliente = CX_origem->fila->Inicio;

    // Percorre a fila de clientes da caixa de origem
    while (NO_cliente != NULL)
    {
        if (NO_cliente->info->numero_cliente == _numero_cliente)
        {
            // Remove o cliente da caixa de origem
            RemoverClienteCodigo (CX_origem->fila, _numero_cliente);
            AtualizaDadosCaixa (CX_origem);
            // Adiciona o cliente à caixa de destino
            AddFila (CX_destino, NO_cliente->info);
            NO_cliente->info->hora_caixa = VerTempoRelogio (S->relogio_supermercado);
            //Atualiza todos os tempos
            AtualizaHorasCliente (NO_cliente->info, S);
            AtualizaTempoEspera (NO_cliente->info, CX_destino);
            AtualizaDadosCaixa (CX_destino);

            printf ("Cliente de código %d transferido da caixa %d para a caixa %d.\n", _numero_cliente, CX_origem->numero_caixa, CX_destino->numero_caixa);
            return;
        }

        NO_cliente = NO_cliente->Prox;
    }
    printf ("Cliente de código %d não encontrado na caixa %d.\n", _numero_cliente, CX_origem->numero_caixa);
}

// CALCULOS
size_t MemoriaOcupadaClientes (LISTA_CLIENTE *LC)
{
    ELEMENTO_CLIENTE *NO = LC->Inicio;
    size_t memoria = 0;

    while (NO != NULL)
    {
        // Memória ocupada pelo cliente
        memoria += sizeof (CLIENTE);
        // Memória ocupada pelo nome do cliente
        memoria += strlen (NO->info->nome_cliente) + 1;
        // Memória ocupada pela lista de produtos comprados do cliente
        memoria += MemoriaOcupadaProdutos (NO->info->produtos_comprados);
        // Memória ocupada pelo nó da lista de clientes
        memoria += sizeof (ELEMENTO_CLIENTE);

        NO = NO->Prox;
    }
    return memoria;
}

