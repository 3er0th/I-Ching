
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
// Releases:
// 
// 20260311 1.0.0.1 - Initial release
// 20260312 1.0.0.2 - Optimised radio button WM_PAINT
// 20260315 1.0.0.3 - Changed some nameing. Some small code optimisations. Switched compiler optimisation from size to speed.
// 20260325 1.0.0.4 - Added the ability to delete saved readings
// 20260403 1.0.0.5 - DPI awreness changes
//
 
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(linker, "/section:.text,RWE /merge:.data=.text /merge:.rdata=.text")

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

#define MAIN_CTRL(id)       (GetDlgItem(g.hwnd_main, (id)))
#define OPEN_CTRL(id)       (GetDlgItem(g.hwnd_open, (id)))
#define SELECTED_ITEM(hwnd) (ListView_GetNextItem((hwnd), -1, LVNI_SELECTED))
#define RECT_CX(rc)         ((rc).right  - (rc).left)
#define RECT_CY(rc)         ((rc).bottom - (rc).top)
#define MAIN_CLIENT_CX	    (600)
#define MAIN_CLIENT_CY      (918)
#define UI_PAD              (12)
#define CMD_CX              (64)
#define CMD_CY              (64)
#define EDIT_CY	            (20)
#define LINE_CY             (16)
#define EDIT_CX             (MAIN_CLIENT_CX - (UI_PAD * 2))
#define MAIN_CMD_Y          (UI_PAD)
#define MAIN_CMD_CAST_X     (UI_PAD)
#define MAIN_CMD_BACK_X     (UI_PAD + (CMD_CX + UI_PAD) * 1)
#define MAIN_CMD_SAVE_X     (UI_PAD + (CMD_CX + UI_PAD) * 2)
#define MAIN_CMD_OPEN_X     (UI_PAD + (CMD_CX + UI_PAD) * 3)
#define EDIT_Y              (UI_PAD +  CMD_CY + UI_PAD)
#define HEX_X               (UI_PAD)
#define HEX_Y               (EDIT_Y + EDIT_CY + UI_PAD)
#define HEX_CX              (MAIN_CLIENT_CX / 2) - (INT)(1.5 * UI_PAD)
#define HEX_CY              (HEX_CX)
#define CHG_X               (UI_PAD + HEX_CX + UI_PAD)
#define CHG_Y               (HEX_Y)
#define CHG_CX              (HEX_CX)
#define CHG_CY              (HEX_CY)
#define HEX_NAME_X          (HEX_X + UI_PAD)
#define HEX_NAME_Y          (HEX_Y + UI_PAD)
#define CHG_NAME_X          (CHG_X + UI_PAD)
#define CHG_NAME_Y          (HEX_NAME_Y)
#define NAME_CX             (HEX_CX - (2 * UI_PAD))
#define HEX_LINE_X          (HEX_NAME_X)
#define CHG_LINE_X          (CHG_NAME_X)
#define LINE_6_Y            (HEX_NAME_Y + (UI_PAD * 4))
#define LINE_5_Y            (LINE_6_Y + (UI_PAD * 3))
#define LINE_4_Y            (LINE_5_Y + (UI_PAD * 3))
#define LINE_3_Y            (LINE_4_Y + (UI_PAD * 3))
#define LINE_2_Y            (LINE_3_Y + (UI_PAD * 3))
#define LINE_1_Y            (LINE_2_Y + (UI_PAD * 3))
#define LINE_CX             (HEX_CX - (3 * UI_PAD))
#define TXT_NAME_X          (UI_PAD)
#define TXT_NAME_Y          (HEX_Y + HEX_CY + UI_PAD)
#define TXT_NAME_CX         (NAME_CX)
#define TXT_NAME_CY         (LINE_CY)
#define TEXT_X              (UI_PAD)
#define TEXT_Y              (TXT_NAME_Y + TXT_NAME_CY + UI_PAD)
#define TEXT_CX             (EDIT_CX)
#define TEXT_CY             (MAIN_CLIENT_CY - TEXT_Y - UI_PAD)

enum e_ctrl_id {
    IDC_CMD_CAST = 1,
    IDC_CMD_BACK,
    IDC_CMD_SAVE,
    IDC_CMD_OPEN,
    IDC_QUERY,
    IDC_TEXT,
    IDC_OPT_HEXAGRAM,
    IDC_OPT_CHANGING,
    IDC_OPT_HEX_LINE6,
    IDC_OPT_HEX_LINE5,
    IDC_OPT_HEX_LINE4,
    IDC_OPT_HEX_LINE3,
    IDC_OPT_HEX_LINE2,
    IDC_OPT_HEX_LINE1,
    IDC_OPT_CHG_LINE6,
    IDC_OPT_CHG_LINE5,
    IDC_OPT_CHG_LINE4,
    IDC_OPT_CHG_LINE3,
    IDC_OPT_CHG_LINE2,
    IDC_OPT_CHG_LINE1,
    IDC_OPEN_LIST,
    IDC_OPEN_DELETE,
    IDC_OPEN_CANCEL,
    IDC_OPEN_OPEN,
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

enum e_button {
    CMD_CAST,
    CMD_BACK,
    CMD_SAVE,
    CMD_OPEN,
    CMD_COUNT,
};

enum e_button_state {
    CMD_NORMAL,
    CMD_OVER,
    CMD_DOWN,
};

#pragma endregion

#pragma region global data and forward function declarations

const BYTE bin_to_wen[64] = { 2, 24, 7, 19, 15, 36, 46, 11, 16, 51, 40, 54, 62, 55, 32, 34, 8, 3, 29, 60, 39, 63, 48, 5, 45, 17, 47, 58, 31, 49, 28, 43, 23, 27, 4, 41, 52, 22, 18, 26, 35, 21, 64, 38, 56, 30, 50, 14, 20, 42, 59, 61, 53, 37, 57, 9, 12, 25, 6, 10, 33, 13, 44, 1 };

typedef struct {
    SYSTEMTIME local_time;             // Timestamp of the reading
    HINSTANCE  hInstance;              // Application instance handle
    HWND       hwnd_main;              // Main window handle
    HWND       hwnd_open;              // Open window handle
    HFONT      font_name;              // Font for hexagram name
    HFONT      font_text;              // Font for hexagram text
    HCURSOR    help_cursor;            // Help cursor for clickable option buttons
    HICON      icon[ICON_COUNT];       // Icons for command buttons, options, numbers and line representations
    RECT       rect_hexagram;          // Hexagram rectangle
    RECT       rect_changing;          // Changed hexagram rectangle
    RECT       rect_hex_name;          // Hexagram name rectangle
    PWCHAR     lines[6];               // Text of the lines of the main hexagram
    PWCHAR     hex_text;               // Text of the main hexagram
    PWCHAR     chg_text;               // Text of the changed hexagram
    DWORD      seed;                   // Random seed for casting
    UINT       save_count;             // Count of saved readings
    WCHAR      query[260];             // User's input, either a hexagram number or a query
    WCHAR      buf_hex[6 * 1024];      // Buffer for the hexagram text
    WCHAR      buf_chg[6 * 1024];      // Buffer for the changed hexagram text
    WCHAR      ini_file[MAX_PATH];     // Path to the 'I Ching.ini' file for the text and saved readings
    BYTE       btn_enabled[CMD_COUNT]; // Button enabled/disabled
    BYTE       btn_state[CMD_COUNT];   // State of each button (normal, hover or pressed)
    BYTE       wen_to_bin[64];         // Mapping from wen (the traditional numbering of hexagrams) to bin (the binary representation used in the program)
    BYTE       hex_bits;               // Binary representation of the main hexagram, where each bit represents a line (0 for yin, 1 for yang)
    BYTE       chg_bits;               // Binary representation of the changing lines, where each bit represents a line that is changing (1 for changing, 0 for static)
    BYTE       hex_num;                // The number of the main hexagram (1-64)
    BYTE       chg_num;                // The number of the changed hexagram (1-64)
    BYTE       opt_num;                // The currently selected option (hexagram, changed hexagram or changing lines)
    BYTE       opt_prev;               // The previously selected option
    BYTE       close;                  // Set to true when the message loop should bail
} Global_t;

Global_t g;

// RDSEED.asm declarations
EXTERN_C BYTE    rdseed_support (VOID);
EXTERN_C DWORD   get_seed       (VOID);

// forward declarations
VOID             calc_hex_all   (VOID);
VOID             calc_hex_num   (DWORD seed, UINT& hex, UINT& chg);
VOID             cmd_back       (VOID);
VOID             cmd_cast       (VOID);
VOID             cmd_open       (VOID);
VOID             cmd_save       (VOID);
VOID             create_open    (HWND hwnd_open);
VOID             create_main    (HWND hwnd);
BYTE             file_exists    (PCWSTR path);
VOID             get_hex_text   (VOID);
VOID             open_delete    (VOID);
VOID             open_selected  (VOID);
VOID             paint_main     (HWND hwnd);
LRESULT CALLBACK subproc_cmd    (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK subproc_edit   (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK subproc_opt    (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK wndproc_main   (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK wndproc_open   (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
#pragma endregion

#pragma region application entrypoint and window procedure

// Entry point of the application.
//=================================================================================================================================================================================
EXTERN_C __declspec(noreturn) void __stdcall WinMainCRTStartup(void) {
    if (!rdseed_support()) {
        MessageBoxW(0, L"This computer's processor lacks support for the RDSEED instruction that's used by the casting technique.\n\nMinimum requirements:\n    INTEL Ivy Bridge or newer\n    AMD  Ryzen or newer", L"I Ching", MB_ICONSTOP);
        ExitProcess(ERROR_NOT_SUPPORTED);
    }

    g.hInstance = GetModuleHandleW(0);

    GetModuleFileNameW(g.hInstance, g.ini_file, _countof(g.ini_file));
    PathRenameExtensionW(g.ini_file, L".ini");

    if (!file_exists(g.ini_file)) {
        WCHAR msg[MAX_PATH + 180];
            
        swprintf_s(msg, _countof(msg), L"Couldn't find the configuration file 'I Ching.ini' in the application directory.\n\nExpected path:\n%s\n\nMake sure the file exists and is in the correct location.", g.ini_file);
        MessageBoxW(0, msg, L"I Ching", MB_ICONERROR);
        ExitProcess(ERROR_FILE_NOT_FOUND);
    }

    // Create the reverse mapping from wen to bin
    for (UINT i = 0; i < 64; i++)
        g.wen_to_bin[ bin_to_wen[i] - 1 ] = i;

    // Load the icon graphics
    for (UINT ico_idx = 0, res_id = IDI_CMD_CAST; ico_idx <= ICON_NUMBER_9; ico_idx++, res_id++)
        g.icon[ico_idx] = LoadIconW(g.hInstance, MAKEINTRESOURCEW(res_id));

    g.opt_num     = IDC_OPT_HEXAGRAM;
    g.help_cursor = LoadCursorW(0, IDC_HELP);

    INITCOMMONCONTROLSEX icc = { sizeof(INITCOMMONCONTROLSEX), ICC_LISTVIEW_CLASSES|ICC_STANDARD_CLASSES };
    InitCommonControlsEx(&icc);

    LoadLibraryW(L"RICHED20");

    WNDCLASSEXW wcex   = { sizeof(wcex) };
    wcex.hbrBackground = CreateSolidBrush(RGB(231, 208, 177));
    wcex.hCursor       = LoadCursorW(0, (LPCWSTR)IDC_ARROW);
    wcex.hIcon         = g.icon[ICON_CMD_CAST_SET];
    wcex.hInstance     = g.hInstance;
    wcex.lpfnWndProc   = wndproc_main;
    wcex.lpszClassName = L"I Ching";

    if (RegisterClassExW(&wcex)) {
        g.hwnd_main = CreateWindowW(wcex.lpszClassName, wcex.lpszClassName, WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX, 0, 0, 0, 0, 0, 0, g.hInstance, 0);

        if (g.hwnd_main) {
            BOOL dark = TRUE;
            MSG  msg;

            DwmSetWindowAttribute(g.hwnd_main, DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof(dark));
            ShowWindow(g.hwnd_main, SW_NORMAL);

            while (!g.close) {
                if (GetMessageW(&msg, g.hwnd_main, 0, 0)) {
                    TranslateMessage(&msg);
                    DispatchMessageW(&msg);
                }
            }
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
        g.close = true;
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

    INT cx = MAIN_CLIENT_CX +  RECT_CX(rcWindow)  - RECT_CX(rcClient);
    INT cy = MAIN_CLIENT_CY +  RECT_CY(rcWindow)  - RECT_CY(rcClient);
    INT  x = mi.rcWork.left + (RECT_CX(mi.rcWork) - cx) / 2;
    INT  y = mi.rcWork.top  + (RECT_CY(mi.rcWork) - cy) / 3;

    SetWindowPos(hwnd_main, 0, x, y, cx, cy, SWP_NOZORDER);

    g.font_name = CreateFontW(-14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Segoe UI");
    g.font_text = CreateFontW(-13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Verdana");
 
    g.btn_enabled[CMD_CAST] = 1;
    g.btn_enabled[CMD_OPEN] = 1;

    hwnd = CreateWindowW(L"BUTTON", 0, WS_CHILD|WS_VISIBLE|SS_OWNERDRAW, MAIN_CMD_CAST_X, MAIN_CMD_Y, CMD_CX, CMD_CY, hwnd_main, (HMENU)IDC_CMD_CAST, 0, 0);
    SetWindowSubclass(hwnd, &subproc_cmd, IDC_CMD_CAST, 0);

    hwnd = CreateWindowW(L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, MAIN_CMD_BACK_X, MAIN_CMD_Y, CMD_CX, CMD_CY, hwnd_main, (HMENU)IDC_CMD_BACK, 0, 0);
    SetWindowSubclass(hwnd, &subproc_cmd, IDC_CMD_BACK, 1);

    hwnd = CreateWindowW(L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, MAIN_CMD_SAVE_X, MAIN_CMD_Y, CMD_CX, CMD_CY, hwnd_main, (HMENU)IDC_CMD_SAVE, 0, 0);
    SetWindowSubclass(hwnd, &subproc_cmd, IDC_CMD_SAVE, 2);

    hwnd = CreateWindowW(L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, MAIN_CMD_OPEN_X, MAIN_CMD_Y, CMD_CX, CMD_CY, hwnd_main, (HMENU)IDC_CMD_OPEN, 0, 0);
    SetWindowSubclass(hwnd, &subproc_cmd, IDC_CMD_OPEN, 3);

    hwnd = CreateWindowW(L"EDIT",   0, WS_CHILD|WS_BORDER|WS_VISIBLE|ES_NOHIDESEL, UI_PAD, EDIT_Y, EDIT_CX, EDIT_CY,  hwnd_main,  (HMENU)IDC_QUERY, 0, 0);
    SendMessageW(hwnd, WM_SETFONT, (WPARAM)g.font_text, 0);
    SendMessageW(hwnd, EM_SETCUEBANNER, TRUE, (LPARAM)L"Enter a query or hexagram number from 1 to 64");
    SetFocus(hwnd);
    SetWindowSubclass(hwnd, &subproc_edit, IDC_QUERY, 0);

    hwnd = CreateWindowW(L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, HEX_NAME_X, HEX_NAME_Y, NAME_CX,  LINE_CY, hwnd_main, (HMENU)IDC_OPT_HEXAGRAM,  0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, IDC_OPT_HEXAGRAM,  0x010000);    

    hwnd = CreateWindowW(L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CHG_NAME_X, CHG_NAME_Y, NAME_CX, LINE_CY, hwnd_main, (HMENU)IDC_OPT_CHANGING,  0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, IDC_OPT_CHANGING, 0);

    hwnd = CreateWindowW(L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, HEX_LINE_X, LINE_6_Y,   LINE_CX,  LINE_CY, hwnd_main, (HMENU)IDC_OPT_HEX_LINE6, 0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, IDC_OPT_HEX_LINE6, 0x010600 | 1 << 5);                                                                                       
                                                                                                                                            
    hwnd = CreateWindowW(L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, HEX_LINE_X, LINE_5_Y,   LINE_CX,  LINE_CY, hwnd_main, (HMENU)IDC_OPT_HEX_LINE5, 0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, IDC_OPT_HEX_LINE5, 0x010500 | 1 << 4);                                                                                       
                                                                                                                                            
    hwnd = CreateWindowW(L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, HEX_LINE_X, LINE_4_Y,   LINE_CX,  LINE_CY, hwnd_main, (HMENU)IDC_OPT_HEX_LINE4, 0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, IDC_OPT_HEX_LINE4, 0x010400 | 1 << 3);                                                                                       
                                                                                                                                            
    hwnd = CreateWindowW(L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, HEX_LINE_X, LINE_3_Y,   LINE_CX,  LINE_CY, hwnd_main, (HMENU)IDC_OPT_HEX_LINE3, 0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, IDC_OPT_HEX_LINE3, 0x010300 | 1 << 2);                                                                                       
                                                                                                                                            
    hwnd = CreateWindowW(L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, HEX_LINE_X, LINE_2_Y,   LINE_CX,  LINE_CY, hwnd_main, (HMENU)IDC_OPT_HEX_LINE2, 0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, IDC_OPT_HEX_LINE2, 0x010200 | 1 << 1);                                                                                       
                                                                                                                                            
    hwnd = CreateWindowW(L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, HEX_LINE_X, LINE_1_Y,   LINE_CX,  LINE_CY, hwnd_main, (HMENU)IDC_OPT_HEX_LINE1, 0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, IDC_OPT_HEX_LINE1, 0x010100 | 1 << 0);                                                                                       
                                                                                                                                            
    hwnd = CreateWindowW(L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CHG_LINE_X,  LINE_6_Y,  LINE_CX,  LINE_CY, hwnd_main, (HMENU)IDC_OPT_CHG_LINE6, 0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, IDC_OPT_CHG_LINE6, 0x000600);                                                                                            
                                                                                                                                            
    hwnd = CreateWindowW(L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CHG_LINE_X,  LINE_5_Y,  LINE_CX,  LINE_CY, hwnd_main, (HMENU)IDC_OPT_CHG_LINE5, 0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, IDC_OPT_CHG_LINE5, 0x000500);                                                                                            
                                                                                                                                            
    hwnd = CreateWindowW(L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CHG_LINE_X,  LINE_4_Y,  LINE_CX,  LINE_CY, hwnd_main, (HMENU)IDC_OPT_CHG_LINE4, 0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, IDC_OPT_CHG_LINE4, 0x000400);                                                                                           
                                                                                                                                            
    hwnd = CreateWindowW(L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CHG_LINE_X,  LINE_3_Y,  LINE_CX,  LINE_CY, hwnd_main, (HMENU)IDC_OPT_CHG_LINE3, 0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, IDC_OPT_CHG_LINE3, 0x000300);                                                                                           
                                                                                                                                            
    hwnd = CreateWindowW(L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CHG_LINE_X,  LINE_2_Y,  LINE_CX,  LINE_CY, hwnd_main, (HMENU)IDC_OPT_CHG_LINE2, 0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, IDC_OPT_CHG_LINE2, 0x000200);                                                                                           
                                                                                                                                            
    hwnd = CreateWindowW(L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CHG_LINE_X,  LINE_1_Y,  LINE_CX,  LINE_CY, hwnd_main, (HMENU)IDC_OPT_CHG_LINE1, 0, 0);
    SetWindowSubclass(hwnd, &subproc_opt, IDC_OPT_CHG_LINE1, 0x000100);

    hwnd = CreateWindowW(RICHEDIT_CLASSW, 0, WS_CHILD|WS_VSCROLL|ES_MULTILINE|WS_VISIBLE, TEXT_X, TEXT_Y, TEXT_CX, TEXT_CY, hwnd_main, (HMENU)IDC_TEXT,  0, 0);
    SendMessageW(hwnd, WM_SETFONT, (WPARAM)g.font_text, 0);
    SendMessageW(hwnd, EM_SETBKGNDCOLOR, 0, RGB(231, 208, 177));
    SendMessageW(hwnd, EM_SETTYPOGRAPHYOPTIONS, TO_ADVANCEDTYPOGRAPHY, TO_ADVANCEDTYPOGRAPHY);
    SendMessageW(hwnd, EM_SETREADONLY, TRUE, 0);

    pf.dwMask     = PFM_ALIGNMENT|PFM_TABSTOPS;
    pf.cTabCount  = 1;
    pf.rgxTabs[0] = 360;
    pf.wAlignment = PFA_FULL_INTERWORD;
    SendMessageW(hwnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf);

    g.rect_hexagram = { HEX_X,      HEX_Y,      HEX_X + HEX_CX,           HEX_Y + HEX_CY  };
    g.rect_changing = { CHG_X,      CHG_Y,      CHG_X + CHG_CX,           CHG_Y + CHG_CY  };
    g.rect_hex_name = { TXT_NAME_X, TXT_NAME_Y, TXT_NAME_X + TXT_NAME_CX, TXT_NAME_Y + TXT_NAME_CY };
}

// Handles the painting of the main window.
//==================================================================================================================================================================================
VOID paint_main(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC         hdc    = BeginPaint(hwnd, &ps);
    HBRUSH      hBrush = CreateSolidBrush(RGB(116, 116, 116));

    FrameRect(hdc, &g.rect_hexagram, hBrush);
    FrameRect(hdc, &g.rect_changing, hBrush);

    if (g.hex_num) {
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, 0);
        SelectObject(hdc, g.font_name);

        PWSTR p = (g.opt_num == IDC_OPT_CHANGING) ? g.buf_chg : g.buf_hex;
        
        TextOutW(hdc, g.rect_hex_name.left, g.rect_hex_name.top, p, (INT)wcslen(p));
    }
        
    DeleteObject(hBrush);
    EndPaint(hwnd, &ps);
}

// Handles the command buttons (cast, back, save and load).
//=================================================================================================================================================================================
LRESULT CALLBACK subproc_cmd(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
    BYTE enabled = g.btn_enabled[dwRefData];

    if (!enabled && message != WM_PAINT)
        return DefSubclassProc(hwnd, message, wParam, lParam);

    switch (message) {
    case WM_LBUTTONDOWN:
        g.btn_state[dwRefData] = CMD_DOWN;
        InvalidateRect(hwnd, 0, FALSE);
        break;

    case WM_LBUTTONUP:
        switch (uIdSubclass) {
        case IDC_CMD_CAST:
            cmd_cast();
            g.btn_state[dwRefData] = g.btn_enabled[dwRefData] ? CMD_OVER : CMD_NORMAL;
            break;

        case IDC_CMD_BACK:
            cmd_back();
            g.btn_state[dwRefData] = g.btn_enabled[dwRefData] ? CMD_OVER : CMD_NORMAL;
            break;

        case IDC_CMD_SAVE:
            cmd_save();
            g.btn_state  [dwRefData] = CMD_NORMAL;
            g.btn_enabled[dwRefData] = 0;
            break;

        case IDC_CMD_OPEN:
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
        INT         delta;
        UINT        icon;
        UINT        edge;
        UINT        flags;
        HDC         hdc    = BeginPaint(hwnd, &ps);
        HBRUSH      hBrush = CreateSolidBrush(RGB(255, 255, 255));

        FillRect(hdc, &ps.rcPaint, hBrush);
        DeleteObject(hBrush);

        switch (dwRefData) {
        case 0: 
            icon = enabled ? ICON_CMD_CAST_SET : ICON_CMD_CAST_OFF;
            break;

        case 1: 
            icon = enabled ? ICON_CMD_BACK_SET : ICON_CMD_BACK_OFF;
            break;

        case 2:
            icon = enabled ? ICON_CMD_SAVE_SET : ICON_CMD_SAVE_OFF;
            break;

        case 3:
            icon = ICON_CMD_OPEN_SET;
            break;
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
        EndPaint(hwnd, &ps);
    } break;

    default:
        return DefSubclassProc(hwnd, message, wParam, lParam);
    }

    return 0;
}

// Keyboard input for the query edit control.
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


// Main hexagram, changing lines and changed hexagram option buttons.
//================================================================================================================================================================================
LRESULT CALLBACK subproc_opt(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
    if (message == WM_ERASEBKGND)   
        return 0;

    BYTE button;

    if (uIdSubclass == IDC_OPT_HEXAGRAM)
        button = TRUE;
    else
    if (uIdSubclass == IDC_OPT_CHANGING)
        button = g.chg_bits;
    else
        button = dwRefData & g.chg_bits;

    if (!button && message != WM_PAINT)
        return DefSubclassProc(hwnd, message, wParam, lParam);

    UINT line = (dwRefData >>  8) & 0x07; // line number
    BYTE main = (dwRefData >> 16) & 0xFF; // main hexagram else change hexagram

    switch (message) {
    case WM_LBUTTONDOWN:
        if (g.opt_num != uIdSubclass) {
            PWSTR p = NULL;

            g.opt_prev = g.opt_num;
            g.opt_num  = (BYTE)uIdSubclass;

            switch (uIdSubclass) {
            case IDC_OPT_HEXAGRAM: 
                p = g.hex_text;
                break;

            case IDC_OPT_CHANGING: 
                p = g.chg_text;
                break;

            case IDC_OPT_HEX_LINE6:
            case IDC_OPT_HEX_LINE5:
            case IDC_OPT_HEX_LINE4:
            case IDC_OPT_HEX_LINE3:
            case IDC_OPT_HEX_LINE2:
            case IDC_OPT_HEX_LINE1:
                p = g.lines[line - 1];
                break;
            }

            SetWindowTextW(MAIN_CTRL(IDC_TEXT), p);

            InvalidateRect(hwnd, 0, FALSE);
            InvalidateRect(MAIN_CTRL(g.opt_prev), 0, FALSE);

            if (g.opt_prev == IDC_OPT_CHANGING || g.opt_num == IDC_OPT_CHANGING)   
                InvalidateRect(g.hwnd_main, &g.rect_hex_name, TRUE);
        } break;

    case WM_MOUSEMOVE:
        if (g.opt_num != uIdSubclass) {
            TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, hwnd, 0 };

            TrackMouseEvent(&tme);
            SetCursor(g.help_cursor);
        } break;

    case WM_MOUSELEAVE:
        SetCursor(0);
        break;

    case WM_PAINT:
        if (g.hex_num) {
            PAINTSTRUCT ps;
            UINT        ico_index;
            HDC         hdc = BeginPaint(hwnd, &ps);
            PWSTR       p   = NULL;

            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, 0);

            if (button) {
                ico_index = (uIdSubclass == g.opt_num) ? ICON_OPT_SET : ICON_OPT_OFF;

                DrawIconEx(hdc, 0, 0, g.icon[ico_index], 16, 16, 0, 0, DI_NORMAL);

                switch (uIdSubclass) {
                case IDC_OPT_HEXAGRAM:
                    p = g.buf_hex;
                    break;

                case IDC_OPT_CHANGING:
                    p = g.buf_chg;
                    break;
                }

                if (p) {
                    SelectObject(hdc, g.font_name);
                    TextOutW(hdc, 24, 0, p, (INT)wcslen(p));
                }
            }

            if (line--) {
                BYTE hex_bit = g.hex_bits & 1 << line;
                BYTE chg_bit = g.chg_bits & 1 << line;
                
                ico_index = 0;

                if (main)
                    ico_index = chg_bit ? (hex_bit ? ICON_LINE_9 : ICON_LINE_6) : (hex_bit ? ICON_LINE_7 : ICON_LINE_8);
                else
                if (g.chg_bits)
                    ico_index = chg_bit ? (hex_bit ? ICON_LINE_8 : ICON_LINE_7) : (hex_bit ? ICON_LINE_7 : ICON_LINE_8);

                if (ico_index) {
                    DrawIconEx(hdc,  32, 3, g.icon[ICON_NUMBER_1 + line],                      8, 10, 0, 0, DI_NORMAL);
                    DrawIconEx(hdc,  56, 3, g.icon[ico_index],                               140, 10, 0, 0, DI_NORMAL);
                    DrawIconEx(hdc, 212, 3, g.icon[ICON_NUMBER_6 - ICON_LINE_6 + ico_index],   8, 10, 0, 0, DI_NORMAL);
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

// Execute the casting process to generate a hexagram based on the user's query or a user-specified hexagram number.
//==========================================================================================================================================================
VOID cmd_cast(VOID) {
    g.seed = get_seed();

    GetWindowTextW(MAIN_CTRL(IDC_QUERY), g.query, _countof(g.query));
 
    g.hex_num = _wtoi(g.query);

    if (g.hex_num >= 1 && g.hex_num <= 64 && wcslen(g.query) < 3) {
        g.seed     = 0;
        g.hex_bits = g.wen_to_bin[g.hex_num - 1];
        g.chg_bits = 0b111111;
        g.chg_num  = bin_to_wen[g.hex_bits ^ g.chg_bits];
    } else {
        GetLocalTime(&g.local_time);
        calc_hex_all();
        g.btn_enabled[CMD_SAVE] = 1;
        InvalidateRect(MAIN_CTRL(IDC_CMD_SAVE), 0, FALSE);
    }

    g.btn_enabled[CMD_CAST] = 0;
    g.btn_enabled[CMD_BACK] = 1;

    get_hex_text();

    SetWindowTextW(MAIN_CTRL(IDC_TEXT),  g.hex_text);
    SendMessageW  (MAIN_CTRL(IDC_QUERY), EM_SETREADONLY, TRUE, 0);
    InvalidateRect(MAIN_CTRL(IDC_CMD_CAST), 0, FALSE);
    InvalidateRect(MAIN_CTRL(IDC_CMD_BACK), 0, FALSE);
    InvalidateRect(g.hwnd_main, &g.rect_hex_name, FALSE);
    SetFocus(g.hwnd_main);
}

// Resets the application state to allow for a new reading.
//=========================================================================================================================================================
VOID cmd_back(VOID) {
    g.hex_bits = 0;
    g.hex_num  = 0;
    g.chg_bits = 0;
    g.chg_num  = 0;
    g.opt_prev = 0;
    g.opt_num  = IDC_OPT_HEXAGRAM;

    g.btn_enabled[CMD_CAST] = 1;
    g.btn_enabled[CMD_BACK] = 0;
    g.btn_enabled[CMD_SAVE] = 0;

    HWND hwnd =MAIN_CTRL(IDC_QUERY);
    SetWindowTextW(hwnd, 0);
    SendMessageW(hwnd, EM_SETREADONLY, FALSE, 0);
    SetFocus(hwnd);

    SetWindowTextW(MAIN_CTRL(IDC_TEXT), 0);
    UpdateWindow  (MAIN_CTRL(IDC_TEXT));
    InvalidateRect(MAIN_CTRL(IDC_CMD_CAST), 0, FALSE);
    InvalidateRect(MAIN_CTRL(IDC_CMD_BACK), 0, FALSE);
    InvalidateRect(MAIN_CTRL(IDC_CMD_SAVE), 0, FALSE);
    InvalidateRect(g.hwnd_main, &g.rect_hexagram, TRUE);
    InvalidateRect(g.hwnd_main, &g.rect_changing, TRUE);
    InvalidateRect(g.hwnd_main, &g.rect_hex_name, TRUE);
}

// Saves the reading to the ini file.
//=========================================================================================================================================================
VOID cmd_save(VOID) {
    WCHAR key[8];
    WCHAR buf[32 + _countof(g.query)];
    INT   count;

    swprintf_s(buf, _countof(buf), L"%d-%02d-%02d %02d:%02d:%02d 0x%05X %s", g.local_time.wYear, g.local_time.wMonth, g.local_time.wDay, g.local_time.wHour, g.local_time.wMinute, g.local_time.wSecond, g.seed, g.query);

    count = GetPrivateProfileIntW(L"Saved", L"Count", 0, g.ini_file);
    count++;

    swprintf_s(key, _countof(key), L"%d", count);

    if (WritePrivateProfileStringW(L"Saved", L"Count", key, g.ini_file))
        WritePrivateProfileStringW(L"Saved", key, buf, g.ini_file);
    else
        MessageBoxW(g.hwnd_main, L"Failed to save the reading. Make sure the application has permission to write to its directory.", L"Error", MB_ICONERROR);
}

// Creates the open window, which allows the user to open previously saved readings.
//=========================================================================================================================================================
VOID cmd_open(VOID) {
    WNDCLASSEXW wcex   = { sizeof(wcex) };
    wcex.hCursor       = LoadCursorW(0, (LPCWSTR)IDC_ARROW);
    wcex.hIcon         = g.icon[ICON_CMD_OPEN_SET];
    wcex.hInstance     = g.hInstance;
    wcex.lpfnWndProc   = wndproc_open;
    wcex.lpszClassName = L"I CHING OPEN";

    RegisterClassExW(&wcex);

    g.hwnd_open = CreateWindowW(wcex.lpszClassName, L"Saved readings", WS_CAPTION|WS_SYSMENU, 0, 0, 0, 0, g.hwnd_main, 0, g.hInstance, 0);

    if (g.hwnd_open) {
        EnableWindow(g.hwnd_main, FALSE);
        ShowWindow(g.hwnd_open, SW_NORMAL);
         
        while (!g.close) {
            MSG msg;

            if (GetMessageW(&msg, g.hwnd_open, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
        }

        g.close = false;

        DestroyWindow(g.hwnd_open);
        EnableWindow(g.hwnd_main, TRUE);
        SetForegroundWindow(g.hwnd_main);
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
        g.close = true;
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_OPEN_DELETE:
            open_delete();
            break;

        case IDC_OPEN_OPEN:   
            open_selected();
            break;

        case IDC_OPEN_CANCEL: 
            g.close = true;  
            break;
        } break;

    case WM_NOTIFY: {
            auto lpHdr = (LPNMHDR)lParam;

            if (lpHdr->code == NM_CLICK) {
                BOOL enabled = SELECTED_ITEM(OPEN_CTRL(IDC_OPEN_LIST)) != -1;

                EnableWindow(OPEN_CTRL(IDC_OPEN_DELETE), enabled);
                EnableWindow(OPEN_CTRL(IDC_OPEN_OPEN),   enabled);
            } else
            if (lpHdr->code == NM_DBLCLK)
                open_selected();
        } break;

    default:
        return DefWindowProcW(hwnd, message, wParam, lParam);
    }

    return 0;
}

// Create the controls for the open window and populates the list with saved readings from the ini file.
//=======================================================================================================
VOID create_open(HWND hwnd_open) {
    RECT        rcClient;
    RECT        rcWindow;
    HWND        hwnd;
    LV_COLUMNW	lvCol = {};

    GetWindowRect(g.hwnd_main, &rcWindow);

    SetWindowPos(hwnd_open, 0, rcWindow.left, rcWindow.top, RECT_CX(rcWindow), RECT_CY(rcWindow), SWP_NOZORDER);
    GetClientRect(hwnd_open, &rcClient);

    INT btn_top = rcClient.bottom - UI_PAD - 24;

    hwnd = CreateWindowW(L"BUTTON", L"Delete", WS_CHILD|WS_VISIBLE|BS_FLAT|BS_PUSHBUTTON, UI_PAD, btn_top, 100, 24, hwnd_open, (HMENU)IDC_OPEN_DELETE, 0, 0);
    SendMessageW(hwnd, WM_SETFONT, (WPARAM)g.font_name, 0);
    EnableWindow(hwnd, FALSE);

    hwnd = CreateWindowW(L"BUTTON", L"Open",   WS_CHILD|WS_VISIBLE|BS_FLAT|BS_PUSHBUTTON, RECT_CX(rcClient) - UI_PAD - 100 - UI_PAD - 100, btn_top, 100, 24, hwnd_open, (HMENU)IDC_OPEN_OPEN, 0, 0);
    SendMessageW(hwnd, WM_SETFONT, (WPARAM)g.font_name, 0);
    EnableWindow(hwnd, FALSE);

    hwnd = CreateWindowW(L"BUTTON", L"Cancel", WS_CHILD|WS_VISIBLE|BS_FLAT|BS_PUSHBUTTON, RECT_CX(rcClient) - UI_PAD - 100, btn_top, 100, 24, hwnd_open, (HMENU)IDC_OPEN_CANCEL, 0, 0);
    SendMessageW(hwnd, WM_SETFONT, (WPARAM)g.font_name, 0);

    hwnd = CreateWindowW(L"SysListView32", 0,  WS_VISIBLE|WS_CHILD|WS_BORDER|LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS, 0, 0, RECT_CX(rcClient), rcClient.bottom - UI_PAD - 24 - UI_PAD, hwnd_open, (HMENU)IDC_OPEN_LIST, 0, 0);
    SendMessageW(hwnd, WM_SETFONT, (WPARAM)g.font_text, 0);
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

    g.save_count = GetPrivateProfileIntW(L"Saved", L"Count", 0, g.ini_file);

    for (UINT i = 1; i <= g.save_count; i++) {
        WCHAR key[10];
        WCHAR buf[400];

        memset(buf, 0, sizeof(buf));

        swprintf_s(key, _countof(key), L"%d", i);

        if (GetPrivateProfileStringW(L"Saved", key, 0, buf, _countof(buf), g.ini_file)) {
            DWORD    seed;
            UINT     hexagram;
            UINT     changing;
            WCHAR    wcsHex[4];
            PWSTR    pTime;
            PWSTR    p      = buf;
            LV_ITEMW lvItem = {};

            p = wcschr(buf, L' ');
            p[0] = 0;

            pTime = p + 1;
            p = wcschr(pTime, L' ');
            p[0] = 0;

            seed = wcstoul(++p, &p, 16);
            p++;

            calc_hex_num(seed, hexagram, changing);
            swprintf_s(wcsHex, _countof(wcsHex), L"%d", hexagram);

            lvItem.mask    = LVIF_TEXT|LVIF_PARAM;
            lvItem.lParam  = seed;
            lvItem.pszText = buf;

            ListView_InsertItem (hwnd, &lvItem);
            ListView_SetItemText(hwnd, lvItem.iItem, 1, pTime);
            ListView_SetItemText(hwnd, lvItem.iItem, 2, wcsHex);

            if (changing) {
                WCHAR wcsChanging[4];

                swprintf_s(wcsChanging, _countof(wcsChanging), L"%d", changing);
                ListView_SetItemText(hwnd, lvItem.iItem, 3, wcsChanging);
            }

            ListView_SetItemText(hwnd, lvItem.iItem, 4, p);
        }
    }

    // size the column widths per header and content
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
VOID open_selected(VOID) {
    HWND   hwnd =OPEN_CTRL(IDC_OPEN_LIST);
    LVITEM lvi;

    lvi.iItem    = SELECTED_ITEM(hwnd);
    lvi.mask     = LVIF_PARAM;
    lvi.iSubItem = 0;

    if (ListView_GetItem(hwnd, &lvi) == TRUE) {
        g.seed = (DWORD)lvi.lParam;
    
        lvi.pszText    = g.query;
        lvi.cchTextMax = _countof(g.query);
        lvi.iSubItem   = 4;
        lvi.mask       = LVIF_TEXT;
        
        if (ListView_GetItem(hwnd, &lvi) == TRUE) {
            calc_hex_all();
            get_hex_text();

            g.btn_enabled[CMD_CAST] = 0;
            g.btn_enabled[CMD_BACK] = 1;
            g.btn_enabled[CMD_SAVE] = 0;

            HWND hwnd_query =MAIN_CTRL(IDC_QUERY); 
            SetWindowTextW(hwnd_query, g.query);
            SendMessageW  (hwnd_query, EM_SETREADONLY, TRUE, 0);
            SetWindowTextW(MAIN_CTRL(IDC_TEXT), g.hex_text);
            InvalidateRect(MAIN_CTRL(IDC_CMD_CAST), 0, FALSE);
            InvalidateRect(MAIN_CTRL(IDC_CMD_BACK), 0, FALSE);
            InvalidateRect(MAIN_CTRL(IDC_CMD_SAVE), 0, FALSE);
            InvalidateRect(g.hwnd_main, &g.rect_hexagram, FALSE);
            InvalidateRect(g.hwnd_main, &g.rect_changing, FALSE);
            InvalidateRect(g.hwnd_main, &g.rect_hex_name, FALSE);

            g.close = true; 
        }
    }
}

// Delete the selected item from the list and update the ini file.
//=======================================================================================================================================================================================
VOID open_delete(VOID) {
    WCHAR  key[8];
    LVITEM lvi;
    HWND   hwnd  =OPEN_CTRL(IDC_OPEN_LIST);

    ListView_DeleteItem(hwnd, SELECTED_ITEM(hwnd));
    g.save_count--;
    
    for (INT i = g.save_count - 1, j = 1; i >= 0; i--, j++) {
        WCHAR wcsDate[20];

        lvi.iItem = i;
        lvi.mask = LVIF_PARAM | LVIF_TEXT;
        lvi.pszText = wcsDate;
        lvi.cchTextMax = _countof(wcsDate);
        lvi.iSubItem = 0;

        if (ListView_GetItem(hwnd, &lvi) == TRUE) {
            WCHAR wcsTime[20];
            DWORD seed = (DWORD)lvi.lParam;

            lvi.pszText = wcsTime;
            lvi.cchTextMax = _countof(wcsTime);
            lvi.iSubItem = 1;
            lvi.mask = LVIF_TEXT;

            if (ListView_GetItem(hwnd, &lvi) == TRUE) {
                WCHAR wcsQuery[_countof(g.query)];

                lvi.pszText = wcsQuery;
                lvi.cchTextMax = _countof(wcsQuery);
                lvi.iSubItem = 4;
                lvi.mask = LVIF_TEXT;

                if (ListView_GetItem(hwnd, &lvi) == TRUE) {
                    WCHAR buf[32 + _countof(g.query)];

                    swprintf_s(key, _countof(key), L"%d", j);
                    swprintf_s(buf, _countof(buf), L"%s %s 0x%05X %s", wcsDate, wcsTime, seed, wcsQuery);

                    WritePrivateProfileStringW(L"Saved", key, buf, g.ini_file);
                }
            }
        }
    }

    swprintf_s(key, _countof(key), L"%d", g.save_count + 1);
    WritePrivateProfileStringW(L"Saved", key, L"", g.ini_file);

    swprintf_s(key, _countof(key), L"%d", g.save_count);
    WritePrivateProfileStringW(L"Saved", L"Count", key, g.ini_file);

    EnableWindow(OPEN_CTRL(IDC_OPEN_DELETE), FALSE);
    EnableWindow(OPEN_CTRL(IDC_OPEN_OPEN),   FALSE);
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

// A limited version of the above function that only calculates the main and change hexagram numbers from a given seed, without modifying any global state.
//=======================================================================================================================================================================================
VOID calc_hex_num(DWORD seed, UINT& hex, UINT& chg) {
    DWORD hex_bits = 0;
    DWORD chg_bits = 0;

    for (UINT i = 0, bit = 1; i < 6; i++, bit <<= 1) {
        switch (seed & 0b111) {
        case 3:
            chg_bits |= bit;
            break;

        case 7:
            chg_bits |= bit;
        case 4: 
        case 5: 
        case 6:
            hex_bits |= bit;
            break;
        }

        seed >>= 3;
    }

    hex = bin_to_wen[hex_bits];
    chg = chg_bits ? bin_to_wen[hex_bits ^ chg_bits] : 0;
}

// Retrieve the text for the main and changed hexagram from the INI file, store pointers to the main hexagram's moving lines. 
//=======================================================================================================================================================================================
VOID get_hex_text(VOID) {
    WCHAR section[16];

    swprintf_s(section, _countof(section), L"Hexagram_%d", g.hex_num);

    if (GetPrivateProfileSectionW(section, g.buf_hex, _countof(g.buf_hex), g.ini_file)) {
        PWSTR p = g.buf_hex;

        for (UINT i = 0; i < 6; i++) {
            p += wcslen(p) + 1;
            g.lines[i] = p;
        }

        p += wcslen(p) + 1;
        g.hex_text = p;
    
        // replace "$$" sequences with double line breaks for paragraphs in the rich text control, since the ini file cannot contain actual line break characters
        while (p = wcsstr(p, L"$$")) {
            p[0] = L'\r';
            p[1] = L'\r';
        }

        if (g.chg_bits) {
            swprintf_s(section, _countof(section), L"Hexagram_%d", g.chg_num);

            if (GetPrivateProfileSectionW(section, g.buf_chg, _countof(g.buf_chg), g.ini_file)) {
                p = g.buf_chg;

                // Skip over the lines text for the change hexagram
                for (UINT i = 0; i < 6; i++)
                    p += wcslen(p) + 1;

                p += wcslen(p) + 1;
                g.chg_text = p;

                // replace "$$" sequences with double line breaks
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
BYTE file_exists(PCWSTR path) {
    DWORD dwAttrib = GetFileAttributesW(path);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
#pragma endregion