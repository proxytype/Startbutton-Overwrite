// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <windows.h>
#include <uxtheme.h>

#pragma comment(lib, "uxtheme.lib")
#pragma comment(lib, "Msimg32.lib")

const LPCWSTR normalICON = L"C:\\spider-big.ico";

HWND taskBar = NULL;
HWND windowsStartButton = NULL;
HWND myStartButton = NULL;

HICON hNormal = NULL;
HICON hHover = NULL;
HICON hClick = NULL;

WNDPROC startButtonProc = NULL;
WNDPROC taskManagerProc = NULL;

RECT rc;

int width = 0;
int height = 0;

int alpha = 0;
int mouseLook = 0;
int mouseButtonState = 0;

void waitForMessage() {
    MSG messages;
    while (GetMessage(&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
}

void locker() {
    waitForMessage();
    locker();
}


LRESULT CALLBACK startButton__callback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_CREATE: {
        return CallWindowProc(startButtonProc, hwnd, message, wParam, lParam);
    }
    case WM_MOUSEMOVE: {
        
        
        if (mouseLook == 0) {
   
            alpha = 25;

            TRACKMOUSEEVENT mEvent;

            mEvent.cbSize = sizeof(mEvent);
            mEvent.hwndTrack = myStartButton;
            mEvent.dwFlags = TME_LEAVE;
            TrackMouseEvent(&mEvent);
            mouseLook = 1;
           
        }

        PostMessage(hwnd, WM_PAINT, 0, 0);

        return CallWindowProc(startButtonProc, hwnd, message, wParam, lParam);
    }
    
    case WM_MOUSELEAVE:
        
        //CallWindowProc(startButtonProc, hwnd, message, wParam, lParam);
        mouseLook = 0;
        alpha = 0;
        PostMessage(hwnd, WM_PAINT, 0, 0);
        return CallWindowProc(startButtonProc, hwnd, message, wParam, lParam);
    case WM_PAINT: {

        PAINTSTRUCT sf;

        RECT c;
        c.left = 0;
        c.right = width;
        c.top = 0;
        c.bottom = height;

        InvalidateRect(hwnd, &c, TRUE);

        HDC hDC = BeginPaint(hwnd, &sf);
        DrawThemeParentBackground(hwnd, hDC, &c);
        
        DrawIconEx(hDC, 7.5, 7.5, hNormal, width - 15, height - 15, 0, NULL, DI_NORMAL);
        
        HDC tempHdc = CreateCompatibleDC(hDC);
        HBITMAP canvas = CreateCompatibleBitmap(hDC, width, height);

        HGDIOBJ oldBmp = SelectObject(tempHdc, canvas);

        BLENDFUNCTION blend = { AC_SRC_OVER, 0, alpha, AC_SRC_ALPHA };
        SetDCBrushColor(tempHdc, RGB(255, 255, 255));
        Rectangle(tempHdc, c.left, c.top, c.right, c.bottom);
        bool res = AlphaBlend(hDC, c.left, c.top, c.right, c.bottom, tempHdc, c.left, c.top, c.right, c.bottom, blend);

        SelectObject(tempHdc, oldBmp);
 
        DeleteObject(canvas);

        EndPaint(hwnd, &sf);

        return  CallWindowProc(startButtonProc, hwnd, message, wParam, lParam);
    }

    case WM_COMMAND: {
        return CallWindowProc(startButtonProc, hwnd, message, wParam, lParam);
    }
    case WM_LBUTTONDOWN:
        CallWindowProc(startButtonProc, hwnd, message, wParam, lParam);

        if (mouseButtonState == 0) {
            mouseButtonState = 1;
            alpha = 45;
        }
        else {
            mouseButtonState = 0;
            alpha = 0;
        }
        
        PostMessage(windowsStartButton, WM_LBUTTONDOWN, 0, 0);
        PostMessage(hwnd, WM_PAINT, 0, 0);
        return 0;
    case WM_LBUTTONUP:
        CallWindowProc(startButtonProc, hwnd, message, wParam, lParam);
        PostMessage(hwnd, WM_PAINT, 0, 0);
        return 0;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

}


DWORD WINAPI init(LPVOID lpParam) {

    taskBar = FindWindow(L"Shell_TrayWnd", NULL);
    windowsStartButton = FindWindowEx(taskBar, 0, L"Start", L"Start");

    GetWindowRect(windowsStartButton, &rc);

    width = rc.right - rc.left;
    height = rc.bottom - rc.top;

    hNormal = (HICON)LoadImage(NULL, normalICON, IMAGE_ICON, width, height, LR_LOADFROMFILE);

    myStartButton = CreateWindowEx(WS_EX_LEFT | WS_EX_TOPMOST & ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE), L"Button", L"", WS_CHILD | WS_VISIBLE | BS_FLAT | BS_BITMAP & ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU), 0, 0, width, height, taskBar, 0, NULL, 0);
    startButtonProc = (WNDPROC)SetWindowLongPtr(myStartButton, GWLP_WNDPROC, (LONG_PTR)startButton__callback);

    ShowWindow(windowsStartButton, SW_HIDE);
    
    locker();
    
    return 1;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{

    HWND hChild = NULL, hLast = NULL;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CreateThread(0, NULL, init, (LPVOID)L"Injecting Start Button", NULL, NULL);
    }
    break;
    }

    return TRUE;
}

