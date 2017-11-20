#pragma once

#ifndef SCENE_DATA_H
#define SCENE_DATA_H

#include <dinput.h>

enum SceneState
{
	INVALID = 0,
	ATTRACT = 1,
	MENU = 2,
	START = 3,
	PAUSED = 4,
	EXIT = 5,
};

struct SceneData
{
	float m_deltaTime; //deltatime - time between frames
	SceneState m_state; //current state of simulation
	unsigned char* m_keyboardState;
	unsigned char* m_previousKeyboardState;
	DIMOUSESTATE* m_mouseState;
};

#endif