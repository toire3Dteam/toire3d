#ifndef TITLE_H_
#define TITLE_H_

#include	"Fade.h"

class TitleScene : public Scene
{
private:

	//================================
	//		��
	enum IMAGE{
		IMG_BACK,
		IMG_MAX
	};

	//================================
	//		�摜�ƃr���[
	iexView *view;
	iex2DObj *image[2];
	Fade *fade;

	//================================
	//		����
	Text *text;


public:
	bool Initialize();
	~TitleScene();
	void Update();
	void Render();
};

#endif