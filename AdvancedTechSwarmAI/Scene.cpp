#include "Scene.h"
#include "SceneData.h"
#include "VBObject.h"
#include <DirectXMath.h>

Scene::Scene(ID3D11Device * _pd3dDevice, HWND _hWnd, HINSTANCE _hInstance)
{
	m_pd3dImmediateDevice = _pd3dDevice;
	m_pd3dImmediateDevice->GetImmediateContext(&m_pd3dImmediateContext);
	m_hWnd = _hWnd;

	m_SceneData = new SceneData();

	m_vbo = new VBObject();
	m_vbo->Initialize(_pd3dDevice);
	m_GameObjects.push_back(m_vbo);
	m_vbo->SetPosition(Vector3(0, 0, 0));
}

Scene::~Scene()
{
}

bool Scene::Tick()
{
	DWORD currentTime = GetTickCount();
	m_SceneData->m_deltaTime = min((float)(currentTime - m_playTime) / 1000.0f, 0.1f);
	m_playTime = currentTime;

	for (auto& gameObject : m_GameObjects)
	{
		gameObject->Tick(m_SceneData);
	}

	for (int i = 0; i < m_vbo->GetInstanceCount(); i++)
	{
		m_vbo->GetInstanceIndex(i).instancePosition = Vector3(10, 10, 10);
	}
	Draw(m_pd3dImmediateContext);
	return true;
}

bool Scene::Draw(ID3D11DeviceContext* context)
{
	for (auto& gameObject : m_GameObjects)
	{
		gameObject->Draw(context);
	}
	return true;
}

XMMATRIX Scene::getMatrices()
{	
	XMMATRIX matrix = XMMatrixIdentity();
	for (auto& gameObject : m_GameObjects)
	{
		matrix *= gameObject->GetWorldMatrix();
	}

	//for (int i = 0; i < m_vbo->GetInstanceCount(); i++)
	//{
	//	matrix *= m_vbo->GetInstanceIndex(i).world_matrix;
	//}
	return matrix;
}


