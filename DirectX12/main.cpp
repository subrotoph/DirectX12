#include "stdafx.h"



void mainloop() {
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    while (true) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {

        }
    }
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nShowCmd) {

    if (!InitWindow(hInstance, nShowCmd, Width, Height, FullScreen)) {
        MessageBox(0, L"Window Initialization - Failed", L"Error", MB_OK);
        return 0;
    }

    if (!InitD3D()) {
        MessageBox(0, L"Failed to initialize direct3d 12", L"Error", MB_OK);
        Cleanup();
        return 1;
    }

    mainloop();
    WaitForPreviousFrame();
    CloseHandle(fenceEvent);
    return 0;
}

bool InitWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool fullscreen) {
    WNDCLASSEX wc;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = NULL;
    wc.cbWndExtra = NULL;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = WindowName;

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, L"Error registering class", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }

    hwnd = CreateWindowEx(NULL,
        WindowName,
        WindowTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        NULL,
        NULL,
        hInstance,
        NULL);

    if (!hwnd) {
        MessageBox(NULL, L"Error creating window", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }

    ShowWindow(hwnd, ShowWnd);
    UpdateWindow(hwnd);

    return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            DestroyWindow(hwnd);
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}