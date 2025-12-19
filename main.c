/*
 * ============================================================================
 * SISTEMA DE GERENCIAMENTO DE LIVROS PARA BIBLIOTECA
 * ============================================================================
 * Descrição: Sistema que permite cadastrar, pesquisar, ordenar e gerenciar
 *            um acervo de até 20 livros com persistência em arquivo.
 * Autor: Hugo Henrique de Matos Frozza
 * Data: 17/11/2025
 * ============================================================================
 */

#include <stdio.h>   // Biblioteca para entrada e saída (printf, scanf, FILE)
#include <string.h>  // Biblioteca para manipulação de strings (strcmp)
#include <stdlib.h>  // Biblioteca para funções utilitárias (system)

// ============================================================================
// CONSTANTES
// ============================================================================
#define TAMANHO_ACERVO 20          // Tamanho máximo do acervo de livros
#define NOME_ARQUIVO "acervo.dat"  // Nome do arquivo para persistência

// ============================================================================
// ESTRUTURA (STRUCT) DO LIVRO
// ============================================================================
/*
 * Estrutura que representa um livro na biblioteca.
 * Contém todos os dados necessários para identificar e catalogar um livro.
 */
struct Livro {
    int codigo;         // Código único de identificação do livro
    char titulo[50];    // Título do livro (máximo 49 caracteres)
    char autor[30];     // Nome do autor (máximo 29 caracteres)
    char area[30];      // Área/categoria do livro (máximo 29 caracteres)
    int ano;            // Ano de publicação
    char editora[30];   // Nome da editora (máximo 29 caracteres)
};

// ============================================================================
// PROTÓTIPOS DAS FUNÇÕES
// ============================================================================
void cadastrarLivros(struct Livro acervo[], int tamanho);
void imprimirLivros(struct Livro acervo[], int tamanho);
void pesquisarLivro(struct Livro acervo[], int tamanho, int codigoBusca);
void ordenarLivros(struct Livro acervo[], int tamanho);
void exibirMenu();
void limparBuffer();
int carregarAcervo(struct Livro acervo[], int tamanho);
void salvarAcervo(struct Livro acervo[], int tamanho);

// ============================================================================
// FUNÇÃO PRINCIPAL (MAIN)
// ============================================================================
/*
 * Função principal que controla o fluxo do programa.
 * Carrega os dados do arquivo ao iniciar e salva ao encerrar.
 */
int main() {
    // Declaração do vetor de structs para armazenar o acervo
    struct Livro acervo[TAMANHO_ACERVO];

    // Variável para armazenar a opção escolhida pelo usuário
    int opcao;

    // Variável para armazenar o código na pesquisa
    int codigoBusca;

    // Inicializa todos os livros com código 0 (indica posição vazia)
    int i;
    for (i = 0; i < TAMANHO_ACERVO; i++) {
        acervo[i].codigo = 0;
    }

    // ========================================================================
    // CARREGA OS DADOS DO ARQUIVO AO INICIAR O PROGRAMA
    // ========================================================================
    int livrosCarregados = carregarAcervo(acervo, TAMANHO_ACERVO);
    if (livrosCarregados > 0) {
        printf("\n*** %d livro(s) carregado(s) do arquivo! ***\n", livrosCarregados);
    } else {
        printf("\n*** Nenhum acervo anterior encontrado. Iniciando novo acervo. ***\n");
    }

    // Laço principal do programa - continua até o usuário escolher sair
    do {
        // Exibe o menu de opções
        exibirMenu();

        // Lê a opção escolhida pelo usuário
        printf("Digite sua opcao: ");
        scanf("%d", &opcao);
        limparBuffer();  // Limpa o buffer do teclado

        // Estrutura de seleção para executar a funcionalidade escolhida
        switch (opcao) {
            case 1:
                // Cadastrar livros
                printf("\n=== CADASTRO DE LIVROS ===\n");
                cadastrarLivros(acervo, TAMANHO_ACERVO);
                break;

            case 2:
                // Imprimir todos os livros
                printf("\n=== LISTA DE LIVROS CADASTRADOS ===\n");
                imprimirLivros(acervo, TAMANHO_ACERVO);
                break;

            case 3:
                // Pesquisar livro por código
                printf("\n=== PESQUISA DE LIVRO ===\n");
                printf("Digite o codigo do livro que deseja pesquisar: ");
                scanf("%d", &codigoBusca);
                limparBuffer();
                pesquisarLivro(acervo, TAMANHO_ACERVO, codigoBusca);
                break;

            case 4:
                // Ordenar livros por ano de publicação
                printf("\n=== ORDENACAO DE LIVROS ===\n");
                ordenarLivros(acervo, TAMANHO_ACERVO);
                printf("Livros ordenados por ano de publicacao com sucesso!\n");
                break;

            case 5:
                // ============================================================
                // SALVA OS DADOS NO ARQUIVO ANTES DE ENCERRAR
                // ============================================================
                printf("\n=== SALVANDO ACERVO ===\n");
                salvarAcervo(acervo, TAMANHO_ACERVO);
                printf("\nEncerrando o programa. Obrigado por usar o sistema!\n");
                break;

            default:
                // Opção inválida
                printf("\nOpcao invalida! Por favor, escolha uma opcao de 1 a 5.\n");
                break;
        }

        // Pausa para o usuário ler as mensagens
        if (opcao != 5) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }

    } while (opcao != 5);  // Continua enquanto não escolher sair

    return 0;  // Retorna 0 indicando execução bem-sucedida
}

// ============================================================================
// FUNÇÃO PARA CARREGAR O ACERVO DO ARQUIVO
// ============================================================================
/*
 * Carrega os livros de um arquivo binário para o vetor de structs.
 *
 * Parâmetros:
 *   - acervo[]: vetor de structs Livro onde os dados serão carregados
 *   - tamanho: tamanho máximo do vetor
 *
 * Retorno:
 *   - Número de livros carregados com sucesso
 *   - 0 se o arquivo não existir ou estiver vazio
 *
 * O arquivo é aberto em modo binário de leitura ("rb").
 * Cada struct é lida sequencialmente usando fread().
 */
int carregarAcervo(struct Livro acervo[], int tamanho) {
    FILE *arquivo;          // Ponteiro para o arquivo
    int quantidade = 0;     // Contador de livros carregados

    // Tenta abrir o arquivo em modo binário de leitura
    arquivo = fopen(NOME_ARQUIVO, "rb");

    // Verifica se o arquivo foi aberto com sucesso
    if (arquivo == NULL) {
        // Arquivo não existe - retorna 0 (nenhum livro carregado)
        return 0;
    }

    // Lê os livros do arquivo enquanto houver dados e espaço no vetor
    while (quantidade < tamanho &&
           fread(&acervo[quantidade], sizeof(struct Livro), 1, arquivo) == 1) {
        // Só conta como carregado se o código for válido (diferente de 0)
        if (acervo[quantidade].codigo != 0) {
            quantidade++;
        }
    }

    // Fecha o arquivo
    fclose(arquivo);

    return quantidade;  // Retorna a quantidade de livros carregados
}

// ============================================================================
// FUNÇÃO PARA SALVAR O ACERVO NO ARQUIVO
// ============================================================================
/*
 * Salva os livros do vetor de structs em um arquivo binário.
 *
 * Parâmetros:
 *   - acervo[]: vetor de structs Livro com os dados a serem salvos
 *   - tamanho: tamanho do vetor
 *
 * O arquivo é aberto em modo binário de escrita ("wb").
 * Isso sobrescreve qualquer arquivo existente com o mesmo nome.
 * Cada struct é escrita sequencialmente usando fwrite().
 */
void salvarAcervo(struct Livro acervo[], int tamanho) {
    FILE *arquivo;          // Ponteiro para o arquivo
    int i;                  // Variável de controle do laço
    int quantidade = 0;     // Contador de livros salvos

    // Abre o arquivo em modo binário de escrita (cria ou sobrescreve)
    arquivo = fopen(NOME_ARQUIVO, "wb");

    // Verifica se o arquivo foi criado/aberto com sucesso
    if (arquivo == NULL) {
        printf("ERRO: Nao foi possivel criar o arquivo para salvar o acervo!\n");
        return;
    }

    // Percorre o vetor e salva apenas os livros com código válido
    for (i = 0; i < tamanho; i++) {
        if (acervo[i].codigo != 0) {
            // Escreve a struct no arquivo
            fwrite(&acervo[i], sizeof(struct Livro), 1, arquivo);
            quantidade++;
        }
    }

    // Fecha o arquivo
    fclose(arquivo);

    printf("Acervo salvo com sucesso! %d livro(s) gravado(s) no arquivo '%s'.\n",
           quantidade, NOME_ARQUIVO);
}

// ============================================================================
// FUNÇÃO PARA EXIBIR O MENU
// ============================================================================
/*
 * Exibe o menu principal com todas as opções disponíveis.
 * Não recebe parâmetros e não retorna valores.
 */
void exibirMenu() {
    printf("\n");
    printf("========================================\n");
    printf("   SISTEMA DE GERENCIAMENTO DE LIVROS   \n");
    printf("========================================\n");
    printf("1 - Cadastrar livros\n");
    printf("2 - Imprimir todos os livros\n");
    printf("3 - Pesquisar livro por codigo\n");
    printf("4 - Ordenar livros por ano de publicacao\n");
    printf("5 - Sair do programa (salva automaticamente)\n");
    printf("========================================\n");
}

// ============================================================================
// FUNÇÃO PARA LIMPAR O BUFFER DO TECLADO
// ============================================================================
/*
 * Limpa o buffer de entrada do teclado.
 * Isso evita problemas com caracteres residuais após usar scanf.
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ============================================================================
// FUNÇÃO PARA CADASTRAR LIVROS
// ============================================================================
/*
 * Cadastra livros no acervo.
 *
 * Parâmetros:
 *   - acervo[]: vetor de structs Livro onde os dados serão armazenados
 *   - tamanho: tamanho máximo do vetor
 *
 * A função pergunta quantos livros deseja cadastrar e solicita os dados
 * de cada livro, armazenando-os nas posições disponíveis do vetor.
 */
void cadastrarLivros(struct Livro acervo[], int tamanho) {
    int quantidade;      // Quantidade de livros a cadastrar
    int cadastrados = 0; // Contador de livros já cadastrados
    int i;               // Variável de controle do laço

    // Conta quantos livros já estão cadastrados
    for (i = 0; i < tamanho; i++) {
        if (acervo[i].codigo != 0) {
            cadastrados++;
        }
    }

    // Verifica se há espaço disponível
    if (cadastrados >= tamanho) {
        printf("O acervo esta cheio! Nao e possivel cadastrar mais livros.\n");
        return;
    }

    // Solicita a quantidade de livros a cadastrar
    printf("Quantos livros deseja cadastrar? (Maximo: %d): ", tamanho - cadastrados);
    scanf("%d", &quantidade);
    limparBuffer();

    // Valida a quantidade informada
    if (quantidade <= 0 || quantidade > (tamanho - cadastrados)) {
        printf("Quantidade invalida!\n");
        return;
    }

    // Encontra a primeira posição livre e cadastra os livros
    int livrosCadastrados = 0;
    for (i = 0; i < tamanho && livrosCadastrados < quantidade; i++) {
        // Verifica se a posição está vazia (código = 0)
        if (acervo[i].codigo == 0) {
            printf("\n--- Livro %d de %d ---\n", livrosCadastrados + 1, quantidade);

            // Solicita o código do livro
            printf("Codigo: ");
            scanf("%d", &acervo[i].codigo);
            limparBuffer();

            // Solicita o título do livro
            printf("Titulo: ");
            fgets(acervo[i].titulo, 50, stdin);
            // Remove o caractere de nova linha do final
            acervo[i].titulo[strcspn(acervo[i].titulo, "\n")] = 0;

            // Solicita o autor do livro
            printf("Autor: ");
            fgets(acervo[i].autor, 30, stdin);
            acervo[i].autor[strcspn(acervo[i].autor, "\n")] = 0;

            // Solicita a área/categoria do livro
            printf("Area: ");
            fgets(acervo[i].area, 30, stdin);
            acervo[i].area[strcspn(acervo[i].area, "\n")] = 0;

            // Solicita o ano de publicação
            printf("Ano de publicacao: ");
            scanf("%d", &acervo[i].ano);
            limparBuffer();

            // Solicita a editora do livro
            printf("Editora: ");
            fgets(acervo[i].editora, 30, stdin);
            acervo[i].editora[strcspn(acervo[i].editora, "\n")] = 0;

            livrosCadastrados++;
            printf("Livro cadastrado com sucesso!\n");
        }
    }

    printf("\nTotal de %d livro(s) cadastrado(s)!\n", livrosCadastrados);
}

// ============================================================================
// FUNÇÃO PARA IMPRIMIR TODOS OS LIVROS
// ============================================================================
/*
 * Exibe todos os livros cadastrados no acervo.
 *
 * Parâmetros:
 *   - acervo[]: vetor de structs Livro com os dados
 *   - tamanho: tamanho do vetor
 *
 * Percorre o vetor e exibe os dados de cada livro que possui código diferente de 0.
 */
void imprimirLivros(struct Livro acervo[], int tamanho) {
    int i;                  // Variável de controle do laço
    int encontrados = 0;    // Contador de livros encontrados

    // Percorre todo o vetor de livros
    for (i = 0; i < tamanho; i++) {
        // Verifica se a posição contém um livro cadastrado
        if (acervo[i].codigo != 0) {
            printf("\n----------------------------------------\n");
            printf("Codigo: %d\n", acervo[i].codigo);
            printf("Titulo: %s\n", acervo[i].titulo);
            printf("Autor: %s\n", acervo[i].autor);
            printf("Area: %s\n", acervo[i].area);
            printf("Ano: %d\n", acervo[i].ano);
            printf("Editora: %s\n", acervo[i].editora);
            printf("----------------------------------------\n");
            encontrados++;
        }
    }

    // Verifica se algum livro foi encontrado
    if (encontrados == 0) {
        printf("Nenhum livro cadastrado no acervo.\n");
    } else {
        printf("\nTotal de livros no acervo: %d\n", encontrados);
    }
}

// ============================================================================
// FUNÇÃO PARA PESQUISAR LIVRO POR CÓDIGO
// ============================================================================
/*
 * Pesquisa um livro pelo seu código de identificação.
 *
 * Parâmetros:
 *   - acervo[]: vetor de structs Livro onde será feita a pesquisa
 *   - tamanho: tamanho do vetor
 *   - codigoBusca: código do livro a ser pesquisado
 *
 * Utiliza um laço while para percorrer o vetor até encontrar o livro
 * ou chegar ao final do vetor.
 */
void pesquisarLivro(struct Livro acervo[], int tamanho, int codigoBusca) {
    int i = 0;              // Índice para percorrer o vetor
    int encontrado = 0;     // Flag que indica se o livro foi encontrado

    // Percorre o vetor usando while até encontrar ou chegar ao final
    while (i < tamanho && !encontrado) {
        // Verifica se o código corresponde ao procurado
        if (acervo[i].codigo == codigoBusca) {
            encontrado = 1;  // Marca que encontrou

            // Exibe os dados do livro encontrado
            printf("\n=== LIVRO ENCONTRADO ===\n");
            printf("----------------------------------------\n");
            printf("Codigo: %d\n", acervo[i].codigo);
            printf("Titulo: %s\n", acervo[i].titulo);
            printf("Autor: %s\n", acervo[i].autor);
            printf("Area: %s\n", acervo[i].area);
            printf("Ano: %d\n", acervo[i].ano);
            printf("Editora: %s\n", acervo[i].editora);
            printf("----------------------------------------\n");
        }
        i++;  // Avança para o próximo elemento
    }

    // Se não encontrou, exibe mensagem
    if (!encontrado) {
        printf("\nLivro com codigo %d NAO ENCONTRADO no acervo.\n", codigoBusca);
    }
}

// ============================================================================
// FUNÇÃO PARA ORDENAR LIVROS POR ANO (BUBBLE SORT)
// ============================================================================
/*
 * Ordena os livros por ano de publicação usando o algoritmo Bubble Sort.
 *
 * Parâmetros:
 *   - acervo[]: vetor de structs Livro a ser ordenado
 *   - tamanho: tamanho do vetor
 *
 * O algoritmo Bubble Sort compara elementos adjacentes e troca-os se estiverem
 * na ordem errada. O processo se repete até que nenhuma troca seja necessária.
 *
 * Complexidade: O(n²) no pior caso
 */
void ordenarLivros(struct Livro acervo[], int tamanho) {
    int i, j;                    // Variáveis de controle dos laços
    struct Livro temp;           // Variável temporária para troca
    int trocou;                  // Flag para otimização do algoritmo

    // Conta quantos livros válidos existem
    int numLivros = 0;
    for (i = 0; i < tamanho; i++) {
        if (acervo[i].codigo != 0) {
            numLivros++;
        }
    }

    // Verifica se há livros para ordenar
    if (numLivros <= 1) {
        printf("Nao ha livros suficientes para ordenar.\n");
        return;
    }

    // Algoritmo Bubble Sort
    // Laço externo: controla o número de passagens
    for (i = 0; i < tamanho - 1; i++) {
        trocou = 0;  // Reinicia a flag de troca

        // Laço interno: compara elementos adjacentes
        for (j = 0; j < tamanho - 1 - i; j++) {
            // Só compara se ambas as posições tiverem livros cadastrados
            if (acervo[j].codigo != 0 && acervo[j + 1].codigo != 0) {
                // Compara os anos de publicação
                // Se o ano atual for maior que o próximo, troca (ordem crescente)
                if (acervo[j].ano > acervo[j + 1].ano) {
                    // Realiza a troca usando variável temporária
                    temp = acervo[j];
                    acervo[j] = acervo[j + 1];
                    acervo[j + 1] = temp;
                    trocou = 1;  // Marca que houve troca
                }
            }
        }

        // Otimização: se não houve troca, o vetor já está ordenado
        if (!trocou) {
            break;
        }
    }
}