#pragma once
#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>

class GameObject;
class VBObject;
struct SceneData;

class Scene
{
public:
	Scene(ID3D11Device* _pd3dDevice, HWND _hWnd, HINSTANCE _hInstance);
	virtual ~Scene();

	bool Tick();
	bool Draw(ID3D11DeviceContext* context);
	DirectX::XMMATRIX getMatrices();

protected:
	DWORD m_playTime;

private:
	SceneData* m_SceneData;
	HWND m_hWnd;
	ID3D11Device* m_pd3dImmediateDevice;
	ID3D11DeviceContext* m_pd3dImmediateContext;
	std::vector<GameObject*> m_GameObjects;
	VBObject* m_vbo;
};