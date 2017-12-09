#include "VBObject.h"
#include "SceneData.h"

VBObject::VBObject()
{
	m_vertexBuffer = 0;
	m_instanceBuffer = 0;
}

VBObject::VBObject(const VBObject& other)
{
}

VBObject::~VBObject()
{
}

bool VBObject::Initialize(ID3D11Device* device)
{
	bool result;


	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}

int VBObject::GetVertexCount() const
{
	return m_vertexCount;
}


int VBObject::GetInstanceCount() const
{
	return m_instanceCount;
}

InstanceType* VBObject::GetInstanceIndex(const unsigned short index)
{
	return m_instances[index];
}

std::vector<InstanceType*> VBObject::GetInstances()
{
	return m_instances;
}

void VBObject::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}

void VBObject::Tick(SceneData * _SD)
{
	if (m_applyPhysics)
	{
		Vector3 _new_velocity = m_velocity + (m_acceleration - m_velocity * m_drag) * _SD->m_deltaTime;
		Vector3 _new_position = m_position + m_velocity * _SD->m_deltaTime;
		m_velocity = _new_velocity;
		m_position = _new_position;
	}

	GameObject::Tick(_SD);
}

void VBObject::Draw(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


bool VBObject::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	
	D3D11_BUFFER_DESC vertexBufferDesc, instanceBufferDesc;
	
	HRESULT result;

	// Set the number of vertices in the vertex array.
	m_vertexCount = 4;

	// Set the number of instances in the array.
	m_instanceCount = 100;

	//BREAKS IF INSTANCE COUNT IS GREATER THAN

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Load the vertex array with data.
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].color = XMFLOAT4(0.2f, 2.0f, 0.2f, 1.0f);

	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].color = XMFLOAT4(0.2f, 0.2f, 2.0f, 1.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].color = XMFLOAT4(2.0f, 0.2f, 0.2f, 1.0f);

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	float x = 0;
	float y = 0;
	int iterator = 0;

	for (int i = 0; i < m_instanceCount; i++)
	{
		x += 4.0f;
		iterator++;
				// Load the instance array with data.
		m_instances.push_back(new InstanceType());
		m_instances[i]->instancePosition = Vector3(x, y, 0.0f);
		m_instances[i]->instanceRotation = Vector3(0, 0, 0);
		m_instances[i]->instanceScale = Vector3(1, 1, 1);
		m_instances[i]->fudge = XMMatrixIdentity();
		m_instances[i]->world_matrix = XMMatrixIdentity();

		if (iterator == 100)
		{
			iterator = 0;
			y += 4;
			x = 0;
		}
	}

	// Set up the description of the instance buffer.
	instanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the instance data.
	instanceData.pSysMem = &m_instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	// Create the instance buffer.
	result = device->CreateBuffer(&instanceBufferDesc, 0, &m_instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	return true;
}

void VBObject::ShutdownBuffers()
{
	// Release the instance buffer.
	if (m_instanceBuffer)
	{
		m_instanceBuffer->Release();
		m_instanceBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void VBObject::RenderBuffers(ID3D11DeviceContext* deviceContext)
{

	D3D11_MAPPED_SUBRESOURCE mappedData;
	deviceContext->Map(m_instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	InstanceType* dataView = reinterpret_cast<InstanceType*>(mappedData.pData);

	for (auto i = 0; i < m_instanceCount; i++)
	{
		//auto position = instances[i];
		//position.instancePosition.z += 10.f;
		//dataView[i] = position;
		dataView[i] = *m_instances[i];
	}
	deviceContext->Unmap(m_instanceBuffer, 0);


	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];

	// Set the buffer strides.
	strides[0] = sizeof(VertexType);
	strides[1] = sizeof(InstanceType);

	// Set the buffer offsets.
	offsets[0] = 0;
	offsets[1] = 0;

	// Set the array of pointers to the vertex and instance buffers.
	bufferPointers[0] = m_vertexBuffer;
	bufferPointers[1] = m_instanceBuffer;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}