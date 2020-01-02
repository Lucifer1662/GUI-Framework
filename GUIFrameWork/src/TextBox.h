#pragma once
#include "Panel.h"
#include "Text.h"

class TextBox : public Panel
{
	friend void OnFocusEnter(void* obj);
	friend void OnFocusLeave(void* obj);
	friend void OnCharEntererd(void* obj, char c);
	friend void OnKeyPress(void *obj, int key, int mod);
	enum State
	{
		Nothing,
		Typing,
		Selection
	}state;
	unsigned  int cursorIndex;
	glm::ivec3 offsetTextAndCursor;
public:
	Text text;
	DrawablePanel cursor;
	TextBox();
	~TextBox();
	void Draw() override;
	bool isCursorAtEnd() const {
		return text.size() == cursorIndex;
	}


};

