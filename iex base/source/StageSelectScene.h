#ifndef STAGE_SELECT_H_
#define STAGE_SELECT_H_

class StageSelectScene : public Scene
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
	~StageSelectScene();
	void Update();
	void Render();
};

#endif