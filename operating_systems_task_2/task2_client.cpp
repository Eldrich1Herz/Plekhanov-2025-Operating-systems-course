#include <windows.h>
#include <commctrl.h>
#include <stdio.h>

#pragma comment(lib, "comctl32.lib")

// Структура для хранения данных окна
typedef struct {
    HWND hEdit, hBtnStart, hBtnMessage, hBtnShared, hStatus;
    HWND hServerWindow;
} AppData;

// Глобальные переменные для разделяемой памяти
HANDLE g_hMapFile = NULL;
LPVOID g_pBuf = NULL;
const char* g_szName = "Local\\MyFileMappingObject";

// Прототипы функций
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL InitControls(HWND hwnd, AppData* pData);
void OnStartProgram(HWND hwnd, AppData* pData);
void OnSendMessage(HWND hwnd, AppData* pData);
void OnWriteToSharedMemory(HWND hwnd, AppData* pData);
HWND FindServerWindow();
BOOL WriteToSharedMemory(int value);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine, int nCmdShow)
{
    // Инициализация common controls
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icex);

    // Регистрация класса окна
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "ControlPanel";
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClassA(&wc)) {
        MessageBoxA(NULL, "Window registration failed", "Error", MB_OK);
        return 1;
    }

    // Создание окна
    HWND hwnd = CreateWindowA(
        "ControlPanel",
        "Control Panel - Program 2",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        NULL, NULL, hInstance, NULL);

    if (!hwnd) {
        MessageBoxA(NULL, "Window creation failed", "Error", MB_OK);
        return 1;
    }

    // Выделяем память для данных приложения
    AppData* pData = (AppData*)malloc(sizeof(AppData));
    
    // Создание разделяемой памяти
    g_hMapFile = CreateFileMappingA(
        INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 256, g_szName);
    
    if (g_hMapFile) {
        g_pBuf = MapViewOfFile(g_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 256);
    }

    // Сохраняем указатель на данные в окне
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pData);

    // Инициализация элементов управления
    if (!InitControls(hwnd, pData)) {
        free(pData);
        return 1;
    }

    // Поиск окна сервера
    pData->hServerWindow = FindServerWindow();

    // Показ окна
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Цикл обработки сообщений
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Освобождение ресурсов
    if (g_pBuf) UnmapViewOfFile(g_pBuf);
    if (g_hMapFile) CloseHandle(g_hMapFile);
    free(pData);

    return (int)msg.wParam;
}

// Инициализация элементов управления
BOOL InitControls(HWND hwnd, AppData* pData) {
    // Создание статического текста
    CreateWindowA("STATIC", "Enter digit (0-9):", 
                WS_VISIBLE | WS_CHILD,
                20, 20, 150, 20, hwnd, NULL, NULL, NULL);

    // Создание поля ввода
    pData->hEdit = CreateWindowA("EDIT", "5", 
                          WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                          20, 50, 100, 25, hwnd, NULL, NULL, NULL);

    // Создание кнопок
    pData->hBtnStart = CreateWindowA("BUTTON", "1. Start via Command Line",
                              WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                              20, 90, 300, 30, hwnd, 
                              (HMENU)1001, NULL, NULL);

    pData->hBtnMessage = CreateWindowA("BUTTON", "2. Send Message",
                                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                20, 130, 300, 30, hwnd, 
                                (HMENU)1002, NULL, NULL);

    pData->hBtnShared = CreateWindowA("BUTTON", "3. Write to Shared Memory",
                               WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                               20, 170, 300, 30, hwnd, 
                               (HMENU)1003, NULL, NULL);

    // Статусная строка
    pData->hStatus = CreateWindowA("STATIC", "Status: Waiting for actions...", 
                WS_VISIBLE | WS_CHILD,
                20, 220, 300, 20, hwnd, (HMENU)1004, NULL, NULL);

    return TRUE;
}

// Обработка запуска программы через командную строку
void OnStartProgram(HWND hwnd, AppData* pData) {
    char digit[10];
    GetWindowTextA(pData->hEdit, digit, 10);

    if (strlen(digit) == 0) {
        MessageBoxA(hwnd, "Enter a digit!", "Error", MB_OK);
        return;
    }

    char cmdLine[100];
    snprintf(cmdLine, sizeof(cmdLine), "server.exe %s", digit);

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    if (CreateProcessA(NULL, cmdLine, NULL, NULL, FALSE, 
                     0, NULL, NULL, &si, &pi)) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        
        MessageBoxA(hwnd, 
            "Program 1 started with command line parameter!", 
            "Success", MB_OK);
        
        // Обновляем статус
        SetWindowTextA(pData->hStatus, 
            "Status: Program 1 started via command line");
    } else {
        MessageBoxA(hwnd, "Failed to start program!", "Error", MB_OK);
    }
}

// Отправка сообщения программе 1
void OnSendMessage(HWND hwnd, AppData* pData) {
    if (!pData->hServerWindow) {
        pData->hServerWindow = FindServerWindow();
    }

    if (!pData->hServerWindow) {
        MessageBoxA(hwnd, 
            "Program 1 window not found!\nStart it first.", 
            "Error", MB_OK);
        return;
    }

    char digit[10];
    GetWindowTextA(pData->hEdit, digit, 10);

    if (strlen(digit) == 0) {
        MessageBoxA(hwnd, "Enter a digit!", "Error", MB_OK);
        return;
    }

    int value = atoi(digit);
    if (value < 0 || value > 9) {
        MessageBoxA(hwnd, "Enter digit 0-9!", "Error", MB_OK);
        return;
    }

    // Отправляем пользовательское сообщение
    PostMessage(pData->hServerWindow, WM_USER + 100, (WPARAM)value, 0);
    
    SetWindowTextA(pData->hStatus, 
        "Status: Message sent to Program 1");
}

// Запись в разделяемую память
void OnWriteToSharedMemory(HWND hwnd, AppData* pData) {
    char digit[10];
    GetWindowTextA(pData->hEdit, digit, 10);

    if (strlen(digit) == 0) {
        MessageBoxA(hwnd, "Enter a digit!", "Error", MB_OK);
        return;
    }

    int value = atoi(digit);
    if (value < 0 || value > 9) {
        MessageBoxA(hwnd, "Enter digit 0-9!", "Error", MB_OK);
        return;
    }

    if (WriteToSharedMemory(value)) {
        SetWindowTextA(pData->hStatus, 
            "Status: Data written to shared memory");
    } else {
        MessageBoxA(hwnd, "Failed to write to shared memory!", 
                  "Error", MB_OK);
    }
}

// Поиск окна сервера
HWND FindServerWindow() {
    return FindWindowA("SquareCalculator", NULL);
}

// Запись в разделяемую память
BOOL WriteToSharedMemory(int value) {
    if (g_pBuf) {
        char buffer[10];
        snprintf(buffer, sizeof(buffer), "%d", value);
        strncpy((char*)g_pBuf, buffer, 10);
        return TRUE;
    }
    return FALSE;
}

// Оконная процедура
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    AppData* pData = (AppData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_COMMAND:
        // Обработка нажатий кнопок
        switch (LOWORD(wParam)) {
        case 1001: // Запуск через командную строку
            OnStartProgram(hwnd, pData);
            break;
        case 1002: // Отправить сообщение
            OnSendMessage(hwnd, pData);
            break;
        case 1003: // Записать в разделяемую память
            OnWriteToSharedMemory(hwnd, pData);
            break;
        }
        return 0;

    default:
        return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
}