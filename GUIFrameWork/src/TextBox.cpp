#include "TextBox.h"
#include "Window.h"




void OnFocusEnter(void * obj)
{
	TextBox* textBox = ((TextBox*)obj);

	glm::ivec2 cursor = textBox->window.cursorPos - vec2(textBox->position + textBox->pivot);

	textBox->cursorIndex = textBox->text.GetIndexAtPosition(cursor);
	vec2 newPos = textBox->text.GetCursorPositionAt(textBox->cursorIndex);
	textBox->cursor.position.x = newPos.x;
	textBox->cursor.position.y = newPos.y;
	textBox->state = TextBox::Typing;
}

void OnFocusLeave(void * obj)
{
	TextBox* textBox = ((TextBox*)obj);
	textBox->state = TextBox::Nothing;
	textBox->cursorIndex = 0;
	textBox->cursor.position.x = 0;
	textBox->cursor.position.y = 0;

}

void OnCharEntererd(void* obj, char c) {
	TextBox* textBox = ((TextBox*)obj);
	if (textBox->state == TextBox::Typing) {
		if (textBox->isCursorAtEnd())
			textBox->text += c;
		else
			textBox->text.insert(textBox->cursorIndex, &c);
		textBox->cursorIndex++;
		vec2 newPos = textBox->text.GetCursorPositionAt(textBox->cursorIndex);
		textBox->cursor.position.x = newPos.x;
		textBox->cursor.position.y = newPos.y;
	}
}



void OnKeyPress(void *obj, int key, int mod) {
	TextBox* textBox = ((TextBox*)obj);
	if (textBox->state == TextBox::Typing) {
		switch (key) {
		case GLFW_KEY_BACKSPACE: {
			if (textBox->isCursorAtEnd())
				textBox->text.pop_back();
			else
				textBox->text.erase(textBox->cursorIndex);

			if (textBox->cursorIndex > 0)
				textBox->cursorIndex--;

			vec2 newPos = textBox->text.GetCursorPositionAt(textBox->cursorIndex);
			textBox->cursor.position.x = newPos.x;
			textBox->cursor.position.y = newPos.y;

		}break;
		case GLFW_KEY_DELETE: {
			if (!textBox->isCursorAtEnd())
				textBox->text.erase(textBox->cursorIndex);
		}break;


		case GLFW_KEY_LEFT: {
			textBox->cursorIndex--;
			vec2 newPos = textBox->text.GetCursorPositionAt(textBox->cursorIndex);
			textBox->cursor.position.x = newPos.x;
			textBox->cursor.position.y = newPos.y;
		}break;
		case GLFW_KEY_RIGHT: {
			textBox->cursorIndex++;
			vec2 newPos = textBox->text.GetCursorPositionAt(textBox->cursorIndex);
			textBox->cursor.position.x = newPos.x;
			textBox->cursor.position.y = newPos.y;
		}break;
		}
	}
}



TextBox::TextBox()
{
	onFocusEnter.Add(this, OnFocusEnter);
	onFocusLeave.Add(this, OnFocusLeave);
	window.charWasInputed.Add(this, OnCharEntererd);
	window.onKeyClick.Add(this, OnKeyPress);
	window.onKeyRepeat.Add(this, OnKeyPress);
	cursor.colour = vec4(1, 1, 1, 1);
}


TextBox::~TextBox()
{
}

void TextBox::Draw()
{
	Panel::Draw();
	window.CreateStencil();
	Panel::Draw();
	window.UseStencil();

	offsetTextAndCursor.x = -cursor.position.x + size.x * 0.9f;
	if (offsetTextAndCursor.x > 0)
		offsetTextAndCursor.x = 0;

	cursor.pivot = position + pivot + offsetTextAndCursor;
	cursor.size.y = text.GetTextSize();
	cursor.size.x = 2;
	cursor.Draw();

	text.position = position + offsetTextAndCursor;
	text.Draw();
	window.EndStencil();


	


}


