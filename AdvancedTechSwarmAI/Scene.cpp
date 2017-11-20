#include "Scene.h"
#include "SceneData.h"
#include "VBObject.h"

Scene::Scene(ID3D11Device * _pd3dDevice, HWND _hWnd, HINSTANCE _hInstance)
{
	m_pd3dImmediateDevice = _pd3dDevice;
	m_pd3dImmediateDevice->GetImmediateContext(&m_pd3dImmediateContext);
	m_hWnd = _hWnd;

	m_SceneData = new SceneData();

	for (int i = 0; i < 10; i++)
	{
		m_vbo = new VBObject;
		m_vbo->Initialize(_pd3dDevice);
		m_GameObjects.push_back(m_vbo);
	}
}

Scene::~Scene()
{
}

bool Scene::Tick()
{
	for (auto& gameObject : m_GameObjects)
	{
		gameObject->Tick(m_SceneData);
	}

	Draw(m_pd3dImmediateContext);
	return true;
}

bool Scene::Draw(ID3D11DeviceContext * context)
{
	for (auto& gameObject : m_GameObjects)
	{
		gameObject->Draw(context);
	}
	return true;
}


