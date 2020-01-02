#pragma once
#include <memory>
#include <string>


class Window;

class _Texture2D
{
	using Texture2D = std::shared_ptr<_Texture2D>;
	friend class Window;
	const std::string& name;
	unsigned int textureId = 0;
	void LoadTexture();
	unsigned int width, height;
	int channels;
public:
	_Texture2D(const std::string& name, Window* window);
	~_Texture2D();

	unsigned int getHeight() {
		return height;
	}
	unsigned int getWidth() {
		return width;
	}
	bool isLoaded();
	operator unsigned int() { return textureId; }
	void WaitForTextureToBeLoaded();
};

using Texture2D = std::shared_ptr<_Texture2D>;



