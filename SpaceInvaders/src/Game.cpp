#include <SFML/Graphics.hpp>
#include "SpaceInvadersScene.h"
#include "Renderer.h"
#include "Console.h"
#include "Ship.h"
#include "SpaceInvadersUI.h"

int main(int argc, char ** argv)
{
	Console::Log("Creating window");

	unsigned int xSize = 1280;
	unsigned int ySize = 800;
	sf::RenderWindow window(sf::VideoMode(xSize, ySize), "SpaceInvaders");

	Console::Log("Initialising scene & renderer");

	SpaceInvadersScene invadersScene((float)xSize, (float)ySize);
	Renderer rend(window);
	SpaceInvadersUI userInterface(invadersScene, window);

	invadersScene.Init();
	userInterface.Init();
	
	invadersScene.SetUI(&userInterface);

	while(window.isOpen())
	{
		invadersScene.StartFrame();

		sf::Event event;

		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window.close();
			else
				invadersScene.HandleEvent(event);

		}

		invadersScene.Simulate();

		window.clear();

		rend.DrawScene(invadersScene);
		rend.DrawUI(userInterface);

		window.display();

		invadersScene.EndFrame();
	}


	Console::Log("Closing...");


	return 0;
}