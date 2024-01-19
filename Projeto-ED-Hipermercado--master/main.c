#include "clientes.h"
#include "caixas.h"
#include "funcionarios.h"
#include "produtos.h"
#include "supermercado.h"
#include "text.h"
#include "simulacao.h"
#include "calculos.h"
#include "uteis.h"

#define INDISPONIVEL 0
#define DISPONIVEL 1
#define TEMPO_MAX_ESPERA 120
#define N_CLIENTES_MAX 10
#define CLIENTES_MAX 100

int RUN (SUPERMERCADO *S)
{
    size_t MEMORIA = MemoriaOcupadaSupermercado (S);
    float MEMORIA_MB = (float)MEMORIA / 1048576.0f;
    printf ("MEMORIA do [%s]= %lu Bytes : [%.2f] MB\n", S->nome_supermercado, MEMORIA, MEMORIA_MB);

    Gravar ("SIMULACAO", S->relogio_supermercado);
    if (TotalClientes (S) <= CLIENTES_MAX)
    {
        Gravar ("Entrada Clientes", S->relogio_supermercado);
        printf ("\n  Entram Clientes  ");
        for (int i = 0; i < 10; i++)
        {
            EntradaCliente (S, S->relogio_supermercado);
        }
    }
    else printf ("\n * * * LOTACAO MAXIMA * * * \n");


    ProcessarCorredor (S);
    ProcessarCaixas (S);
    SairCliente (S);
    Gravar ("// SIMULACAO", S->relogio_supermercado);
    WaitSegundos (1);
    return 1;
}

int menu()
{
    int opcao;

    //system("cls");
    printf ("\n #---------------------------------------------------------#");
    printf ("\n | (1) Abrir Caixa                                         |");
    printf ("\n | (2) Fechar Caixa                                        |");
    printf ("\n | (3) Ver Estatisticas                                    |");
    printf ("\n | (4) Alterar Max Clientes em Caixa                       |");
    printf ("\n | (5) Ver Empregados ocupados                             |");
    printf ("\n | (6) Ver caixas                                          |");
    printf ("\n | (7) Hora Atual                                          |");
    printf ("\n | (8) Mostrar Clientes Atendidos                          |");
    printf ("\n | (9) ENCERRAR PROGRAMA                                   |");
    printf ("\n |---------------------------------------------------------|");
    printf ("\n | (0) Retomar Programa                                    |");
    printf ("\n #---------------------------------------------------------#");
    do
    {
        printf ("\nQual a sua opcao: ");
        scanf ("%d", &opcao);
    }
    while (opcao < 0 || opcao > 9);
    return opcao;
}

int ExecutarOpcao (int opcao, SUPERMERCADO *S)
{
    int geral = 0;

    switch (opcao)
    {
    case 0:
        // Continuar para o programa
        return 0;

        break;
    case 1:
        // Abrir Caixa
        printf ("\nOpcao: Abrir Caixa");
        AbrirCaixa (S);
        RedistribuirClientes (EscolherCaixa (S->caixas), S);

        break;
    case 2:
        // Fechar Caixa
        printf ("\nOpcao: Fechar Caixa");
        MostrarListaCaixasAbertas (S->caixas);
        printf ("\n Qual caixa deseja encerrar? \n");
        geral = 0;

        while (geral < 2 || geral > 25)
        {
            printf ("\nNUMERO > ");
            scanf ("%d", &geral);
        }
        FecharCaixaNum (geral, S);
        RedistribuirClientesCaixa (geral, S);
        break;
    case 3:
        // Ver Estatisticas
        printf ("\nOpcao: Ver Estatisticas");
        GerarEstatisticas (S);
        MostrarEstatisticas (S);
        break;
    case 4:
        // Alterar Max Clientes em Caixa
        printf ("\nOpcao: Alterar Max Clientes por Caixa");
        printf ("\n Valor maximo atual : &d \n", S->n_max_clientes);
        printf ("\nNovo valor > ");
        scanf ("%d", &geral);
        S->n_max_clientes = geral;
        Gravar ("Alterar Max Clientes", S->relogio_supermercado);
        break;
    case 5:
        // Ver Empregados ocupados
        printf ("\nOpcao: Ver Empregados ocupados");
        MostrarListaFuncionarioOcupados (S->funcionario, S);
        break;
    case 6:
        // Ver caixas
        printf ("\nOpcao: Ver caixas");
        printf ("\n\t\tCaixas abertas");
        MostrarListaCaixasAbertas (S->caixas);
        printf ("\n\t\tCaixas fechadas");
        MostrarListaCaixasFechadas (S->caixas);
        Gravar ("Ver Caixas", S->relogio_supermercado);
        break;
    case 7:
        // Hora Atual
        printf ("\nOpcao: Hora Atual");
        printf ("HORA ATUAL : %s", ConverterTimeToChar (VerTempoRelogio (S->relogio_supermercado)));
        Gravar ("Ver Hora Atual", S->relogio_supermercado);
        break;
    case 8:
        //Mostrar Clientes Atendidos
        printf ("\nOpcao: Mostrar Clientes Atendidos");
        ELEMENTO_CAIXA *NO_caixa = S->caixas->Inicio;
        while (NO_caixa != NULL)
        {
            MostrarListaCliente (NO_caixa->info->atendidos);
            NO_caixa = NO_caixa->Prox;
        }
        Gravar ("Mostrar Clientes Atendidos", S->relogio_supermercado);
        break;
    case 9:
        // ENCERRAR PROGRAMA
        printf ("\nOpcao: ENCERRAR PROGRAMA");
        printf ("\nOpcao: TEM A CERETEZA? \n (0) NAO  :  (1) SIM\n > ");
        scanf ("%d", &geral);
        if (geral == 1)
        {
            Gravar ("Utilizador fechou o programa", S->relogio_supermercado);
            return 1;
        }
        else break;
    default:
        printf ("\nOpcao invalida!");
    }
    return 0;
}

int main()
{
    srand (time (NULL));
    setlocale (LC_ALL, "pt_PT");
    Relogio R;
    int velocidade_clock = 10;
    StartRelogio (&R, velocidade_clock, "09:00:00");
    printf ("HORA DE ARRANQUE %s", ConverterTimeToChar (VerTempoRelogio (R)));

    //CLIENTES
    LISTA_CLIENTE *LC = CriarListaCliente();
    LC = ImportarFicheiroClientes ("clientes.txt", R);

    //FUNCIONARIOS
    LISTA_FUNCIONARIO *LF = CriarListaFuncionario();
    LF = ImportarFicheiroFuncionarios ("funcionarios.txt", R);

    //PRODUTOS
    LISTA_PRODUTO *LP = CriarListaProduto();
    LP = ImportarFicheiroProdutos ("produtos.txt", R);

    //CAIXAS
    LISTA_CAIXA *LCX = CriarListaCaixa();
    MostrarListaCaixas (LCX);

    //SUPERMERCADO
    SUPERMERCADO *S = CriarSupermercado ("CONTINENTE", R, LC, LF, LCX, LP, "22:00:00", TEMPO_MAX_ESPERA, N_CLIENTES_MAX);
    AbrirSupermercado (S, R);

    //SIMULACAO
    printf ("\n//%d", S->clientes_dentro->NEL);


    int tecla;
    int continuar = 1;

    while (continuar)
    {
        printf ("\n\t\t - - - - - - - Programa ED - - - - - - -  \n");
        //printf ("\t\t     * Precione ENTER para PARAR * \n");
        RUN (S);

        if (kbhit())   // Verifica se uma tecla foi pressionada
        {
            tecla = getch(); // Obtém o código da tecla pressionada
            if (tecla == 13)   // 13 é o código ASCII da tecla 'Enter'
            {
                Gravar ("UTILIZADOR COMANDA", S->relogio_supermercado);
                printf ("Programa Interrompido.\n");
                int opcao;
                do
                {
                    opcao = menu();
                    tecla = ExecutarOpcao (opcao, S);
                }
                while (opcao != 0 && tecla != 1);

                if (tecla == 0)
                {
                    printf ("O programa foi retomado.\n");
                    Gravar ("// UTILIZADOR COMANDA", S->relogio_supermercado);
                }
                else
                {
                    continuar = 0;
                    Gravar ("// UTILIZADOR COMANDA", S->relogio_supermercado);
                }
            }
        }
    }

    GerarEstatisticas (S);
    MostrarEstatisticas (S);
    GravarEstatisticas (S);
    DestruirSupermercado (S);
    S = NULL;

    printf ("\n____________________________ENCERRADA SIMULACAO__________________________________");
    Gravar ("* * * * ENCERRADA EXECUCAO * * * *", R);

    return 0;
}
