#include "StdAfx.h"   
static HHOOK g_hHook = NULL;  
static CWnd* g_pWnd  = NULL;  
class CKeyboardHook{
	bool Init(HINSTANCE hInst, CWnd *pWnd);
	void Close();


};
BOOL CKeyboardHook::Init(HINSTANCE hInst, CWnd *pWnd)  
{  
	g_pWnd = pWnd;  
	g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL,CKeyboardHook::KeyboardHookProc,hInst,NULL);  
	return g_hHook == NULL ? FALSE : TRUE;  
}  

void CKeyboardHook::Close()  
{  
	UnhookWindowsHookEx(g_hHook);  
}  

LRESULT CALLBACK CKeyboardHook::KeyboardHookProc(int code, WPARAM wParam, LPARAM lParam)  
{  
	if(code == HC_ACTION)  
	{  
		PKBDLLHOOKSTRUCT pStruct = (PKBDLLHOOKSTRUCT)lParam;  
		switch(pStruct->vkCode)  
		{  
		case 'a':  
		case 'A':  
		case 's':  
		case 'S':  
		case 'd':  
		case 'D':  
		case 'j':  
		case 'J':  
		case ' ':  
			if (wParam == WM_KEYDOWN)  
			{  
				g_pWnd->PostMessage(WM_KEYDOWN, pStruct->vkCode, pStruct->scanCode);                                 
			}  
			return TRUE;  
			break;  
		default:  return CallNextHookEx(g_hHook, code, wParam, lParam);  
		}  
	}  
	return CallNextHookEx(g_hHook, code, wParam, lParam);  
}  