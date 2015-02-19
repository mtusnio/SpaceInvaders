#ifndef H_TEXTURECACHER
#define H_TEXTURECACHER

namespace sf
{
	class Texture;
}

#include <string>
#include <map>

class TextureCacher
{
public:
	TextureCacher() { };
	virtual ~TextureCacher();


	// Clears the cache and frees any textures still in memory.
	// If they are still in use this might cause dangling pointers!
	virtual void Clear();

	// Caches or returns a cached version of the texture. Keep in mind,
	// path to the same file can be supplied in multiple ways, thus resulting
	// in multiple copies of the same texture still in memory.
	// This will persist until, say, the textures are stored
	// against their hashes, and not paths.
	virtual sf::Texture * LoadTexture(const std::string & path);

private:
	// The cache can literally blow up in our face if we do a shallow copy,
	// prevent it
	TextureCacher(const TextureCacher & cache);
	void operator=(const TextureCacher & cache);

	std::map<std::string, sf::Texture*> m_Textures;
};


#endif