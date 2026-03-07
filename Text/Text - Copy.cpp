// Text.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>
#include <intrin.h>

PSTR get_line (PSTR startt, PCSTR find, PSTR* end);

int main(void) {
    PSTR p1;
    PSTR p2;
    CHAR buf[8192];
    CHAR ini_file[MAX_PATH];

    GetModuleFileNameA(0, ini_file, sizeof(ini_file));

    p1 = StrRChrA(ini_file, 0, '\\');

    if (!p1)
          return 1;

    strcpy(p1 + 1, "I-Ching.ini");

    GetPrivateProfileStringA("Hexagram_01", "Text", "", buf, sizeof(buf), ini_file);
    printf(buf);

    for (int i = 1; i <= 64; i++) {
        DWORD   bytes_read;
        CHAR    sect_name[20];
        CHAR    file_name[20];
        HANDLE  file_handle;
        LARGE_INTEGER 
                file_size;

        sprintf_s(file_name, sizeof(file_name), "Hex_%02d.txt", i);

        file_handle = CreateFileA(file_name, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);

        if (file_handle == INVALID_HANDLE_VALUE) {
            printf("Failed to open: %s", file_name);
            ExitProcess(1);
        }

        GetFileSizeEx(file_handle, &file_size);

        ReadFile(file_handle, buf, file_size.LowPart, &bytes_read, 0);
        CloseHandle(file_handle);

        sprintf_s(sect_name, sizeof(file_name), "Hexagram_%02d", i);

        p1 = get_line(buf, "$Title: ", &p2);
        if (p1) {
            WritePrivateProfileStringA(sect_name, "Title", p1, ini_file);

            for (int l = 1; l <= 6; l++) {
                CHAR line[20];

                sprintf_s(line, sizeof(line), "$Line %d: ", l);
                p1 = get_line(p2, line, &p2);
                if (p1) {
                    sprintf_s(line, sizeof(line), "Line_%d", l);
                    WritePrivateProfileStringA(sect_name, line, p1, ini_file);
                }
            }

            p1 = StrStrA(p2, "$Text: ");

            if (p1) {
                buf[bytes_read] = 0;
                p1 += 7;    

                //WritePrivateProfileStringA(sect_name, "Text", p1, ini_file);
                for (int l = 1; l <= 8; l++) {
                    CHAR line[20];

                    p2 = StrStrA(p1, "\r\n\r\n");
                    if (p2) {
                       *p2 = 0;
                    }
                    
                    sprintf_s(line, sizeof(line), "Text_%d", l);
                    WritePrivateProfileStringA(sect_name, line, p1, ini_file);

                    if (!p2)
                        break;
                    p1 = p2 + 4;
                }
            }
        }
    }

    WritePrivateProfileStringA(0, 0, 0, ini_file);
    return 0;
}

PSTR get_line(PSTR start, PCSTR find, PSTR* end) {
    PSTR   p1;
    PSTR   p2;

    p1 = StrStrA(start, find);

    if (p1) {
        p1 += strlen(find);
        p2 = StrStrA(p1, "\r\n");

        if (p2) {
            *p2  = 0;
            *end = p2 + 2;

            return p1;
        }
    }

    return 0;
}