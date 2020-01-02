#pragma once
#include <GL\glew.h>
#include "Object.h"
#include <glm\glm.hpp>
#include "Buffer.h"
#include "_Font.h"
#include "Window.h"


class Text: public Object
{

	static thread_local unsigned int program, colourLocation, positionLocation, scaleLocation;
	Buffer<GL_ARRAY_BUFFER> vbo;
	Buffer<GL_ELEMENT_ARRAY_BUFFER> ibo;
	unsigned int pixelSize;
	vec2 scale;
	vec3 colour;
	Font font;
	string text;
	unsigned int numberOfTrianglex3;
	unsigned int GetNumberOfDrawAbleCharacters() const;

	friend void WindowSizeChanged(void* obj, int w, int h);
	void RecaculateScale();
	void UpdateTextMesh();
public:
	Text();
	~Text();
	inline void SetFont(const char* fontName) {
		font = window.GetFont(fontName);
		RecaculateScale();
	}
	inline void SetSize(unsigned int size) {
		pixelSize = size;
		RecaculateScale();
	}
	inline const unsigned int GetTextSize()const { return pixelSize; }
	void SetText(string text);
	void AppendToText(string appendige);
	inline const Font& GetFont() const {return font;}
	inline const unsigned int size() const { return text.size(); };

	Text& operator=(string& arg);
	Text& operator=(const char* arg);
	Text& operator=(char arg);
	Text& operator+=(const string& rhs);
	Text& operator+=(const char* rhs);
	Text& operator+=(char rhs);
	void pop_back();
	char back()const { return text.back(); }
	vec2 GetCursorPositionAt(unsigned int index) const;
	unsigned int GetIndexAtPosition(glm::vec2 pos) const;
	
	template<typename T>
	void insert(unsigned int offset, T t) {
		text.insert(offset, t);
		UpdateTextMesh();
	}

	inline void erase(unsigned int i, unsigned int count = 1) {
		text.erase(i, count);
		UpdateTextMesh();
	}


	

	void Draw() override;
};

