# Курс операционных систем

Учебный проект по курсу «Операционные системы». Реализация практических заданий по работе с процессами, потоками, памятью и межпроцессному взаимодействию в Windows и Linux.

## Структура проекта

```
├── operating_systems_task/           # Основное задание
├── operating_systems_task_2/         # Задание 2
├── operating_systems_task_3/         # Задание 3
├── operating_systems_task4/          # Задание 4
├── operating_systems_creative_assignment/  # Творческое задание
│
└── Документация:
    ├── Задание №1. Перехват и работа с сообщениями (Windows)
    ├── Задание №2. Разделяемая память и обмен сообщениями
    ├── Задание №3. Перехват сообщений другой программы
    ├── Задание №4. Передача информации через разделяемую память
    ├── Работа в Linux
    └── Разделение ресурсов. Критические секции
```

## Темы заданий

| Задание | Тема | ОС |
|---------|------|-----|
| **№1** | Перехват и работа с сообщениями | Windows |
| **№2** | Разделяемая память и обмен сообщениями | Windows |
| **№3** | Перехват сообщений другой программы | Windows |
| **№4** | Передача информации между приложениями | Windows |
| **Linux** | Работа в Linux среде | Linux |
| **Критические секции** | Разделение ресурсов, синхронизация | Windows/Linux |

## Технологии

- **Windows API** — работа с процессами, сообщениями, разделяемой памятью
- **Linux** — системные вызовы, процессы, потоки
- **Синхронизация** — мьютексы, семафоры, критические секции

## Описание заданий

### Задание №1: Перехват и работа с сообщениями

Изучение механизма сообщений в Windows API. Реализация перехвата и обработки системных сообщений.

### Задание №2: Разделяемая память и обмен сообщениями

Реализация межпроцессного взаимодействия (IPC) через:
- Разделяемую память (Shared Memory)
- Обмен сообщениями (Message Queue)

### Задание №3: Перехват сообщений другой программы

Работа с хуками (Hooks) Windows для перехвата сообщений других приложений.

### Задание №4: Передача информации между приложениями

Расширенная реализация IPC с использованием разделяемой памяти и каналов связи.

### Работа в Linux

Базовые операции с файловой системой, процессами и правами доступа в Linux.

### Критические секции

Организация синхронизации доступа к общим ресурсам:
- Мьютексы (Mutex)
- Семафоры (Semaphore)
- Критические секции (Critical Section)

## Запуск

### Windows

```bash
# Компиляция с MinGW
gcc program.c -o program.exe

# Запуск
./program.exe
```

### Linux

```bash
# Компиляция
gcc program.c -o program -lpthread

# Запуск
./program
```

## Требования

- **Windows:** MinGW / Visual Studio, Windows SDK
- **Linux:** GCC, glibc
- **Язык:** C/C++

## Ссылки

- [Windows API Documentation](https://docs.microsoft.com/en-us/windows/win32/api/)
- [Linux man-pages](https://www.man7.org/linux/man-pages/)
- [Inter-Process Communication](https://docs.microsoft.com/en-us/windows/win32/ipc/interprocess-communications)
- [Windows API Documentation](https://docs.microsoft.com/en-us/windows/win32/api/)
- [Linux man-pages](https://www.man7.org/linux/man-pages/)
- [Inter-Process Communication](https://docs.microsoft.com/en-us/windows/win32/ipc/interprocess-communications)
