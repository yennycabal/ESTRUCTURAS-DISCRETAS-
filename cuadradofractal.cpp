#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include "resource.h"

HINSTANCE hInst;

class Figura
{
public:
    HRGN figura;

    void F(HDC hdc, int x, int y, int a)
    {
        HRGN c = CreateRectRgn(x-a, y-a, x+a, y+a);
        CombineRgn(figura, figura, c, RGN_OR);

        FillRgn(hdc, c, (HBRUSH)GetStockObject(WHITE_BRUSH));
        FrameRgn(hdc, figura, (HBRUSH)GetStockObject(BLACK_BRUSH), 1, 1);
        DeleteObject(c);

        Sleep(50);

        if (a >10)
        {
            F(hdc, x-a, y-a, a/2);
            F(hdc, x-a, y+a, a/2);
            F(hdc, x+a, y-a, a/2);
            F(hdc, x+a, y+a, a/2);
        }
    }
};

Figura G;

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_PAINT:
        {
            HDC hdc;
            PAINTSTRUCT ps;

            hdc = BeginPaint(hwndDlg, &ps);
            Rectangle(hdc, 10, 10, 500, 400);

            G.figura = CreateRectRgn(0, 0, 0, 0);
            G.F(hdc, 250, 200, 50);
            DeleteObject(G.figura);

            EndPaint(hwndDlg, &ps);

            return TRUE;
        }

        case WM_INITDIALOG:
            return TRUE;

        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
            return TRUE;

        case WM_COMMAND:
            return TRUE;
    }

    return FALSE;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst = hInstance;
    InitCommonControls();

    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
