#include "DirectX_Rendering_Base.h"
#include <windowsx.h>

namespace
{
	// Member Function To Handle Messages
	DirectX_Rendering_Base* foreRunnerEngine = 0;
}

/*
Call Back WndProc Function. 
*/
LRESULT CALLBACK WndProc(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Return A Member Function To Check Messages Before Window Creation
	foreRunnerEngine->HandleMessages(hWindow, message, wParam, lParam);
}


DirectX_Rendering_Base::DirectX_Rendering_Base(HINSTANCE hInstance)
{
	// Initialize All Variables To Default Behavior
	mainHInstance = hInstance;
	main3DDriverType = D3D_DRIVER_TYPE_HARDWARE;
	mainWindowHeight = 1280;
	mainWindowWidth = 720;
	mainHWNDWindow = 0;
	mainEnginePaused = false;
	mainEngineMinimized = false;
	mainEngineMaxmized = false;
	mainEngineResized = false;
	main3DDevice = 0;
	main3DDeviceContext = 0;
	mainSwapChain = 0;
	mainDepthStencilBuffer = 0;
	mainRenderTargetView = 0;
	mainDepthStencilView = 0;

	// Zero The Memory For The ViewPort
	ZeroMemory(&mainViewPort, sizeof(D3D11_VIEWPORT));

	// Assign foreRunnerEngine
	foreRunnerEngine = this;

}

DirectX_Rendering_Base::~DirectX_Rendering_Base()
{
	// Release All Interfaces
	mainRenderTargetView->Release();
	mainDepthStencilBuffer->Release();
	mainSwapChain->Release();
	mainDepthStencilView->Release();

	// Check If Device Context Is True
	if (main3DDeviceContext)
		main3DDeviceContext->ClearState();

	main3DDeviceContext->Release();
	main3DDevice->Release();

}

int DirectX_Rendering_Base::MainLoop()
{
	// Game/Main Message Loop
	// Initialize Message As 0
	MSG currentMessage = { 0 };

	// Reset Timer
	mainTimer.reset();

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
			// Start Counting
			mainTimer.count();
			
			// Check if Paused
			if (!mainEnginePaused)
			{
				// Update then Render
				Update(mainTimer.deltaTime());
				Render();
			}
			else
			{
				// Put To Sleep
				Sleep(100);
			}
		}
	}

	return (int)currentMessage.wParam;
}

bool DirectX_Rendering_Base::InitateWindow()
{
	return false;
}

/*
Will Write Later
*/
void DirectX_Rendering_Base::ResizeWindow()
{
}

LRESULT DirectX_Rendering_Base::HandleMessages(HWND hWindow, UINT currentMessage, WPARAM wParam, LPARAM lParam)
{
	// Switch Incoming Message
	switch (currentMessage)
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			// Engine Is Paused
			mainEnginePaused = true;
			// Pause Timer
			mainTimer.pause();
		}
		else
		{
			// Engine Is Running
			mainEnginePaused = false;
			// Start Timer
			mainTimer.start();
		}
		return 0;
	// Resize Case
	case WM_SIZE:
		// Save New Size
		mainWindowHeight = HIWORD(lParam);
		mainWindowWidth = LOWORD(lParam);
		// Check Device
		if (main3DDevice)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				// Pause Engine
				mainEnginePaused = true;
				bool mainEngineMinimized = true;
				bool mainEngineMaxmized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				mainEnginePaused = false;
				bool mainEngineMinimized = true;
				bool mainEngineMaxmized = false;
				ResizeWindow();
			}
			else if (wParam == SIZE_RESTORED)
			{
				if (mainEngineMaxmized)
				{
					mainEnginePaused = false;
					mainEngineMaxmized = false;
					ResizeWindow();
				}
				else if (mainEngineMinimized)
				{
					mainEnginePaused = false;
					mainEngineMinimized = false;
					ResizeWindow();
				}
				else if (mainEngineResized)
				{
					// Do Nothing Will Window is Being Resized
				}
				else
				{
					ResizeWindow();
				}
			}
		}
		return 0;

	case WM_ENTERSIZEMOVE:
		// Pause Engine
		mainEnginePaused = true;
		mainEngineResized = true;
		// Stop Timer
		mainTimer.pause();
		return 0;

	case WM_EXITSIZEMOVE:
		// Pause Engine
		mainEnginePaused = false;
		mainEngineResized = false;
		// Start Timer
		mainTimer.start();
		// Resize Window
		ResizeWindow();
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_MENUCHAR:
		return MAKELRESULT(0, MNC_CLOSE);

	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 600;
		((MINMAXINFO*)lParam)->ptMaxTrackSize.y = 800;
		return 0;

	case WM_LBUTTONDOWN:
		OnLeftMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_RBUTTONDOWN:
		OnLeftMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

	case WM_LBUTTONUP:
		OnLeftMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_RBUTTONUP:
		OnRightMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

	default:
		return DefWindowProc(hWindow, currentMessage, wParam, lParam);
	}

	return 0;
}

bool DirectX_Rendering_Base::InitializeMainWindow()
{
	// Create a window class
	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(WNDCLASSEX); // Size of Window in Bytes (Not Width and Height)
	windowClass.style = CS_HREDRAW | CS_VREDRAW; // Style of Window 
	windowClass.lpfnWndProc = WndProc; // Callback Function
	windowClass.cbClsExtra = 0; // Extra Bytes To Allocate For Window 
	windowClass.cbWndExtra = 0; // Extra Bytes To Allocate For Window Instance
	windowClass.hInstance = mainHInstance; // Handle Instance For Window Class
	windowClass.hIcon = nullptr; // Null Icon
	windowClass.hCursor = LoadCursor(mainHInstance, IDC_ARROW); // Cursor Look
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
	HWND hWindow = CreateWindow("Forerunner Engine", "Main Engine Scene", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, mainRectangle.right - mainRectangle.left, mainRectangle.bottom - mainRectangle.top, nullptr, nullptr, mainHInstance, nullptr);
	// Check Window Creation
	if (!hWindow)
		return E_FAIL;

	// Display Window. Call Show Window Function
	ShowWindow(hWindow, SW_SHOW);
	UpdateWindow(hWindow);

	return true;
}

bool DirectX_Rendering_Base::InitializeDirect3D()
{
	// Create Device and Context
	
	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hResultDevice = D3D11CreateDevice(0, main3DDriverType, 0, 0, 0, 0, D3D11_SDK_VERSION, &main3DDevice, &featureLevel, &main3DDeviceContext);

	// Check To See if Created
	if (FAILED(hResultDevice))
	{
		MessageBox(0, "Device Creation Failed", 0, 0);
		return false;
	}

	// Create A DXGI_SWAP_CHAIN
	DXGI_SWAP_CHAIN_DESC swapDesc;
	swapDesc.BufferDesc.Width = mainWindowWidth;
	swapDesc.BufferDesc.Height = mainWindowHeight;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_A8_UNORM;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.BufferCount = 1;
	swapDesc.OutputWindow = mainHWNDWindow;
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Flags = 0;

	IDXGIDevice* dxgiDevice = 0;
	hResultDevice(main3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));









	return false;
}
