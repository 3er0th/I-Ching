
/* README.md

I Ching, also known as the Book of Changes, is an ancient Chinese divination text that has been used for ~3 millennia to provide guidance and insight into various aspects of life. 
It consists of 64 hexagrams, each made up of six lines that can be either broken (yin) or unbroken (yang). The hexagrams are used to represent different situations and their 
potential outcomes, and they are often consulted for decision-making and understanding the flow of events.

This program is an implementation of the I Ching that employs hardware-based entropy, sourced from Johnson-Nyquist thermal noise to generate high-quality random numbers.

See the RDSEED instruction mentioned in this Wikipedia article for more information.
https://en.wikipedia.org/wiki/RDRAND

Leibniz, the co-inventor of calculus with Newton, was inspired by the I Ching, particularly its binary system, which influenced the development of modern binary code used 
in computing. He saw connections between the I Ching's hexagrams and binary representation, linking ancient Chinese philosophy to contemporary mathematics and technology.

https://therealsamizdat.com/2016/07/15/eco-the-i-ching-and-the-binary-calculus/

In particular, see Figure 14.1 and the associated text. Binary from thousands of years ago.
*/

//
// 20260311 1.0.0.1 - Initial release
// 20260312 1.0.0.2 - Optimised radio button WM_PAINT
// 20260315 1.0.0.3 - Changed some nameing. Some small code optimisations. Switched compiler optimisation from size to speed.
//
 
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define _NO_CRT_STDIO_INLINE

#include <windows.h>
#include <CommCtrl.h>
#include <stdio.h>
#include <richedit.h>
#include <shlwapi.h>
#include <intrin.h>
#include <dwmapi.h>
#include "resource.h"

#pragma region defines consts etc

#define RECT_CX(rc)     ((rc).right  - (rc).left)
#define RECT_CY(rc)     ((rc).bottom - (rc).top)

#define MAIN_CLIENT_CX	(600)
#define MAIN_CLIENT_CY  (918)

#define UI_PAD          (12)

#define CMD_CX          (64)
#define CMD_CY          (64)

#define EDIT_CY	        (20)
#define LINE_CY         (16)

#define EDIT_CX         (MAIN_CLIENT_CX - (UI_PAD * 2))

#define MAIN_CMD_Y      (UI_PAD)
#define MAIN_CMD_CAST_X (UI_PAD)
#define MAIN_CMD_BACK_X (UI_PAD + (CMD_CX + UI_PAD) * 1)
#define MAIN_CMD_SAVE_X (UI_PAD + (CMD_CX + UI_PAD) * 2)
#define MAIN_CMD_OPEN_X (UI_PAD + (CMD_CX + UI_PAD) * 3)

#define EDIT_Y          (UI_PAD +  CMD_CY + UI_PAD)

#define HEX_X           (UI_PAD)
#define HEX_Y           (EDIT_Y + EDIT_CY + UI_PAD)
#define HEX_CX          (MAIN_CLIENT_CX / 2) - (INT)(1.5 * UI_PAD)
#define HEX_CY          (HEX_CX)
#define CHG_X           (UI_PAD + HEX_CX + UI_PAD)
#define CHG_Y           (HEX_Y)
#define CHG_CX          (HEX_CX)
#define CHG_CY          (HEX_CY)

#define HEX_NAME_X      (HEX_X + UI_PAD)
#define HEX_NAME_Y      (HEX_Y + UI_PAD)
#define CHG_NAME_X      (CHG_X + UI_PAD)
#define CHG_NAME_Y      (HEX_NAME_Y)
#define NAME_CX         (HEX_CX - (2 * UI_PAD))

#define HEX_LINE_X      (HEX_NAME_X)
#define CHG_LINE_X      (CHG_NAME_X)

#define LINE_6_Y        (HEX_NAME_Y + (UI_PAD * 4))
#define LINE_5_Y        (LINE_6_Y + (UI_PAD * 3))
#define LINE_4_Y        (LINE_5_Y + (UI_PAD * 3))
#define LINE_3_Y        (LINE_4_Y + (UI_PAD * 3))
#define LINE_2_Y        (LINE_3_Y + (UI_PAD * 3))
#define LINE_1_Y        (LINE_2_Y + (UI_PAD * 3))
#define LINE_CX         (HEX_CX - (3 * UI_PAD))

#define TXT_NAME_X      (UI_PAD)
#define TXT_NAME_Y      (HEX_Y + HEX_CY + UI_PAD)
#define TXT_NAME_CX     (NAME_CX)
#define TXT_NAME_CY     (LINE_CY)

#define TEXT_X          (UI_PAD)
#define TEXT_Y          (TXT_NAME_Y + TXT_NAME_CY + UI_PAD)
#define TEXT_CX         (EDIT_CX)
#define TEXT_CY         (MAIN_CLIENT_CY - TEXT_Y - UI_PAD)

#define LIST_ITEM(hwnd) (ListView_GetNextItem((hwnd), -1, LVNI_SELECTED))

enum e_hwnd {
    HWND_MAIN,
    HWND_CMD_CAST,
    HWND_CMD_BACK,
    HWND_CMD_SAVE,
    HWND_CMD_OPEN,
    HWND_QUERY,
    HWND_TEXT,
    HWND_OPT_HEXAGRAM,
    HWND_OPT_HEX_LINE6,
    HWND_OPT_HEX_LINE5,
    HWND_OPT_HEX_LINE4,
    HWND_OPT_HEX_LINE3,
    HWND_OPT_HEX_LINE2,
    HWND_OPT_HEX_LINE1,
    HWND_OPT_CHANGING,
    HWND_OPT_CHG_LINE6,
    HWND_OPT_CHG_LINE5,
    HWND_OPT_CHG_LINE4,
    HWND_OPT_CHG_LINE3,
    HWND_OPT_CHG_LINE2,
    HWND_OPT_CHG_LINE1,
    HWND_OPEN,
    HWND_OPEN_LIST,
    HWND_OPEN_OPEN,
    HWND_OPEN_CANCEL,
    HWND_COUNT,
};

enum e_rect {
    RECT_HEX,
    RECT_CHG,
    RECT_NAME,
    RECT_COUNT,
};

enum e_font {
    FONT_NAME,
    FONT_TEXT,
    FONT_COUNT,
};

enum e_icon {
    ICON_CMD_CAST_SET,
    ICON_CMD_BACK_SET,
    ICON_CMD_SAVE_SET,
    ICON_CMD_OPEN_SET,
    ICON_CMD_CAST_OFF,
    ICON_CMD_BACK_OFF,
    ICON_CMD_SAVE_OFF,
    ICON_OPT_OFF ,
    ICON_OPT_SET,
    ICON_LINE_6,
    ICON_LINE_7,
    ICON_LINE_8,
    ICON_LINE_9,
    ICON_NUMBER_1,
    ICON_NUMBER_2,
    ICON_NUMBER_3,
    ICON_NUMBER_4,
    ICON_NUMBER_5,
    ICON_NUMBER_6,
    ICON_NUMBER_7,
    ICON_NUMBER_8,
    ICON_NUMBER_9,
    ICON_COUNT,
};

enum e_btn {
    CMD_CAST,
    CMD_BACK,
    CMD_SAVE,
    CMD_OPEN,
    CMD_COUNT,
};

enum e_btn_state {
    CMD_NORMAL,
    CMD_OVER,
    CMD_DOWN,
};

#pragma endregion

#pragma region global data and forward function declarations

const BYTE bin_to_wen[64] = { 2, 24, 7, 19, 15, 36, 46, 11, 16, 51, 40, 54, 62, 55, 32, 34, 8, 3, 29, 60, 39, 63, 48, 5, 45, 17, 47, 58, 31, 49, 28, 43, 23, 27, 4, 41, 52, 22, 18, 26, 35, 21, 64, 38, 56, 30, 50, 14, 20, 42, 59, 61, 53, 37, 57, 9, 12, 25, 6, 10, 33, 13, 44, 1 };

typedef struct {
    HINSTANCE hInstance;              // Handle to the application instance, used for loading resources and creating windows
    HWND      hwnd[HWND_COUNT];       // Main window, command buttons, query edit, text area, options, load dialog and its controls
    HFONT     font[FONT_COUNT];       // Fonts for hexagram name and text
    RECT      rect[RECT_COUNT];       // Rectangles for the hexagram, the changed hexagram and name areas
    HICON     icon[ICON_COUNT];       // Icons for command buttons, options and line representations
    BYTE      btn_enabled[CMD_COUNT]; // Button enabled or disabled
    BYTE      btn_state[CMD_COUNT];   // State of each button (normal, hover or pressed)
    BYTE      wen_to_bin[64];         // Mapping from wen (the traditional numbering of hexagrams) to bin (the binary representation used in the program)
    HCURSOR   hCurHelp;               // Help cursor for clickable options
    FILETIME  ft;                     // Timestamp of the reading
    BOOL      close;                  // Indicates when the current window message loop should bail
    DWORD     seed;                   // The random seed obtained via RDSEED, used for casting hexagrams
    DWORD     hex_bits;               // The binary representation of the main hexagram, where each bit represents a line (0 for yin, 1 for yang)
    DWORD     chg_bits;               // The binary representation of the changing lines, where each bit represents a line that is changing (1 for changing, 0 for static)
    UINT      hex_num;                // The number of the main hexagram (1-64)
    UINT      chg_num;                // The number of the changed hexagram (1-64)
    UINT      opt_num;                // The currently selected option (hexagram, changed hexagram or changing lines)
    UINT      opt_prev;               // The previously selected option
    PWCHAR    lines[6];               // Pointers to the text for each of the moving/changing lines of the main hexagram
    PWCHAR    hex_text;               // Pointer to the text for the main hexagram
    PWCHAR    chg_text;               // Pointer to the text for the changed hexagram
    WCHAR     query[260];             // The user's input, either a hexagram number or a text query
    WCHAR     buf_hex[6 * 1024];      // Buffer for the hexagram text
    WCHAR     buf_chg[6 * 1024];      // Buffer for the changed hexagram text
    WCHAR     ini_file[MAX_PATH];     // Path to the 'I Ching.ini' file for the text and saved readings
} Global_t;

Global_t g;

// RDSEED.asm
EXTERN_C BOOL    rdseed_support(VOID);
EXTERN_C DWORD   get_seed      (VOID);

// forward declarations
VOID             cmd_back      (VOID);
VOID             cmd_cast      (VOID);
VOID             cmd_open      (VOID);
VOID             cmd_save      (VOID);
VOID             create_open   (HWND hwnd);
VOID             create_main   (HWND hwnd);
VOID             calc_hex_all  (VOID);
VOID             calc_hex_num  (DWORD seed, DWORD& hex, DWORD& chg);
VOID             get_hex_text  (VOID);
BOOL             file_exists   (PCWSTR path);
VOID             open_item     (VOID);
VOID             paint_main    (HWND hwnd);
LRESULT CALLBACK subproc_cmd   (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK subproc_edit  (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK subproc_opt   (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK wndproc_main  (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK wndproc_open  (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
#pragma endregion

#pragma region application entrypoint and window procedure

// The entry point of the application. It checks for RDSEED support, loads resources, initializes the main window, and starts the message loop to handle user interactions.
//=================================================================================================================================================================================
EXTERN_C __declspec(noreturn) void __stdcall WinMainCRTStartup(void) {
    if (!rdseed_support()) {
        MessageBoxW(0, L"This computer's processor lacks support for the RDSEED instruction that's used by the casting technique.\n\nMinimum requirements:\n    INTEL Ivy Bridge or newer\n    AMD  Ryzen or newer", L"I Ching", MB_ICONSTOP);
        ExitProcess(ERROR_NOT_SUPPORTED);
    }

    g.hInstance = GetModuleHandleW(0);

    GetModuleFileNameW(0, g.ini_file, _countof(g.ini_file));
    PathRenameExtensionW(g.ini_file, L".ini");

    if (!file_exists(g.ini_file)) {
        WCHAR msg[MAX_PATH + 180];

        swprintf_s(msg, _countof(msg), L"Couldn't find the configuration file 'I Ching.ini' in the application directory.\n\nExpected path:\n%s\n\nMake sure the file exists and is in the correct location.", g.ini_file);
        MessageBoxW(0, msg, L"I Ching", MB_ICONERROR);
        ExitProcess(ERROR_FILE_NOT_FOUND);
    }

    for (INT i = 0; i < 64; i++) // Create the reverse mapping from wen to bin
        g.wen_to_bin[bin_to_wen[i] - 1] = i;

    for (INT ico_idx = ICON_CMD_CAST_SET, res_id = IDI_CMD_CAST; ico_idx <= ICON_NUMBER_9; ico_idx++, res_id++)
        g.icon[ico_idx] = LoadIconW(g.hInstance, MAKEINTRESOURCEW(res_id));

    g.opt_num  = HWND_OPT_HEXAGRAM;
    g.hCurHelp = LoadCursorW(0, IDC_HELP);

    INITCOMMONCONTROLSEX icc = { sizeof(INITCOMMONCONTROLSEX), ICC_LISTVIEW_CLASSES|ICC_TAB_CLASSES|ICC_STANDARD_CLASSES };
    InitCommonControlsEx(&icc);

    LoadLibraryW(L"RICHED20");

    WNDCLASSEXW wcex   = { sizeof(wcex) };
    wcex.hbrBackground = CreateSolidBrush(RGB(231, 208, 177));
    wcex.hCursor       = LoadCursorW(0, (LPCWSTR)IDC_ARROW);
    wcex.hIcon         = g.icon[ICON_CMD_CAST_SET];
    wcex.hInstance     = g.hInstance;
    wcex.lpfnWndProc   = wndproc_main;
    wcex.lpszClassName = L"I-Ching";

    if (RegisterClassExW(&wcex)) {
        HWND hwnd;

        g.hwnd[HWND_MAIN] = hwnd = CreateWindowExW(0, wcex.lpszClassName, wcex.lpszClassName, WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX, 0, 0, 0, 0, 0, 0, g.hInstance, 0);

        if (hwnd) {
            BOOL dark = TRUE;

            DwmSetWindowAttribute(hwnd, DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof(dark));
            ShowWindow(hwnd, SW_NORMAL);
            SetFocus(g.hwnd[HWND_QUERY]);

            while (!g.close) {
                MSG msg;

                if (GetMessageW(&msg, hwnd, 0, 0)) {
                    TranslateMessage(&msg);
                    DispatchMessageW(&msg);
                }
            }

            ShowWindow(hwnd, SW_HIDE);
        }
    }

    ExitProcess(ERROR_SUCCESS);
}

// The window procedure for the main window.
//==================================================================================================================================================================================
LRESULT CALLBACK wndproc_main(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        create_main(hwnd);
        break;
    
    case WM_CLOSE:
        g.close = TRUE;
        break;

    case WM_PAINT:
        paint_main(hwnd);
        break;

    default:
        return DefWindowProcW(hwnd, message, wParam, lParam);
    }

    return 0;
}
#pragma endregion

#pragma region main window and control events

// Initializes the main window and its controls.
//=================================================================================================================================================================================
VOID create_main(HWND hwnd_main) {
    POINT       ptCursor;
    RECT        rcWindow;
    RECT        rcClient;
    HWND        hwnd;
    PARAFORMAT  pf = { sizeof(pf) };
    MONITORINFO mi = { sizeof(mi) };

    GetCursorPos(&ptCursor);

    HMONITOR hMonitor = MonitorFromPoint(ptCursor, MONITOR_DEFAULTTOPRIMARY);

    GetMonitorInfoW(hMonitor, &mi);

    GetWindowRect(hwnd_main, &rcWindow);
    GetClientRect(hwnd_main, &rcClient);

    INT cx = MAIN_CLIENT_CX +  RECT_CX(rcWindow) - RECT_CX(rcClient);
    INT cy = MAIN_CLIENT_CY +  RECT_CY(rcWindow) - RECT_CY(rcClient);
    INT  x = mi.rcWork.left + (RECT_CX(mi.rcWork) - cx) / 2;
    INT  y = mi.rcWork.top  + (RECT_CY(mi.rcWork) - cy) / 3;

    SetWindowPos(hwnd_main, 0, x, y, cx, cy, SWP_NOZORDER);

    g.font[FONT_NAME] = CreateFontW(-14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Segoe UI");
    g.font[FONT_TEXT] = CreateFontW(-13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Verdana");
 
    g.btn_enabled[CMD_CAST] = 1;
    g.btn_enabled[CMD_OPEN] = 1;

    g.hwnd[HWND_CMD_CAST] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|SS_OWNERDRAW, MAIN_CMD_CAST_X, MAIN_CMD_Y, CMD_CX, CMD_CY, hwnd_main, (HMENU)HWND_CMD_CAST, 0, 0);
    SetWindowSubclass(hwnd, &subproc_cmd, HWND_CMD_CAST, 0);

    g.hwnd[HWND_CMD_BACK] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, MAIN_CMD_BACK_X, MAIN_CMD_Y, CMD_CX, CMD_CY, hwnd_main, (HMENU)HWND_CMD_BACK, 0, 0);
    SetWindowSubclass(hwnd, &subproc_cmd, HWND_CMD_BACK, 1);

    g.hwnd[HWND_CMD_SAVE] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, MAIN_CMD_SAVE_X, MAIN_CMD_Y, CMD_CX, CMD_CY, hwnd_main, (HMENU)HWND_CMD_SAVE, 0, 0);
    SetWindowSubclass(hwnd, &subproc_cmd, HWND_CMD_SAVE, 2);

    g.hwnd[HWND_CMD_OPEN] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, MAIN_CMD_OPEN_X, MAIN_CMD_Y, CMD_CX, CMD_CY, hwnd_main, (HMENU)HWND_CMD_OPEN, 0, 0);
    SetWindowSubclass(hwnd, &subproc_cmd, HWND_CMD_OPEN, 3);

    g.hwnd[HWND_QUERY]    = hwnd = CreateWindowExW(0, L"EDIT",   0, WS_CHILD|WS_BORDER|WS_VISIBLE|ES_NOHIDESEL, UI_PAD, EDIT_Y, EDIT_CX, EDIT_CY,  hwnd_main,  (HMENU)HWND_QUERY, 0, 0);
    SendMessageW(hwnd, WM_SETFONT, (WPARAM)g.font[FONT_TEXT], 0);
    SendMessageW(hwnd, EM_SETCUEBANNER, TRUE, (LPARAM)L"Enter a query or hexagram number from 1 to 64");
    SetWindowSubclass(hwnd, &subproc_edit, HWND_QUERY, 0);

    g.hwnd[HWND_OPT_HEXAGRAM]  = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, HEX_NAME_X, HEX_NAME_Y, NAME_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_HEXAGRAM,  0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, HWND_OPT_HEXAGRAM,  0x010000);    

    g.hwnd[HWND_OPT_HEX_LINE6] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, HEX_LINE_X, LINE_6_Y,   LINE_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_HEX_LINE6, 0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, HWND_OPT_HEX_LINE6, 0x010600 | 1 << 5);                                                                                       
                                                                                                                                            
    g.hwnd[HWND_OPT_HEX_LINE5] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, HEX_LINE_X, LINE_5_Y,   LINE_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_HEX_LINE5, 0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, HWND_OPT_HEX_LINE5, 0x010500 | 1 << 4);                                                                                       
                                                                                                                                            
    g.hwnd[HWND_OPT_HEX_LINE4] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, HEX_LINE_X, LINE_4_Y,   LINE_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_HEX_LINE4, 0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, HWND_OPT_HEX_LINE4, 0x010400 | 1 << 3);                                                                                       
                                                                                                                                            
    g.hwnd[HWND_OPT_HEX_LINE3] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, HEX_LINE_X, LINE_3_Y,   LINE_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_HEX_LINE3, 0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, HWND_OPT_HEX_LINE3, 0x010300 | 1 << 2);                                                                                       
                                                                                                                                            
    g.hwnd[HWND_OPT_HEX_LINE2] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, HEX_LINE_X, LINE_2_Y,   LINE_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_HEX_LINE2, 0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, HWND_OPT_HEX_LINE2, 0x010200 | 1 << 1);                                                                                       
                                                                                                                                            
    g.hwnd[HWND_OPT_HEX_LINE1] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, HEX_LINE_X, LINE_1_Y,   LINE_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_HEX_LINE1, 0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, HWND_OPT_HEX_LINE1, 0x010100 | 1 << 0);                                                                                       
                                                                                                                                            
    g.hwnd[HWND_OPT_CHANGING]  = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CHG_NAME_X, CHG_NAME_Y, NAME_CX, LINE_CY, hwnd_main, (HMENU)HWND_OPT_CHANGING,  0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, HWND_OPT_CHANGING, 0);                                                                                            
                                                                                                                                            
    g.hwnd[HWND_OPT_CHG_LINE6] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CHG_LINE_X,  LINE_6_Y,  LINE_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_CHG_LINE6, 0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, HWND_OPT_CHG_LINE6, 0x000600);                                                                                            
                                                                                                                                            
    g.hwnd[HWND_OPT_CHG_LINE5] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CHG_LINE_X,  LINE_5_Y,  LINE_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_CHG_LINE5, 0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, HWND_OPT_CHG_LINE5, 0x000500);                                                                                            
                                                                                                                                            
    g.hwnd[HWND_OPT_CHG_LINE4] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CHG_LINE_X,  LINE_4_Y,  LINE_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_CHG_LINE4, 0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, HWND_OPT_CHG_LINE4, 0x000400);                                                                                           
                                                                                                                                            
    g.hwnd[HWND_OPT_CHG_LINE3] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CHG_LINE_X,  LINE_3_Y,  LINE_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_CHG_LINE3, 0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, HWND_OPT_CHG_LINE3, 0x000300);                                                                                           
                                                                                                                                            
    g.hwnd[HWND_OPT_CHG_LINE2] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CHG_LINE_X,  LINE_2_Y,  LINE_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_CHG_LINE2, 0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, HWND_OPT_CHG_LINE2, 0x000200);                                                                                           
                                                                                                                                            
    g.hwnd[HWND_OPT_CHG_LINE1] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CHG_LINE_X,  LINE_1_Y,  LINE_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_CHG_LINE1, 0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, HWND_OPT_CHG_LINE1, 0x000100);

    g.hwnd[HWND_TEXT] = hwnd = CreateWindowExW(0, RICHEDIT_CLASSW, 0, WS_CHILD|WS_VSCROLL|ES_MULTILINE|WS_VISIBLE, TEXT_X, TEXT_Y, TEXT_CX, TEXT_CY, hwnd_main, (HMENU)HWND_TEXT, 0, 0);
    SendMessageW(hwnd, WM_SETFONT, (WPARAM)g.font[FONT_TEXT], 0);
    SendMessageW(hwnd, EM_SETBKGNDCOLOR, 0, RGB(231, 208, 177));
    SendMessageW(hwnd, EM_SETTYPOGRAPHYOPTIONS, TO_ADVANCEDTYPOGRAPHY, TO_ADVANCEDTYPOGRAPHY);
    SendMessageW(hwnd, EM_SETREADONLY, TRUE, 0);

    pf.dwMask     = PFM_ALIGNMENT|PFM_TABSTOPS;
    pf.cTabCount  = 1;
    pf.rgxTabs[0] = 360;
    pf.wAlignment = PFA_FULL_INTERWORD;
    SendMessageW(hwnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf);

    g.rect[RECT_HEX]  = { HEX_X,      HEX_Y,      HEX_X + HEX_CX,           HEX_Y + HEX_CY  };
    g.rect[RECT_CHG]  = { CHG_X,      CHG_Y,      CHG_X + CHG_CX,           CHG_Y + CHG_CY  };
    g.rect[RECT_NAME] = { TXT_NAME_X, TXT_NAME_Y, TXT_NAME_X + TXT_NAME_CX, TXT_NAME_Y + TXT_NAME_CY };
}

// Handles the painting of the main window.
//==================================================================================================================================================================================
VOID paint_main(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC         hdc    = BeginPaint(hwnd, &ps);
    HBRUSH      hBrush = CreateSolidBrush(RGB(116, 116, 116));

    // hexagram and change rectangles
    FrameRect(hdc, &g.rect[RECT_HEX], hBrush);
    FrameRect(hdc, &g.rect[RECT_CHG], hBrush);

    if (g.hex_num) {
        PWSTR p;

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, 0);
        SelectObject(hdc, g.font[FONT_NAME]);

        p = (g.opt_num == HWND_OPT_CHANGING) ? g.buf_chg : g.buf_hex;
        
        TextOutW(hdc, g.rect[RECT_NAME].left, g.rect[RECT_NAME].top, p, (INT)wcslen(p));
    }
        
    DeleteObject(hBrush);
    EndPaint(hwnd, &ps);
}

// Handles the interaction with the command buttons (cast, back, save and load).
//=================================================================================================================================================================================
LRESULT CALLBACK subproc_cmd(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
    BOOL enabled = g.btn_enabled[dwRefData];

    if (!enabled && message != WM_PAINT)
        return DefSubclassProc(hwnd, message, wParam, lParam);

    switch (message) {
    case WM_LBUTTONDOWN:
        g.btn_state[dwRefData] = CMD_DOWN;
        InvalidateRect(hwnd, 0, FALSE);
        break;

    case WM_LBUTTONUP:
        switch (uIdSubclass) {
        case HWND_CMD_CAST:
            cmd_cast();
            g.btn_state[dwRefData] = g.btn_enabled[dwRefData] ? CMD_OVER : CMD_NORMAL;
            break;

        case HWND_CMD_BACK:
            cmd_back();
            g.btn_state[dwRefData] = g.btn_enabled[dwRefData] ? CMD_OVER : CMD_NORMAL;
            break;

        case HWND_CMD_SAVE:
            cmd_save();
            g.btn_state[dwRefData] = CMD_NORMAL;
            g.btn_enabled[dwRefData] = 0;
            break;

        case HWND_CMD_OPEN:
            cmd_open();
            break;
        }

        InvalidateRect(hwnd, 0, FALSE);
        break;

    case WM_MOUSEMOVE:
        if (g.btn_state[dwRefData] == CMD_NORMAL) {
            TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, hwnd, 0 };

            g.btn_state[dwRefData] = CMD_OVER;
            TrackMouseEvent(&tme);
            InvalidateRect(hwnd, 0, FALSE);
        } break;

    case WM_MOUSELEAVE:
        g.btn_state[dwRefData] = CMD_NORMAL;
        InvalidateRect(hwnd, 0, FALSE);
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        INT         icon;
        INT         delta;
        UINT        edge;
        UINT        flags;
        HDC         hdc    = BeginPaint(hwnd, &ps);
        HBRUSH      hBrush = CreateSolidBrush(RGB(255, 255, 255));

        FillRect(hdc, &ps.rcPaint, hBrush);

        switch (dwRefData) {
            case 0: icon = enabled ? ICON_CMD_CAST_SET : ICON_CMD_CAST_OFF; break;  
            case 1: icon = enabled ? ICON_CMD_BACK_SET : ICON_CMD_BACK_OFF; break;
            case 2: icon = enabled ? ICON_CMD_SAVE_SET : ICON_CMD_SAVE_OFF; break;
            case 3: icon =           ICON_CMD_OPEN_SET;                     break;
        }

        switch (g.btn_state[dwRefData]) {
        case CMD_NORMAL:
            delta = 0;
            edge  = EDGE_ETCHED;
            flags = BF_RECT|BF_MONO;
            break;

        case CMD_OVER:
            delta = -2;
            edge  = EDGE_RAISED;
            flags = BF_RECT;
            break;

        case CMD_DOWN:
            delta = +2;
            edge  = EDGE_SUNKEN;
            flags = BF_RECT;
            break;
        }

        DrawIconEx(hdc, 8 + delta, 8 + delta, g.icon[icon], 48, 48, 0, 0, DI_NORMAL);
        DrawEdge(hdc, &ps.rcPaint, edge, flags);

        DeleteObject(hBrush);
        EndPaint(hwnd, &ps);
    } break;

    default:
        return DefSubclassProc(hwnd, message, wParam, lParam);
    }

    return 0;
}

// Handles the keyboard input for the query edit control.
//=================================================================================================================================================================================
LRESULT CALLBACK subproc_edit(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
    if (wParam == VK_RETURN) {
        if (message == WM_KEYUP)
            cmd_cast();
        else
        if (message == WM_CHAR)
            return 0; // prevent beep
    }

    return DefSubclassProc(hwnd, message, wParam, lParam);
}


// Handles the interaction with the hexagram and changing lines option buttons.
//================================================================================================================================================================================
LRESULT CALLBACK subproc_opt(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
    if (message == WM_ERASEBKGND)   
        return 0;

    BOOL button;

    if (uIdSubclass == HWND_OPT_HEXAGRAM)
        button = TRUE;
    else
    if (uIdSubclass == HWND_OPT_CHANGING)
        button = g.chg_bits;
    else
        button = dwRefData & g.chg_bits;

    if (!button && message != WM_PAINT)
        return DefSubclassProc(hwnd, message, wParam, lParam);

    UINT line = (dwRefData >>  8) & 0xFF; // line number
    BOOL main = (dwRefData >> 16) & 0xFF; // main hexagram else change hexagram

    switch (message) {
    case WM_LBUTTONDOWN:
        if (g.opt_num != uIdSubclass) {
            PWSTR p = NULL;

            g.opt_prev = g.opt_num;
            g.opt_num  = (BYTE)uIdSubclass;

            switch (uIdSubclass) {
            case HWND_OPT_HEXAGRAM: 
                p = g.hex_text;
                break;

            case HWND_OPT_CHANGING: 
                p = g.chg_text;
                break;

            case HWND_OPT_HEX_LINE6:
            case HWND_OPT_HEX_LINE5:
            case HWND_OPT_HEX_LINE4:
            case HWND_OPT_HEX_LINE3:
            case HWND_OPT_HEX_LINE2:
            case HWND_OPT_HEX_LINE1:
                p = g.lines[line - 1];
                break;
            }

            SetWindowTextW(g.hwnd[HWND_TEXT], p);

            InvalidateRect(hwnd, 0, FALSE);
            InvalidateRect(g.hwnd[g.opt_prev], 0, FALSE);

            if (g.opt_prev == HWND_OPT_CHANGING || g.opt_num == HWND_OPT_CHANGING)   
                InvalidateRect(g.hwnd[HWND_MAIN], &g.rect[RECT_NAME], TRUE);
        }
        break;

    case WM_MOUSEMOVE:
        if (g.opt_num != uIdSubclass) {
            TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, hwnd, 0 };

            TrackMouseEvent(&tme);
            SetCursor(g.hCurHelp);
        } break;

    case WM_MOUSELEAVE:
        SetCursor(0);
        break;

    case WM_PAINT:
        if (g.hex_num) {
            PAINTSTRUCT ps;
            HDC         hdc = BeginPaint(hwnd, &ps);
            PWSTR       p   = NULL;

            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, 0);

            if (button) {
                if (uIdSubclass == g.opt_num)
                    DrawIconEx(hdc, 0, 0, g.icon[ICON_OPT_SET], 16, 16, 0, 0, DI_NORMAL);
                else
                    DrawIconEx(hdc, 0, 0, g.icon[ICON_OPT_OFF], 16, 16, 0, 0, DI_NORMAL);

                switch (uIdSubclass) {
                case HWND_OPT_HEXAGRAM:
                    p = g.buf_hex;
                    break;

                case HWND_OPT_CHANGING:
                    p = g.buf_chg;
                    break;
                }

                if (p) {
                    SelectObject(hdc, g.font[FONT_NAME]);
                    TextOutW(hdc, 24, 0, p, (int)wcslen(p));
                }
            }

            if (line--) {
                BOOL hex_bit = g.hex_bits & 1 << line;
                BOOL chg_bit = g.chg_bits & 1 << line;
                UINT ico_idx = 0;

                if (main)
                    ico_idx = chg_bit ? (hex_bit ? ICON_LINE_9 : ICON_LINE_6) : (hex_bit ? ICON_LINE_7 : ICON_LINE_8);
                else
                if (g.chg_bits)
                    ico_idx = chg_bit ? (hex_bit ? ICON_LINE_8 : ICON_LINE_7) : (hex_bit ? ICON_LINE_7 : ICON_LINE_8);

                if (ico_idx) {
                    DrawIconEx(hdc,  32, 3, g.icon[ICON_NUMBER_1 + line],                    8, 10, 0, 0, DI_NORMAL);
                    DrawIconEx(hdc,  56, 3, g.icon[ico_idx],                               140, 10, 0, 0, DI_NORMAL);
                    DrawIconEx(hdc, 212, 3, g.icon[ICON_NUMBER_6 - ICON_LINE_6 + ico_idx],   8, 10, 0, 0, DI_NORMAL);
                }
            } break;
    
            EndPaint(hwnd, &ps);
        } break;

    default:
        return DefSubclassProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
#pragma endregion

#pragma region button commands

// Executes the casting process to generate a hexagram based on the user's query or a specified hexagram number.
//==========================================================================================================================================================
VOID cmd_cast(VOID) {
    g.seed = get_seed();

    SendMessageW(g.hwnd[HWND_QUERY], WM_GETTEXT, _countof(g.query), (LPARAM)g.query);
 
    g.hex_num = _wtoi(g.query);

    if (g.hex_num >= 1 && g.hex_num <= 64 && wcslen(g.query) < 3) {
        g.seed     = 0;
        g.hex_bits = g.wen_to_bin[g.hex_num - 1];
        g.chg_bits = 0b111111;
        g.chg_num  = bin_to_wen[g.hex_bits ^ g.chg_bits];
    } else {
        GetSystemTimeAsFileTime(&g.ft);
        calc_hex_all();
        g.btn_enabled[CMD_SAVE] = 1;
        InvalidateRect(g.hwnd[HWND_CMD_SAVE], 0, FALSE);
    }

    g.btn_enabled[CMD_CAST] = 0;
    g.btn_enabled[CMD_BACK] = 1;

    get_hex_text();

    SetWindowTextW(g.hwnd[HWND_TEXT], g.hex_text);
    SendMessageW(g.hwnd[HWND_QUERY], EM_SETREADONLY, TRUE, 0);
    SetFocus(g.hwnd[HWND_MAIN]);
    
    InvalidateRect(g.hwnd[HWND_CMD_CAST], 0, FALSE);
    InvalidateRect(g.hwnd[HWND_CMD_BACK], 0, FALSE);
    InvalidateRect(g.hwnd[HWND_MAIN], &g.rect[RECT_NAME], FALSE);
}

// Resets the application state to allow for a new reading.
//=========================================================================================================================================================
VOID cmd_back(VOID) {
    g.hex_bits = 0;
    g.hex_num  = 0;
    g.chg_bits = 0;
    g.chg_num  = 0;
    g.opt_prev = 0;
    g.opt_num  = HWND_OPT_HEXAGRAM;

    g.btn_enabled[CMD_CAST] = 1;
    g.btn_enabled[CMD_BACK] = 0;
    g.btn_enabled[CMD_SAVE] = 0;

    SetWindowTextW(g.hwnd[HWND_QUERY], 0);
    SendMessageW(g.hwnd[HWND_QUERY], EM_SETREADONLY, FALSE, 0);
    SetFocus(g.hwnd[HWND_QUERY]);

    SetWindowTextW(g.hwnd[HWND_TEXT], 0);
    UpdateWindow(g.hwnd[HWND_TEXT]);

    InvalidateRect(g.hwnd[HWND_CMD_CAST], 0, FALSE);
    InvalidateRect(g.hwnd[HWND_CMD_BACK], 0, FALSE);
    InvalidateRect(g.hwnd[HWND_CMD_SAVE], 0, FALSE);

    InvalidateRect(g.hwnd[HWND_MAIN], &g.rect[RECT_HEX],  TRUE);
    InvalidateRect(g.hwnd[HWND_MAIN], &g.rect[RECT_CHG],  TRUE);
    InvalidateRect(g.hwnd[HWND_MAIN], &g.rect[RECT_NAME], TRUE);
}

// Saves the reading to the ini file.
//=========================================================================================================================================================
VOID cmd_save(VOID) {
    WCHAR key[10];
    WCHAR buf[400];
    WCHAR query[300];
    INT   count;

    GetWindowTextW(g.hwnd[HWND_QUERY], query, _countof(query));

    swprintf_s(buf, _countof(buf), L"0x%08X 0x%08X 0x%05X %s", g.ft.dwHighDateTime, g.ft.dwLowDateTime, g.seed, query);

    count = GetPrivateProfileIntW(L"Saved", L"SaveCount", 0, g.ini_file);
    count++;

    swprintf_s(key, _countof(key), L"%d", count);

    if (WritePrivateProfileStringW(L"Saved", L"SaveCount", key, g.ini_file))
        WritePrivateProfileStringW(L"Saved", key, buf, g.ini_file);
    else
        MessageBoxW(g.hwnd[HWND_MAIN], L"Failed to save the reading. Make sure the application has permission to write to its directory.", L"Error", MB_ICONERROR);
}

// Creates and manages the load window, which allows the user to view and open previously saved readings.
//=========================================================================================================================================================
VOID cmd_open(VOID) {
    WNDCLASSEXW wcex   = { sizeof(wcex) };
    wcex.hCursor       = LoadCursorW(0, (LPCWSTR)IDC_ARROW);
    wcex.hIcon         = g.icon[ICON_CMD_OPEN_SET];
    wcex.hInstance     = g.hInstance;
    wcex.lpfnWndProc   = wndproc_open;
    wcex.lpszClassName = L"I-CHING_OPEN";

    if (RegisterClassExW(&wcex)) {
        g.hwnd[HWND_OPEN] = CreateWindowExW(0, wcex.lpszClassName, L"Saved readings", WS_CAPTION|WS_SYSMENU, 0, 0, 0, 0, g.hwnd[HWND_MAIN], 0, g.hInstance, 0);

        if (g.hwnd[HWND_OPEN]) {
            EnableWindow(g.hwnd[HWND_MAIN], FALSE);
            ShowWindow(g.hwnd[HWND_OPEN], SW_NORMAL);
            
            while (!g.close) {
                MSG msg;

                if (GetMessageW(&msg, g.hwnd[HWND_OPEN], 0, 0)) {
                    TranslateMessage(&msg);
                    DispatchMessageW(&msg);
                }
            }

            g.close = FALSE;

            EnableWindow(g.hwnd[HWND_MAIN], TRUE);
            DestroyWindow(g.hwnd[HWND_OPEN]);
        }
        
        UnregisterClass(wcex.lpszClassName, g.hInstance);
    }
}
#pragma endregion

#pragma region open window and control events

// Window procedure for the open window. Handles the creation of controls, loading of saved readings, and user interactions within the load window.
//=========================================================================================================================================================
LRESULT CALLBACK wndproc_open(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        create_open(hwnd);
        break;

    case WM_CLOSE:
        g.close = TRUE;
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
            case IDC_LOAD_OPEN:   open_item();     break;
            case IDC_LOAD_CANCEL: g.close = TRUE;  break;
        } 
        break;

    case WM_NOTIFY: {
            auto lpHdr = (LPNMHDR)lParam;

            if (lpHdr->code == NM_CLICK)
                EnableWindow(g.hwnd[HWND_OPEN_OPEN], LIST_ITEM(g.hwnd[HWND_OPEN_LIST]) != -1);
            else
            if (lpHdr->code == NM_DBLCLK && LIST_ITEM(g.hwnd[HWND_OPEN_LIST]) != -1)
                open_item();
        } break;

    default:
        return DefWindowProcW(hwnd, message, wParam, lParam);
    }

    return 0;
}

// Creates the controls for the load window and populates the list with saved readings from the ini file.
//=======================================================================================================
VOID create_open(HWND hwnd_open) {
    RECT        rcClient;
    RECT        rcWindow;
    HWND        hwnd;
    LV_COLUMNW	lvCol = {};

    GetWindowRect(g.hwnd[HWND_MAIN], &rcWindow);

    SetWindowPos(hwnd_open, 0, rcWindow.left, rcWindow.top, RECT_CX(rcWindow), RECT_CY(rcWindow), SWP_NOZORDER);
    GetClientRect(hwnd_open, &rcClient);
    
    g.hwnd[HWND_OPEN_OPEN]   = hwnd = CreateWindowExW(0, L"BUTTON", L"Open", WS_CHILD|WS_VISIBLE|BS_FLAT|BS_PUSHBUTTON, RECT_CX(rcClient) - UI_PAD - 100 - UI_PAD - 100, rcClient.bottom - UI_PAD - 24, 100, 24, hwnd_open, (HMENU)IDC_LOAD_OPEN, 0, 0);
    SendMessageW(hwnd, WM_SETFONT, (WPARAM)g.font[FONT_NAME], 0);
    EnableWindow(hwnd, FALSE);

    g.hwnd[HWND_OPEN_CANCEL] = hwnd = CreateWindowExW(0, L"BUTTON", L"Cancel", WS_CHILD|WS_VISIBLE|BS_FLAT|BS_PUSHBUTTON, RECT_CX(rcClient) - UI_PAD - 100, rcClient.bottom - UI_PAD - 24, 100, 24, hwnd_open, (HMENU)IDC_LOAD_CANCEL, 0, 0);
    SendMessageW(hwnd, WM_SETFONT, (WPARAM)g.font[FONT_NAME], 0);

    g.hwnd[HWND_OPEN_LIST]   = hwnd = CreateWindowExW(0, L"SysListView32", 0,  WS_VISIBLE|WS_CHILD|WS_BORDER|LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS, 0, 0, RECT_CX(rcClient), rcClient.bottom - UI_PAD - 24 - UI_PAD, hwnd_open, (HMENU)HWND_OPEN_LIST, 0, 0);
    SendMessageW(hwnd, WM_SETFONT, (WPARAM)g.font[FONT_TEXT], 0);
    ListView_SetExtendedListViewStyle(hwnd, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_LABELTIP);

    lvCol.mask     = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
    lvCol.pszText  = (PWCHAR)L"Date";
    lvCol.fmt      = LVCFMT_LEFT;
    lvCol.iSubItem = 0;
    ListView_InsertColumn(hwnd, 0, &lvCol);

    lvCol.pszText  = (PWCHAR)L"Time";
    lvCol.iSubItem = 1;
    ListView_InsertColumn(hwnd, lvCol.iSubItem, &lvCol);

    lvCol.pszText  = (PWCHAR)L"Hexagram";
    lvCol.fmt      = LVCFMT_RIGHT;
    lvCol.iSubItem = 2;
    ListView_InsertColumn(hwnd, lvCol.iSubItem, &lvCol);

    lvCol.pszText  = (PWCHAR)L"Change";
    lvCol.iSubItem = 3;
    ListView_InsertColumn(hwnd, lvCol.iSubItem, &lvCol);

    lvCol.pszText  = (PWCHAR)L"Query";
    lvCol.fmt      = LVCFMT_LEFT;
    lvCol.iSubItem = 4;
    ListView_InsertColumn(hwnd, lvCol.iSubItem, &lvCol);

    INT save_count = GetPrivateProfileIntW(L"Saved", L"SaveCount", 0, g.ini_file);

    for (INT i = 1; i <= save_count; i++) {
        WCHAR key[10];
        WCHAR buf[400];

        memset(buf, 0, sizeof(buf));

        swprintf_s(key, _countof(key), L"%d", i);

        if (GetPrivateProfileStringW(L"Saved", key, 0, buf, _countof(buf), g.ini_file)) {
            DWORD      cast;
            DWORD      hex;
            DWORD      chg;
            WCHAR      wcsHex[4];
            WCHAR      wcsDate[20];
            WCHAR      wcsTime[20];
            PWSTR      p      = buf;
            SYSTEMTIME st     = {};
            FILETIME   ft     = {};
            LV_ITEMW   lvItem = {};

            ft.dwHighDateTime = wcstoul(  p, &p, 16);
            ft.dwLowDateTime  = wcstoul(++p, &p, 16);
            cast              = wcstoul(++p, &p, 16);
            p++;

            FileTimeToSystemTime(&ft, &st);
            GetDateFormatW(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &st, 0, wcsDate, _countof(wcsDate));
            GetTimeFormatW(LOCALE_USER_DEFAULT, TIME_NOSECONDS, &st, 0, wcsTime, _countof(wcsTime));

            calc_hex_num(cast, hex, chg);
            swprintf_s(wcsHex, _countof(wcsHex), L"%d", hex);

            lvItem.mask    = LVIF_TEXT|LVIF_PARAM;
            lvItem.lParam  = cast;
            lvItem.pszText = wcsDate;

            ListView_InsertItem(hwnd, &lvItem);
            ListView_SetItemText(hwnd, lvItem.iItem, 1, wcsTime);
            ListView_SetItemText(hwnd, lvItem.iItem, 2, wcsHex);

            if (chg) {
                WCHAR wcsChg[4];

                swprintf_s(wcsChg, _countof(wcsChg), L"%d", chg);
                ListView_SetItemText(hwnd, lvItem.iItem, 3, wcsChg);
            }

            ListView_SetItemText(hwnd, lvItem.iItem, 4, p);
        }
    }

    UINT col_width;
    UINT hdr_width;
    UINT sum   = 0;
    UINT width = 0;

    for (UINT col = 0; col <= 4; col++) {
        ListView_SetColumnWidth(hwnd, col, LVSCW_AUTOSIZE);
        col_width = ListView_GetColumnWidth(hwnd, col);

        ListView_SetColumnWidth(hwnd, col, LVSCW_AUTOSIZE_USEHEADER);
        hdr_width = ListView_GetColumnWidth(hwnd, col); 

        width = max(col_width, hdr_width);
        sum  += width;

        if (col == 4)
            width += (rcClient.right - sum);

        ListView_SetColumnWidth(hwnd, col, width);
    }
}

// Open the selected reading from the list view in the load window.
//=======================================================================================================================================================================================
VOID open_item(VOID) {
    HWND   hwnd = g.hwnd[HWND_OPEN_LIST];
    LVITEM lvi;

    lvi.iItem    = LIST_ITEM(hwnd);
    lvi.mask     = LVIF_PARAM;
    lvi.iSubItem = 0;

    if (ListView_GetItem(hwnd, &lvi) == TRUE) {
        g.seed = (DWORD)lvi.lParam;
    
        lvi.pszText    = g.query;
        lvi.cchTextMax = _countof(g.query);
        lvi.iSubItem   = 4;
        lvi.mask       = LVIF_TEXT;
        
        if (ListView_GetItem(hwnd, &lvi) == TRUE) {
            RECT rc;
            INT  old_num = g.chg_num;

            calc_hex_all();

            g.btn_enabled[CMD_CAST] = 0;
            g.btn_enabled[CMD_BACK] = 1;
            g.btn_enabled[CMD_SAVE] = 0;

            get_hex_text();

            SetWindowTextW(g.hwnd[HWND_QUERY], g.query);
            SendMessageW(g.hwnd[HWND_QUERY], EM_SETREADONLY, TRUE, 0);

            SetWindowTextW(g.hwnd[HWND_TEXT], g.hex_text);
            
            InvalidateRect(g.hwnd[HWND_CMD_CAST], 0, FALSE);
            InvalidateRect(g.hwnd[HWND_CMD_BACK], 0, FALSE);
            InvalidateRect(g.hwnd[HWND_CMD_SAVE], 0, FALSE);

            GetWindowRect(g.hwnd[HWND_OPT_HEXAGRAM], &rc);
            InvalidateRect(g.hwnd[HWND_MAIN], &rc, FALSE);

            for (UINT i = HWND_OPT_HEX_LINE6; i <= HWND_OPT_HEX_LINE1; i++)
                InvalidateRect(g.hwnd[i], 0, FALSE);

            if (old_num) {
                GetWindowRect(g.hwnd[HWND_OPT_CHANGING], &rc);
                InvalidateRect(g.hwnd[HWND_MAIN], &rc, FALSE);

                for (UINT i =  HWND_OPT_CHG_LINE6; i <= HWND_OPT_CHG_LINE1; i++)
                    InvalidateRect(g.hwnd[i], 0, FALSE);
            }

            InvalidateRect(g.hwnd[HWND_MAIN], &g.rect[RECT_NAME], TRUE);
            g.close = TRUE;
        }
    }
}
#pragma endregion

#pragma region hexagram calc, text_retrieval and other support functions

// Calculate the main and changing hexagram numbers, as well as the bitmasks for the hexagram lines and changing lines.
//=======================================================================================================================================================================================
VOID calc_hex_all(VOID) {
    DWORD seed     = g.seed;
    BYTE  hex_bits = 0;
    BYTE  chg_bits = 0;

    for (UINT i = 0, bit = 1; i < 6; i++, bit <<= 1) {
        switch (seed & 0b111) {
        case 3:
            chg_bits |= bit;
            break;

        case 7:
            chg_bits |= bit;
        case 4: case 5: case 6:
            hex_bits |= bit;
            break;
        }

        seed >>= 3;
    }

    g.hex_bits = hex_bits;
    g.chg_bits = chg_bits;
    g.hex_num  = bin_to_wen[hex_bits];
    g.chg_num  = chg_bits ? bin_to_wen[hex_bits ^ chg_bits] : 0;
}

// A limited version of the above function that only calculates the main and change hexagram numbers from a given seed, without modifying any global state. Used in the open list.
//=======================================================================================================================================================================================
VOID calc_hex_num(DWORD seed, DWORD& hex, DWORD& chg) {
    DWORD hex_bits = 0;
    DWORD chg_bits = 0;

    for (UINT i = 0, v = seed, bit = 1; i < 6; i++, bit <<= 1) {
        switch (v & 0b111) {
        case 3:
            chg_bits |= bit;
            break;

        case 7:
            chg_bits |= bit;
        case 4: case 5: case 6:
            hex_bits |= bit;
            break;
        }

        v >>= 3;
    }

    hex = bin_to_wen[hex_bits];
    chg = chg_bits ? bin_to_wen[hex_bits ^ chg_bits] : 0;
}

// Retrieve the text for the main and changed hexagram from the INI file, store pointers to the main hexagram's moving lines. 
//=======================================================================================================================================================================================
VOID get_hex_text(VOID) {
    WCHAR section[32];

    swprintf_s(section, _countof(section), L"Hexagram_%d", g.hex_num);

    if (GetPrivateProfileSectionW(section, g.buf_hex, _countof(g.buf_hex), g.ini_file)) {
        PWSTR p = g.buf_hex;

        for (UINT i = 0; i < 6; i++) {
            p += wcslen(p) + 1;
            g.lines[i] = p;
        }

        p += wcslen(p) + 1;
        g.hex_text = p;
    
        // convert any "$$" sequences in the text to double line breaks for proper formatting in the rich text control, since the INI file cannot contain actual line break characters
        while (p = wcsstr(p, L"$$")) {
            p[0] = L'\r';
            p[1] = L'\r';
        }

        if (g.chg_bits) {
            swprintf_s(section, _countof(section), L"Hexagram_%d", g.chg_num);

            if (GetPrivateProfileSectionW(section, g.buf_chg, _countof(g.buf_chg), g.ini_file)) {
                p = g.buf_chg;

                for (UINT i = 0; i < 6; i++)
                    p += wcslen(p) + 1;

                p += wcslen(p) + 1;
                g.chg_text = p;

                // convert any "$$" sequences in the text to double line breaks for proper formatting in the rich text control, since the INI file cannot contain actual line break characters
                while (p = wcsstr(p, L"$$")) {
                    p[0] = L'\r';
                    p[1] = L'\r';
                }
            }
        }
    }
}

// Return wether the file exists
//======================================================================================================================================================================================
BOOL file_exists(PCWSTR path) {
    DWORD dwAttrib = GetFileAttributesW(path);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
#pragma endregion