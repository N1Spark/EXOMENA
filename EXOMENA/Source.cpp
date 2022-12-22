#include <windows.h>
#include <fstream>
#include <tchar.h>
#include <windowsX.h>
#include <string>
#include "resource.h"
#pragma comment(lib, "comctl32")
#include <CommCtrl.h>
using std::wstring;
using namespace std;

HWND hKeyboard[27];
HWND hLastKey = NULL;
HWND hStart;
HWND hText, hMiss, hTime, hChar_c, hProg, hProc;
TCHAR text[255], buffer_s[255];
fstream words;
string s_buf;
int mistake = 0, percents = 0;
int rand_count, sym_co = 0;
int corr = 0;
int count_;

BOOL CALLBACK DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void KeyDownHandler(HWND hwnd, WPARAM wParam, LPARAM lParam);

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        for (int i = 0; i < 27; i++)
        {
            hKeyboard[i] = GetDlgItem(hwnd, IDC_A + i);
        }
        words.open(TEXT("Words.txt"));
        rand_count = rand() % (1 - 10) + 1;
        for (int i = 0; i < rand_count; i++)
        {
            getline(words, s_buf);
        }
        for (int i = 0; i < s_buf.size(); i++)
        {
            text[i] = s_buf[i];
        }
        sym_co = _tcslen(text);
        wsprintf(buffer_s, TEXT("%d"), sym_co);
        count_ = sym_co - 1;
        hStart = GetDlgItem(hwnd, IDC_START);
        hText = GetDlgItem(hwnd, IDC_EDIT1);
        hMiss = GetDlgItem(hwnd, IDC_EDIT2);
        hProg = GetDlgItem(hwnd, IDC_PROGRESS1);
        hProc = GetDlgItem(hwnd, IDC_EDIT5);
        SendMessage(hProg, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
        SendMessage(hProg, PBM_SETPOS, 0, 100);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_START)
        {
            SetWindowText(hText, text);
            SetWindowText(hMiss, TEXT("0"));
            SetWindowText(hProc, TEXT("0"));
            SetWindowText(hChar_c, buffer_s);
            EnableWindow(hStart, FALSE);
        }
        break;
    case WM_SYSKEYDOWN:
        KeyDownHandler(hwnd, wParam, lParam);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return TRUE;
}

void KeyDownHandler(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    EnableWindow(hLastKey, FALSE);
    int num_key = 65;
    int vkey = (TCHAR)wParam;
    WCHAR str[50];
    wsprintf(str, L"vkey: %c", vkey);
    
    for (int i = 0; num_key <= 90; i++, num_key++)
    {
        if (vkey == num_key)
        {
            EnableWindow(hKeyboard[i], TRUE);
            hLastKey = hKeyboard[i];
        }
        else if (vkey == 32)
        {
            EnableWindow(hKeyboard[26], TRUE);
            hLastKey = hKeyboard[26];
        }
    }
    if (TCHAR(vkey + 32) == (int)text[0] || (vkey == 32 && text[0] == ' '))
    {
        int i;
        for (i = 1; i < sym_co; i++)
        {
            text[i - 1] = text[i];
            text[sym_co - 1] = '\0';
        }
        corr++;
        percents = (corr * 100) / count_;
        wsprintf(buffer_s, TEXT("%d"), percents);
        SetWindowText(hProc, buffer_s);
        SendMessage(hProg, PBM_SETPOS, percents, 0);
        sym_co--;
        SetWindowText(hText, text);
    }
    else
    {
        mistake++;
        wsprintf(buffer_s, TEXT("%d"), mistake);
        SetWindowText(hMiss, buffer_s);
      
    }
    if (text[0] == '\0')
    {
        EnableWindow(hText, TRUE);
        SetFocus(hText);
        int result = count_ - mistake;
        wsprintf(buffer_s, TEXT("Кол-во правильных клавиш = %d / %d"), result, count_);
        SetWindowText(hwnd, buffer_s);
    }
}