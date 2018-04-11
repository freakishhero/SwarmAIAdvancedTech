#include "GraphicsHandler.h"


GraphicsHandler::GraphicsHandler()
{
	m_Direct3D = nullptr;
	m_Camera = nullptr;
	m_shader = nullptr;
}


GraphicsHandler::GraphicsHandler(const GraphicsHandler& other)
{
}


GraphicsHandler::~GraphicsHandler()
{
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = nullptr;
	}
}


bool GraphicsHandler::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Create the Direct3D object.
	m_Direct3D = new Direct3D();
	if (!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new Camera;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(125.0f, 90.0f, -250.0f);

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	m_shader = new Shader;
	if (!m_shader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_shader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the shader.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsHandler::Shutdown()
{
	// Release the color shader object.
	if (m_shader)
	{
		m_shader->Shutdown();
		delete m_shader;
		m_shader = nullptr;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = nullptr;
	}

	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		m_Direct3D = nullptr;
	}
	return;
}


bool GraphicsHandler::Frame()
{
	bool result;
	//Camera movement


	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}
	return true;
}

Direct3D* GraphicsHandler::GetDirect3D()
{
	return m_Direct3D;
}

Camera * GraphicsHandler::getCamera()
{
	return m_Camera;
}

Shader * GraphicsHandler::getShader()
{
	return m_shader;
}


bool GraphicsHandler::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	//m_Direct3D->BeginScene(0.5f, 0.8f, 0.8f, 1.0f);
	m_Direct3D->BeginScene(0.6f, 0.3f, 0.15f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Render the model using the color shader.
	result = m_shader->Render(m_Direct3D->GetDeviceContext(), 3, 100000, worldMatrix, viewMatrix,
		projectionMatrix);
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();
	return true;
}