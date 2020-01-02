#include "_Texture2D.h"
#include "SOIL\SOIL.h"
#include "GL\glew.h"


_Texture2D::_Texture2D(const std::string &name, Window* loadTexture):name(name)
{
	if (loadTexture)
		LoadTexture();
}


_Texture2D::~_Texture2D()
{
	glDeleteTextures(1, &textureId);
}


void _Texture2D::LoadTexture()
{
	unsigned char* data = SOIL_load_image(name.c_str(), (int*)&width, (int*)&height, &channels, SOIL_LOAD_AUTO);
	//textureId = SOIL_load_OGL_texture(name.c_str(), 0, 0, 0);
	textureId = SOIL_create_OGL_texture(data, width, height, channels, 0, SOIL_FLAG_INVERT_Y);

	SOIL_free_image_data(data);
}



bool _Texture2D::isLoaded()
{
	return textureId > 0;
}

void _Texture2D::WaitForTextureToBeLoaded()
{
	while (textureId == 0)
	{

	}
}
