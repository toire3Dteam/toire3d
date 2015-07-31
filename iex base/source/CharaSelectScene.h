#ifndef CHARA_SELECT_H_
#define CHARA_SELECT_H_

class Fade;

class CharaSelectScene : public Scene
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
	~CharaSelectScene();
	void Update();
	void Render();
};

#endif