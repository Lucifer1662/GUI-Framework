#include "Panel.h"
#include "Window.h"

void OnMouseClickDownStatic(void*obj, int button) {
	Panel* panel = (Panel*)obj;
	if (panel->isCursorOverPanel()) {
		panel->onClickDown(button);
		panel->onFocusEnter();
		panel->isInFocus = true;
	}
	else if(panel->isInFocus) {
		panel->isInFocus = false;
		panel->onFocusLeave();
	}

}

void OnMouseClickUpStatic(void*obj, int button) {
	Panel* panel = (Panel*)obj;
	if (panel->isCursorOverPanel())
		panel->onClickUp(button);
	
}

void OnMouseClickRepeatStatic(void* obj, int button){
	Panel* panel = (Panel*)obj;
	if (panel->isCursorOverPanel())
		panel->onClickRepeat(button);
}


bool Panel::isCursorOverPanel()
{
	vec2 cursorPos = window.cursorPos;
	vec2 pos = pivot;
	pos.x += position.x;
	pos.y += position.y;
	return cursorPos.x > pos.x && cursorPos.x < pos.x + size.x
		&& cursorPos.y < pos.y && cursorPos.y > pos.y - size.y;
}


void DrawablePanel::Draw()
{
	vec3 pos = position;
	pos.x += pivot.x;
	pos.y += pivot.y;
	if (texture) {
		window.DrawRect(pos, size, colour, texture);
	}
	else {
		window.DrawRect(pos, size, colour);
	}

}

DrawablePanel::DrawablePanel() :size({ 1,1 }), colour{ 1,1,1,1 } {

}


Panel::Panel() 
{
	window.onMouseClickDown.Add(this, OnMouseClickDownStatic);
	window.onMouseClickUp.Add(this, OnMouseClickUpStatic);
	window.onMouseClickRepeat.Add(this, OnMouseClickRepeatStatic);
}


Panel::~Panel()
{
}
