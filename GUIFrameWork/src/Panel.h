#pragma once
#include "Object.h"
#include <glm\glm.hpp>
#include "_Texture2D.h"
#include "Event.h"


//just a panel that draws to the screen
class DrawablePanel : public Object {

public:
	glm::vec2 size;
	glm::vec4 colour;
	glm::ivec3 pivot;
	Texture2D texture;
	void Draw() override;
	DrawablePanel();
};

//a panel that draws to the screen but has events as well
class Panel : public DrawablePanel
{
	
	friend void OnMouseClickDownStatic(void* obj, int button);
	friend void OnMouseClickUpStatic(void* obj, int button);
	friend void OnMouseClickRepeatStatic(void* obj, int button);
	bool isCursorOverPanel();
	bool isInFocus = false;

public:
	//events
	Event<void, int> onClickDown;
	Event<void, int> onClickRepeat;
	Event<void, int> onClickUp;
	Event<void> onFocusEnter;
	Event<void> onFocusLeave;

	inline bool IsInFocus() { return isInFocus; }
	
	Panel();
	virtual ~Panel();
};

