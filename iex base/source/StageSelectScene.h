#ifndef STAGE_SELECT_H_
#define STAGE_SELECT_H_

class StageSelectScene : public Scene
{
private:

	//================================
	//		��
	enum IMAGE{
		IMG_BACK,
		IMG_MAX
	};

	//================================
	//		�摜�ƃr���[�֌W
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