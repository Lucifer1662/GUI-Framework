#include "Text.h"
#include <string>
#include "Program.h"
#include "Window.h"
#include "_Texture2D.h"
using std::string;


thread_local unsigned int Text::program, Text::colourLocation, Text::positionLocation, Text::scaleLocation;

void WindowSizeChanged(void* obj, int w, int h) {
	((Text*)obj)->RecaculateScale();
}

void Text::RecaculateScale() {
	if (font) {
		float scal = pixelSize / (float)font->GetSize();
		scale.x = scal / window.GetHalfWidth();
		scale.y = scal / window.GetHalfHeight();
	}
}

void Text::UpdateTextMesh()
{
	
	unsigned numberOfDrawableCharacters = GetNumberOfDrawAbleCharacters();
	numberOfTrianglex3 = numberOfDrawableCharacters * 6;
	if (numberOfDrawableCharacters == 0)
		return;
	vector<vec4> vertices;
	vertices.reserve(numberOfDrawableCharacters * 4);
	vector<unsigned int> indices;
	indices.reserve(numberOfDrawableCharacters * 6);

	unsigned int cursor = 0;
	unsigned int line = 0;
	font->texture->WaitForTextureToBeLoaded();
	int texsizex = font->texture->getWidth();
	int texsizey = font->texture->getHeight();


	vec2 mid1, mid2;
	vec2 adjSize1, adjSize2;


	size_t i = 0;
	for (int j = 0; j < text.size(); j++)
	{

		if (text[j] == '\n') {
			line += font->GetLineHeight();
			cursor = 0;
			continue;
		}

		const FontCharacter& fontCharacter = font->GetFontCharacter(text[j]);
		if (text[j] != ' ') {
	
			adjSize1 = vec2((float)fontCharacter.width / texsizex, -(float)fontCharacter.height / texsizey);
			mid1 = vec2((float)fontCharacter.x / texsizex, (float)fontCharacter.y / texsizey);

			mid2 = vec2((int)cursor + fontCharacter.xoffset, -(int)(fontCharacter.yoffset - line));
			adjSize2 = vec2(fontCharacter.width , -(int)(fontCharacter.height));


			indices.emplace_back(i);
			indices.emplace_back(i + 1);
			indices.emplace_back(i + 2);

			indices.emplace_back(i);
			indices.emplace_back(i + 2);
			indices.emplace_back(i + 3);

			i += 4;

			vertices.emplace_back(mid2.x, mid2.y, mid1.x, mid1.y);
			vertices.emplace_back(mid2.x, mid2.y + adjSize2.y, mid1.x, mid1.y + adjSize1.y);
			vertices.emplace_back(mid2.x + adjSize2.x, mid2.y + adjSize2.y, mid1.x + adjSize1.x, mid1.y + adjSize1.y);
			vertices.emplace_back(mid2.x + adjSize2.x, mid2.y, mid1.x + adjSize1.x, mid1.y);


		}
		cursor += ((float)fontCharacter.xadvance);
	}


	vbo.SetData(&vertices[0], vertices.size() * sizeof(vec4));
	ibo.SetData(&indices[0], indices.size() * sizeof(unsigned int));

}

Text::Text() : colour(1,1,1)
{
	if (program == 0) {

		ibo.SetUsage(GL_DYNAMIC_DRAW);
		vbo.SetUsage(GL_DYNAMIC_DRAW);

		string vertexShader = R"V0G0N(
#version 330 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec2 vUv;

uniform vec3 offset;
uniform vec2 scale;

out vec2 uv;

void main() {
	uv = vUv;
    gl_Position = vPosition * vec4(scale,1,1) + vec4(offset,0);
}            

)V0G0N";


		string fragmentShdader =
			R"V0G0N(
#version 330 core

in vec2 uv;
out vec4 fragColor;

uniform vec4 colour;
uniform sampler2D texture;

void main(){
	vec4 text = texture(texture,uv);
	if(text.w < 0.1f)
		discard;
	else
		fragColor = colour * text;

}            

)V0G0N";

		program = CreateProgram(vertexShader, fragmentShdader);

		
		positionLocation = glGetUniformLocation(program, "offset");
		scaleLocation = glGetUniformLocation(program, "scale");
		colourLocation = glGetUniformLocation(program, "colour");
		glUniform1i(glGetUniformLocation(program, "texture"), 0);
	
	}

	Window& win = window;
	window.windowSizeChanged.Add(this, WindowSizeChanged);
	RecaculateScale();
}


Text::~Text()
{
}

unsigned int Text::GetNumberOfDrawAbleCharacters() const
{
	unsigned int result = 0;
	for (char character : text)
		if (character > 32 && character < 127)
			result++;
	
	return result;
}



void Text::SetText(string t)
{
	Text::text = std::move(t);
	UpdateTextMesh();
}

void Text::AppendToText(string appendige)
{
	text.append(appendige);
	UpdateTextMesh();
	
}

Text & Text::operator=(string& arg)
{
	Text::text = std::move(arg);
	UpdateTextMesh();
	return *this;
}

Text & Text::operator=(const char * arg)
{
	Text::text = std::move(arg);
	UpdateTextMesh();
	return *this;
}

Text & Text::operator=(char arg)
{
	Text::text = arg;
	UpdateTextMesh();
	return *this;
}

Text & Text::operator+=(const string & rhs)
{
	text.append(rhs);
	UpdateTextMesh();
	return *this;
}

Text & Text::operator+=(const char * rhs)
{
	text.append(rhs);
	UpdateTextMesh();
	return *this;
}

Text & Text::operator+=(char rhs)
{
	text += rhs;
	UpdateTextMesh();
	return *this;
}

void Text::pop_back()
{	if (text.size() > 0) {
		text.pop_back();
		UpdateTextMesh();
	}
}

vec2 Text::GetCursorPositionAt(unsigned int index) const
{
	if (GetNumberOfDrawAbleCharacters() == 0)
		return {0,0};

	
	unsigned int cursor = 0;
	unsigned int line = 0;


	for (int j = 0; j < index; j++)
	{

		if (text[j] == '\n') {
			line += font->GetLineHeight();
			cursor = 0;
			continue;
		}

		cursor += font->GetFontCharacter(text[j]).xadvance;
	}

	
	
	return vec2(cursor, line) * (pixelSize / (float)font->GetSize());
}

unsigned int Text::GetIndexAtPosition(glm::vec2 pos) const
{

	glm::vec2 cursorPos;


	for (unsigned int j = 0; j < text.size(); j++)
	{
		if (glm::distance(cursorPos, pos) < pixelSize/2)
			return j;

		if (text[j] == '\n') {
			cursorPos.y += font->GetLineHeight();
			cursorPos.x = 0;
			continue;
		}

		cursorPos.x += font->GetFontCharacter(text[j]).xadvance;
	}

	return text.size();
}

void Text::Draw()
{   
	
	glUseProgram(program);
	
	glUniform3fv(positionLocation, 1, &(window.PixelSpaceToWindowSpace(position))[0]);
	glUniform2fv(scaleLocation, 1, &scale[0] );
	glUniform4fv(colourLocation, 1, &colour[0]);

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, *font->texture.get());

	

	ibo.Bind();
	vbo.Bind();
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * 4, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * 4, (void*)(2 * 4));


	
	glDrawElements(GL_TRIANGLES, numberOfTrianglex3, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}
