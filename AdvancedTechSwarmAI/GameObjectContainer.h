#pragma once

#include <vector>
#include "GameObject.h"

class GameObjectContainer
{
public:
	GameObjectContainer() = default;
	~GameObjectContainer() = default;
	static std::vector<GameObject*> m_gameObjects1;
private:
};
