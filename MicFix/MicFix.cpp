// MicFix.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <windows.h>

unsigned short lastValue = 0;
HKEY hKey;
HANDLE hEvent;
DWORD buffer;
unsigned long type = REG_DWORD, keySize = 1024;
const DWORD targetVal = 0;

void waitForChange();
void enableMic();
unsigned short getValue();

int main(int argc, char* argv[])
{
	LONG result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Policies\\Microsoft\\Windows\\AppPrivacy", 0, KEY_ALL_ACCESS, &hKey);
	if (result == ERROR_SUCCESS)
	{
		//no error occured and no args parsed, hide console.
		if (argc == 1) 
		{
			ShowWindow(GetConsoleWindow(), SW_HIDE);
		}

		lastValue = getValue();
		printf("Startup setting: :%d\n", lastValue);
		if (lastValue > 1)
			enableMic();

	    //create event for registry change monitoring
		hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		//RegNotifyChangeKeyValue(hKey, FALSE, REG_NOTIFY_CHANGE_LAST_SET, hEvent, TRUE); 

		//run monitoring
		while(true)
			waitForChange();

		//cleanup
		RegCloseKey(hKey);
	}
	else {
		printf("Key open failed, aborting. Try running as Administrator!\n");
		system("pause");
	}

    return 0;
}

void waitForChange()
{
	//create event for registry change monitoring
	RegNotifyChangeKeyValue(hKey, FALSE, REG_NOTIFY_CHANGE_LAST_SET, hEvent, TRUE);
	WaitForSingleObject(hEvent, INFINITE);

	RegQueryValueEx(hKey, L"LetAppsAccessMicrophone", NULL, &type, (LPBYTE)&buffer, &keySize);
	lastValue = getValue();
	printf("Setting changed to: :%d\n", lastValue);

	if (lastValue > 1)
		enableMic();
}

void enableMic()
{
	RegSetValueEx(hKey, TEXT("LetAppsAccessMicrophone"), 0, REG_DWORD, (const BYTE*)&targetVal, sizeof(DWORD));
	printf("Re-enabled microphone!\n");
}

unsigned short getValue()
{
	RegQueryValueEx(hKey, L"LetAppsAccessMicrophone", NULL, &type, (LPBYTE)&buffer, &keySize);
	return (unsigned short)buffer;
}