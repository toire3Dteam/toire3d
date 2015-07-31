#ifndef TITLE_H_
#define TITLE_H_

#include	"Fade.h"

class TitleScene : public Scene
{
private:

	//================================
	//		—ñ‹“
	enum IMAGE{
		IMG_BACK,
		IMG_MAX
	};

	//================================
	//		‰æ‘œ‚Æƒrƒ…[
	iexView *view;
	iex2DObj *image[2];
	Fade *fade;

	//================================
	//		•¶š
	Text *text;


public:
	bool Initialize();
	~TitleScene();
	void Update();
	void Render();
};

#endif