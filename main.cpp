#define WINVER 0x0500
#include <windows.h>
#include <iostream>

using namespace std;

void copyToClipboard(string text)
{
    const char* output = text.c_str();
    const size_t len = strlen(output) + 1;
    HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(hMem), output, len);
    GlobalUnlock(hMem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
}

void pasteFromClipboard() {
    if ( !IsClipboardFormatAvailable(CF_TEXT))
    {
        return;
    }

	if ( !OpenClipboard(GetDesktopWindow()) )
    {
        return;
	}

	HGLOBAL hg=GetClipboardData(CF_TEXT);
	if (hg)
	{
		LPCSTR strData =(LPCSTR)GlobalLock(hg);
		if ( strData )
		{
      // Your TXT in StrData
			GlobalUnlock(hg);
		}
	}
	CloseClipboard();
}

void mouseLeftClick ( )
{
  INPUT    Input={0};

  // Press the "Mouse Left" button
  Input.type      = INPUT_MOUSE;
  Input.mi.dwFlags  = MOUSEEVENTF_LEFTDOWN;
  ::SendInput(1,&Input,sizeof(INPUT));

  // Release the "Mouse Left" button
  ::ZeroMemory(&Input,sizeof(INPUT));
  Input.type      = INPUT_MOUSE;
  Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP;
  ::SendInput(1,&Input,sizeof(INPUT));
}

simulateKeys() {
    // input event.
    INPUT ip;

    // Set up a generic keyboard event.
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
    Sleep(200);
    for (int i = 0; i < 3; i++)
    {
        mouseLeftClick();
    }
/*
    // Press the "Enter" key
    ip.ki.wVk = 0x0D; // virtual-key code for the "Enter" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "Enter" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));*/
}

void CheckMouseButtonStatus()
{
   // Check the mouse left button is pressed or not
   if ((GetKeyState(VK_LBUTTON) & 0x80) != 0)
   {
      cout << "Mouse pressed";
      simulateKeys();
   }
}

int main()
{
    string text;
    cout << "Enter the text you want to be copied" << endl;
    getline(cin, text);
    cout << "'" << text << "'" << " is copied" << endl;

    // Copy the string input to clipboard
    copyToClipboard(text);

    // Paste from clipboard
    pasteFromClipboard();

    while(1)
    {
        CheckMouseButtonStatus();

        // Check the mouse right button is pressed or not
        if ((GetKeyState(VK_RBUTTON) & 0x80) != 0)
        {
            // Go back to the initial state of the program using recursion
            main();
        }
    }

    return 0;
}
