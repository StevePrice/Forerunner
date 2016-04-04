/*
Creator: Haval Ahmed
Notice: All Code is © Haval Ahmed. Any similarities are not intended. 
Description: DirectX Rendering Header File for Forerunner Engine
*/

#ifndef _DIRECTX_RENDERER_H
#define _DIRECTX_RENDERER_H

#include "Header_File_Linker.h"

class DirectX_Renderer
{
public:
	// Constructor and Deconstructor
	DirectX_Renderer();
	virtual ~DirectX_Renderer();

	// Main loop of Forerunner Engine
	int mainLoop();

	// Base Methods For Window Creation, Resizing, Updating, Rendering, Call Backs, and Event Handling
	virtual bool initateWindow();
	virtual void resizeWindow();
	virtual LRESULT CALLBACK handleMessages(HWND hWindow, UINT currentMessage, WPARAM wParam, LPARAM lParam);

	// Window and Application Information
	HINSTANCE sceneHInstance() const;
	HWND sceneHWND() const;
	int screenAspectRatio() const;

	// Load and Unload Content Functions
	virtual bool loadContent();
	virtual void unLoadContent();

	// Update and Render Function
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	// Win32 Event Handling Functions 
	virtual void onLeftMouseUp(WPARAM currentState, int xPosition, int yPosition) {};
	virtual void onLeftMouseDown(WPARAM currentState, int xPosition, int yPosition) {};
	virtual void onRightMouseUp(WPARAM currentState, int xPosition, int yPosition) {};
	virtual void onRightMouseDown(WPARAM currentState, int xPosition, int yPosition) {};
	virtual void onLeftMouseDoubleClick(WPARAM currentState, int xPosition, int yPosition) {};
	virtual void onRightMouseDoubleClick(WPARAM currentState, int xPosition, int yPosition) {};
	virtual void onMouseHover(WPARAM currentState, int xPosition, int yPosition) {};

	// Controller Function (Game Pad Movements)
	// XInput Functions. Allow user to decide what each button corresponds to.


protected:
	// Window and Direct3D Initialization Functions
	bool initializeWindow();
	bool initializeDirect3D();

	// HINSTANCE and HWND Variables
	HINSTANCE hInstance;
	HWND handleWindow;

	// Application aspect ratio
	int windowHeight;
	int windowWidth;

	// Timer object
	Timer engineTime;

	// Application status variables
	bool enginePaused;
	bool engineMinimized;
	bool engineMaximized;
	bool engineResized;

	// Direct3D Driver Type
	D3D_DRIVER_TYPE driverType;

	// IDGXI Interfaces and Direct3D Devices
	ID3D11Device* direct3DDevice;						// Direct3D Device
	ID3D11DeviceContext * direct3DDeviceContext;		// Device Context
	IDXGISwapChain* firstSwapChain;						// First Swap Chain
	IDXGISwapChain* secondSwapChain;					// Second Swap Chain
	ID3D11Texture2D* stencilBufferDepth;				// Stencil Buffer
	ID3D11RenderTargetView* renderTargetView;			// Render Target (Back Buffer)
	ID3D11DepthStencilView* depthStencilView;			// Stencil View
	D3D11_VIEWPORT topViewPort;							// Top View Port
	D3D11_VIEWPORT bottomViewPort;						// Bottom View Port


};

#endif