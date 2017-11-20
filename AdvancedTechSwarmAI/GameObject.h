#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <DirectXMath.h>
#include "Vector.h"
#include <d3d11.h>
#include <memory>

struct SceneData;
struct DrawData;

using namespace DirectX;

class GameObject
{
public:
	GameObject();
	~GameObject();

	virtual void Tick(SceneData* _SD);
	virtual void Draw(ID3D11DeviceContext* _DD) = 0;

	//Getters
	Vector3 GetPosition() { return m_position; }
	Vector3 GetScale() { return m_scale; }
	Vector3 GetVelocity() { return m_velocity; }
	Vector3 GetAcceleration() { return m_acceleration; }

	float GetYaw() { return m_yaw; }
	float GetPitch() { return m_pitch; }
	float GetRoll() { return m_roll; }
	float GetDrag() { return m_drag; }

	bool IsPhysicsApplied() { return m_applyPhysics; }

	//Setters
	void AddPosition(Vector3 _position) { m_position += _position; }
	void SetPosition(Vector3 _position) { m_position = _position; }
	void SetScale(Vector3 _scale) { m_scale = _scale; }
	void SetVelocity(Vector3 _velocity) { m_velocity = _velocity; }
	void SetAcceleration(Vector3 _acceleration) { m_acceleration = _acceleration; }
	void SetYawPitchRoll(Vector3 _YawPitchRoll) { m_yaw = _YawPitchRoll.x; m_pitch = _YawPitchRoll.y; m_roll = _YawPitchRoll.z; }

	void SetYaw(float _yaw) { m_yaw = _yaw; }
	void SetPitch(float _pitch) { m_pitch = _pitch; }
	void SetRoll(float _roll) { m_roll = _roll; }
	void SetDrag(float _drag) { m_drag = _drag; }

	void SetApplyPhysics(bool _physics) { m_applyPhysics = _physics; }
	void TogglePhysics() { m_applyPhysics = !m_applyPhysics; }

protected:
	XMMATRIX m_world_matrix;
	XMMATRIX m_rotation_matrix;
	XMMATRIX m_fudge;
	Vector3 m_position;
	Vector3 m_velocity;
	Vector3 m_acceleration;
	Vector3 m_scale;
	float m_yaw;
	float m_pitch;
	float m_roll;
	float m_drag;
	bool m_applyPhysics;

private:
};

#endif