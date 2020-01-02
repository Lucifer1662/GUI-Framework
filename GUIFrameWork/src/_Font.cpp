#include "_Font.h"
#include "Window.h"
#include <fstream>
#include <iostream>


_Font::_Font(const std::string& name, Window* window): name(name)
{
	if (window)
		Load(window);
}


_Font::~_Font()
{
}

void _Font::Load(Window* window)
{
	std::fstream stream = std::fstream(name);

	string line;

	std::getline(stream, line);
	int sizePos = line.find(" size") + 6;
	size = atoi(line.substr(sizePos, line.find(' ', sizePos)).c_str());
	std::getline(stream, line);
	lineHeight = atoi(&line[18]);
	std::getline(stream, line);

	//get texture here
	int quotationMarkOffset = line.find('"')+1;
	texture = window->GetTexture(name.substr(0, name.find_last_of('\\') + 1)
		+ line.substr(quotationMarkOffset, line.size() - quotationMarkOffset - 1));
	texture->WaitForTextureToBeLoaded();
	
	std::getline(stream, line);

	//now the line has "chars count=96"
	size_t amountOfCharacters = atoi(&line[12]);
	for (size_t i = 0; i < amountOfCharacters; i++)
	{
		std::getline(stream, line);
		//"id=54"3
		size_t id = atoi(&line[8]);
		characters[id].char_id = id;
		//"x=0"2
		characters[id].x = atoi(&line[18]);
		//"y=0"2
		characters[id].y = texture->getHeight() - atoi(&line[25]);
		//"width=0"6
		characters[id].width = atoi(&line[36]);
		//"height=0"7
		characters[id].height = atoi(&line[48]);
		//"xoffset=0"8
		characters[id].xoffset = atoi(&line[61]);
		//"yoffset=0"8 
		characters[id].yoffset = atoi(&line[74]);
		//"xadvance=0"9 
		characters[id].xadvance = atoi(&line[88]);

	}

	


}
