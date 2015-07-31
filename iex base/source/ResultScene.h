#ifndef RESULT_SELECT_H_
#define RESULT_SELECT_H_

class Fade;

class ResultScene : public Scene
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
	~ResultScene();
	void Update();
	void Render();
};

#endif