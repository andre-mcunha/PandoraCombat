#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HISTORICO 20
#define MAX_ESTAMINA 1000
#define MIN_ESTAMINA 0
#define MAX_HEALTH 1000
#define MIN_COMBO_STAMINA 750

// Estrutura para armazenar a pontuação dos jogadores
typedef struct Pontuacao
{
  int vida;
  int estamina;
  char jogador[100];
  char histAtt[1000];
  struct Pontuacao *next;
} Pontuacao;

// Função para criar um novo nó
Pontuacao *criar_novo_no(int vida, int estamina, char *jogador, char *histAtt)
{
  Pontuacao *novo_no = (Pontuacao *)malloc(sizeof(Pontuacao));
  if (novo_no == NULL)
  {
    printf("Erro ao alocar memória\n");
    exit(1);
  }
  novo_no->vida = vida;
  novo_no->estamina = estamina;
  strcpy(novo_no->jogador, jogador);
  strcpy(novo_no->histAtt, histAtt);
  novo_no->next = NULL;
  return novo_no;
}

// Função para inserir um novo nó no final da lista ligada
void inserir_pontuacao(Pontuacao **cabeca, int vida, int estamina, char *jogador, char *histAtt)
{

  Pontuacao *novo_no = criar_novo_no(vida, estamina, jogador, histAtt);

  // Se a lista estiver vazia, o novo nó será a cabeça da lista
  if (*cabeca == NULL)
  {
    *cabeca = novo_no;
  }
  else
  {
    Pontuacao *temp = *cabeca;
    // Percorre até o final da lista
    while (temp->next != NULL)
    {
      temp = temp->next;
    }
    // Insere o novo nó no final
    temp->next = novo_no;
  }
}

// Função para obter o tamanho da lista ligada
int obter_tamanho_lista(Pontuacao *cabeca)
{
  int tamanho = 0;
  Pontuacao *temp = cabeca;
  while (temp != NULL)
  {
    tamanho++;
    temp = temp->next;
  }
  return tamanho;
}

// Função para verificar se a lista está vazia
int lista_vazia(Pontuacao *cabeca)
{
  if (cabeca == NULL)
  {
    return 1;
  }
  return 0;
}

// Função para obter ultimo elemento da lista
Pontuacao *ultimo_elemento(Pontuacao *cabeca)
{
  Pontuacao *temp = cabeca;
  if (cabeca == NULL)
  {
    return NULL; // A lista está vazia
  }

  // Percorre a lista até encontrar o último elemento
  while (temp->next != NULL)
  {
    temp = temp->next;
  }

  return temp;
}

// Função para mostrar os itens da lista
void mostrar_lista(Pontuacao *cabeca)
{

  while (!lista_vazia(cabeca))
  {
    printf("Jogador: %s, Vida: %d, Estamina: %d, Historico: %s\n",
           cabeca->jogador, cabeca->vida, cabeca->estamina, cabeca->histAtt);
    cabeca = (Pontuacao *)cabeca->next;
  }
}

void remove_x(Pontuacao **cabeca, int x)
{
  int tamanho = obter_tamanho_lista(*cabeca);
  // Se x for maior ou igual ao tamanho da lista, remove todos os nós exceto a cabeça
  if (x >= tamanho - 2)
  {
    Pontuacao *current, *next;

    current = (*cabeca)->next;

    while (current != NULL)
    {
      next = current->next;
      free(current);
      current = next;
    }

    // Manter apenas a cabeça e definir o próximo nó como NULL
    (*cabeca)->next = NULL;
  }
  else
  {
    int nodesToKeep = tamanho - x - 1;
    Pontuacao *current = *cabeca;
    Pontuacao *prev = NULL;
    int count = 0;

    // Encontra o nó antes do primeiro a ser removido
    while (count < nodesToKeep)
    {
      prev = current;
      current = current->next;
      count++;
    }

    // Libera os nós a partir do nó encontrado
    while (current != NULL)
    {
      Pontuacao *next = current->next;
      free(current);
      current = next;
    }

    // Define o próximo do nó anterior como NULL
    if (prev != NULL)
    {
      prev->next = NULL;
    }
    else
    {
      (*cabeca) = NULL; // Se prev for NULL, significa que a lista foi totalmente removida exceto a cabeça
    }
  }
}

// Função para liberar a memória alocada para a lista
void liberar_lista(Pontuacao *cabeca)
{
  Pontuacao *temp;
  while (cabeca != NULL)
  {
    temp = cabeca;
    cabeca = cabeca->next;
    free(temp);
  }
}

// Função para calcular multiplicador
int calcular_multiplicador(int estamina)
{
  if (estamina > 750)
  {
    return 1;
  }
  else if (estamina > 500)
  {
    return 2;
  }
  else if (estamina > 250)
  {
    return 3;
  }
  else
  {
    return 4;
  }
}

// Função para mostrar a pontuação em uma posição específica da lista
void mostrar_pontuacao(Pontuacao *cabeca, int posicao)
{
  Pontuacao *temp = cabeca;

  if (lista_vazia(cabeca))
  {
    printf("A lista está vazia.\n");
    return;
  }

  for (int i = 1; i < posicao; i++)
  {
    if (temp != NULL)
    {
      temp = temp->next;
    }
    else
    {
      printf("Posição %d fora do alcance da lista.\n", posicao);
      return;
    }
  }

  if (temp != NULL)
  {
    printf("%s [%d|%d] (x%d)\n", temp->jogador, temp->vida, temp->estamina,
           calcular_multiplicador(temp->estamina));
  }
  else
  {
    printf("Posição %d fora do alcance da lista.\n", posicao);
  }
}

// Função para mostrar o histórico de ataques
void mostrar_historico(Pontuacao *cabeca)
{
  // Declaração de variáveis
  char historico[500] = {'\0'};
  char resultado[500] = {'\0'};
  Pontuacao *temp = ultimo_elemento(cabeca);
  int tamanhoComVirgula, count, tamanho;

  count = 0;

  // Obter histAtt do último nó
  strcpy(historico, temp->histAtt);

  // Iterar sobre o histAtt a partir do fim da string e adicionar os caracteres encontrar 20 ',' ou chegar ao início da string
  tamanhoComVirgula = strlen(historico) - 1;

  for (int i = tamanhoComVirgula; i >= 0 && count <= 20; i--)
  {
    if (historico[i] == ',')
    {
      count++;
    }
    else
    {
      strncat(resultado, &historico[i], 1);
    }
  }

  // Inverter a string resultado
  tamanho = strlen(resultado);
  for (int i = 0; i < tamanho / 2; i++)
  {
    char temp = resultado[i];
    resultado[i] = resultado[tamanho - i - 1];
    resultado[tamanho - i - 1] = temp;
  }

  // Imprimir o histórico de ataques
  printf("H: %s\n", resultado);
}

// Função para criar tabela de combos
void criar_tabela_combos(int combos[12][12])
{
  int i, j;
  int valores[12][12] = {
      {0, 4, 8, 12, 16, -20, -16, 12, -8, -4, 0, 20},
      {-4, 0, 4, 8, 12, 16, -20, -16, -12, 16, 0, 20},
      {-8, -4, 0, 4, 8, 12, 16, -20, -16, 12, 0, 20},
      {-12, -8, -4, 0, 4, -8, 12, 16, 20, -16, 0, 20},
      {-16, -12, -8, -4, 0, 4, -8, 12, 16, 20, 0, 20},
      {20, -16, -12, 8, -4, 0, 4, 8, 12, -16, 0, 20},
      {16, 20, -16, -12, 8, -4, 0, -8, -4, -8, 0, 20},
      {-12, 16, 20, -16, -12, -8, 8, 0, 16, -20, 0, 20},
      {8, 12, 16, -20, -16, -12, 4, -16, 0, 20, 0, 20},
      {4, -16, -12, 16, -20, 16, 8, 20, -20, 0, 0, 20},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {-20, -20, -20, -20, -20, -20, -20, -20, -20, -20, 0, 0}};
  for (i = 0; i < 12; i++)
  {
    for (j = 0; j < 12; j++)
    {
      combos[i][j] = valores[i][j];
    }
  }
}

int jogador_morto(Pontuacao *jogador1, Pontuacao *jogador2)
{

  if (jogador1->vida <= 0 || jogador2->vida <= 0)
  {
    if (jogador1->vida <= 0)
    {
      printf("Jogador 2 venceu o jogo!\n");
      return 1;
    }
    else
    {
      printf("Jogador 1 venceu o jogo!\n");
      return 1;
    }
  }
  else
  {
    return 0;
  }
}

// Função para verificar se o caracter corresponde a um ataque
int verificar_caracter_permitido(char c)
{
  char caracteres_permitidos[] = "ZPAETRCBOMD";
  // Verifica se o caractere está na lista de caracteres permitidos
  return strchr(caracteres_permitidos, c) != NULL;
}

void removerEspacosFinais(char *str)
{
  int length = strlen(str);
  int i = length - 1;

  // Percorre a string da direita para a esquerda
  while (i >= 0 && isspace(str[i]))
  {
    str[i] = '\0'; // Substitui espaços por terminadores nulos
    i--;
  }
}

// Função para verificar se uma string é igual a "LUCIOREVERSALX" com X sendo um número de até dois dígitos
int isLUCIOREVERSALX(const char *str, int *x)
{
  int target_len, str_len, start_index;
  const char *target = "TARZANTABORDA";

  target_len = strlen(target);

  // Verifica se a string tem o tamanho adequado
  str_len = strlen(str);
  if (str_len < target_len + 1 ||
      str_len > target_len + 2)
  { // Tamanho deve ser prefixo + 1 ou 2 dígitos
    return 0;
  }

  // Verifica se os primeiros caracteres correspondem ao prefixo
  for (int i = 0; i < target_len; ++i)
  {
    if (str[i] != target[i])
    {
      return 0; // Encontrou um caractere que não corresponde
    }
  }

  // Verifica se os últimos caracteres são dígitos
  start_index = target_len;
  // num_len = str_len - target_len;
  for (int i = start_index; i < str_len; ++i)
  {
    if (!isdigit(str[i]))
    {
      return 0; // Encontrou um caractere que não é um dígito
    }
  }

  // Converte os últimos caracteres para inteiro
  *x = atoi(str + start_index); // converte a substring para inteiro a começar do índice start_index

  return 1; // É uma string válida "LUCIOREVERSALX"
}

// Função para ler os ataques de um jogador
int ler_ataques(char *ataques, Pontuacao *jogador, int *lucioOk, int *cheatCodeOk)
{
  int tamanho;

  if (fgets(ataques, 50, stdin) != NULL)
  {
    // Remove o caractere de nova linha, se presente
    ataques[strcspn(ataques, "\n")] = '\0';
    removerEspacosFinais(ataques);
    tamanho = strlen(ataques);

    if (ataques[0] == '\n' || ataques[0] == '\0')
    {
      return 1; // limpa listas e termina programa
    }

    // Verifica se a string contém espaços
    for (int i = 0; i < tamanho; i++)
    {
      if (isspace((unsigned char)ataques[i]))
      {
        return 1; // limpa listas e termina programa
      }
    }

    if (tamanho <= 4)
    {
      for (int i = 0; ataques[i] != '\0'; i++)
      {
        // Verifica se é uma letra maiúscula
        if (!isupper((unsigned char)ataques[i]))
        {
          return 1; // limpa listas e termina programa
        }
        if (!verificar_caracter_permitido(ataques[i]))
        {
          return 1; // limpa listas e termina programa
        }
      }
    }
    else
    {
      if (strncmp(ataques, "Modo-Jesus", 10) == 0 ||
          strncmp(ataques, "Alt-F4", 6) == 0 ||
          strncmp(ataques, "Kebab", 5) == 0 ||
          strncmp(ataques, "Hiroshima", 9) == 0 ||
          strncmp(ataques, "Nood-Mode", 9) == 0)
      {
        *cheatCodeOk = 1;
        return 2; // Identificou Cheat Code
      }

      if (isLUCIOREVERSALX(ataques, lucioOk) == 1)
      {
        if (jogador->estamina >= 500 && jogador->estamina <= 900)
        {
          return 2;
        }
        else
        {
          printf("Estamina insuficiente\n");
          *lucioOk = 0;
          return 2;
        }
      }

      if (strcmp(ataques, "DADBAD") == 0 ||
          strcmp(ataques, "STTEACC") == 0 ||
          strcmp(ataques, "TATAPAAA") == 0 ||
          strcmp(ataques, "ARROZAO") == 0)
      {
        if (jogador->estamina < MIN_COMBO_STAMINA)
        {
          printf("Estamina insuficiente\n");
          return 2;
        }
      }
      else
      {
        return 1; // limpa listas e termina programa
      }
    }
  }
  else
  {
    return 1; // limpa listas e termina programa
  }
  return 0;
}

// Função para converter ataques em números
int converter_ataque(char ataque)
{
  switch (ataque)
  {
  case 'Z':
    return 0;
  case 'P':
    return 1;
  case 'A':
    return 2;
  case 'E':
    return 3;
  case 'T':
    return 4;
  case 'R':
    return 5;
  case 'C':
    return 6;
  case 'B':
    return 7;
  case 'O':
    return 8;
  case 'M':
    return 9;
  case 'D':
    return 10;
  case ' ':
    return 11;
  default:
    return -1; // caracter inválido
  }
}

// Aplicar dano Simples
void aplicar_dano_simples(char *ataquesJ1, char *ataquesJ2, Pontuacao *jogador1, Pontuacao *jogador2, int combos[12][12])
{

  int linha, coluna, resultado, multiplicadorJ1, multiplicadorJ2, tamanho,
      vidaJ1, vidaJ2, staminaJ1, staminaJ2;

  vidaJ1 = 0;
  staminaJ1 = 0;
  vidaJ2 = 0;
  staminaJ2 = 0;

  tamanho = strlen(ataquesJ1);
  multiplicadorJ1 = calcular_multiplicador(jogador1->estamina);
  multiplicadorJ2 = calcular_multiplicador(jogador2->estamina);

  for (int i = 0; i < tamanho; i++)
  {
    linha = converter_ataque(ataquesJ1[i]);
    coluna = converter_ataque(ataquesJ2[i]);
    resultado = combos[linha][coluna];

    if (linha <= 9 && coluna <= 9)
    {
      if (resultado > 0)
      {
        // Jogador 1 causa dano no jogador 2
        staminaJ1 -= 25;
        staminaJ2 -= 25;
        vidaJ2 -= (resultado * multiplicadorJ2);
      }
      else if (resultado < 0)
      {
        // Jogador 2 causa dano no jogador 1
        staminaJ1 -= 25;
        staminaJ2 -= 25;
        vidaJ1 += (resultado * multiplicadorJ1); // += pois o valor é negativo
      }
      else
      {
        staminaJ1 -= 25;
        staminaJ2 -= 25;
      }
    }
    else
    {
      if (resultado > 0)
      {
        // Jogador 1 causa dano no jogador 2
        vidaJ2 -= (resultado * multiplicadorJ2);
        staminaJ2 += 25;
        staminaJ1 -= 25;
      }
      else if (resultado < 0)
      {
        // Jogador 2 causa dano no jogador 1
        vidaJ1 += (resultado * multiplicadorJ1);
        staminaJ1 += 25;
        staminaJ2 -= 25;
      }
      else if (resultado == 0 && linha <= 9)
      {
        staminaJ2 -= 10;
        vidaJ2 += multiplicadorJ2 * 10;
        staminaJ1 -= 25;
      }
      else if (resultado == 0 && coluna <= 9)
      {
        staminaJ1 -= 10;
        vidaJ1 += multiplicadorJ1 * 10;
        staminaJ2 -= 25;
      }
      else
      {
        if (linha == 10 && coluna == 10)
        {
          staminaJ1 -= 10;
          staminaJ2 -= 10;
          vidaJ1 += multiplicadorJ1 * 10;
          vidaJ2 += multiplicadorJ2 * 10;
        }

        if (linha == 10 && coluna == 11)
        {
          staminaJ1 -= 10;
          vidaJ1 += multiplicadorJ1 * 10;
          staminaJ2 += 25;
        }

        if (linha == 11 && coluna == 10)
        {
          staminaJ1 += 25;
          staminaJ2 -= 10;
          vidaJ2 += multiplicadorJ2 * 10;
        }

        if (linha == 11 && coluna == 11)
        {
          staminaJ1 += 25;
          staminaJ2 += 25;
        }
      }
    }

    // Imprime mensagem de debug para saber que contas estão a ser feitas
    //printf("[%c,%c]=%d \n", ataquesJ1[i], ataquesJ2[i], resultado);
    //printf("VidaJ1: %d, VidaJ2: %d, StaminaJ1: %d, StaminaJ2: %d\n", vidaJ1, vidaJ2, staminaJ1, staminaJ2);

    jogador1->vida = jogador1->vida + vidaJ1 > MAX_HEALTH
                         ? MAX_HEALTH
                         : jogador1->vida + vidaJ1;
    jogador2->vida = jogador2->vida + vidaJ2 > MAX_HEALTH
                         ? MAX_HEALTH
                         : jogador2->vida + vidaJ2;
    jogador1->estamina = jogador1->estamina + staminaJ1 < MIN_ESTAMINA
                             ? MIN_ESTAMINA
                             : (jogador1->estamina + staminaJ1 > MAX_ESTAMINA
                                    ? MAX_ESTAMINA
                                    : jogador1->estamina + staminaJ1);
    jogador2->estamina = jogador2->estamina + staminaJ2 < MIN_ESTAMINA
                             ? MIN_ESTAMINA
                             : (jogador2->estamina + staminaJ2 > MAX_ESTAMINA
                                    ? MAX_ESTAMINA
                                    : jogador2->estamina + staminaJ2);

    // Atualiza o historico e adiciona uma virgula entre os ataques
    inserir_pontuacao(&jogador1, jogador1->vida, jogador1->estamina, "P#1", strncat(jogador1->histAtt, &ataquesJ1[i], 1));
    inserir_pontuacao(&jogador2, jogador2->vida, jogador2->estamina, "P#2", strncat(jogador2->histAtt, &ataquesJ2[i], 1));
    jogador1 = jogador1->next;
    jogador2 = jogador2->next;
    strcat(jogador1->histAtt, ",");
    strcat(jogador2->histAtt, ",");

    // Resetar variáveis
    vidaJ1 = 0;
    staminaJ1 = 0;
    vidaJ2 = 0;
    staminaJ2 = 0;
  }
  //printf("\n");
}

// Aplicar dano de combos
void aplicar_dano_combo(char *ataquesJ1, char *ataquesJ2, Pontuacao *jogador1, Pontuacao *jogador2)
{
  ;

  if (strcmp(ataquesJ1, "ARROZAO") == 0)
  {
    jogador1->estamina -= 500;
    jogador2->vida -= 500;
  }

  if (strcmp(ataquesJ2, "ARROZAO") == 0)
  {
    jogador2->estamina -= 500;
    jogador1->vida -= 500;
  }

  if (strcmp(ataquesJ1, "DADBAD") == 0)
  {
    jogador1->estamina -= 400;
    jogador2->vida -= 400;
  }

  if (strcmp(ataquesJ2, "DADBAD") == 0)
  {
    jogador2->estamina -= 400;
    jogador1->vida -= 400;
  }

  if (strcmp(ataquesJ1, "STTEACC") == 0)
  {
    jogador1->estamina -= 300;
    jogador2->vida -= 300;
  }

  if (strcmp(ataquesJ2, "STTEACC") == 0)
  {
    jogador2->estamina -= 300;
    jogador1->vida -= 300;
  }

  if (strcmp(ataquesJ1, "TATAPAAA") == 0)
  {
    jogador1->estamina -= 200;
    jogador2->vida -= 200;
  }

  if (strcmp(ataquesJ2, "TATAPAAA") == 0)
  {
    jogador2->estamina -= 200;
    jogador1->vida -= 200;
  }

  if (strcmp(ataquesJ1, " ") == 0)
  {
    jogador1->estamina = jogador1->estamina + 25 > MAX_ESTAMINA
                             ? MAX_ESTAMINA
                             : jogador1->estamina + 25;
  }

  if (strcmp(ataquesJ2, " ") == 0)
  {
    jogador2->estamina = jogador2->estamina + 25 > MAX_ESTAMINA
                             ? MAX_ESTAMINA
                             : jogador2->estamina + 25;
  }

  inserir_pontuacao(&jogador1, jogador1->vida, jogador1->estamina, "P#1", strcat(jogador1->histAtt, ataquesJ1));
  inserir_pontuacao(&jogador2, jogador2->vida, jogador2->estamina, "P#2", strcat(jogador2->histAtt, ataquesJ2));
  jogador1 = jogador1->next;
  jogador2 = jogador2->next;

  // Adiciona uma virgula depois de cada ataque
  strcat(jogador1->histAtt, ",");
  strcat(jogador2->histAtt, ",");
}

// Função para calcular o dano de um ataque e atualizar stats Jogadores
int aplicar_dano(char *ataquesJ1, char *ataquesJ2, Pontuacao *jogador1, Pontuacao *jogador2, int combos[12][12])
{
  int tamanhoAttJ1, tamanhoAttJ2;

  tamanhoAttJ1 = strlen(ataquesJ1);
  tamanhoAttJ2 = strlen(ataquesJ2);

  if (tamanhoAttJ1 <= 4 && tamanhoAttJ2 <= 4)
  {
    aplicar_dano_simples(ataquesJ1, ataquesJ2, jogador1, jogador2, combos);
  }
  else
  {
    aplicar_dano_combo(ataquesJ1, ataquesJ2, jogador1, jogador2);
  }

  return jogador_morto(ultimo_elemento(jogador1), ultimo_elemento(jogador2));
}

// Funçao para escrever combinacao de ataques simples
void print_att_simples(char *ataquesJ1, char *ataquesJ2)
{

  int tamanhoAttJ1 = strlen(ataquesJ1);
  int tamanhoAttJ2 = strlen(ataquesJ2);

  int maior = tamanhoAttJ1 > tamanhoAttJ2 ? tamanhoAttJ1 : tamanhoAttJ2;
  int menor = tamanhoAttJ1 < tamanhoAttJ2 ? tamanhoAttJ1 : tamanhoAttJ2;

  // Preenche com ' ' os espacos em falta
  for (int i = menor; i <= maior; i++)
  {
    if (tamanhoAttJ1 > tamanhoAttJ2)
    {
      if (i == maior)
      {
        ataquesJ2[i] = '\0';
      }
      else
      {
        ataquesJ2[i] = ' ';
      }
    }
    else if (tamanhoAttJ2 > tamanhoAttJ1)
    {
      if (i == maior)
      {
        ataquesJ1[i] = '\0';
      }
      else
      {
        ataquesJ1[i] = ' ';
      }
    }
  }

  for (int i = 0; i < maior; i++)
  {
    printf("[%c,%c]", ataquesJ1[i], ataquesJ2[i]);
  }
  printf("\n");
}

// Funçao para escrever combinacao de ataques especiais
void print_att_combo(char *ataquesJ1, char *ataquesJ2)
{

  int tamanhoAttJ1 = strlen(ataquesJ1);
  int tamanhoAttJ2 = strlen(ataquesJ2);

  if (tamanhoAttJ1 > 4 && tamanhoAttJ2 > 4)
  {
    printf("[%s,%s]\n", ataquesJ1, ataquesJ2);
  }
  else
  {
    if (tamanhoAttJ1 <= 4)
    {
      strcpy(ataquesJ1, " ");
    }
    else
    {
      strcpy(ataquesJ2, " ");
    }
    printf("[%s,%s]\n", ataquesJ1, ataquesJ2);
  }
}

// Função para escrever a combinação de ataques
void print_ataques(char *ataquesJ1, char *ataquesJ2)
{

  int tamanhoAttJ1 = strlen(ataquesJ1);
  int tamanhoAttJ2 = strlen(ataquesJ2);

  int maior = tamanhoAttJ1 > tamanhoAttJ2 ? tamanhoAttJ1 : tamanhoAttJ2;

  if (maior >= 5)
  {
    // Tem COMBO
    print_att_combo(ataquesJ1, ataquesJ2);
  }
  else
  {
    print_att_simples(ataquesJ1, ataquesJ2);
  }
}

// Função para atualizar o histórico
void atualizar_historico(char *historico, const char *ataques)
{

  int tamanho = strlen(ataques);
  if (tamanho <= 4)
  {
    // adiciona uma virgula após cada caracter
    for (int i = 0; i < tamanho; i++)
    {
      strncat(historico, &ataques[i], 1);
      if (i < tamanho - 1)
      {
        strcat(historico, ",");
      }
    }
    strcat(historico, ",");
  }
  else
  {
    strcat(historico, ataques);
    // adiciona uma virgula entre os ataques
    strcat(historico, ",");
  }

  // imprime o historico para debug
  // printf("Historico: %s\n", historico);
}

// Função para aplicar cheat code
void aplicar_cheat_code(Pontuacao *cabeca1, Pontuacao *cabeca2, char *ataques)
{

  int x;

  if (strncmp(ataques, "Modo-Jesus", 10) == 0)
  {
    // Apaga todos os nós exceto a cabeça
    remove_x(&cabeca1, 10000);
    remove_x(&cabeca2, 10000);
    // inserir_pontuacao(&cabeca1, ultimo_elemento(cabeca1)->vida, ultimo_elemento(cabeca1)->estamina, "P#1", ultimo_elemento(cabeca1)->histAtt);
    // inserir_pontuacao(&cabeca2, ultimo_elemento(cabeca2)->vida, ultimo_elemento(cabeca2)->estamina, "P#2", ultimo_elemento(cabeca2)->histAtt);
  }
  else if (strncmp(ataques, "Alt-F4", 6) == 0)
  {
    // Jogador 1 - Restaura a estamina a X pontos. X deve ser um número positivo.
    x = atoi(ataques + 6);
    if (x > 0)
    {
      ultimo_elemento(cabeca1)->estamina = ultimo_elemento(cabeca1)->estamina + x > MAX_ESTAMINA
                                               ? MAX_ESTAMINA
                                               : ultimo_elemento(cabeca1)->estamina + x;
    }
  }
  else if (strncmp(ataques, "Kebab", 5) == 0)
  {
    // Jogador 2 - Restaura a estamina a X pontos. X deve ser um número positivo.
    x = atoi(ataques + 5);
    if (x > 0)
    {
      ultimo_elemento(cabeca2)->estamina = ultimo_elemento(cabeca2)->estamina + x > MAX_ESTAMINA
                                               ? MAX_ESTAMINA
                                               : ultimo_elemento(cabeca2)->estamina + x;
    }
  }
  else if (strncmp(ataques, "Hiroshima", 9) == 0)
  {
    // Jogador 1 - Restaura a vida a X pontos. X deve ser um número positivo.
    x = atoi(ataques + 9);
    if (x > 0)
    {
      ultimo_elemento(cabeca1)->vida = ultimo_elemento(cabeca1)->vida + x > MAX_HEALTH
                                           ? MAX_HEALTH
                                           : ultimo_elemento(cabeca1)->vida + x;
    }
  }
  else if (strncmp(ataques, "Nood-Mode", 9) == 0)
  {
    // Jogador 2 - Restaura a vida a X pontos. X deve ser um número positivo.
    x = atoi(ataques + 9);
    if (x > 0)
    {
      ultimo_elemento(cabeca2)->vida = ultimo_elemento(cabeca2)->vida + x > MAX_HEALTH
                                           ? MAX_HEALTH
                                           : ultimo_elemento(cabeca2)->vida + x;
    }
  }
}

int main()
{

  // Inicializa e preenche a tabela de combos
  int combos[12][12];
  Pontuacao *jogador1 = NULL;
  Pontuacao *jogador2 = NULL;
  char ataquesJ1[30];
  char ataquesJ2[30];
  char histAttJ1[200] = {'\0'};
  char histAttJ2[200] = {'\0'};
  int ler_ataquesOK, lucioX, cheatCodeOK;

  criar_tabela_combos(combos);
  inserir_pontuacao(&jogador1, 1000, 1000, "P#1", histAttJ1);
  inserir_pontuacao(&jogador2, 1000, 1000, "P#2", histAttJ2);
  inserir_pontuacao(&jogador1, 1000, 1000, "P#1", histAttJ1);
  inserir_pontuacao(&jogador2, 1000, 1000, "P#2", histAttJ2);

  lucioX = 0;
  cheatCodeOK = 0;

  while (!jogador_morto(jogador1, jogador2))
  {
    // Menu Jogador 1
    do
    {
      mostrar_pontuacao(jogador1, obter_tamanho_lista(jogador1));
      mostrar_historico(jogador1);
      printf("I:");
      ler_ataquesOK = ler_ataques(ataquesJ1, ultimo_elemento(jogador1), &lucioX, &cheatCodeOK);
      if (ler_ataquesOK == 1)
      {
        printf("Entrada invalida\n");
        // Liberar a memória alocada
        liberar_lista(jogador1);
        liberar_lista(jogador2);
        return 0; // Erro ao ler os ataques
      }

      if (cheatCodeOK == 1)
      {
        aplicar_cheat_code(jogador1, jogador2, ataquesJ1);
      }

      if (lucioX != 0)
      {
        remove_x(&jogador1, lucioX);
        remove_x(&jogador2, lucioX);
        inserir_pontuacao(&jogador1, ultimo_elemento(jogador1)->vida, ultimo_elemento(jogador1)->estamina, "P#1", ultimo_elemento(jogador1)->histAtt);
        inserir_pontuacao(&jogador2, ultimo_elemento(jogador2)->vida, ultimo_elemento(jogador2)->estamina, "P#2", ultimo_elemento(jogador2)->histAtt);
        // Coloca uma virgula no historico
        strcat(ultimo_elemento(jogador1)->histAtt, ",");
        printf(">>> Voltou %d ataques\n", lucioX);
        lucioX = 0;
      }
    } while (ler_ataquesOK == 2);

    // Menu Jogador 2
    do
    {
      mostrar_pontuacao(jogador2, obter_tamanho_lista(jogador2));
      mostrar_historico(jogador2);
      printf("I:");
      ler_ataquesOK = ler_ataques(ataquesJ2, ultimo_elemento(jogador2), &lucioX, &cheatCodeOK);
      if (ler_ataquesOK == 1)
      {
        printf("Entrada invalida\n");
        // Liberar a memória alocada
        liberar_lista(jogador1);
        liberar_lista(jogador2);
        return 0; // Erro ao ler os ataques
      }

      if (cheatCodeOK == 1)
      {
        aplicar_cheat_code(jogador1, jogador2, ataquesJ2);
      }

      if (lucioX != 0)
      {
        remove_x(&jogador1, lucioX);
        remove_x(&jogador2, lucioX);
        inserir_pontuacao(&jogador1, ultimo_elemento(jogador1)->vida, ultimo_elemento(jogador1)->estamina, "P#1", ultimo_elemento(jogador1)->histAtt);
        inserir_pontuacao(&jogador2, ultimo_elemento(jogador2)->vida, ultimo_elemento(jogador2)->estamina, "P#2", ultimo_elemento(jogador2)->histAtt);
        // Coloca uma virgula no historico
        strcat(ultimo_elemento(jogador1)->histAtt, ",");
        printf(">>> Voltou %d ataques\n", lucioX);
        lucioX = 0;
      }
    } while (ler_ataquesOK == 2);

    print_ataques(ataquesJ1, ataquesJ2);

    if (aplicar_dano(ataquesJ1, ataquesJ2, ultimo_elemento(jogador1), ultimo_elemento(jogador2), combos) == 1)
    {
      liberar_lista(jogador1);
      liberar_lista(jogador2);
      exit(0); // Termina o programa se aplicar dano detetar um jogador morto
    };
  }

  // Liberar a memória alocada
  liberar_lista(jogador1);
  liberar_lista(jogador2);

  return 0;
}