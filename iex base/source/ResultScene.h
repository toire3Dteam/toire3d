#ifndef RESULT_SELECT_H_
#define RESULT_SELECT_H_

class Fade;

class ResultScene : public Scene
{
private:

	//================================
	//		—ñ‹“
	enum IMAGE{
		IMG_BACK,
		IMG_MAX
	};

	//================================
	//		‰æ‘œ‚Æƒrƒ…[ŠÖŒW
	iexView *view;
	iex2DObj *image[IMG_MAX];
	Fade *fade;

	Text *text;

public:
	bool Initialize();
	~ResultScene();
	void Update();
	void Render();
};

#endif