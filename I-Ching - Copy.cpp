
//
//

#pragma comment (linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifdef _IMPORT_H_
    #include "Import.h"
#endif

#define _NO_CRT_STDIO_INLINE

#include <windows.h>
#include <stdio.h>
#include <richedit.h>
#include <shlwapi.h>
#include <intrin.h>
#include <dwmapi.h>
#include "resource.h"

#ifdef _IMPORT_H_
    #include "Import.h"
#endif

#define RECT_CX(rc)     ((rc).right - (rc).left)
#define RECT_CY(rc)     ((rc).bottom - (rc).top)

#define I_CHING         (L"I-Ching")
#define MAIN_CLIENT_CX	(600)
#define MAIN_CLIENT_CY  (800)

#define UI_PAD          (12)

#define CMD_CX          (64)
#define CMD_CY          (64)

#define EDIT_CY	        (20)
#define LINE_CY         (16)

#define EDIT_CX         (MAIN_CLIENT_CX - (UI_PAD * 2))

#define MAIN_CMD_Y      (UI_PAD)
#define MAIN_CMD_CAST_X (UI_PAD)
#define MAIN_CMD_BACK_X (UI_PAD +  CMD_CX + UI_PAD)
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
#define HEX_NAME_Y      (HEX_Y + UI_PAD )
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
#define TXT_NAME_Y      (HEX_Y + HEX_CX + UI_PAD)
#define TXT_NAME_CX     (NAME_CX)
#define TXT_NAME_CY     (LINE_CY)

#define TEXT_X          (UI_PAD)
#define TEXT_Y          (TXT_NAME_Y + TXT_NAME_CY + UI_PAD)
#define TEXT_CX         (EDIT_CX)
#define TEXT_CY         (MAIN_CLIENT_CY - TEXT_Y - UI_PAD)

enum e_hwnd {
    HWND_MAIN,
    HWND_CMD_CAST,
    HWND_CMD_BACK,
    HWND_CMD_SAVE,
    HWND_CMD_LOAD,
    HWND_EDIT,
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
    FONT_CTRL,
    FONT_COUNT,
};

enum e_icon {
    ICON_MAIN,
    ICON_CMD_CAST_ON,
    ICON_CMD_BACK_ON,
    ICON_CMD_SAVE_ON,
    ICON_CMD_LOAD_ON,
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
    CMD_LOAD,
    CMD_COUNT,
};

enum e_btn_state {
    CMD_NORMAL,
    CMD_OVER,
    CMD_DOWN,
};

const BYTE       bin_to_wen[64] = { 2, 24, 7, 19, 15, 36, 46, 11, 16, 51, 40, 54, 62, 55, 32, 34, 8, 3, 29, 60, 39, 63, 48, 5, 45, 17, 47, 58, 31, 49, 28, 43, 23, 27, 4, 41, 52, 22, 18, 26, 35, 21, 64, 38, 56, 30, 50, 14, 20, 42, 59, 61, 53, 37, 57, 9, 12, 25, 6, 10, 33, 13, 44, 1 };
const BITMAPINFO main_bmpInfo   = { sizeof(BITMAPINFO), MAIN_CLIENT_CX, -MAIN_CLIENT_CY, 1, 24, BI_RGB };

typedef struct {
    HINSTANCE hInstance;
    HWND      hwnd[HWND_COUNT];
    HFONT     font[FONT_COUNT];
    RECT      rect[RECT_COUNT];
    HICON     icon[ICON_COUNT];
    HCURSOR   hCurHelp;
    PBYTE     pPixels;
    DWORD     seed;
    BYTE      hex_bits;
    BYTE      hex_num;
    BYTE      chg_bits;
    BYTE      chg_num;
    BYTE      line_num;
    BYTE      last_num;
    BYTE      wen_to_bin[64];
    BYTE      btn_awake[CMD_COUNT];
    BYTE      btn_state[CMD_COUNT];
    WCHAR     ini_file[MAX_PATH];
    WCHAR     hex_text[2560];
    WCHAR     chg_text[2560];
    WCHAR     hex_title[40];
    WCHAR     chg_title[40];
    WCHAR     lines[6][2048];
    WCHAR     query[260];
} Global_t;

Global_t g;

VOID             cmd_cast     (VOID);
VOID             cmd_back     (VOID);
VOID             btn_save     (VOID);
VOID             cmd_load     (VOID);
VOID             get_text     (VOID);
BOOL             is_rdseed    (VOID);
VOID             main_create  (HWND hwnd);
VOID             main_paint   (HWND hwnd);
LRESULT CALLBACK wndproc_main (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK wndproc_cmd  (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK wndproc_edit (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK wndproc_opt  (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

#ifdef _IMPORT_H_
    EXTERN_C __declspec(noreturn) void __stdcall win_main(void)
#else
    EXTERN_C __declspec(noreturn) void __stdcall WinMainCRTStartup(void)
#endif
{
    HRSRC hResource;

    if (!is_rdseed()) {
        MessageBoxW(0, L"This computer's processor lacks support for the RDSEED instruction that's used by the casting technique.\n\nMinimum requirements:\n    INTEL Ivy Bridge or newer\n    AMD  Ryzen or newer", I_CHING, MB_ICONSTOP);
        ExitProcess(1);
    }

    g.hInstance = GetModuleHandleW(0);
    
    for (UINT i = 0; i < 64; i++)   // Create the reverse mapping from wen to bin
        g.wen_to_bin[bin_to_wen[i] - 1] = i;

    for (UINT ico_idx = ICON_MAIN, res_id = IDI_MAIN; ico_idx <= ICON_NUMBER_9; ico_idx++, res_id++)
        g.icon[ico_idx] = LoadIconW(g.hInstance, MAKEINTRESOURCEW(res_id));

    hResource = FindResourceW(nullptr, MAKEINTRESOURCEW(IDB_BACKGROUND), L"BINARY");

    if (hResource) {
        DWORD   size  = SizeofResource(0, hResource);
        HGLOBAL hData = LoadResource  (0, hResource);

        if (hData)  {
            if (LockResource(hData)) {
                #pragma pack(push, 1)
                typedef struct {    // just the parts needed, we don't need the color table for 24-bit bitmaps
                    WORD  FileType;
                    DWORD FileSize;
                    WORD  Reserved1;
                    WORD  Reserved2;;
                    DWORD BitmapOffset;
                } bitmap_header, *pbitmap_header;
                #pragma pack(pop)

                auto bh = (pbitmap_header)hData;

                g.pPixels = (PBYTE)(hData) + bh->BitmapOffset;
            }
        }
    }

    g.line_num = HWND_OPT_HEXAGRAM;
    g.hCurHelp = LoadCursorA(0, IDC_HELP);

    INITCOMMONCONTROLSEX icc = { sizeof(INITCOMMONCONTROLSEX), ICC_LISTVIEW_CLASSES|ICC_TAB_CLASSES|ICC_STANDARD_CLASSES };
    InitCommonControlsEx(&icc);

    GetModuleFileNameW(g.hInstance, g.ini_file, _countof(g.ini_file));
    PathRenameExtensionW(g.ini_file, L".ini");

    WNDCLASSEXW wcex   = { sizeof(wcex) };
    wcex.lpfnWndProc   = wndproc_main;
    wcex.hInstance     = g.hInstance;
    wcex.hCursor       = LoadCursorW(0, (LPCWSTR)IDC_ARROW);
    wcex.hIconSm       = wcex.hIcon = g.icon[ICON_MAIN];
    wcex.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
    wcex.lpszClassName = I_CHING;

    if (RegisterClassExW(&wcex)) {
        g.hwnd[HWND_MAIN] = CreateWindowExW(0, wcex.lpszClassName, wcex.lpszClassName, WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX, CW_USEDEFAULT, MAIN_CLIENT_CX, CW_USEDEFAULT, MAIN_CLIENT_CY, nullptr, nullptr, g.hInstance, nullptr);

        if (g.hwnd[HWND_MAIN]) {
            MSG  msg;
            BOOL dark = TRUE;

            DwmSetWindowAttribute(g.hwnd[HWND_MAIN], DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof(dark));
            ShowWindow(g.hwnd[HWND_MAIN], SW_NORMAL);
            UpdateWindow(g.hwnd[HWND_MAIN]);
         
            while (GetMessageW(&msg, nullptr, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
        }
    }

    ExitProcess(0);
}

LRESULT CALLBACK wndproc_main(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        main_create(hwnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_PAINT:
        main_paint(hwnd);
        break;

   default:
        return DefWindowProcW(hwnd, message, wParam, lParam);
    }

    return 0;
}

VOID main_create(HWND hwnd_main) {
    POINT       ptCursor;
    HMONITOR    hMonitor;
    RECT        rcWindow;
    RECT        rcClient;
    HWND        hwnd;
    PARAFORMAT  pf = { sizeof(pf) };
    MONITORINFO mi = { sizeof(mi) };

    GetCursorPos(&ptCursor);
    hMonitor = MonitorFromPoint(ptCursor, MONITOR_DEFAULTTOPRIMARY);

    GetMonitorInfoW(hMonitor, &mi);

    GetWindowRect(hwnd_main, &rcWindow);
    GetClientRect(hwnd_main, &rcClient);

    INT cx = MAIN_CLIENT_CX + (RECT_CX(rcWindow) - RECT_CX(rcClient));
    INT cy = MAIN_CLIENT_CY + (RECT_CY(rcWindow) - RECT_CY(rcClient));
    INT  x = mi.rcWork.left + ((RECT_CX(mi.rcWork) - cx) / 2);
    INT  y = mi.rcWork.top  + ((RECT_CY(mi.rcWork) - cy) / 3);

    SetWindowPos(hwnd_main, 0, x, y, cx, cy, SWP_NOZORDER);

    g.font[FONT_NAME] = CreateFontW(-13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Verdana");
    g.font[FONT_TEXT] = CreateFontW(-12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Arial");
    g.font[FONT_CTRL] = CreateFontW(-11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Verdana");

    SendMessageW(hwnd_main, WM_SETFONT, (WPARAM)g.font[FONT_CTRL], 0);

    g.btn_awake[CMD_CAST] = 1;
    g.btn_awake[CMD_LOAD] = 1;

    g.hwnd[HWND_CMD_CAST] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|SS_OWNERDRAW, MAIN_CMD_CAST_X, MAIN_CMD_Y, CMD_CX, CMD_CY, hwnd_main, (HMENU)HWND_CMD_CAST, 0, 0);
    SetWindowSubclass(hwnd, &wndproc_cmd, HWND_CMD_CAST, 0);

    g.hwnd[HWND_CMD_BACK] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, MAIN_CMD_BACK_X, MAIN_CMD_Y, CMD_CX, CMD_CY, hwnd_main, (HMENU)HWND_CMD_BACK, 0, 0);
    SetWindowSubclass(hwnd, &wndproc_cmd, HWND_CMD_BACK, 1);

    g.hwnd[HWND_CMD_SAVE] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, MAIN_CMD_SAVE_X, MAIN_CMD_Y, CMD_CX, CMD_CY, hwnd_main, (HMENU)HWND_CMD_SAVE, 0, 0);
    SetWindowSubclass(hwnd, &wndproc_cmd, HWND_CMD_SAVE, 2);

    g.hwnd[HWND_CMD_LOAD] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, MAIN_CMD_OPEN_X, MAIN_CMD_Y, CMD_CX, CMD_CY, hwnd_main, (HMENU)HWND_CMD_LOAD, 0, 0);
    SetWindowSubclass(hwnd, &wndproc_cmd, HWND_CMD_LOAD, 3);

    g.hwnd[HWND_EDIT] = hwnd = CreateWindowExW(0, L"EDIT", 0, WS_CHILD|WS_BORDER|WS_CHILDWINDOW|WS_VISIBLE|ES_NOHIDESEL, UI_PAD, EDIT_Y, EDIT_CX, EDIT_CY, hwnd_main, (HMENU)HWND_EDIT, 0, 0);
    SetWindowSubclass(hwnd, &wndproc_edit, HWND_EDIT, 0);

    SendMessageW(hwnd, WM_SETFONT, (WPARAM)g.font[FONT_TEXT], 0);
    SendMessageW(hwnd, EM_SETREADONLY, FALSE, 0);
    SendMessageW(hwnd, EM_SETCUEBANNER, 1, (LPARAM)L"Enter a query or a hexagram number from 1 to 64");

    g.hwnd[HWND_OPT_HEXAGRAM]  = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, HEX_NAME_X, HEX_NAME_Y,   NAME_CX, LINE_CY, hwnd_main, (HMENU)HWND_OPT_HEXAGRAM,  0, 0);
    SetWindowSubclass(hwnd, &wndproc_opt, HWND_OPT_HEXAGRAM,  0x010000);    

    g.hwnd[HWND_OPT_HEX_LINE6] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, HEX_LINE_X, LINE_6_Y,     LINE_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_HEX_LINE6, 0, 0);
    SetWindowSubclass(hwnd, &wndproc_opt, HWND_OPT_HEX_LINE6, 0x010600 | 1 << 5);                                                                                         
                                                                                                                                              
    g.hwnd[HWND_OPT_HEX_LINE5] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, HEX_LINE_X, LINE_5_Y,     LINE_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_HEX_LINE5, 0, 0);
    SetWindowSubclass(hwnd, &wndproc_opt, HWND_OPT_HEX_LINE5, 0x010500 | 1 << 4);                                                                                         
                                                                                                                                              
    g.hwnd[HWND_OPT_HEX_LINE4] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, HEX_LINE_X, LINE_4_Y,     LINE_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_HEX_LINE4, 0, 0);
    SetWindowSubclass(hwnd, &wndproc_opt, HWND_OPT_HEX_LINE4, 0x010400 | 1 << 3);                                                                                         
                                                                                                                                              
    g.hwnd[HWND_OPT_HEX_LINE3] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, HEX_LINE_X, LINE_3_Y,     LINE_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_HEX_LINE3, 0, 0);
    SetWindowSubclass(hwnd, &wndproc_opt, HWND_OPT_HEX_LINE3, 0x010300 | 1 << 2);                                                                                         
                                                                                                                                              
    g.hwnd[HWND_OPT_HEX_LINE2] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, HEX_LINE_X, LINE_2_Y,     LINE_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_HEX_LINE2, 0, 0);
    SetWindowSubclass(hwnd, &wndproc_opt, HWND_OPT_HEX_LINE2, 0x010200 | 1 << 1);                                                                                         
                                                                                                                                              
    g.hwnd[HWND_OPT_HEX_LINE1] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, HEX_LINE_X, LINE_1_Y,     LINE_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_HEX_LINE1, 0, 0);
    SetWindowSubclass(hwnd, &wndproc_opt, HWND_OPT_HEX_LINE1, 0x010100 | 1 << 0);                                                                                         
                                                                                                                                              
    g.hwnd[HWND_OPT_CHANGING]  = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CHG_NAME_X, CHG_NAME_Y,   NAME_CX, LINE_CY, hwnd_main, (HMENU)HWND_OPT_CHANGING,  0, 0);
    SetWindowSubclass(hwnd, &wndproc_opt, HWND_OPT_CHANGING, 0);                                                                                              
                                                                                                                                              
    g.hwnd[HWND_OPT_CHG_LINE6] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CHG_LINE_X,  LINE_6_Y,    LINE_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_CHG_LINE6, 0, 0);
    SetWindowSubclass(hwnd, &wndproc_opt, HWND_OPT_CHG_LINE6, 0x000600);                                                                                              
                                                                                                                                              
    g.hwnd[HWND_OPT_CHG_LINE5] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CHG_LINE_X,  LINE_5_Y,    LINE_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_CHG_LINE5, 0, 0);
    SetWindowSubclass(hwnd, &wndproc_opt, HWND_OPT_CHG_LINE5, 0x000500);                                                                                              
                                                                                                                                              
    g.hwnd[HWND_OPT_CHG_LINE4] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CHG_LINE_X,  LINE_4_Y,    LINE_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_CHG_LINE4, 0, 0);
    SetWindowSubclass(hwnd, &wndproc_opt, HWND_OPT_CHG_LINE4, 0x000400);                                                                                             
                                                                                                                                              
    g.hwnd[HWND_OPT_CHG_LINE3] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CHG_LINE_X,  LINE_3_Y,    LINE_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_CHG_LINE3, 0, 0);
    SetWindowSubclass(hwnd, &wndproc_opt, HWND_OPT_CHG_LINE3, 0x000300);                                                                                             
                                                                                                                                              
    g.hwnd[HWND_OPT_CHG_LINE2] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CHG_LINE_X,  LINE_2_Y,    LINE_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_CHG_LINE2, 0, 0);
    SetWindowSubclass(hwnd, &wndproc_opt, HWND_OPT_CHG_LINE2, 0x000200);                                                                                             
                                                                                                                                              
    g.hwnd[HWND_OPT_CHG_LINE1] = hwnd = CreateWindowExW(0, L"BUTTON", 0, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CHG_LINE_X,  LINE_1_Y,    LINE_CX,  LINE_CY, hwnd_main, (HMENU)HWND_OPT_CHG_LINE1, 0, 0);
    SetWindowSubclass(hwnd, &wndproc_opt, HWND_OPT_CHG_LINE1, 0x000100);

    g.rect[RECT_HEX]  = { HEX_X,      HEX_Y,      HEX_X + HEX_CX,           HEX_Y + HEX_CY  };
    g.rect[RECT_CHG]  = { CHG_X,      CHG_Y,      CHG_X + CHG_CX,           CHG_Y + CHG_CY  };
    g.rect[RECT_NAME] = { TXT_NAME_X, TXT_NAME_Y, TXT_NAME_X + TXT_NAME_CX, TXT_NAME_Y + TXT_NAME_CY };

    LoadLibraryW(L"RICHED20");

    g.hwnd[HWND_TEXT] = hwnd = CreateWindowExW(WS_EX_TRANSPARENT, RICHEDIT_CLASSW, 0, WS_CHILD|WS_VSCROLL|ES_MULTILINE|WS_VISIBLE, TEXT_X, TEXT_Y, TEXT_CX, TEXT_CY, hwnd_main, (HMENU)HWND_TEXT, 0, 0);

    SendMessageW(hwnd, WM_SETFONT, (WPARAM)g.font[FONT_TEXT], 0);

    pf.dwMask     = PFM_ALIGNMENT|PFM_TABSTOPS;
    pf.cTabCount  = 1;
    pf.rgxTabs[0] = 360;
    pf.wAlignment = PFA_FULL_INTERWORD;

    hwnd = g.hwnd[HWND_TEXT];

    SendMessageW(hwnd, EM_SETTYPOGRAPHYOPTIONS, TO_ADVANCEDTYPOGRAPHY, TO_ADVANCEDTYPOGRAPHY);
    SendMessageW(hwnd, EM_SETREADONLY, TRUE, 0);
    SendMessageW(hwnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf);

    SetFocus(g.hwnd[HWND_EDIT]);
}

VOID main_paint(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC         hdc    = BeginPaint(hwnd, &ps);
    HBRUSH      hBrush = CreateSolidBrush(RGB(116, 116, 116));

    StretchDIBits(hdc,  0, 0, MAIN_CLIENT_CX, MAIN_CLIENT_CY, 0, 0, MAIN_CLIENT_CX, MAIN_CLIENT_CY, g.pPixels, &main_bmpInfo, DIB_RGB_COLORS, SRCCOPY);

    FrameRect(hdc, &g.rect[RECT_HEX], hBrush);
    FrameRect(hdc, &g.rect[RECT_CHG], hBrush);

    if (g.hex_num) {
        PWSTR p;

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, 0);
        SelectObject(hdc, g.font[FONT_NAME]);

        p = (g.line_num == HWND_OPT_CHANGING) ? g.chg_title : g.hex_title;
        
        TextOutW(hdc, g.rect[RECT_NAME].left, g.rect[RECT_NAME].top, p, wcslen(p));
    }
        
    DeleteObject(hBrush);
    EndPaint(hwnd, &ps);
}

LRESULT CALLBACK wndproc_cmd(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
    BOOL enabled = g.btn_awake[dwRefData];

    switch (message) {
    case WM_LBUTTONDOWN:
        if (enabled) {
            g.btn_state[dwRefData] = CMD_DOWN;
            InvalidateRect(hwnd, 0, FALSE);
        } break;

    case WM_LBUTTONUP:
        if (enabled) {
            switch (uIdSubclass) {
            case HWND_CMD_CAST:
                cmd_cast();
                break;

            case HWND_CMD_BACK:
                cmd_back();
                break;

            case HWND_CMD_SAVE:
                btn_save();
                break;

            case HWND_CMD_LOAD:
                cmd_load();
                break;
            }

            g.btn_state[dwRefData] = g.btn_awake[dwRefData] ? CMD_OVER : CMD_NORMAL;
            InvalidateRect(hwnd, 0, FALSE);
        } break;

    case WM_MOUSEMOVE:
        if (enabled && g.btn_state[dwRefData] == CMD_NORMAL) {
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
        UINT        flags  = BF_RECT;
        HDC         hdc    = BeginPaint(hwnd, &ps);
        HBRUSH      hBrush = CreateSolidBrush(RGB(255, 255, 255));

        FillRect(hdc, &ps.rcPaint, hBrush);

        switch (dwRefData) {
            case 0: icon = enabled ? ICON_CMD_CAST_ON : ICON_CMD_CAST_OFF; break;  
            case 1: icon = enabled ? ICON_CMD_BACK_ON : ICON_CMD_BACK_OFF; break;
            case 2: icon = enabled ? ICON_CMD_SAVE_ON : ICON_CMD_SAVE_OFF; break;
            case 3: icon =           ICON_CMD_LOAD_ON;                     break;
        }

        switch (g.btn_state[dwRefData]) {
        case CMD_NORMAL:
            delta  = 0;
            edge   = EDGE_ETCHED;
            flags |= BF_MONO;
            break;

        case CMD_OVER:
            delta  = -2;
            edge   = EDGE_RAISED;
            break;

        case CMD_DOWN:
            delta  = +2;
            edge   = EDGE_SUNKEN;
            break;
        }

        DrawIconEx(hdc, 8 + delta, 8 + delta, g.icon[icon], 48, 48, 0, 0, DI_IMAGE | DI_MASK);
        DrawEdge(hdc, &ps.rcPaint, edge, flags);

        DeleteObject(hBrush);
        EndPaint(hwnd, &ps);
    } break;

    default:
        return DefSubclassProc(hwnd, message, wParam, lParam);
    }

    return 0;
}

LRESULT CALLBACK wndproc_edit(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
    if (message == WM_KEYUP && wParam == VK_RETURN)
        cmd_cast();
    else
    if (message == WM_CHAR  && wParam == VK_RETURN)
        return 0; // prevent the default beep

    return DefSubclassProc(hwnd, message, wParam, lParam);
}

LRESULT CALLBACK wndproc_opt(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
    BOOL enabled;
    BYTE bits = (dwRefData >>  0) & 0xFF;
    UINT line = (dwRefData >>  8) & 0xFF;
    BOOL main = (dwRefData >> 16) & 0xFF;

    switch (uIdSubclass) {
        case HWND_OPT_HEXAGRAM: enabled = TRUE;              break;
        case HWND_OPT_CHANGING: enabled = g.chg_bits;        break;
        default:                enabled = bits & g.chg_bits; break;
    }

    switch (message) {
    case WM_LBUTTONDOWN:
        if (enabled && g.line_num != uIdSubclass) {
            PWSTR p;

            g.last_num = g.line_num;
            g.line_num = uIdSubclass;

            switch (uIdSubclass) {
            case HWND_OPT_HEXAGRAM: p = g.hex_text;
                break;

            case HWND_OPT_CHANGING: p = g.chg_text;
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

            SendMessageW(g.hwnd[HWND_TEXT], WM_SETTEXT, 0, (LPARAM)p);
            InvalidateRect(g.hwnd[g.last_num], 0, FALSE);
            InvalidateRect(g.hwnd[g.line_num], 0, FALSE);
            InvalidateRect(g.hwnd[HWND_MAIN], &g.rect[RECT_NAME], FALSE);
            InvalidateRect(g.hwnd[HWND_MAIN], &g.rect[RECT_HEX], FALSE);
        }
        break;

    case WM_ERASEBKGND:
        break;

    case WM_MOUSEMOVE:
        if (enabled && g.line_num != uIdSubclass) {
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

            if (enabled) {
                if (uIdSubclass == g.line_num)
                    DrawIconEx(hdc, 0, 0, g.icon[ICON_OPT_SET], 16, 16, 0, 0, DI_IMAGE | DI_MASK);
                else
                    DrawIconEx(hdc, 0, 0, g.icon[ICON_OPT_OFF], 16, 16, 0, 0, DI_IMAGE | DI_MASK);

                switch (uIdSubclass) {
                case HWND_OPT_HEXAGRAM:
                    p = g.hex_title;
                    break;

                case HWND_OPT_CHANGING:
                    p = g.chg_title;
                    break;
                }

                if (p) {
                    SelectObject(hdc, g.font[FONT_NAME]);
                    TextOutW(hdc, 24, 0, p, wcslen(p));
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
                    DrawIconEx(hdc,  32, 3, g.icon[ICON_NUMBER_1 + line],                    8, 10, 0, 0, DI_IMAGE | DI_MASK);
                    DrawIconEx(hdc,  56, 3, g.icon[ico_idx],                               140, 10, 0, 0, DI_IMAGE | DI_MASK);
                    DrawIconEx(hdc, 212, 3, g.icon[ICON_NUMBER_6 - ICON_LINE_6 + ico_idx],   8, 10, 0, 0, DI_IMAGE | DI_MASK);
                }
            } break;
    
            EndPaint(hwnd, &ps);
        } break;

    default:
        return DefSubclassProc(hwnd, message, wParam, lParam);
    }

    return 0;
}

VOID cmd_cast(VOID) {
    _asm {
        _empty_the_entropy_buffer: 
            rdseed eax
            jc     _empty_the_entropy_buffer

        _get_a_freshly_minted_seed:  
            rdseed eax
            jnc    _get_a_freshly_minted_seed
            mov    g.seed, eax
    }

    g.hex_bits = 0;
    g.hex_num  = 0;
    g.chg_bits = 0;
    g.chg_num  = 0;

    BOOL save = TRUE;

    SendMessageW(g.hwnd[HWND_EDIT], WM_GETTEXT, _countof(g.query), (LPARAM)g.query);
 
    g.hex_num = _wtoi(g.query);

    if (g.hex_num >= 1 && g.hex_num <= 64 && wcslen(g.query) < 3) {
        g.seed     = 0;
        g.hex_bits = g.wen_to_bin[g.hex_num - 1];
        g.chg_bits = 0b111111;
        g.chg_num  = bin_to_wen[g.hex_bits ^ g.chg_bits];
        save       = FALSE;
    } else {
        g.seed &= 0b111111111111111111;

        for (UINT i = 0, v = g.seed, bit = 1; i < 6; i++, bit <<= 1) {
            switch (v & 0b111) {
            case 3:
                g.chg_bits |= bit;
                break;

            case 7:
                g.chg_bits |= bit;
            case 4: case 5: case 6:
                g.hex_bits |= bit;
                break;
            }

            v >>= 3;
        }

        g.hex_num = bin_to_wen[g.hex_bits];
        g.chg_num = g.chg_bits ? bin_to_wen[g.hex_bits ^ g.chg_bits] : 0;
    }

    g.btn_awake[CMD_CAST] = 0;
    g.btn_awake[CMD_BACK] = 1;

    if (save)
        g.btn_awake[CMD_SAVE] = 1;

    get_text();

    SendMessageW(g.hwnd[HWND_TEXT], WM_SETTEXT, 0, (LPARAM)g.hex_text);
    SendMessageW(g.hwnd[HWND_EDIT], EM_SETREADONLY, TRUE, 0);
    SetFocus(g.hwnd[HWND_MAIN]);
    InvalidateRect(g.hwnd[HWND_MAIN], 0, FALSE);
}

VOID cmd_back(VOID) {
    g.hex_bits = 0;
    g.hex_num  = 0;
    g.chg_bits = 0;
    g.chg_num  = 0;
    g.line_num = HWND_OPT_HEXAGRAM;
    g.last_num = 0;

    g.btn_awake[CMD_CAST] = 1;
    g.btn_awake[CMD_BACK] = 0;
    g.btn_awake[CMD_SAVE] = 0;


    SendMessageW(g.hwnd[HWND_EDIT], WM_SETTEXT, 0, 0);
    SendMessageW(g.hwnd[HWND_TEXT], WM_SETTEXT, 0, 0);
    SendMessageW(g.hwnd[HWND_EDIT], EM_SETREADONLY, FALSE, 0);
    SetFocus(g.hwnd[HWND_EDIT]);
    InvalidateRect(g.hwnd[HWND_MAIN], 0, FALSE);
}

VOID btn_save(VOID) {
}

VOID cmd_load(VOID) {
}

VOID get_text(VOID) {
    WCHAR para[1024];
    WCHAR section[32];
    WCHAR key[32];

    swprintf_s(section, _countof(section), L"Hexagram_%02d", g.hex_num);

    if (GetPrivateProfileStringW(section, L"Title", 0, para, _countof(para), g.ini_file))
        swprintf_s(g.hex_title, _countof(g.hex_title), L"%d. %s", g.hex_num, para);

    if (g.chg_bits) {
        for (UINT i = 0; i < 6; i++) {
            if (g.chg_bits & (1 << i)) {
                swprintf_s(key, _countof(key), L"Line_%d", i + 1);

                if (GetPrivateProfileStringW(section, key, 0, para, _countof(para), g.ini_file)) {
                    PCWSTR pNum = g.hex_bits & (1 << i) ? L"Nine" : L"Six";

                    wcscpy_s(g.lines[i], _countof(g.lines[0]), pNum);
                
                    switch (i) {
                    case 0: wcscat_s(g.lines[i], _countof(g.lines[0]), L" at the beginning means:");
                        break;
        
                    case 1: wcscat_s(g.lines[i], _countof(g.lines[0]), L" in the second place means:");
                        break;

                    case 2: wcscat_s(g.lines[i], _countof(g.lines[0]), L" in the third place means:");
                        break;

                    case 3: wcscat_s(g.lines[i], _countof(g.lines[0]), L" in the fourth place means:");
                        break;

                    case 4: wcscat_s(g.lines[i], _countof(g.lines[0]), L" in the fith place means:");
                        break;

                    case 5: wcscat_s(g.lines[i], _countof(g.lines[0]), L" at the top means:");
                        break;
                    
                    }

                    wcscat_s(g.lines[i], _countof(g.lines[0]), L"\r\n\r\n       ");
                    wcscat_s(g.lines[i], _countof(g.lines[0]), para);
                }
            }
        }
    }

    wcscpy_s(g.hex_text, _countof(g.hex_text), L"      ");

    for (UINT i = 1; i < 10; i++) {
        swprintf_s(key, _countof(key), L"Text_%d", i);

        if (GetPrivateProfileStringW(section, key, 0, para, sizeof(para), g.ini_file)) {
            wcscat_s(g.hex_text, _countof(g.hex_text), para);
            wcscat_s(g.hex_text, _countof(g.hex_text), L"\r\r"); //L"\r\n\r\n");
        } else break;
    }

    if (g.chg_bits) {
        swprintf_s(section, _countof(section), L"Hexagram_%02d", g.chg_num);

        if (GetPrivateProfileStringW(section, L"Title", 0, para, _countof(para), g.ini_file))
            swprintf_s(g.chg_title, _countof(g.chg_title), L"%d. %s", g.chg_num, para);

        wcscpy_s(g.chg_text, _countof(g.chg_text), L"       ");

        for (UINT i = 1; i < 10; i++) {
            swprintf_s(key, _countof(key), L"Text_%d", i);

            if (GetPrivateProfileStringW(section, key, 0, para, sizeof(para), g.ini_file)) {
                wcscat_s(g.chg_text, _countof(g.chg_text), para);
                wcscat_s(g.chg_text, _countof(g.chg_text), L"\r\r");
            } else break;
        }
    }
}

__declspec(naked) BOOL is_rdseed(VOID) {
    _asm {
        mov     eax, 1      ; check for RDSEED instruction support
        cpuid
        mov     eax, ecx
        shr     eax, 30
        and     eax, 1
        ret
    }
}

extern "C" {
    #pragma function(memset)
    PVOID __cdecl memset(PVOID dst, int value, size_t count) {
        __stosb((PBYTE)dst, value, count);
        return dst;
    }

    #pragma function(memcpy)
    PVOID _cdecl memcpy(_Out_writes_bytes_all_(itm_size) PVOID dst, _In_reads_bytes_(itm_size) void const* src, _In_ size_t itm_size) {
        __movsb((PBYTE)dst, (PBYTE)src, itm_size);
        return dst;
    }
}