/*
** Função : Desenvolver um sistema de gerenciamento de tarefas (To-Do List) em C, aplicando diferentes estruturas de dados para armazenar e manipular as tarefas.
** Autor : Marlon Rufino do Nascimento 2º período turma A 2024.1
** Data : 31/05/2024
** Observações:
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>
#include <time.h>
#include <stdlib.h>

// Estruturas de dados
typedef struct ToDoList {
    char descricao[100];
    int prioridade; // 1 (baixa), 2 (média), 3 (alta)
    char prazo[11]; // formato DD/MM/YYYY e formato char para suportar o 0 no inicio
} ToDoList;

typedef struct ListaToDo {
    ToDoList tarefa;
    struct ListaToDo *ant, *prox;
} ListaToDo;

typedef struct PilhaToDo {
    ToDoList tarefaConcluida; 
    struct PilhaToDo *prox;
} PilhaToDo;

typedef struct FilaToDo {
    ToDoList tarefa;
    struct FilaToDo *prox;
} FilaToDo;

typedef struct DequeToDo {
    ToDoList tarefa;
    struct DequeToDo *prox, *ant;
} DequeToDo;

// Funções
ToDoList* CriarTarefa(char *descricao, int prioridade, char *prazo) {
    ToDoList *novaTarefa = (ToDoList *)malloc(sizeof(ToDoList));
    if (novaTarefa == NULL) {
        printf("Erro: Não foi possível alocar memória para a nova tarefa.\n");
        return NULL;
    }
    strcpy(novaTarefa->descricao, descricao);
    novaTarefa->prioridade = prioridade;
    strcpy(novaTarefa->prazo, prazo);

    return novaTarefa;
}

void obterDataAtual(char *dataAtual) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(dataAtual, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

int compararDatas(char *data1, char *data2) {
    // Converter datas para inteiros
    int dia1, mes1, ano1;
    int dia2, mes2, ano2;
    sscanf(data1, "%d/%d/%d", &dia1, &mes1, &ano1);
    sscanf(data2, "%d/%d/%d", &dia2, &mes2, &ano2);

    // Comparar anos
    if (ano1 > ano2)
        return 1;
    else if (ano1 < ano2)
        return -1;

    // Comparar meses
    if (mes1 > mes2)
        return 1;
    else if (mes1 < mes2)
        return -1;

    // Comparar dias
    if (dia1 > dia2)
        return 1;
    else if (dia1 < dia2)
        return -1;

    // Datas são iguais
    return 0;
}

int calcularDiasRestantes(char *prazo) {
    // Obter a data atual
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    int diaAtual = local->tm_mday;
    int mesAtual = local->tm_mon + 1; // Os meses em tm_mon são indexados a partir de 0
    int anoAtual = local->tm_year + 1900; // Ano atual contém anos desde 1900

    // Extrair a data do prazo da tarefa
    int diaPrazo, mesPrazo, anoPrazo;
    sscanf(prazo, "%d/%d/%d", &diaPrazo, &mesPrazo, &anoPrazo);

    // Converter as datas para dias julianos
    int julianAtual = diaAtual + mesAtual * 30 + anoAtual * 365;
    int julianPrazo = diaPrazo + mesPrazo * 30 + anoPrazo * 365;

    // Calcular os dias restantes
    int diasRestantes = julianPrazo - julianAtual;
    return diasRestantes;
}

bool verifica_Tarefa_vazia(ListaToDo *lista) {
    return lista == NULL;
}

void adicionarInicioDeque(DequeToDo **inicio, DequeToDo **final, ToDoList tarefa) {
    DequeToDo *nova = (DequeToDo *)malloc(sizeof(DequeToDo));
    if (nova == NULL) {
        printf("Erro: Não foi possível alocar memória para a nova tarefa.\n");
        return;
    }
    nova->tarefa = tarefa;
    nova->ant = NULL;
    nova->prox = *inicio;
    if (*inicio != NULL) {
        (*inicio)->ant = nova;
    } else {
        *final = nova;  // Se o deque estava vazio, o final também aponta para a nova tarefa
    }
    *inicio = nova;
}

void adicionarFinalDeque(DequeToDo **inicio, DequeToDo **final, ToDoList tarefa) {
    DequeToDo *nova = (DequeToDo *)malloc(sizeof(DequeToDo));
    if (nova == NULL) {
        printf("Erro: Não foi possível alocar memória para a nova tarefa.\n");
        return;
    }
    nova->tarefa = tarefa;
    nova->prox = NULL;

    if (*final == NULL) {
        nova->ant = NULL;
        *inicio = nova;  // Se o deque estava vazio, o início também aponta para a nova tarefa
        *final = nova;
        return;
    }

    nova->ant = *final;
    (*final)->prox = nova;
    *final = nova;
}

void removerInicioDeque(DequeToDo **inicio, DequeToDo **final) {
    if (*inicio == NULL) {
        printf("Erro: O deque está vazio.\n");
        return;
    }
    DequeToDo *temp = *inicio;
    *inicio = (*inicio)->prox;
    if (*inicio != NULL) {
        (*inicio)->ant = NULL;
    } else {
        *final = NULL;  // O deque está vazio
    }
    free(temp);
    printf("Tarefa removida do início com sucesso!\n");
}

void removerFinalDeque(DequeToDo **inicio, DequeToDo **final) {
    if (*final == NULL) {
        printf("Erro: O deque está vazio.\n");
        return;
    }
    DequeToDo *temp = *final;
    *final = (*final)->ant;
    if (*final != NULL) {
        (*final)->prox = NULL;
    } else {
        *inicio = NULL;  // O deque está vazio
    }
    free(temp);
    printf("Tarefa removida do final com sucesso!\n");
}

void exibirDeque(DequeToDo *inicio) {
    DequeToDo *atual = inicio;
    printf("\nTarefas no deque:\n");
    while (atual != NULL) {
        printf("\nDescrição: %s\n", atual->tarefa.descricao);
        printf("Prioridade: %d\n", atual->tarefa.prioridade);
        atual = atual->prox;
    }
}


void adicionarNaLista(ListaToDo **listaTarefas, ToDoList tarefa) {
    ListaToDo *nova_tarefa = (ListaToDo *)malloc(sizeof(ListaToDo));
    if (nova_tarefa == NULL) {
        printf("Erro: Não foi possível alocar memória para a nova tarefa.\n");
        return;
    }
    nova_tarefa->tarefa = tarefa;
    nova_tarefa->prox = NULL;

    if (*listaTarefas == NULL) {
        nova_tarefa->ant = NULL;
        *listaTarefas = nova_tarefa; // Se a lista estiver vazia, a nova tarefa é o único nó na lista
        return;
    }

    ListaToDo *temp = *listaTarefas;
    while (temp->prox != NULL) {
        temp = temp->prox;
    }
    temp->prox = nova_tarefa;
    nova_tarefa->ant = temp; // O nó anterior à nova tarefa é o último nó atual
}

void copiarDequeParaLista(DequeToDo *dequeInicio, ListaToDo **listaTarefas) {
    DequeToDo *atual = dequeInicio;
    while (atual != NULL) {
        // Verifica se a tarefa já existe na lista
        ListaToDo *temp = *listaTarefas;
        bool tarefaExistente = false;
        while (temp != NULL) {
            if (strcmp(temp->tarefa.descricao, atual->tarefa.descricao) == 0) {
                tarefaExistente = true;
                break;
            }
            temp = temp->prox;
        }

        // Se a tarefa não existir na lista, adicione-a
        if (!tarefaExistente) {
            adicionarNaLista(listaTarefas, atual->tarefa);
        }

        atual = atual->prox;
    }
}

void enfileirarTarefaPeloPrazo(FilaToDo **filaTarefas, DequeToDo **inicio, DequeToDo **final, ToDoList tarefa) {
    // Obtém a data atual
    char dataAtual[11];
    obterDataAtual(dataAtual);

    // Verifica se o dia da tarefa é igual ao dia atual
    if (strncmp(tarefa.prazo, dataAtual, 2) == 0) {
        // Adiciona a tarefa ao deque
        adicionarFinalDeque(inicio, final, tarefa);
        printf("Tarefa agendada para hoje e adicionada ao deque com sucesso!\n");
        return;
    }

    // Caso o dia da tarefa não seja igual ao dia atual, adiciona-a à fila
    FilaToDo *novaTarefa = (FilaToDo *)malloc(sizeof(FilaToDo));
    if (novaTarefa == NULL) {
        printf("Erro: Não foi possível alocar memória para a nova tarefa.\n");
        return;
    }
    novaTarefa->tarefa = tarefa;
    novaTarefa->prox = NULL;

    if (*filaTarefas == NULL) {
        *filaTarefas = novaTarefa;
    } else {
        FilaToDo *atual = *filaTarefas;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novaTarefa;
    }
    printf("Tarefa agendada com sucesso!\n");
}


void marcar_tarefa_concluida(ListaToDo **listaTarefas, PilhaToDo **pilhaTarefasConcluidas, DequeToDo **deque, char *descricao) {
    ListaToDo *atual = *listaTarefas;
    while (atual != NULL) {
        if (strcmp(atual->tarefa.descricao, descricao) == 0) {
            // Encontramos a tarefa
            PilhaToDo *novaTarefaConcluida = (PilhaToDo *)malloc(sizeof(PilhaToDo));
            if (novaTarefaConcluida == NULL) {
                printf("\nErro: Não foi possível alocar memória para a tarefa concluída.\n");
                return;
            }
            novaTarefaConcluida->tarefaConcluida = atual->tarefa;
            novaTarefaConcluida->prox = *pilhaTarefasConcluidas;
            *pilhaTarefasConcluidas = novaTarefaConcluida;

            // Remover a tarefa da lista
            if (atual->ant != NULL) {
                atual->ant->prox = atual->prox;
            } else {
                *listaTarefas = atual->prox; // Se for a primeira tarefa
            }
            if (atual->prox != NULL) {
                atual->prox->ant = atual->ant;
            }
            free(atual);

            // Remover a tarefa do deque
            DequeToDo *atualDeque = *deque;
            while (atualDeque != NULL) {
                if (strcmp(atualDeque->tarefa.descricao, descricao) == 0) {
                    if (atualDeque->ant != NULL) {
                        atualDeque->ant->prox = atualDeque->prox;
                    } else {
                        *deque = atualDeque->prox; // Se for a primeira tarefa do deque
                    }
                    if (atualDeque->prox != NULL) {
                        atualDeque->prox->ant = atualDeque->ant;
                    }
                    free(atualDeque);
                    break;
                }
                atualDeque = atualDeque->prox;
            }

            printf("\nTarefa marcada como concluída com sucesso!\n");
            return;
        }
        atual = atual->prox;
    }
    printf("\nErro: Tarefa não encontrada na lista.\n");
}

void exibirTarefasPendentes(ListaToDo *listaTarefas) {
    ListaToDo *atual = listaTarefas;
    printf("\nTarefas pendentes:\n");
    while (atual != NULL) {
        printf("\nDescrição: %s\n", atual->tarefa.descricao);
        printf("Prioridade: %d\n", atual->tarefa.prioridade);
        atual = atual->prox;
    }
}

void exibirTarefasPelaPrioridade(ListaToDo *tarefas_pendentes) {
    printf("\nTarefas pendentes:\n");

    // Percorre a lista de tarefas para cada nível de prioridade
    for (int prioridade = 3; prioridade >= 1; prioridade--) {
        ListaToDo *atual = tarefas_pendentes;
        printf("\nPrioridade %d:\n", prioridade);
        
        // Percorre a lista de tarefas para exibir as tarefas da prioridade atual
        while (atual != NULL) {
            if (atual->tarefa.prioridade == prioridade) {
                printf("Descrição: %s\n", atual->tarefa.descricao);
            }
            atual = atual->prox;
        }
    }
}

void exibirTarefasConcluidas(PilhaToDo *tarefas_concluidas) {
    char dataAtual[11];
    obterDataAtual(dataAtual);

    PilhaToDo *atual = tarefas_concluidas;
    printf("\nTarefas concluídas:\n");
    while (atual != NULL) {
        printf("\nDescrição: %s\n", atual->tarefaConcluida.descricao);
        printf("Prioridade: %d\n", atual->tarefaConcluida.prioridade);
        printf("Data de finalização: %s\n", dataAtual);
        atual = atual->prox;
    }
}

void exibirTarefasAgendadas(FilaToDo *filaTarefas) {
    char dataAtual[11];
    obterDataAtual(dataAtual);

    FilaToDo *atual = filaTarefas;
    printf("\nTarefas agendadas:\n");
    while (atual != NULL) {
        printf("\nDescrição: %s\n", atual->tarefa.descricao);
        printf("Prioridade: %d\n", atual->tarefa.prioridade);
        int dias_restantes = calcularDiasRestantes(atual->tarefa.prazo);
        if (dias_restantes < 0) {
            printf("Erro ao calcular os dias restantes para %s.\n", atual->tarefa.descricao);
        } else {
            printf("Faltam: %d dias\n", dias_restantes);
        }
        atual = atual->prox;
    }
}


void buscarTarefa(ListaToDo *listaTarefas, PilhaToDo *pilhaTarefasConcluidas, FilaToDo *filaTarefas, char *descricao) {
    bool encontrou = false;

    // Busca na lista de tarefas pendentes
    ListaToDo *atual = listaTarefas;
    while (atual != NULL) {
        if (strcmp(atual->tarefa.descricao, descricao) == 0) {
            encontrou = true;
            printf("\nTarefa \"%s\" encontrada:\n", descricao);
            printf("Estado: Pendente\n");
            printf("Prioridade: %d\n", atual->tarefa.prioridade);
            return;
        }
        atual = atual->prox;
    }

    // Busca na pilha de tarefas concluídas
    PilhaToDo *atualConcluidas = pilhaTarefasConcluidas;
    while (atualConcluidas != NULL) {
        if (strcmp(atualConcluidas->tarefaConcluida.descricao, descricao) == 0) {
            encontrou = true;
            printf("\nTarefa \"%s\" encontrada:\n", descricao);
            printf("Estado: Concluída\n");
            return;
        }
        atualConcluidas = atualConcluidas->prox;
    }

    // Busca na fila de tarefas agendadas
    FilaToDo *atualAgendadas = filaTarefas;
    while (atualAgendadas != NULL) {
        if (strcmp(atualAgendadas->tarefa.descricao, descricao) == 0) {
            encontrou = true;
            printf("\nTarefa \"%s\" encontrada:\n", descricao);
            printf("Estado: Agendada\n");
            printf("Prioridade: %d\n", atualAgendadas->tarefa.prioridade);
            // Calcula e exibe os dias restantes até o prazo
            int diasRestantes = calcularDiasRestantes(atualAgendadas->tarefa.prazo);
            printf("Faltam %d dias para o prazo\n", diasRestantes);
            return;
        }
        atualAgendadas = atualAgendadas->prox;
    }

    if (!encontrou) {
        printf("\nTarefa \"%s\" não encontrada.\n", descricao);
    }
}


int main() {
    setlocale(LC_ALL, "portuguese");

    // Inicializar a estrutura de tarefa
    ToDoList *tarefa;

    // Criar lista de tarefas
    ListaToDo *listaTarefas = NULL;

    // Criar pilha de tarefas concluídas
    PilhaToDo *pilhaTarefasConcluidas = NULL;

    // Criar fila de tarefas com data futura
    FilaToDo *filaTarefas = NULL;

    // Criar deque de tarefas
    DequeToDo *dequeInicio = NULL;
    DequeToDo *dequeFinal = NULL;

    int opcao;
    do {
        printf("\n =========== MENU ===========\n");
        printf("1. Adicionar tarefa no início do deque\n");
        printf("2. Adicionar tarefa no final do deque\n");
        printf("3. Marcar tarefa como concluída\n");
        printf("4. Agendar tarefa pelo prazo\n");
        printf("5. Remover tarefa do início\n");
        printf("6. Remover tarefa do final\n");
        printf("7. Exibir tarefas pendentes\n");
        printf("8. Exibir tarefas concluídas\n");
        printf("9. Exibir tarefas agendadas\n");
        printf("10. Exibir tarefas pela prioridade\n");
        printf("11. Buscar tarefa\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                printf("\nDigite a descrição da tarefa: ");
                char descricao[100];
                scanf(" %[^\n]", descricao);
                int prioridade;
                do {
                    printf("\nDigite a prioridade da tarefa (1-baixa, 2-média, 3-alta): ");
                    scanf("%d", &prioridade);
                    if (prioridade < 1 || prioridade > 3) {
                        printf("\nPrioridade inválida! Digite novamente.\n");
                    }
                } while (prioridade < 1 || prioridade > 3);
                char prazo[11] = "";
                
                tarefa = CriarTarefa(descricao, prioridade, prazo);
                adicionarInicioDeque(&dequeInicio, &dequeFinal, *tarefa);
                printf("Tarefa adicionada com sucesso!\n");
                break;
            }
            case 2: {
                printf("\nDigite a descrição da tarefa: ");
                char descricao[100];
                scanf(" %[^\n]", descricao);
                int prioridade;
                do {
                    printf("\nDigite a prioridade da tarefa (1-baixa, 2-média, 3-alta): ");
                    scanf("%d", &prioridade);
                    if (prioridade < 1 || prioridade > 3) {
                        printf("\nPrioridade inválida! Digite novamente.\n");
                    }
                } while (prioridade < 1 || prioridade > 3);
                char prazo[11] = "";
                
                tarefa = CriarTarefa(descricao, prioridade, prazo);
                adicionarFinalDeque(&dequeInicio, &dequeFinal, *tarefa);
                printf("Tarefa adicionada com sucesso!\n");
                break;
            }
            case 3: {
                if(verifica_Tarefa_vazia(dequeInicio)){
                    printf("\nNão há tarefas na lista!\n");
                    break;
                }

                printf("\nDigite a descrição da tarefa: ");
                char descricao[100];
                scanf(" %[^\n]", descricao);
                marcar_tarefa_concluida(&listaTarefas, &pilhaTarefasConcluidas, &dequeInicio, descricao);
                break;
            }
            case 4: {
                printf("\nDigite a descrição da tarefa: ");
                char descricao[100];
                scanf(" %[^\n]", descricao);
                int prioridade;
                do {
                    printf("\nDigite a prioridade da tarefa (1-baixa, 2-média, 3-alta): ");
                    scanf("%d", &prioridade);
                    if (prioridade < 1 || prioridade > 3) {
                        printf("\nPrioridade inválida! Digite novamente.\n");
                    }
                } while (prioridade < 1 || prioridade > 3);
                char dataAtual[11];
                obterDataAtual(dataAtual);
               char prazo[11];
             do {
                printf("\nDigite o prazo da tarefa (formato DD/MM/YYYY): ");
                scanf("%s", prazo);

                if (strlen(prazo) != 10 || prazo[2] != '/' || prazo[5] != '/'){
                    printf("\nPrazo inválido, digite novamente.\n");
                    continue; // Irá reiniciar o loop sem verificar a data atual
                }

                if (compararDatas(prazo, dataAtual) < 0) {
                    printf("\nPrazo não pode ser anterior à data atual, digite novamente.\n");
                    continue; // Irá reiniciar o loop sem verificar a data atual
                }

            } while (strlen(prazo) != 10 || prazo[2] != '/' || prazo[5] != '/' || compararDatas(prazo, dataAtual) < 0);

                tarefa = CriarTarefa(descricao, prioridade, prazo);
                enfileirarTarefaPeloPrazo(&filaTarefas,&dequeInicio, &dequeFinal ,*tarefa);
                break;
            }
            case 5: {
                if(verifica_Tarefa_vazia(dequeInicio)){
                    printf("\nO deque está vazio!\n");
                    break;
                }
                removerInicioDeque(&dequeInicio, &dequeFinal);
                break;
            }
            case 6: {
                if(verifica_Tarefa_vazia(dequeFinal)){
                    printf("\nO deque está vazio!\n");
                    break;
                }
                removerFinalDeque(&dequeInicio, &dequeFinal);
                break;
            }
            case 7: {
                if(verifica_Tarefa_vazia(dequeFinal)){
                    printf("\nNão há tarefas na lista!\n");
                    break;
                }
                copiarDequeParaLista(dequeInicio, &listaTarefas);
                exibirTarefasPendentes(listaTarefas);
                break;
            }
            case 8: {
                if(verifica_Tarefa_vazia(pilhaTarefasConcluidas) && verifica_Tarefa_vazia(pilhaTarefasConcluidas)){
                    printf("\nNão há tarefas concluídas na pilha!\n");
                    break;
                }
                exibirTarefasConcluidas(pilhaTarefasConcluidas);
                break;
            }
            case 9: {
                if(filaTarefas == NULL){
                    printf("\nNão há tarefas agendadas!\n");
                    break;
                }
                exibirTarefasAgendadas(filaTarefas);
                break;
            }
            case 10: {
                if(verifica_Tarefa_vazia(listaTarefas)){
                    printf("\nNão há tarefas na lista!\n");
                    break;
                }
                exibirTarefasPelaPrioridade(listaTarefas);
                break;
            }
           case 11: {
                if(verifica_Tarefa_vazia(listaTarefas) && verifica_Tarefa_vazia(filaTarefas) && verifica_Tarefa_vazia(pilhaTarefasConcluidas)){
                    printf("\nNão há tarefas na lista!\n");
                    break;
                } 
                    printf("\nDigite a descrição da tarefa que deseja buscar: ");
                    char descricao[100];
                    scanf(" %[^\n]", descricao);

                    buscarTarefa(listaTarefas, pilhaTarefasConcluidas, filaTarefas, descricao);
                    break;

            }
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
}
    } while (opcao != 0);
    return 0;
}
