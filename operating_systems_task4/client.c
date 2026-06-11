#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define INPUT_FILE "server_input.txt"
#define OUTPUT_FILE "server_output.txt"

int main(int argc, char *argv[]) {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    printf("=========================================\n");
    printf("Клиент для Практической работы №4\n");
    printf("=========================================\n");
    
    if (argc < 3) {
        printf("Использование: %s <PID_сервера> <число1> [число2 ...]\n", argv[0]);
        printf("Пример: %s 1234 5 3 8 1 9\n", argv[0]);
        return 1;
    }

    // Запись данных в файл для сервера
    printf("Клиент: Запись чисел в %s: ", INPUT_FILE);
    FILE* file_out = fopen(INPUT_FILE, "w");
    for (int i = 2; i < argc; i++) {
        fprintf(file_out, "%s ", argv[i]);
        printf("%s ", argv[i]);
    }
    fclose(file_out);
    printf("\nКлиент: Данные записаны в файл ввода\n");

    printf("Клиент: Пожалуйста, выполните команду '1' на сервере для обработки данных\n");
    printf("Затем выполните команду '2' на сервере для генерации вывода\n");
    printf("Нажмите Enter для чтения результатов...");
    getchar(); getchar();

    // Чтение результатов от сервера
    printf("\nКлиент: Чтение отсортированных данных из %s...\n", OUTPUT_FILE);
    FILE* file_in = fopen(OUTPUT_FILE, "r");
    if (file_in == NULL) {
        printf("Клиент: Файл вывода не найден. Сервер возможно не обработал данные.\n");
        return 1;
    }

    int num, count = 0;
    printf("=========================================\n");
    printf("Клиент: Полученные отсортированные данные:\n");
    printf("-----------------------------------------\n");
    
    while (fscanf(file_in, "%d", &num) == 1) {
        printf("%d\n", num);
        count++;
    }
    
    fclose(file_in);
    printf("-----------------------------------------\n");
    printf("Клиент: Получено %d отсортированных чисел\n", count);
    printf("=========================================\n");

    return 0;
}