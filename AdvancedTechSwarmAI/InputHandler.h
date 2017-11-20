#pragma once
#ifndef _INPUTHANDLER_H_
#define _INPUTHANDLER_H_

class InputHandler
{
public:
	InputHandler();
	InputHandler(const InputHandler&);
	~InputHandler();

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool m_keys[256];
};

#endif