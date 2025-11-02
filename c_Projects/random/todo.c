#include <windows.h>

#define IDC_TASK_EDIT      101
#define IDC_ADD_BUTTON     102
#define IDC_REMOVE_BUTTON  103
#define IDC_TASK_LIST      104

static HWND hEdit, hList;
static HINSTANCE hInst;

// Forward declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Entry point for a Unicode, 64-bit Win32 GUI application
int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
) {
    WNDCLASSEXW wc = { sizeof(wc) };
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"TodoAppClass";

    if (!RegisterClassExW(&wc)) {
        MessageBoxW(NULL, L"Failed to register window class.", L"Error", MB_ICONERROR);
        return 0;
    }

    hInst = hInstance;
    HWND hWnd = CreateWindowExW(
        0,
        wc.lpszClassName,
        L"To-Do List",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        NULL, NULL, hInstance, NULL
    );

    if (!hWnd) {
        MessageBoxW(NULL, L"Window creation failed.", L"Error", MB_ICONERROR);
        return 0;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        // Create edit box for new task
        hEdit = CreateWindowExW(
            0, L"EDIT", NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            10, 10, 260, 25,
            hWnd, (HMENU)IDC_TASK_EDIT, hInst, NULL
        );

        // Add button
        CreateWindowExW(
            0, L"BUTTON", L"Add",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            280, 10, 50, 25,
            hWnd, (HMENU)IDC_ADD_BUTTON, hInst, NULL
        );

        // Remove button
        CreateWindowExW(
            0, L"BUTTON", L"Remove",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            340, 10, 50, 25,
            hWnd, (HMENU)IDC_REMOVE_BUTTON, hInst, NULL
        );

        // List box to display tasks
        hList = CreateWindowExW(
            WS_EX_CLIENTEDGE, L"LISTBOX", NULL,
            WS_CHILD | WS_VISIBLE | LBS_STANDARD | WS_VSCROLL,
            10, 45, 380, 200,
            hWnd, (HMENU)IDC_TASK_LIST, hInst, NULL
        );
        break;

    case WM_COMMAND: {
        int wmId    = LOWORD(wParam);
        int wmEvent = HIWORD(wParam);

        if (wmId == IDC_ADD_BUTTON && wmEvent == BN_CLICKED) {
            // Read text from edit
            wchar_t buf[256];
            GetWindowTextW(hEdit, buf, _countof(buf));
            if (wcslen(buf) > 0) {
                SendMessageW(hList, LB_ADDSTRING, 0, (LPARAM)buf);
                SetWindowTextW(hEdit, L"");
            }
        }
        else if (wmId == IDC_REMOVE_BUTTON && wmEvent == BN_CLICKED) {
            int idx = (int)SendMessageW(hList, LB_GETCURSEL, 0, 0);
            if (idx != LB_ERR) {
                SendMessageW(hList, LB_DELETESTRING, (WPARAM)idx, 0);
            }
        }
        break;
    }

    case WM_SIZE:
        // Responsive resizing
        {
            RECT rc;
            GetClientRect(hWnd, &rc);
            // edit: full width minus buttons
            SetWindowPos(hEdit, NULL, 10, 10, rc.right - 140, 25, SWP_NOZORDER);
            SetWindowPos(GetDlgItem(hWnd, IDC_ADD_BUTTON), NULL, rc.right - 120, 10, 50, 25, SWP_NOZORDER);
            SetWindowPos(GetDlgItem(hWnd, IDC_REMOVE_BUTTON), NULL, rc.right - 60, 10, 50, 25, SWP_NOZORDER);
            SetWindowPos(hList, NULL, 10, 45, rc.right - 20, rc.bottom - 55, SWP_NOZORDER);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProcW(hWnd, message, wParam, lParam);
    }
    return 0;
}
