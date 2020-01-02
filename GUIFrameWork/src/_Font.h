#pragma once
#include "_Texture2D.h"

struct FontCharacter {
	char char_id;
	size_t x;
	size_t y;
	size_t width;
	size_t height;
	int xoffset;
	size_t yoffset;
	size_t xadvance;
};

class Window;

class _Font
{
	
	FontCharacter characters[127];
	unsigned int lineHeight;
	unsigned int size;
	const std::string& name;
public:
	Texture2D texture;
	_Font(const std::string& name, Window* window);
	~_Font();
	void Load(Window* window);
	inline unsigned int GetSize() { return size; }
	inline unsigned int GetLineHeight() { return lineHeight; }
	inline const FontCharacter& GetFontCharacter(char character) { return characters[character]; };
};
typedef std::shared_ptr<_Font> Font;

