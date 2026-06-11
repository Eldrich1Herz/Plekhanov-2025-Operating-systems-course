#include <Date.au3>

; Путь к лог-файлу
Global $log = @ScriptDir & "\log_lab3.txt"

; Имя окна программы, с которой работаем
Global $targetTitle = "SquareApp"   ; Изменили на частичное совпадение

; Записываем время запуска
_LogWrite("===== Запуск скрипта: " & _NowTime() & " =====")

; Включаем отслеживание клавиш
HotKeySet("{0}", "_KeyPressed")
HotKeySet("{1}", "_KeyPressed")
HotKeySet("{2}", "_KeyPressed")
HotKeySet("{3}", "_KeyPressed")
HotKeySet("{4}", "_KeyPressed")
HotKeySet("{5}", "_KeyPressed")
HotKeySet("{6}", "_KeyPressed")
HotKeySet("{7}", "_KeyPressed")
HotKeySet("{8}", "_KeyPressed")
HotKeySet("{9}", "_KeyPressed")

; Дополнительное действие
HotKeySet("x", "_ExtraKey")

; Горячая клавиша для выхода
HotKeySet("^{q}", "_ExitScript")  ; Ctrl+Q для выхода

; Основной цикл работы
While 1
    Sleep(100)
WEnd

; ------------------------------------
; ФУНКЦИИ
; ------------------------------------

; Реакция на нажатие клавиши 0–9
Func _KeyPressed()
    Local $key = @HotKeyPressed
    Local $activeTitle = WinGetTitle("[ACTIVE]")
    
    ; Отладочная информация
    ConsoleWrite("Нажата клавиша: " & $key & " | Активное окно: '" & $activeTitle & "'" & @CRLF)
    
    ; Проверяем, что активно нужное окно
    If WinActive($targetTitle) Then
        _LogWrite("Нажата клавиша: " & $key & " | Время: " & _NowTime())
        ConsoleWrite("✓ Записано в лог!" & @CRLF)
    Else
        ConsoleWrite("✗ Окно не активно - запись пропущена" & @CRLF)
    EndIf
EndFunc

; Дополнительное действие при нажатии X
Func _ExtraKey()
    Local $activeTitle = WinGetTitle("[ACTIVE]")
    ConsoleWrite("Нажата X | Активное окно: '" & $activeTitle & "'" & @CRLF)
    
    If WinActive($targetTitle) Then
        _LogWrite("Нажата X — отправлен Enter в программу | " & _NowTime())
        Send("{ENTER}")
        ConsoleWrite("✓ Отправлен Enter!" & @CRLF)
    Else
        ConsoleWrite("✗ Окно не активно - Enter не отправлен" & @CRLF)
    EndIf
EndFunc

; Запись строки в лог
Func _LogWrite($text)
    Local $file = FileOpen($log, 1)
    FileWriteLine($file, $text)
    FileClose($file)
EndFunc

; Выход по Ctrl+Q
Func _ExitScript()
    ConsoleWrite("Завершение скрипта..." & @CRLF)
    Exit
EndFunc

; При завершении скрипта запишем время
OnAutoItExitRegister("ExitLogger")

Func ExitLogger()
    _LogWrite("===== Завершение скрипта: " & _NowTime() & " =====")
EndFunc