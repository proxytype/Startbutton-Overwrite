# Start Button Overwrite
Quick and dirty example how to customize windows taskbar start button.

![alt text](https://raw.githubusercontent.com/proxytype/Startbutton-Overwrite/main/start-overwrite.gif)

Inject DLL to explorer.exe to Find the start button in the taskbar, hide it and create new button, place it over the existing button.
attach events to the custom button and hide the original one.

WIN32 Functions:

* CreateThread
* FindWindow
* FindWindowEx
* LoadImage
* CreateWindowEx
* SetWindowLongPtr
* ShowWindow
* GetMessage
* TranslateMessage
* DispatchMessage
* CallWindowProc
* PostMessage
* DrawIconEx
* DrawThemeParentBackground
* CreateCompatibleDC
* CreateCompatibleBitmap
* SelectObject
* BeginPaint
* EndPaint
* DeleteObject
* DefWindowProc
