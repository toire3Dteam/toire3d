#ifndef MODE_SELECT_H_
#define MODE_SELECT_H_

class Fade;

class ModeSelectScene : public Scene
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
	~ModeSelectScene();
	void Update();
	void Render();
};

#endif