#pragma once
#include "TDNLIB.h"

//+---------------------------------
//	基本のコレクション用のイラスト
//+---------------------------------

// アイコン用のデータ
struct MOVIE_ICON_DATA
{
	tdn2DObj* pIcon;	// アイコン
	std::string sTitle;	// タイトル
	std::string sText;	// 文字
	Vector2 vPos;		// 場所
	float iAlpha;		// 透明度

	MOVIE_ICON_DATA();
};

// 実行した時用のデータ (1280x720の画像とか)
struct MOVIE_DATA
{
	tdnMovie* pMovie;	// ムービー
	tdn2DObj* pPic;		// 出力先画像
	std::string sTitle;	// タイトル
	std::string sText;	// 文字

	MOVIE_DATA();
};

class BaseMovie
{
public:
	BaseMovie();
	virtual ~BaseMovie();

	// アイコン関連
	// virtual void UpdateIcon();
	void RenderIcon(int iX, int iY);
	void RenderIconNonPurchase(int iX, int iY);	//	未購入版アイコン
	void RenderIconInfo(int iX, int iY);

	// ★かなり質の良い画像を読み込むので選択したときに画像をロードさせる
	// 一括で読み込むと大変なことになるはず。
	virtual void InitExe() = 0;
	virtual void RereaseExe(); // 見終わった画像はその都度解放する
	bool UpdateExe();
	void CtrlExe(int iDevice);	// 画像操作
	void RenderExe();
	void RenderExeInfo(int iX, int iY);

	//virtual void AnLock();	//　ロック解除

	// アクセサ
	MOVIE_ICON_DATA GetMovieIcon() { return m_tagMovieIcon; }	// イラストアイコンの構造体
	MOVIE_DATA GetMovie() { return m_tagMovie; }				// イラストの構造体
																	
	void SetIconPos(Vector2 vPos) { m_tagMovieIcon.vPos = vPos; }	// 閲覧用のポジションの設定
	bool isRock() { return m_bRock; }

	int GetPrice() { return m_iPrice; }
protected:

	MOVIE_ICON_DATA	m_tagMovieIcon;	// アイコンの構造体
	MOVIE_DATA		m_tagMovie;		// 開いた時のイラスト


	bool m_bRock;			// 封印されてるロック　(お金で買えない・サムネが鍵マークに)
	int m_iPrice;			// 値段
};

//+---------------------------------
//	それぞれのムービー
//+---------------------------------
class OpMovie :public BaseMovie
{
public:
	OpMovie();
	~OpMovie() {};

	// ★かなり質の良い画像を読み込むので選択したときに画像をロードさせる
	void InitExe();

private:

};

class OldOpMovie :public BaseMovie
{
public:
	OldOpMovie();
	~OldOpMovie() {};

	// ★かなり質の良い画像を読み込むので選択したときに画像をロードさせる
	void InitExe();

private:

};
