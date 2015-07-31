#ifndef COLLECTION_H_
#define COLLECTION_H_

class CollectionScene : public Scene
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
	~CollectionScene();
	void Update();
	void Render();
};

#endif