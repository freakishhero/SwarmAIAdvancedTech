#include "GameObject.h"
#include "SceneData.h"

GameObject::GameObject()
{
	m_position = Vector3(0, 0, 0);
	m_yaw = 0.0f;
	m_pitch = 0.0f;
	m_roll = 0.0f;
	m_scale = Vector3(1, 1, 1);

	m_world_matrix = XMMatrixIdentity();

	m_fudge = XMMatrixIdentity();
}

GameObject::~GameObject()
{
}

void GameObject::Tick(SceneData* _SD)
{
	if (m_applyPhysics)
	{
		Vector3 _new_velocity = m_velocity + (m_acceleration - m_velocity * m_drag) * _SD->m_deltaTime;
		Vector3 _new_position = m_position + m_velocity * _SD->m_deltaTime;
		m_velocity = _new_velocity;
		m_position = _new_position;
	}
}