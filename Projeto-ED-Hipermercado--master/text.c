#include "clientes.h"
#include "caixas.h"
#include "produtos.h"
#include "funcionarios.h"
#include "simulacao.h"
#include "uteis.h"

//LER DADOS
//LER DADOS DE FICHEIROS PARA A LISTA DE CLIENTE
LISTA_CLIENTE *ImportarFicheiroClientes (char *nFicheiro, Relogio *R)
{
    FILE *F = fopen (nFicheiro, "rt");
    if (!F || !nFicheiro) return;

    LISTA_CLIENTE *L = CriarListaCliente();
    char BUFFER[1001];

    while (!feof (F))
    {
        fgets (BUFFER, 1000, F);
        //printf("Linha: [%s]\n", BUFFER);

        //remove o ultimo enter (/n) de cada linha do ficheiro
        /*size_t len = strcspn (BUFFER, "\n");
        if (len < strlen (BUFFER))
        {
            BUFFER[len] = '\0';
        }*/
        RemoverCaracterNull (BUFFER);

        int CODIGO;
        char *NOME;
        int i = 0;
        char *CAMPOS[10];
        char *token = strtok (BUFFER, "\t");

        while (token != NULL)
        {
            /*printf("%s\n", token);
            if(i == 0)
                CODIGO = atoi(token);
            if(i == 1)
                NOME = token;
                */

            CAMPOS[i] = token;
            token = strtok (NULL, "\t");
            i++;
        }
        CODIGO = atoi (CAMPOS[0]);
        NOME = CAMPOS[1];
        //printf("CODIGO: %d \t\t NOME: %s", CODIGO, NOME);

        CLIENTE *C = CriarCliente (CODIGO, NOME);
        InserirInicioCliente (L, C);
        //MostrarCliente(C);
        //DestruirCliente(C);
    }
    fclose (F);
    Gravar (__FUNCTION__, R);
    return L;
}

//LER DADOS DE FICHEIROS PARA A LISTA DE FUNCIONARIOS
LISTA_FUNCIONARIO *ImportarFicheiroFuncionarios (char *nFicheiro, Relogio *R)
{
    FILE *F = fopen (nFicheiro, "rt");
    if (!F) return;

    LISTA_FUNCIONARIO *LF = CriarListaFuncionario();
    char BUFFER[1001];

    while (!feof (F))
    {
        fgets (BUFFER, 1000, F);
        //printf("Linha: [%s]\n", BUFFER);

        //remove o ultimo enter (/n) de cada linha do ficheiro
        RemoverCaracterNull (BUFFER);

        int CODIGO;
        char *NOME;
        int i = 0;
        char *CAMPOS[10];
        char *token = strtok (BUFFER, "\t");

        while (token != NULL)
        {
            CAMPOS[i] = token;
            token = strtok (NULL, "\t");
            i++;
        }
        CODIGO = atoi (CAMPOS[0]);
        NOME = CAMPOS[1];
        //printf("CODIGO: %d \t\t NOME: %s", CODIGO, NOME);

        FUNCIONARIO *Func = CriarFuncionario (CODIGO, NOME);
        //MostrarFuncionario(Func);
        InserirInicioFuncionario (LF, Func);
        //DestruirFuncionario(Func);
    }
    fclose (F);
    Gravar (__FUNCTION__, R);
    return LF;
}

//LER DADOS DE FICHEIROS PARA A LISTA DE PRODUTOS
LISTA_PRODUTO *ImportarFicheiroProdutos (char *nFicheiro, Relogio *R)
{
    FILE *F = fopen (nFicheiro, "rt");
    if (!F) return;

    LISTA_PRODUTO *L = CriarListaProduto();
    char BUFFER[1001];

    while (!feof (F))
    {
        fgets (BUFFER, 1000, F);

        RemoverCaracterNull (BUFFER);

        int CODIGO;
        char *NOME;
        float PRECO;
        float TCOMPRA;
        float TCAIXA;

        int i = 0;
        char *CAMPOS[10];
        char *token = strtok (BUFFER, "\t");

        while (token != NULL)
        {
            CAMPOS[i] = token;
            token = strtok (NULL, "\t");
            i++;
        }

        CODIGO = atoi (CAMPOS[0]);
        NOME = CAMPOS[1];
        PRECO = atof (CAMPOS[2]);
        TCOMPRA = atof (CAMPOS[3]);
        TCAIXA = atof (CAMPOS[4]);

        PRODUTO *P = CriarProduto (CODIGO, NOME, PRECO, TCOMPRA, TCAIXA);
        InserirProduto (L, P);
        //MostrarProduto(P);
        //DestruirProduto(P);

    }
    Gravar (__FUNCTION__, R);
    fclose (F);
    return L;
}

//GRAVAR DADOS TXT
//GRAVAR OPERACOES EXECUTADAS NO PROGRAMA EM FICHEIRO TXT
void GravarTXT (char* operacao, Relogio* R)
{
    // Abre o arquivo em modo de acrescento ("a")
    FILE* F = fopen ("OPERACOES_EFETUADAS.txt", "a");

    if (!F)
    {
        printf ("Erro ao abrir o arquivo.\n");
        return;
    }

    // Obtem a data e hora atual
    time_t tempo_atual = VerTempoRelogio (R);
    struct tm* infoTempo = localtime (&tempo_atual);

    // Formata a data e hora
    char dataHora[20];
    strftime (dataHora, sizeof (dataHora), "%Y-%m-%d %H:%M:%S", infoTempo);

    // Verifica se o arquivo está vazio
    fseek (F, 0, SEEK_END);
    if (ftell (F) == 0)
    {
        // Grava o cabeçalho
        fprintf (F, "---------- OPERAÇÕES EFETUADAS AUTOMATICAMENTE ----------\n");
        fprintf (F, "Registro\tData e Hora\tOperação\n");
    }

    // Incrementa o número de registro
    static int numero_registro = 1;

    // Grava o conteúdo no arquivo
    fprintf (F, "%d\t%s\t%s\n", numero_registro, dataHora, operacao);

    // Incrementa o número de registro para o próximo
    numero_registro++;

    // Fecha o arquivo
    fclose (F);
}

//GRAVAR ESTATISTICAS DO SUPERMERCADO EM FICHEIRO TXT
void GravarEstatisticasTXT (SUPERMERCADO* S)
{
    FILE* F = fopen ("ESTATISTICAS.txt", "a");

    if (!F)
    {
        printf ("Erro ao abrir o arquivo.\n");
        return;
    }

    ELEMENTO_CAIXA* NO_caixa = S->caixas->Inicio;
    ESTATISTICAS* dados = S->dados_estatisticas;

    // Obtem a data e hora atual
    time_t tempo_atual = VerTempoRelogio (S->relogio_supermercado);
    struct tm* infoTempo = localtime (&tempo_atual);

    // Formata a data e hora
    char dataHora[20];
    strftime (dataHora, sizeof (dataHora), "%Y-%m-%d %H:%M:%S", infoTempo);

    // Verifica se o arquivo está vazio
    fseek (F, 0, SEEK_END);
    if (ftell (F) == 0)
    {
        // Grava o cabeçalho
        fprintf (F, "---- ESTATÍSTICAS DO %s----\n", S->nome_supermercado);
        fprintf (F, "%d\tATENDIDOS\n", dados->clientes_atendidos);
        fprintf (F, "%d\tCAIXAS\n", dados->caixas_abertas);
        fprintf (F, "%d\tPRODUTOS VENDIDOS\n", dados->produtos_vendidos);
        fprintf (F, "%d\tPRODUTOS OFERECIDOS\n", dados->produtos_oferecidos);
        fprintf (F, "%.2f\tFATURACAO\n", dados->faturacao_total);
        fprintf (F, "%.2f\tPERDAS\n", dados->perdas);
        fprintf (F, "%.2fmin\tTEMPO OPERACAO\n", (float)dados->tempo_operacao / 60);
    }
    else
    {
        fprintf (F, "\n");
        fprintf (F, "---- ESTATÍSTICAS DO %s----\n", S->nome_supermercado);
        fprintf (F, "%d\tATENDIDOS\n", dados->clientes_atendidos);
        fprintf (F, "%d\tCAIXAS\n", dados->caixas_abertas);
        fprintf (F, "%d\tPRODUTOS VENDIDOS\n", dados->produtos_vendidos);
        fprintf (F, "%d\tPRODUTOS OFERECIDOS\n", dados->produtos_oferecidos);
        fprintf (F, "%.2f\tFATURACAO\n", dados->faturacao_total);
        fprintf (F, "%.2f\tPERDAS\n", dados->perdas);
        fprintf (F, "%.2fmin\tTEMPO OPERACAO\n", (float)dados->tempo_operacao / 60);
    }

    // Grava o conteúdo no arquivo

    // Fecha o arquivo
    fclose (F);
}

//GRAVAR DADOS CSV
//GRAVAR OPERACOES EXECUTADAS NO PROGRAMA EM FICHEIRO CSV
void Gravar (char *operacao, Relogio *R)
{
    GravarTXT (operacao, R);
    // Abre o arquivo em modo de acrescento ("a")
    FILE* F = fopen ("OPERACOES_EFETUADAS.csv", "a");

    if (!F)
    {
        printf ("Erro ao abrir o arquivo.\n");
        return;
    }

    // Obtem a data e hora atual
    time_t tempo_atual = VerTempoRelogio (R);
    struct tm* infoTempo = localtime (&tempo_atual);

    // Formata a data e hora
    char dataHora[20];
    strftime (dataHora, sizeof (dataHora), "%Y-%m-%d %H:%M:%S", infoTempo);

    // Verifica se o arquivo está vazio
    fseek (F, 0, SEEK_END);
    if (ftell (F) == 0)
    {
        // Grava o cabeçalho
        fprintf (F, "---------- OPERAÇÕES EFETUADAS AUTOMATICAMENTE ----------\n");
        fprintf (F, "Registro; Data e Hora; Operacao \n");
    }

    // Incrementa o número de registro
    static int numero_registro = 1;

    // Grava o conteúdo no formato CSV
    fprintf (F, "%d; %s; %s\n", numero_registro, dataHora, operacao);

    // Fecha o arquivo
    fclose (F);

    // Incrementa o número de registro para o próximo
    numero_registro++;

    //printf ("\nConteudo gravado com sucesso.\n");
}

//GRAVAR ESTATISTICAS DO SUPERMERCADO EM FICHEIRO CSV
void GravarEstatisticas (SUPERMERCADO *S)
{
    GravarEstatisticasTXT (S);
    FILE* F = fopen ("ESTATISTICAS.csv", "a");

    if (!F)
    {
        printf ("Erro ao abrir o arquivo.\n");
        return;
    }

    ELEMENTO_CAIXA *NO_caixa = S->caixas->Inicio;
    ESTATISTICAS *dados = S->dados_estatisticas;

    // Obtem a data e hora atual
    time_t tempo_atual = VerTempoRelogio (S->relogio_supermercado);
    struct tm* infoTempo = localtime (&tempo_atual);

    // Formata a data e hora
    char dataHora[20];
    strftime (dataHora, sizeof (dataHora), "%Y-%m-%d %H:%M:%S", infoTempo);

    // Verifica se o arquivo está vazio
    fseek (F, 0, SEEK_END);
    if (ftell (F) == 0)
    {
        // Grava o cabeçalho
        fprintf (F, " ; ATENDEU; CAIXAS; PRODUTOS VENDIDOS; PRODUTOS OFERECIDOS; FATURACAO; PERDAS; TEMPO OPERACAO(min);\n");
        fprintf (F, "%s; %d; %d; %d; %d; %.2f; %.2f; %.2f;\n", S->nome_supermercado, dados->clientes_atendidos, dados->caixas_abertas, dados->produtos_vendidos, dados->produtos_oferecidos, dados->faturacao_total, dados->perdas, (float)dados->tempo_operacao / 60);
    }
    else
    {
        fprintf (F, "\n");
        fprintf (F, " ; ATENDEU; CAIXAS; PRODUTOS VENDIDOS; PRODUTOS OFERECIDOS; FATURACAO; PERDAS; TEMPO OPERACAO(min);\n");
        fprintf (F, "%s; %d; %d; %d; %d; %.2f; %.2f; %.2f;\n", S->nome_supermercado, dados->clientes_atendidos, dados->caixas_abertas, dados->produtos_vendidos, dados->produtos_oferecidos, dados->faturacao_total, dados->perdas, (float)dados->tempo_operacao / 60);

    }

    // Grava o conteúdo no formato CSV

    // Fecha o arquivo
    fclose (F);
}
