#include <windows.h>
#include <tchar.h>
#include <time.h>
#include <ctime>
#include<commctrl.h>
#include <windowsX.h>
#include "resource.h"

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

HANDLE thread1, thread2;
HWND hTime, hProgress, hRadio1;
TCHAR str_time[50];
SYSTEMTIME st;


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

DWORD WINAPI ThreadControlBar(LPVOID lp)
{
	HWND hProg = (HWND)lp;
	while (TRUE)
	{
		SendMessage(hProg, PBM_SETPOS, rand() % 200+1, 0);
		Sleep(250);
	}
	return 0;
}

DWORD WINAPI ThreadTime(LPVOID lp) {
	/*HWND htime = HWND(lp);
	while (TRUE) {
		SendMessage(hTime, WM_SETTEXT, 0, LPARAM(str_time));
	}
	return 0;*/
	HWND hProgress = (HWND)lp;
	struct Time {
		int Minutes = 2;
		int Seconds = 30;
	} Time;

	while (TRUE) {
		wsprintf(str_time, TEXT("%d:%d"), Time.Minutes, Time.Seconds);
		SetWindowText(hProgress, str_time);
		if (Time.Seconds <= 0) {
			Time.Seconds = 60;
			if (Time.Minutes <= 0) {
				Time.Seconds--;
				ExitThread;
				MessageBox(hTime, TEXT("Время вышло :("), TEXT("Это конец"), MB_OK | MB_ICONINFORMATION);
				return 0;
			}
			else {
				Time.Seconds--;
				Time.Minutes--;
			}
		}
		else {
			Time.Seconds--;
		}
		Sleep(1000);
	}
	return 0;
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG: {
		hTime = GetDlgItem(hWnd, IDC_TIME);
		hRadio1 = GetDlgItem(hWnd, IDC_RADIO1);

		hProgress = GetDlgItem(hWnd, IDC_PROGRESS1);
		SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 200));
		SendMessage(hProgress, PBM_SETSTEP, 1, 0);
		SendMessage(hProgress, PBM_SETPOS, 0, 0);
		SendMessage(hProgress, PBM_SETBARCOLOR, 0, LPARAM(RGB(0, 214, 120)));

		thread1 = CreateThread(NULL, 0, ThreadControlBar, hProgress, 0, NULL);
		thread2 = CreateThread(NULL, 0, ThreadTime, hTime, 0, NULL);
	}
	break;

	case WM_COMMAND: {
		ResumeThread(thread1);
		ResumeThread(thread2);
	}
	break;

	//case WM_PAINT:{
	//	srand(time(0));
	//	GetSystemTime(&st);
	//	wsprintf(str_time, _T("%d:%d:%d"), st.wHour, st.wMinute, st.wSecond); // %d-%02d-%02d
	//	wprintf(_T("%s"), str_time);
	//	
	//}
	//break;
	//case WM_TIMER: {
	//	
	//}
	//break;

	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	}
	return FALSE;
}