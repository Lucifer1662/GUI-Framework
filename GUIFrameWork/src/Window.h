#pragma once
#include <vector>
#include "Object.h"
#include <thread>
#define GLEW_STATIC
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include <string>
#include "Mesh.h"
#include "_Texture2D.h"
#include "Event.h"
#include "TextureCache.h"
#include "FontCache.h"
#include "_Font.h"
#include "Program.h"
#include "ProgramCache.h"

using std::string;
using glm::vec4;
#include "Buffer.h"




class Rect {
	unsigned int program, programUV, colourUVLocation, colourLocation, positionLocation, positionUVLocation, scaleLocation, scaleUVLocation;
	Buffer<GL_ARRAY_BUFFER> vbo;
	Buffer<GL_ELEMENT_ARRAY_BUFFER> ibo;
public:

	
	Rect& operator=(const Rect&) = delete;
	Rect(const Rect& rect) = delete;

	Rect() {
		float verts[8] = {
			0, 0,
			0,  1,
			1,  1,
			1, 0,
		};



		vbo.SetUsage(GL_STATIC_DRAW);
		vbo.SetData(verts, sizeof(verts));


		unsigned int indices[6] = {
			0,1,2,
			0,2,3
		};
		ibo.SetUsage(GL_STATIC_DRAW);
		ibo.SetData(indices, sizeof(indices));


		string vertexShader = R"V0G0N(
#version 330 core

layout(location = 0) in vec4 vPosition;

uniform vec3 offset;
uniform vec2 scale;

out vec2 uv;

void main() {
	uv = vPosition.xy;
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
	fragColor = colour * texture(texture,uv);

}            

)V0G0N";

		programUV = CreateProgram(vertexShader, fragmentShdader);

		
		positionUVLocation = glGetUniformLocation(programUV, "offset");
		scaleUVLocation = glGetUniformLocation(programUV, "scale");
		colourUVLocation = glGetUniformLocation(programUV, "colour");
		glUniform1i(glGetUniformLocation(programUV, "texture"), 0);



		vertexShader = R"V0G0N(
#version 330 core

layout(location = 0) in vec4 vPosition;

uniform vec3 offset;
uniform vec2 scale;

void main() {
    gl_Position = vPosition * vec4(scale,1,1) + vec4(offset,0);
}            

)V0G0N";


		fragmentShdader =
			R"V0G0N(
#version 330 core

out vec4 fragColor;

uniform vec4 colour;

void main(){
	fragColor = colour;

}            

)V0G0N";

		program = CreateProgram(vertexShader, fragmentShdader);

		positionLocation = glGetUniformLocation(programUV, "offset");
		scaleLocation = glGetUniformLocation(programUV, "scale");
		colourLocation = glGetUniformLocation(programUV, "colour");
	}



	void Draw(const vec3& position, const vec2& size, const vec4& colour, unsigned int textureId) {
		glUseProgram(programUV);

		glUniform3fv(positionUVLocation, 1, &position[0]);
		glUniform2fv(scaleUVLocation, 1, &size[0]);
		glUniform4fv(colourUVLocation, 1, &colour[0]);

		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, textureId);

	
		ibo.Bind();
		vbo.Bind();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void Draw(const vec3& position, const vec2& size, const vec4& colour) {
		glUseProgram(program);

		glUniform3fv(positionLocation, 1, &position[0]);
		glUniform2fv(scaleLocation, 1, &size[0]);
		glUniform4fv(colourLocation, 1, &colour[0]);



		
		ibo.Bind();
		vbo.Bind();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);


		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
};


struct GLFWINIT {
	GLFWINIT() {
		if (!glfwInit()) {
			fprintf(stderr, "Failed to initialize GLFW\n");
			return;
		}

	}

};

struct GG {
	GG(GLFWwindow*& window, int w, int h) {
		static GLFWINIT glfwINIT = GLFWINIT();
		window = glfwCreateWindow(w, h, "UI", NULL, NULL);
		glfwMakeContextCurrent(window);
		if (glewInit() != GLEW_OK) {
			fprintf(stderr, "Failed to initialize GLEW\n");
			glfwDestroyWindow(window);
			window = nullptr;
		}
		
	}

};

template<typename... Args>
class WindowEvent : public Event<void, Args...> {
	template<std::size_t ...I>
	void call_func(int i, std::index_sequence<I...>)
	{
		this->Call(std::get<I>(args[i])...);
		//*(Window*)(this)(std::get<I>(args[i])...);
	}
	vector <std::tuple<Args...>> args;

public:
	void Process() {
		for (size_t i = 0; i < args.size(); i++)
			call_func(i, std::index_sequence_for<Args...>{});
		args.clear();
	}
	
	void operator ()(Args... args) {
		WindowEvent::args.push_back(std::make_tuple(args...));
	}


};


class Window
{
	GG g;
	
	Window(const Window& window) = delete;
	friend void WindowSizeChanged(GLFWwindow* glfwWindow, int width, int height);

	GLFWwindow* window;
	int width, height, widthHalf, heightHalf;
	string windowName;
	vector<Texture2D> texturesThatNeedLoading;
	Texture2DCache textureCache;
	FontCache fontCache;
	Rect rect;
	

public:
	static thread_local Window* myWindow;

	std::vector<Object*> objects;
	vec3 backGroundColour;

	vec2 cursorPos;
	bool mouseButtonDown;

	void DrawRect(vec3 position, const vec2& size, const vec4& colour);
	void DrawRect(vec3 position, const vec2& size, const vec4& colour, const Texture2D& texture);
	Window(int width, int height, string name);
	void MainLoop();
	Texture2D GetTexture(const string& name);
	Font GetFont(const string& name);



	unsigned int GetWidth() { return width; }
	unsigned int GetHeight() { return height; }
	unsigned int GetHalfWidth() { return widthHalf; }
	unsigned int GetHalfHeight() { return heightHalf; }


	
	
	WindowEvent<int, int> windowSizeChanged;
	WindowEvent<char> charWasInputed;
	WindowEvent<int, int> onKeyClick;
	WindowEvent<int, int> onKeyRelease;
	WindowEvent<int, int> onKeyRepeat;
	WindowEvent<int, int, int> onKey;
	
	WindowEvent<int> onMouseClickDown;
	WindowEvent<int> onMouseClickUp;
	WindowEvent<int> onMouseClickRepeat;
	WindowEvent<int, int> onMouseMoved;

	//Creates a stencil and  draw to create a stencil on the screen
	void CreateStencil();

	//Draws things in the stencil
	void UseStencil();

	//Clears the stecil created
	void EndStencil();

	template<typename T>
	T& CreateObject() {
		T* t = new T();
		objects.push_back(t);
		return *t;
	}

	inline glm::vec3 PixelSpaceToWindowSpace(vec3 v) {
		v.y = height - v.y;
		v.x = v.x / widthHalf - 1;
		v.y = v.y / heightHalf - 1;

		
		return v;
	}


	~Window();
};

