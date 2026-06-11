#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <signal.h>
#include <string.h>

#define INPUT_FILE "server_input.txt"
#define OUTPUT_FILE "server_output.txt"

// Структура узла однонаправленного списка
typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* head = NULL;

// Создание нового узла списка
Node* create_node(int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) return NULL;
    new_node->data = value;
    new_node->next = NULL;
    return new_node;
}

// Вставка элемента в список с сохранением сортировки
void insert_sorted(int value) {
    Node* new_node = create_node(value);
    if (new_node == NULL) return;

    if (head == NULL || value < head->data) {
        new_node->next = head;
        head = new_node;
        return;
    }

    Node* current = head;
    while (current->next != NULL && current->next->data < value) {
        current = current->next;
    }
    new_node->next = current->next;
    current->next = new_node;
}

// Очистка списка
void clear_list() {
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;
}

// Чтение данных из файла и сортировка
void read_data_from_file() {
    printf("Сервер: Чтение данных из %s...\n", INPUT_FILE);
    
    FILE* file = fopen(INPUT_FILE, "r");
    if (file == NULL) {
        printf("Сервер: Файл ввода не найден\n");
        return;
    }

    clear_list();
    int num, count = 0;
    
    while (fscanf(file, "%d", &num) == 1) {
        insert_sorted(num);
        count++;
    }
    
    fclose(file);
    printf("Сервер: Прочитано %d чисел в отсортированный список\n", count);
    
    // Вывод списка для проверки
    Node* current = head;
    printf("Сервер: Отсортированный список: ");
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

// Запись отсортированных данных в файл
void write_data_to_file() {
    printf("Сервер: Запись отсортированных данных в %s...\n", OUTPUT_FILE);
    
    FILE* file = fopen(OUTPUT_FILE, "w");
    if (file == NULL) {
        printf("Сервер: Ошибка создания файла вывода\n");
        return;
    }

    Node* current = head;
    int count = 0;
    
    while (current != NULL) {
        fprintf(file, "%d\n", current->data);
        current = current->next;
        count++;
    }
    
    fclose(file);
    printf("Сервер: Записано %d отсортированных чисел в файл\n", count);
}

// Обработчик Ctrl+C для корректного завершения
BOOL CtrlHandler(DWORD fdwCtrlType) {
    switch (fdwCtrlType) {
        case CTRL_C_EVENT:
            printf("\nСервер: Завершение работы...\n");
            clear_list();
            remove(INPUT_FILE);
            remove(OUTPUT_FILE);
            exit(0);
        default:
            return FALSE;
    }
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    printf("=========================================\n");
    printf("Сервер для Практической работы №4\n");
    printf("Однонаправленный список с сортировкой\n");
    printf("=========================================\n");
    
    // Установка обработчика Ctrl+C
    SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);
    
    printf("PID сервера: %d\n", GetCurrentProcessId());
    printf("Сервер запущен...\n");
    printf("Команды:\n");
    printf("  1 - Чтение данных из файла и сортировка\n");
    printf("  2 - Запись отсортированных данных в файл\n");
    printf("  3 - Очистка списка\n");
    printf("  0 - Выход\n");
    printf("=========================================\n");

    int command;
    while (1) {
        printf("\nВведите команду (0-3): ");
        scanf("%d", &command);
        
        switch (command) {
            case 1:
                read_data_from_file();
                break;
            case 2:
                write_data_to_file();
                break;
            case 3:
                clear_list();
                printf("Сервер: Список очищен\n");
                break;
            case 0:
                printf("Сервер: Завершение работы...\n");
                clear_list();
                remove(INPUT_FILE);
                remove(OUTPUT_FILE);
                return 0;
            default:
                printf("Сервер: Неизвестная команда\n");
        }
    }
    
    return 0;
}