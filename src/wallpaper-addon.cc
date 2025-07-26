#define UNICODE
#define _UNICODE
#include <napi.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "user32.lib")

class WallpaperManager {
private:
    // Find the WorkerW window behind desktop icons - using the same logic from your original code
    static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam) {
        HWND hwnd = FindWindowExW(hWnd, NULL, L"SHELLDLL_DefView", NULL);
        if (hwnd) {
            HWND* ret = (HWND*)lParam;
            *ret = FindWindowExW(NULL, hWnd, L"WorkerW", NULL);
            return FALSE;
        }
        return TRUE;
    }

    static BOOL CALLBACK EnumChildWndProc(HWND hWnd, LPARAM lParam) {
        WCHAR szClassName[100] = { 0 };
        if (!GetClassNameW(hWnd, szClassName, 100))
            return TRUE;
        if (wcscmp(szClassName, L"WorkerW") == 0) {
            HWND* ret = (HWND*)lParam;
            *ret = hWnd;
            return FALSE;
        }
        return TRUE;
    }

public:
    static HWND FindWorkerWindow() {
        // Find Program Manager window
        HWND progman = FindWindowW(L"Progman", NULL);
        if (!progman) {
            return NULL;
        }

        // Send message to spawn WorkerW behind desktop icons
        DWORD_PTR result;
        LRESULT lResult = SendMessageTimeout(progman, 0x052C, NULL, NULL, SMTO_NORMAL, 1000, &result);
        if (lResult == 0) {
            return NULL;
        }

        // Use the same working logic to find WorkerW
        HWND worker = NULL;

        // First try to find as child of Progman
        EnumChildWindows(progman, EnumChildWndProc, (LPARAM)&worker);

        // If not found as child, enumerate all windows (original fallback)
        if (!worker) {
            EnumWindows(EnumWindowsProc, (LPARAM)&worker);
        }

        return worker;
    }

    static bool SetWindowBehindDesktop(HWND targetWindow) {
        if (!targetWindow || !IsWindow(targetWindow)) {
            return false;
        }

        // Find the WorkerW window
        HWND workerWindow = FindWorkerWindow();
        if (!workerWindow) {
            return false;
        }

        // Set the target window as a child of WorkerW
        HWND result = SetParent(targetWindow, workerWindow);
        if (!result) {
            return false;
        }

        // Get WorkerW dimensions and fit the window
        RECT workerRect;
        if (GetWindowRect(workerWindow, &workerRect)) {
            int width = workerRect.right - workerRect.left;
            int height = workerRect.bottom - workerRect.top;
            
            // Position and size the window to fill the WorkerW area
            SetWindowPos(targetWindow, HWND_BOTTOM, 0, 0, width, height, 
                        SWP_SHOWWINDOW | SWP_NOACTIVATE);
        }

        return true;
    }

    static HWND GetWorkerWindow() {
        return FindWorkerWindow();
    }

    static bool RestoreWindow(HWND targetWindow) {
        if (!targetWindow || !IsWindow(targetWindow)) {
            return false;
        }

        // Remove from WorkerW by setting parent to desktop
        HWND result = SetParent(targetWindow, GetDesktopWindow());
        return (result != NULL);
    }
};

// Convert string to HWND
HWND StringToHWND(const std::string& hwndStr) {
    try {
        uintptr_t hwndValue = std::stoull(hwndStr);
        return reinterpret_cast<HWND>(hwndValue);
    } catch (...) {
        return NULL;
    }
}

// Convert HWND to string
std::string HWNDToString(HWND hwnd) {
    uintptr_t hwndValue = reinterpret_cast<uintptr_t>(hwnd);
    return std::to_string(hwndValue);
}

// N-API function to set window behind desktop
Napi::Value SetWindowBehindDesktop(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected window handle as argument").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "Window handle must be a string").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string hwndStr = info[0].As<Napi::String>().Utf8Value();
    HWND targetWindow = StringToHWND(hwndStr);

    if (!targetWindow) {
        Napi::Error::New(env, "Invalid window handle").ThrowAsJavaScriptException();
        return env.Null();
    }

    bool success = WallpaperManager::SetWindowBehindDesktop(targetWindow);
    return Napi::Boolean::New(env, success);
}

// N-API function to get WorkerW window handle
Napi::Value GetWorkerWindow(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    HWND workerWindow = WallpaperManager::GetWorkerWindow();
    if (!workerWindow) {
        return env.Null();
    }

    std::string hwndStr = HWNDToString(workerWindow);
    return Napi::String::New(env, hwndStr);
}

// N-API function to restore window from behind desktop
Napi::Value RestoreWindow(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected window handle as argument").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "Window handle must be a string").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string hwndStr = info[0].As<Napi::String>().Utf8Value();
    HWND targetWindow = StringToHWND(hwndStr);

    if (!targetWindow) {
        Napi::Error::New(env, "Invalid window handle").ThrowAsJavaScriptException();
        return env.Null();
    }

    bool success = WallpaperManager::RestoreWindow(targetWindow);
    return Napi::Boolean::New(env, success);
}

// N-API function to check if window is valid
Napi::Value IsValidWindow(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        return Napi::Boolean::New(env, false);
    }

    if (!info[0].IsString()) {
        return Napi::Boolean::New(env, false);
    }

    std::string hwndStr = info[0].As<Napi::String>().Utf8Value();
    HWND targetWindow = StringToHWND(hwndStr);

    bool isValid = (targetWindow != NULL) && IsWindow(targetWindow);
    return Napi::Boolean::New(env, isValid);
}

// Initialize the addon
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "setWindowBehindDesktop"),
                Napi::Function::New(env, SetWindowBehindDesktop));
    
    exports.Set(Napi::String::New(env, "getWorkerWindow"),
                Napi::Function::New(env, GetWorkerWindow));
    
    exports.Set(Napi::String::New(env, "restoreWindow"),
                Napi::Function::New(env, RestoreWindow));
    
    exports.Set(Napi::String::New(env, "isValidWindow"),
                Napi::Function::New(env, IsValidWindow));

    return exports;
}

NODE_API_MODULE(wallpaper_addon, Init)