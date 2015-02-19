#ifndef H_RENDERER
#define H_RENDERER


#include <SFML/Graphics.hpp>

#include "IRenderer.h"

class Renderer : public IRenderer
{
public:
	Renderer(sf::RenderWindow & wnd);

	virtual ~Renderer() { };

	virtual void DrawScene(const IScene & scene) const;
	virtual void DrawUI(const IUserInterface & ui) const;

private:
	template<class T>
	void Draw(const std::vector<T> & sprites) const;

	sf::RenderWindow * m_Window;
};



#endif