#ifndef BUTTON_H
#define BUTTON_H

#pragma once

#include "Renderer.h"

#include <stdlib.h>


class Button :	public Renderer
{
	
public:
	Button(float width, float height);
	~Button();

	// The width of the button
	float MaxWidth;
	// The height of the button
	float MaxHeight;

	// x and y Position of the button
	int xPos, yPos;

	class TextRenderer *Text;

	// The three textures of the button
	Texture2D normalTexture;
	Texture2D hoverTexture;
	Texture2D pressedTexture;

	virtual void Render() override;
	virtual void Update(float deltaTime) override;

	virtual Component* GetComp(const type_info& type) override;
	
	// Function pointer to a Static Function that returns null with no Parameters
	void (*PressFunc)() = NULL;

private:

	Transform oldTrans;

	// The width of the button
	float Width;
	// The height of the button
	float Height;

	// The states of the buttton
	enum ButtonState
	{
		NORMAL,
		HOVER,
		PRESSED
	};
	
	// The current state of this button
	ButtonState state;

	void InitButton();

	float m_PressedTime;
	
};

#endif

