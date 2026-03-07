// Text.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>
#include <intrin.h>

const BYTE bin_to_wen[64] = { 2, 24, 7, 19, 15, 36, 46, 11, 16, 51, 40, 54, 62, 55, 32, 34, 8, 3, 29, 60, 39, 63, 48, 5, 45, 17, 47, 58, 31, 49, 28, 43, 23, 27, 4, 41, 52, 22, 18, 26, 35, 21, 64, 38, 56, 30, 50, 14, 20, 42, 59, 61, 53, 37, 57, 9, 12, 25, 6, 10, 33, 13, 44, 1 };

      BYTE wen_to_bin[64];

PSTR get_line (PSTR startt, PCSTR find, PSTR* end);

int main(void) {
    PSTR  p1;
    PSTR  p2;
    CHAR  buf[8192];
    CHAR  ini_file[MAX_PATH];
    FILE* h;

    for (UINT i = 0; i < 64; i++)   // Create the reverse mapping from wen to bin
        wen_to_bin[bin_to_wen[i] - 1] = i;

    GetModuleFileNameA(0, ini_file, sizeof(ini_file));

    p1 = StrRChrA(ini_file, 0, '\\');

    if (!p1)
          return 1;

    strcpy(p1 + 1, "I-Ching.ini");

    //GetPrivateProfileStringA("Hexagram_01", "Text", "", buf, sizeof(buf), ini_file);
    //printf(buf);

    h = fopen(ini_file, "w+");

    for (int i = 1; i <= 64; i++) {
        DWORD   bytes_read;
        CHAR    file_name[20];
        HANDLE  file_handle;
        LARGE_INTEGER 
                file_size;
        BYTE    hex_bits = wen_to_bin[i - 1];

        sprintf_s(file_name, sizeof(file_name), "Hex_%02d.txt", i);

        file_handle = CreateFileA(file_name, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);

        if (file_handle == INVALID_HANDLE_VALUE) {
            printf("Failed to open: %s", file_name);
            ExitProcess(1);
        }

        GetFileSizeEx(file_handle, &file_size);

        ReadFile(file_handle, buf, file_size.LowPart, &bytes_read, 0);
        CloseHandle(file_handle);

        fprintf(h, "[Hexagram_%d]\n", i);
        
        p1 = get_line(buf, "$Title: ", &p2);
        if (p1) {
            fprintf(h, "%d. %s\n", i, p1);

            for (int l = 0; l < 6; l++) {
                CHAR line[20];

                sprintf_s(line, sizeof(line), "$Line %d: ", l + 1);

                p1 = get_line(p2, line, &p2);

                if (p1) {
                    PCSTR pNum = hex_bits & (1 << (l)) ? "Nine" : "Six";
                    PCSTR pTxt = nullptr;

                    switch (l) {
                    case 0: pTxt = " at the beginning means";
                        break;

                    case 1: pTxt = " in the second place means";
                        break;

                    case 2: pTxt = " in the third place means";
                        break;

                    case 3: pTxt = " in the fourth place means";
                        break;

                    case 4: pTxt = " in the fith place means";
                        break;

                    case 5: pTxt = " at the top means";
                        break;
                    }

                    fprintf(h, "%s\n", p1);
                }
            }

            p1 = StrStrA(p2, "$Text: ");

            if (p1) {
                buf[bytes_read] = 0;
                p1 += 7;    

                for (int l = 1; l <= 8; l++) {
                    p2 = StrStrA(p1, "\r\n\r\n");
                    if (p2) {
                       *p2 = 0;
                    }
                    
                    if (l == 1)
                        fprintf(h, "%s", p1);
                    else
                        fprintf(h, "$$%s", p1);

                    if (!p2) {
                        fprintf(h, "\n\n");
                        break;
                    }

                    p1 = p2 + 4;
                }
            }
        }
    }

    //WritePrivateProfileStringA(0, 0, 0, ini_file);
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