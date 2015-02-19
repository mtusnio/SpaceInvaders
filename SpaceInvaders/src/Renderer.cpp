#include "Renderer.h"
#include "IScene.h"
#include "Entity.h"
#include "IUserInterface.h"

Renderer::Renderer(sf::RenderWindow & wnd)
{
	m_Window = &wnd;
}


void Renderer::DrawScene(const IScene & scene) const
{
	auto ents = scene.GetEntities();
	Draw(ents);
}

void Renderer::DrawUI(const IUserInterface & ui) const
{
	auto sprites = ui.GetElements();
	Draw(sprites);
}

template<class T>
void Renderer::Draw(const std::vector<T> & sprites) const
{
	for(T spr : sprites)
	{
		m_Window->draw(*spr);
	}
}