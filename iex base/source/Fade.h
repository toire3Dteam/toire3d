#ifndef FADE_H_
#define	FADE_H_

//=======================================================================
//						フェードクラス
//=======================================================================
//	※変数名とかかぶると思うので、委譲で使ってください
/*	×	*/
/*	class sceneMain : public Scene, Fade{
					～*/


/*	○	*/
/*	class sceneMain : public Scene{
	private:
		Fade *fade;
					～*/

class Fade
{
public:
	int mode;				/*	フェードイン、フェードアウト、何もしない	*/
	int speed;				/*	フェードする速度	*/
	BYTE limit;				/*	どこまでフェードするか(0～255の範囲)	*/
	BYTE alpha;				/*	α値(0～255)	*/
	DWORD color;			/*	カラー	*/

	enum FADE_FLAG{ FADE_STOP, FADE_IN, FADE_OUT };

	//	初期化
	Fade();
	void Initialize();


	// 設定
	void Set(int Mode, int Speed, BYTE Limit, BYTE Alpha, DWORD Color = 0x00000000);	/*	フル設定版	*/
	void Set(int Mode, int Speed, BYTE Limit);											/*	途中で追加したいとき版	*/
	void Set(int Mode, int Speed);														/*	簡易版	*/


	//	更新・描画
	void Update();
	void Render();

	bool isFadeStop(){ return (mode == FADE_STOP); }
};

#endif // !_FADE_H_
