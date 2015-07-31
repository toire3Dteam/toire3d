#ifndef OPTION_H_
#define OPTION_H_

class OptionScene : public Scene
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
	~OptionScene();
	void Update();
	void Render();
};

#endif