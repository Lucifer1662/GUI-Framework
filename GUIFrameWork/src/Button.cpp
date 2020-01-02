#include "Button.h"
#include <glm\glm.hpp>
using glm::vec4;
#include "Window.h"
#include <GLFW\glfw3.h>


static void OnClick(void* obj, int but) {
	if (but == 0) {
		Button* button = (Button*)obj;
		button->colour = vec4(1, 0, 0, 1);
	}
}

static void OnEndClick(void* obj, int but) {
	if (but == 0) {
		Button* button = (Button*)obj;
		button->colour = vec4(1, 1, 1, 1);
	}
}



Button::Button()
{
	onClickDown.Add(this, OnClick);
	window.onMouseClickUp.Add(this, OnEndClick);
}


Button::~Button()
{
}
