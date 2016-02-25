/*
Creator: Haval Ahmed
Notice: All Code is © Haval Ahmed
Description: DirectX Rendering Header File for Forerunner Engine
*/

#ifndef _DIRECTX_RENDERING_BASE_H
#define _DIRECTX_RENDERING_BASE_H

#include <d3d11.h>
#include <windows.h>
#include <windowsx.h>
#include "Timer.h"
#include "Model.h"
#include "Camera.h"


class DirectX_Rendering_Base
{
public:
	// Constructor and De constructor
	DirectX_Rendering_Base(HINSTANCE hInstance);
	virtual ~DirectX_Rendering_Base();

	// Main Message Loop
	int MainLoop();

	// Base Methods For Window Creation, Resizing, Updating, Rendering, Call Backs, and Event Handling
	virtual bool InitateWindow();
	virtual void ResizeWindow();
	virtual LRESULT HandleMessages(HWND hWindow, UINT currentMessage, WPARAM wParam, LPARAM lParam);
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	// Mouse Handling Events. 
	virtual void OnLeftMouseUp(WPARAM currentState, int xPosition, int yPosition){}
	virtual void OnLeftMouseDown(WPARAM currentState, int xPosition, int yPosition){}
	virtual void OnRightMouseUp(WPARAM currentState, int xPosition, int yPosition){}
	virtual void OnRightMouseDown(WPARAM currentState, int xPosition, int yPosition){}
	virtual void OnMouseDoubleClick(WPARAM currentState, int xPosition, int yPosition){}


protected:
	// Main Window and Direct3D Initialization Functions
	bool InitializeMainWindow();
	bool InitializeDirect3D();

	// HINSTANCE AND HWND VARIABLES
	HINSTANCE mainHInstance;
	HWND mainHWNDWindow;

	// Timer For Rendering and Updating
	Timer mainTimer;

	// Application Timer Status Variable
	bool mainEnginePaused;
	bool mainEngineMinimized;
	bool mainEngineMaxmized;
	bool mainEngineResized;

	// Window Height and Width
	int mainWindowHeight;
	int mainWindowWidth;

	// Direct3D Driver Type
	D3D_DRIVER_TYPE main3DDriverType;

	// Interface and Direct3D Devices
	ID3D11Device* main3DDevice;
	ID3D11DeviceContext* main3DDeviceContext;
	IDXGISwapChain* mainSwapChain;
	ID3D11Texture2D* mainDepthStencilBuffer;
	ID3D11RenderTargetView* mainRenderTargetView;
	ID3D11DepthStencilView* mainDepthStencilView;
	D3D11_VIEWPORT mainViewPort;


};

#endif