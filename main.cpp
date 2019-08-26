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

void CheckMouseButtonStatus()
{
   // Check the mouse left button is pressed or not
   if ((GetKeyState(VK_LBUTTON) & 0x80) != 0)
   {
      cout << "Mouse pressed";
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
        Sleep(100);

        // Check the mouse right button is pressed or not
        if ((GetKeyState(VK_RBUTTON) & 0x80) != 0)
        {
            // Go back to the initial state of the program using recursion
            main();
        }
    }

    return 0;
}
