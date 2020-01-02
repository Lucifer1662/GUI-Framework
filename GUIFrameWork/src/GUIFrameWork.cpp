#include "Window.h"
#include "Panel.h"
#include "_Texture2D.h"
#include <Windows.h>
#include <iostream>
#include "Button.h"
#include "Text.h"
#include "TextBox.h"
Font font;

class MyWindow : public Window {
public:
	MyWindow(): Window(800, 600, "GUI"){
		///*
		Panel& bluePanel = CreateObject<Panel>();
		bluePanel.position = vec3(0, 81, 0);
		bluePanel.size = vec2(800, 81);
		bluePanel.texture = Window::GetTexture("assets\\texture1.png");
		
		bluePanel.onClickDown.Add([](int button) {
			Panel& bluePanel1 = myWindow->CreateObject<Panel>();
			bluePanel1.position = vec3(0, 300, 0);
			bluePanel1.size = vec2(800, 100);
			bluePanel1.texture = myWindow->GetTexture("assets\\texture2.bmp");

		});
		
		Button& button1 = CreateObject<Button>();
		button1.position = vec3(0, 500, 0);
		button1.size = { 100,100 };

		backGroundColour = vec3(0.1f, 0, 0);
		Text& text = CreateObject<Text>();
		text.position = vec3(0, 0, 0);
		text.SetFont("assets\\Fonts\\Arial.fnt");
		text.SetSize(20);
		text.SetText("Hello World");


		TextBox& textBox = CreateObject<TextBox>();
		textBox.position = vec3(100, 300, 0);
		textBox.pivot = vec3(0, 50, 0);
		textBox.size = vec2(300, 50);
		textBox.colour = vec4(0.1f, 0.1f, 0.1f, 1);
		textBox.text.SetFont("assets\\Fonts\\Arial.fnt");
		textBox.text.SetSize(50);
		textBox.text.SetText("textBox1");

		
		//*/
	}

};

template<typename T>
class ObjectThread : public std::thread{
	T* obj;
public:
	template<typename... Args >
	ObjectThread() : std::thread([&]() {
		T object = {Args...};
		obj = &object;
		object.MainLoop();
	}) {

	}
	
};

#include <condition_variable>

std::condition_variable cv;
std::mutex m;
std::unique_lock<std::mutex>lock (m) ;

void refresh(GLFWwindow* window) {
	std::cout << "Refresh" << std::endl;
}

void main() {
	

	glm::tmat4x4

	MyWindow windowThread1;
	windowThread1.MainLoop();
	while (true) {}


}