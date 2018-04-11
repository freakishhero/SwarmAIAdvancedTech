#include "Scene.h"
#include "SceneData.h"
#include "VBObject.h"
#include <DirectXMath.h>
#include <time.h>

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

	m_target_location = Vector3(200, 200, 0);
	m_move_to_target = false;
}

Scene::~Scene()
{
	if (m_SceneData)
	{
		delete m_SceneData;
		m_SceneData = nullptr;
	}

	if (m_vbo)
	{
		delete m_vbo;
		m_vbo = nullptr;
	}
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

		if (m_move_to_target)
		{
			if (m_vbo->GetInstanceIndex(i)->instancePosition.x <= m_target_location.x)
			{
				m_vbo->GetInstanceIndex(i)->instancePosition.x += 6.37f * m_SceneData->m_deltaTime;
			}
			if (m_vbo->GetInstanceIndex(i)->instancePosition.x >= m_target_location.x)
			{
				m_vbo->GetInstanceIndex(i)->instancePosition.x -= 6.59f * m_SceneData->m_deltaTime;
			}
			if (m_vbo->GetInstanceIndex(i)->instancePosition.y <= m_target_location.y)
			{
				m_vbo->GetInstanceIndex(i)->instancePosition.y += 6.49f * m_SceneData->m_deltaTime;
			}
			if (m_vbo->GetInstanceIndex(i)->instancePosition.y >= m_target_location.y)
			{
				m_vbo->GetInstanceIndex(i)->instancePosition.y -= 6.35f * m_SceneData->m_deltaTime;
			}
		}
		else 
		{
			if (i % 2)
			{
				m_vbo->GetInstanceIndex(i)->instancePosition = Vector3(m_vbo->GetInstanceIndex(i)->instancePosition.x + 3.0f * m_SceneData->m_deltaTime,
					m_vbo->GetInstanceIndex(i)->instancePosition.y,
					m_vbo->GetInstanceIndex(i)->instancePosition.z);
			}
			else
			{
				m_vbo->GetInstanceIndex(i)->instancePosition = Vector3(m_vbo->GetInstanceIndex(i)->instancePosition.x - 3.0f * m_SceneData->m_deltaTime,
					m_vbo->GetInstanceIndex(i)->instancePosition.y,
					m_vbo->GetInstanceIndex(i)->instancePosition.z);
			}

			if (i % 7)
			{
				m_vbo->GetInstanceIndex(i)->instancePosition = Vector3(m_vbo->GetInstanceIndex(i)->instancePosition.x,
					m_vbo->GetInstanceIndex(i)->instancePosition.y + 3.0f * m_SceneData->m_deltaTime,
					m_vbo->GetInstanceIndex(i)->instancePosition.z);
			}

			if (i % 3)
			{
				m_vbo->GetInstanceIndex(i)->instancePosition = Vector3(m_vbo->GetInstanceIndex(i)->instancePosition.x,
					m_vbo->GetInstanceIndex(i)->instancePosition.y - 3.0f * m_SceneData->m_deltaTime,
					m_vbo->GetInstanceIndex(i)->instancePosition.z);
			}
		}
		
		
			
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

	for (int i = 0; i < m_vbo->GetInstanceCount(); i++)
	{
//		matrix *= m_vbo->GetInstanceIndex(i)->world_matrix;
	}
	return matrix;
}


