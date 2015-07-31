#ifndef CHARA_SELECT_H_
#define CHARA_SELECT_H_

class Fade;

class CharaSelectScene : public Scene
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
	~CharaSelectScene();
	void Update();
	void Render();
};

#endif