/*
Creator: Haval Ahmed 
Notice: All Code is © Haval Ahmed
Description: Win Main Initialization for Forerunner Engine
*/


#include <windows.h>

// Function Declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT InitiateWindow(HINSTANCE hInstance, int nShowCmd);

/*
Entry Point into Program. 
Main Message Loop. This is where we call our Update and Render Functions
*/
int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR cmdLine, _In_ int nShowCmd)
{

	// We unreferenced two parameters: hPrevInstance as it is always Null and we don't use it in our case and cmdLine which is the command line string of the program.
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(cmdLine);

	/* 
	WNDCLASSEX Creation Function Call
	Initializes and Initiates Window Class
	Win32 Properties and Values Defined in Window Class
	 */

	// We Want to Check If The Creation Succeed or Failed
	if (FAILED(InitiateWindow(hInstance, nShowCmd)))
		return 0;

	// Game/Main Message Loop
	// Initialize Message As 0
	MSG currentMessage = { 0 };
	// Loop Until We Get A Quit Message
	while (WM_QUIT != currentMessage.message)
	{
		// Peek To See the Next Message. If An Event Happens, Translate and Dispatch Message else we will call our Update and Render Functions
		if (PeekMessage(&currentMessage, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&currentMessage);
			DispatchMessage(&currentMessage);
		}
		else
		{
			// Update Frame and Then Render
			// DirectX->Update()
			// DirectX->Render()
		}
	}

	return static_cast<int>(currentMessage.wParam);

}

/*
 Initiate and Create WNDCLASSEX Class
 Description: Registers Class and Creates Win32 Window
 Parameters: HINSTANCE and nShowCMD
 Returns: WNDCLASSEX Class Object
*/

HRESULT InitiateWindow(HINSTANCE hInstance, int nShowCmd)
{
	// Create a window class
	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(WNDCLASSEX); // Size of Window in Bytes (Not Width and Height)
	windowClass.style = CS_HREDRAW | CS_VREDRAW; // Style of Window 
	windowClass.lpfnWndProc = WndProc; // Callback Function
	windowClass.cbClsExtra = 0; // Extra Bytes To Allocate For Window 
	windowClass.cbWndExtra = 0; // Extra Bytes To Allocate For Window Instance
	windowClass.hInstance = hInstance; // Handle Instance For Window Class
	windowClass.hIcon = nullptr; // Null Icon
	windowClass.hCursor = LoadCursor(hInstance, IDC_ARROW); // Cursor Look
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Background Color
	windowClass.lpszMenuName = nullptr; // Menu Name
	windowClass.lpszClassName = "Forerunner Engine"; // Class Name

	// Register Class and Check If True
	if (!RegisterClassEx(&windowClass))
		return E_FAIL;

	// Create A Rectangle For Window Creation
	RECT mainRectangle = { 0, 0, 1280, 720 };
	// Adjust Rectangle Window
	AdjustWindowRect(&mainRectangle, WS_OVERLAPPEDWINDOW, FALSE);
	// Create A Window. Needs to be a HWND which is a handle window
	HWND hWindow = CreateWindowA("Forerunner Engine", "Main Engine Scene", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, mainRectangle.right - mainRectangle.left, mainRectangle.bottom - mainRectangle.top, nullptr, nullptr, hInstance, nullptr);
	// Check Window Creation
	if (!hWindow)
		return E_FAIL;

	// Display Window. Call Show Window Function
	ShowWindow(hWindow, nShowCmd);

	return S_OK;
}

/*
Call Back Function. Receives and Handles Messages
*/
LRESULT CALLBACK WndProc(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT pStruct;
	HDC handleDC;

	// Switch Incoming Message
	switch (message)
	{
	case WM_PAINT:
		handleDC = BeginPaint(hWindow, &pStruct);
		EndPaint(hWindow, &pStruct);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWindow, message, wParam, lParam);
	}

	return 0;
}