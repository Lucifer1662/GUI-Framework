#include "Window.h"
#include <iostream>
#include <tuple>
using std::cout;
using std::endl;
using uint = unsigned int;

#include "Buffer.h"




thread_local Window* Window::myWindow;



void Window::DrawRect(vec3 position, const vec2& size, const vec4& colour)
{
	rect.Draw(PixelSpaceToWindowSpace(position), size / vec2(widthHalf, heightHalf), colour);
}

void Window::DrawRect(vec3 position, const vec2 & size, const vec4 & colour, const Texture2D & texture)
{
	rect.Draw(PixelSpaceToWindowSpace(position), size/vec2(widthHalf,heightHalf), colour, *texture.get());
}


void Window::MainLoop()
{
	
	if (window == nullptr)
		return;

	Window* win = myWindow;
	std::thread nongGuiThread = std::thread([&]() {
		win;
		while (true) {
			win->windowSizeChanged.Process();
			win->charWasInputed.Process();
			win->onKeyClick.Process();
			win->onKeyRelease.Process();
			win->onKeyRepeat.Process();
			win->onKey.Process();
			win->onMouseClickDown.Process();
			win->onMouseClickUp.Process();
			win->onMouseClickRepeat.Process();
			win->onMouseMoved.Process();
		}
	
	
	});
	
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(window)){
		glfwWaitEvents();

		
		
		for (Texture2D& texture: texturesThatNeedLoading) {
			texture->LoadTexture();
		}
		texturesThatNeedLoading.clear();

		glClearColor(backGroundColour.x, backGroundColour.y, backGroundColour.z, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (size_t i = 0; i < objects.size(); i++)
		{
			objects[i]->Draw();
		}
		

		glfwSwapBuffers(window);
	}
}

Texture2D Window::GetTexture(const string & name)
{
	

	if (textureCache.isLoaded(name) || glfwGetCurrentContext() == window)
		return textureCache.Get(name);
	Texture2D texture = textureCache.CreateBlank(name);
	texturesThatNeedLoading.push_back(texture);
	return texture;
}

Font Window::GetFont(const string & name)
{
	return fontCache.Get(name);
}

void Window::CreateStencil()
{
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF); // Write to stencil buffer
	glDepthMask(GL_FALSE); // Don't write to depth buffer
	glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
}

void Window::UseStencil()
{
	glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
	glStencilMask(0x00); // Don't write anything to stencil buffer
	glDepthMask(GL_TRUE); // Write to depth buffer

}

void Window::EndStencil()
{
	glDisable(GL_STENCIL_TEST);
}

static void WindowSizeChanged(GLFWwindow* glfwWindow, int width, int height) {
	Window::myWindow->width = width;
	Window::myWindow->height = height;
	Window::myWindow->widthHalf = width / 2;
	Window::myWindow->heightHalf = height / 2;
	glViewport(0, 0, width, height);
	Window::myWindow->windowSizeChanged(width,height);
}

void CharCallback(GLFWwindow *window, unsigned int c) {
	std::cout << (char)c << std::endl;
	Window::myWindow->charWasInputed(c);
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	
	switch (action)
	{
	case GLFW_PRESS: {
		Window::myWindow->onKeyClick(key, mods);
	}
	break;

	case GLFW_RELEASE: {
		Window::myWindow->onKeyRelease(key, mods);
	}
	 break;
	case GLFW_REPEAT: {
		Window::myWindow->onKeyRepeat(key, mods);
	}
	 break;
	}
	Window::myWindow->onKey(key, mods, action);
}

Window::Window(int w, int h, string name): g(window,w,h), windowName(std::move(name)) , textureCache(*this), fontCache(*this), windowSizeChanged()
{
	

	

	glfwGetWindowSize(window, &width, &height);
	
	myWindow = this;

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int mode, int s) {
			switch (mode)
			{
			case GLFW_PRESS:
				myWindow->onMouseClickDown(button);
				break;
			case GLFW_RELEASE:
				myWindow->onMouseClickUp(button);
				break;
			case GLFW_REPEAT:
				myWindow->onMouseClickRepeat(button);
				break;
			}

	});

	
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) { Window::myWindow->cursorPos = vec2(x, y); });

	glfwSetWindowSizeCallback(window, WindowSizeChanged);

	glfwSetCharCallback(window, CharCallback);

	glfwSetKeyCallback(window, KeyCallback);

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
		myWindow->onMouseMoved(x, y);
		myWindow->cursorPos.x = x;
		myWindow->cursorPos.y = y;
		
	});

	
	widthHalf = width/2;
	heightHalf = height/2;


}





Window::~Window()
{

}

