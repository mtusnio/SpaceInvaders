#ifndef H_IRENDERER
#define H_IRENDERER

class IScene;
class IUserInterface;

class IRenderer
{
public:
	virtual ~IRenderer() { };

	// Draws the given scene on screen
	virtual void DrawScene(const IScene & scene) const = 0;

	// Draws the UI
	virtual void DrawUI(const IUserInterface & ui) const = 0;

};


#endif