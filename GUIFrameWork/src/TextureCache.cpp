#include "TextureCache.h"



TextureCache::TextureCache()
{
}


TextureCache::~TextureCache()
{

}


Texture2D TextureCache::CreateBlank(const std::string& name)
{
	Texture2D texture = textures[name].lock();
	textures[name] = texture = std::make_shared<_Texture2D>(textures.find(name)->first, false);
	return texture;
}


Texture2D TextureCache::GetTexture(const std::string& name)
{
	Texture2D texture = textures[name].lock();
	if (!texture)
		textures[name] = texture = std::make_shared<_Texture2D>(textures.find(name)->first);
	return texture;
}

bool TextureCache::isTextureLoaded(const std::string & name)
{
	return (bool)textures[name].lock();
}





