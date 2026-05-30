//IPcalculator
#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<CommCtrl.h>
#include<cstdio>
#include"resource.h"

/*
----------------------
Prefix:
255.0.0.0 /8
255.255.0.0 /16
255.255.255.0 /24
255.255.255.128	/25
----------------------
*/

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, (DLGPROC)DlgProc, NULL);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		SetFocus(GetDlgItem(hwnd, IDC_IPADDRESS));
		SendMessage(GetDlgItem(hwnd, IDC_SPIN_PREFIX), UDM_SETRANGE, 0, MAKEWORD(32, 0));
		break;
	case WM_COMMAND:
	{
		HWND hIPaddress = GetDlgItem(hwnd, IDC_IPADDRESS);
		HWND hIPmask = GetDlgItem(hwnd, IDC_IPMASK);
		HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
		DWORD dwIPaddress = 0;
		DWORD dwIPmask = 0;
		DWORD dwPrefix = 0;
		CHAR szPrefix[3] = {};
		//https://learn.microsoft.com/en-us/windows/win32/controls/bumper-ip-address-control-reference-messages
		switch (LOWORD(wParam))
		{
		case IDC_IPADDRESS:
			if (HIWORD(wParam) == EN_CHANGE)
			{
				SendMessage(hIPaddress, IPM_GETADDRESS, 0, (LPARAM)&dwIPaddress);
				DWORD dwFirst = FIRST_IPADDRESS(dwIPaddress);
				LPARAM lpMask = 0;
				if (dwFirst < 128)lpMask = MAKEIPADDRESS(255, 0, 0, 0);
				else if (dwFirst < 192)lpMask = MAKEIPADDRESS(255, 255, 0, 0);
				else if (dwFirst < 224)lpMask = MAKEIPADDRESS(255, 255, 255, 0);
				SendMessage(hIPmask, IPM_SETADDRESS, 0, lpMask);
			}
			break;
		case IDC_IPMASK:
			if (HIWORD(wParam) == EN_CHANGE)
			{
				//TODO: Set prefix, depend on Mask;
				/*SendMessage(hIPmask, IPM_GETADDRESS, 0, (LPARAM)&dwIPmask);
				for (dwPrefix = 0; dwIPmask; dwPrefix++)dwIPmask <<= 1;

				sprintf(szPrefix, "%i", dwPrefix);
				SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)szPrefix);*/
			}
			break;
		case IDC_EDIT_PREFIX:
			SendMessage(hEditPrefix, WM_GETTEXT, 3, (LPARAM)&szPrefix);
			dwPrefix = atoi(szPrefix);
			dwIPmask = UINT_MAX;
			for (int i = 0; i < 32 - dwPrefix; i++)dwIPmask <<= 1;
			SendMessage(hIPmask, IPM_SETADDRESS, 0, dwIPmask);
			break;
		case IDC_BUTTON_RESET:
			SendMessage(hIPaddress, IPM_CLEARADDRESS, 0, 0);
			SendMessage(hIPmask, IPM_CLEARADDRESS, 0, 0);
			break;

		case IDOK:
			break;
		case IDCANCEL:EndDialog(hwnd, 0);
		}
	}
	break;
	case WM_NOTIFY:
	{
		HWND hIPmask = GetDlgItem(hwnd, IDC_IPMASK);
		HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
		DWORD dwIPmask = 0;
		DWORD dwPrefix = 0;
		CHAR szPrefix[3] = {};
		SendMessage(hIPmask, IPM_GETADDRESS, 0, (LPARAM)&dwIPmask);
		for (dwPrefix = 0; dwIPmask; dwPrefix++)dwIPmask <<= 1;
		sprintf(szPrefix, "%i", dwPrefix);
		SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)szPrefix);
	}
	break;
	case WM_CLOSE:EndDialog(hwnd, 0);
	}
	return FALSE;
}