#pragma once
#ifndef _VBOBJECT_H_
#define _VBOJECT_H_
#include <vector>

#include <d3d11.h>
#include <directxmath.h>

#include "GameObject.h"

using namespace DirectX;

struct InstanceType
{
	Vector3 instancePosition;
	Vector3 instanceRotation;
	Vector3 instanceScale;
};

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
	int GetVertexCount() const;
	int GetInstanceCount() const;
	InstanceType* GetInstanceIndex(const unsigned short index);
	std::vector<InstanceType*> GetInstances();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_instanceBuffer;
	std::vector<InstanceType*> m_instances;
	D3D11_SUBRESOURCE_DATA vertexData, instanceData;
	int m_vertexCount;
	int m_instanceCount;
};

#endif