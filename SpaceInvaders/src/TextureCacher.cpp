#include "TextureCacher.h"

#include <SFML/Graphics.hpp>
#include <string>

#include "Console.h"

TextureCacher::~TextureCacher()
{
	Clear();
}


void TextureCacher::Clear()
{
	for(auto it = m_Textures.begin(); it != m_Textures.end(); it++)
	{
		delete it->second;
	}

	m_Textures.clear();
}

sf::Texture * TextureCacher::LoadTexture(const std::string & path)
{
	if(m_Textures.find(path) != m_Textures.end())
		return m_Textures[path];

	sf::Texture * tex = new sf::Texture();
	
	if(!tex->loadFromFile(path))
	{
		Console::Log("Could not load texture from path: " + path, Console::Severity::WARNING);

		delete tex;
		return nullptr;
	}

	tex->setSmooth(true);
	m_Textures[path] = tex;
	return tex;
}