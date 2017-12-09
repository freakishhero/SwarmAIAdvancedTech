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
		for (auto& boid : m_vbo->GetInstances())
		{
			if (boid != m_vbo->GetInstances()[i])
			{
				if (fabs(Vector3::Distance(boid->instancePosition, m_vbo->GetInstances()[i]->instancePosition) < 0.001))
				{
					boid->instancePosition = (boid->instancePosition + m_vbo->GetInstances()[i]->instancePosition * m_SceneData->m_deltaTime);
				}
			}
		}
		/*if(i % 2)
		m_vbo->GetInstanceIndex(i)->instancePosition = Vector3(m_vbo->GetInstanceIndex(i)->instancePosition.x + 0.1, m_vbo->GetInstanceIndex(i)->instancePosition.y, m_vbo->GetInstanceIndex(i)->instancePosition.z);
		else
		m_vbo->GetInstanceIndex(i)->instancePosition = Vector3(m_vbo->GetInstanceIndex(i)->instancePosition.x - 0.1, m_vbo->GetInstanceIndex(i)->instancePosition.y, m_vbo->GetInstanceIndex(i)->instancePosition.z);
		*/
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
		matrix *= m_vbo->GetInstanceIndex(i)->world_matrix;
	}
	return matrix;
}


