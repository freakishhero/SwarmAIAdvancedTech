#include "Application.h"
#include "Scene.h"

Application::Application()
{
	m_Input = nullptr;
	m_Graphics = nullptr;
}

Application::Application(const Application& other)
{
}


Application::~Application()
{
}

bool Application::Initialize()
{
	int screenWidth;
	int screenHeight;
	bool result;


	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	//Create the window
	InitializeWindow(screenWidth, screenHeight);

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = std::make_unique<InputHandler>();
	if (!m_Input)
	{
		MessageBox(0, L"Input Handler failed to initialize.",
			L"Error", MB_OK);
		return false;
	}

	// Initialize the input object.
	m_Input->Initialize();

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = std::make_unique<GraphicsHandler>();
	if (!m_Graphics)
	{
		MessageBox(0, L"Graphics Handler failed to initialize.",
			L"Error", MB_OK);
		return false;
	}

	// Initialize the graphics object.
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	m_Scene = std::make_unique<Scene>(m_Graphics->GetDirect3D()->GetDevice(), m_hwnd, m_hinstance);
	if (!m_Scene)
	{
		MessageBox(0, L"Scene failed to initialize.",
			L"Error", MB_OK);
		return false;
	}

	return true;
}

void Application::Shutdown()
{
	// Release the graphics object.
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
	}

	// Release the input object.
	if (m_Input)
	{

	}

	// Shutdown the window.
	ShutdownWindows();

	return;
}

void Application::Run()
{
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
				done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}

	}

	return;
}

bool Application::Frame()
{
	bool result;
	Camera* cam = m_Graphics->getCamera();

	// Check if the user pressed escape and wants to exit the application.
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}
	
	//Camera movement
	if (m_Input->IsKeyDown(VK_UP))
	{
		cam->SetPosition(cam->GetPosition().x, cam->GetPosition().y , cam->GetPosition().z + 1.0f);
	}
	if (m_Input->IsKeyDown(VK_DOWN))
	{
		cam->SetPosition(cam->GetPosition().x, cam->GetPosition().y, cam->GetPosition().z - 1.0f);
	}
	if (m_Input->IsKeyDown(VK_LEFT))
	{
		cam->SetPosition(cam->GetPosition().x - 1.0f, cam->GetPosition().y , cam->GetPosition().z);
	}
	if (m_Input->IsKeyDown(VK_RIGHT))
	{
		cam->SetPosition(cam->GetPosition().x + 1.0f, cam->GetPosition().y, cam->GetPosition().z);
	}

	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame();
	if (!result)
	{
		return false;
	}

	m_Graphics->getShader()->setWorldMatrix(m_Scene->getMatrices());
	// Do the frame processing for the graphics object.
	result = m_Scene->Tick();
	if (!result)
	{
		return false;
	}


	return true;
}

LRESULT CALLBACK Application::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
	{
		// If a key is pressed send it to the input object so it can record that state.
		m_Input->KeyDown((unsigned int)wparam);
		return 0;
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		// If a key is released then send it to the input object so it can unset the state for that key.
		m_Input->KeyUp((unsigned int)wparam);
		return 0;
	}

	// Any other messages send to the default message handler as our application won't make use of them.
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}

void Application::InitializeWindow(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.	
	applicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"DirectX Swarm AI Simulation";

	// Setup the windows class with default settings.
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_HAND);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 1024;
		screenHeight = 720;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	/*m_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW, 
		m_applicationName,
		m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, 
		posY, 
		screenWidth, 
		screenHeight, 
		NULL, 
		NULL, 
		m_hinstance,
		NULL);*/

	//Create our Extended Window
	m_hwnd = CreateWindowEx(    
		WS_EX_APPWINDOW,    //Extended style
		m_applicationName,    //Name of our windows class
		m_applicationName,    //Name in the title bar of our window
		WS_OVERLAPPEDWINDOW,    //style of our window
		posX, posY,    //Window position
		screenWidth,    //Width of our window
		screenHeight,    //Height of our window
		NULL,    //Handle to parent window
		NULL,    //Handle to a Menu
		m_hinstance,    //Specifies instance of current program
		NULL    //used for an MDI client window
		);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(true);
	return;
}

void Application::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}
	
	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;
	
	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	applicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return applicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}
