#pragma once
#ifndef _VBOBJECT_H_
#define _VBOJECT_H_

#include <d3d11.h>
#include <directxmath.h>
#include "GameObject.h"

using namespace DirectX;

class VBObject : public GameObject
{
public:
	VBObject();
	VBObject(const VBObject&);
	~VBObject();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Tick(SceneData* _SD);
	void Draw(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};
};

#endif