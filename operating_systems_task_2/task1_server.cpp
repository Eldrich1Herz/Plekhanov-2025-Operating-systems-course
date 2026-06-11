#include <windows.h>
#include <stdio.h>

// Структура для хранения данных окна
typedef struct {
    int currentDigit;
    int squaredValue;
} WindowData;

// Прототипы функций
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void UpdateDisplay(HWND hwnd, int digit);

// Глобальные переменные для разделяемой памяти
HANDLE hMapFile = NULL;
LPVOID pBuf = NULL;
const char* szName = "Local\\MyFileMappingObject";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine, int nCmdShow)
{
    // Парсим командную строку
    int initialValue = -1;
    if (strlen(lpCmdLine) > 0) {
        initialValue = atoi(lpCmdLine);
        if (initialValue < 0 || initialValue > 9) {
            initialValue = -1;
        }
    }

    // Создаем разделяемую память
    hMapFile = CreateFileMappingA(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,
        256,
        szName);

    if (hMapFile) {
        pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 256);
    }

    // Регистрируем класс окна
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "SquareCalculator";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClassA(&wc)) {
        MessageBoxA(NULL, "Window registration failed", "Error", MB_OK);
        return 1;
    }

    // Создаем окно
    HWND hwnd = CreateWindowA(
        "SquareCalculator",
        "Square Calculator - Program 1",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        NULL, NULL, hInstance, NULL);

    if (!hwnd) {
        MessageBoxA(NULL, "Window creation failed", "Error", MB_OK);
        return 1;
    }

    // Выделяем память для данных окна
    WindowData* pData = (WindowData*)malloc(sizeof(WindowData));
    pData->currentDigit = initialValue;
    pData->squaredValue = (initialValue != -1) ? initialValue * initialValue : -1;

    // Сохраняем указатель на данные в окне
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pData);

    // Показываем и обновляем окно
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Если есть начальное значение, обновляем отображение
    if (initialValue != -1) {
        UpdateDisplay(hwnd, initialValue);
    }

    // Цикл обработки сообщений
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Освобождаем ресурсы
    free(pData);
    if (pBuf) UnmapViewOfFile(pBuf);
    if (hMapFile) CloseHandle(hMapFile);

    return (int)msg.wParam;
}

// Функция чтения из разделяемой памяти
int ReadFromSharedMemory() {
    if (pBuf) {
        char* sharedData = (char*)pBuf;
        if (strlen(sharedData) > 0) {
            return atoi(sharedData);
        }
    }
    return -1;
}

// Функция обновления отображения
void UpdateDisplay(HWND hwnd, int digit) {
    WindowData* pData = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    if (pData) {
        pData->currentDigit = digit;
        pData->squaredValue = (digit != -1) ? digit * digit : -1;
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
    }
}

// Оконная процедура
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    WindowData* pData = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_KEYDOWN:
        // Обрабатываем нажатия цифровых клавиш
        if (wParam >= '0' && wParam <= '9') {
            UpdateDisplay(hwnd, wParam - '0');
        }
        return 0;

    case WM_USER + 100:  // Наше пользовательское сообщение
        {
            int newDigit = (int)wParam;
            if (newDigit >= 0 && newDigit <= 9) {
                UpdateDisplay(hwnd, newDigit);
            }
        }
        return 0;

    case WM_TIMER:
        // Проверяем разделяемую память каждые 500ms
        if (wParam == 1) {
            int sharedDigit = ReadFromSharedMemory();
            if (sharedDigit != -1 && (!pData || sharedDigit != pData->currentDigit)) {
                UpdateDisplay(hwnd, sharedDigit);
            }
        }
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rect;
            GetClientRect(hwnd, &rect);

            char text[256];
            
            if (!pData || pData->currentDigit == -1) {
                // Текст по умолчанию
                const char* message = 
                    "Waiting for data...\n\n"
                    "Use:\n"
                    "- Keys 0-9\n"
                    "- Messages from Program 2\n"
                    "- Shared memory";
                
                strncpy(text, message, sizeof(text) - 1);
                text[sizeof(text) - 1] = '\0';
            } else {
                // Форматируем результат вычисления
                snprintf(text, sizeof(text), "%d squared = %d", 
                    pData->currentDigit, pData->squaredValue);
            }

            SetBkMode(hdc, TRANSPARENT);
            DrawTextA(hdc, text, -1, &rect, 
                    DT_CENTER | DT_VCENTER | DT_WORDBREAK);

            EndPaint(hwnd, &ps);
        }
        return 0;

    case WM_CREATE:
        // Устанавливаем таймер для проверки разделяемой памяти
        SetTimer(hwnd, 1, 500, NULL);
        return 0;

    default:
        return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
}