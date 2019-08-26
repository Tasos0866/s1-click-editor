#define WINVER 0x0500
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

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
    // input events.
    INPUT ip1;
    INPUT ip2;

    // Set up a generic keyboard events.
    ip1.type = INPUT_KEYBOARD;
    ip1.ki.wScan = 0; // hardware scan code for key
    ip1.ki.time = 0;
    ip1.ki.dwExtraInfo = 0;
    ip2.type = INPUT_KEYBOARD;
    ip2.ki.wScan = 0; // hardware scan code for key
    ip2.ki.time = 0;
    ip2.ki.dwExtraInfo = 0;

    // Press the "CTRL" key
    ip2.ki.wVk = 0x11; // virtual-key code for the "CTRL" key
    ip2.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip2, sizeof(INPUT));

    // Press the "V" key
    ip1.ki.wVk = 0x56; // virtual-key code for the "V" key
    ip1.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip1, sizeof(INPUT));

    // Release the "V" key
    ip1.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip1, sizeof(INPUT));

    // Release the "CTRL" key
    ip2.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip2, sizeof(INPUT));
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
    Sleep(85);

    for (int i = 0; i < 3; i++)
    {
        mouseLeftClick();
    }

    // Paste from clipboard
    pasteFromClipboard();

    // Press the "Enter" key
    ip.ki.wVk = 0x0D; // virtual-key code for the "Enter" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "Enter" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
}

void CheckMouseButtonStatus()
{
   // Check the mouse left button is pressed or not
   if ((GetKeyState(VK_LBUTTON) & 0x80) != 0)
   {
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
    cout << "Right click in order to pause" << endl;


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
