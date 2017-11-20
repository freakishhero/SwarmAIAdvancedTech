#pragma once
#ifndef _APPLICATION_H_
#define _APPLICATION_H_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memory>
#include "InputHandler.h"
#include "GraphicsHandler.h"

class Scene;

class Application
{
public:
	Application();
	Application(const Application&);
	~Application();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindow(int&, int&);
	void ShutdownWindows();

	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	std::unique_ptr<InputHandler> m_Input;
	std::unique_ptr<GraphicsHandler> m_Graphics;
	std::unique_ptr<Scene> m_Scene;
};
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static Application* applicationHandle = 0;
#endif