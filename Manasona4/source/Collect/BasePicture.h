#pragma once
#include "TDNLIB.h"

//+---------------------------------
//	基本のコレクション用のイラスト
//+---------------------------------
//illust

// アイコン用のデータ
struct PICTURE_ICON_DATA
{
	tdn2DObj* pIcon;	// アイコン
	std::string sTitle;	// タイトル
	std::string sText;	// 文字
	Vector2 vPos;		// 場所
	float iAlpha;		// 透明度

	PICTURE_ICON_DATA();
};

// 実行した時用のデータ (1280x720の画像とか)
struct PICTURE_DATA
{
	tdn2DObj* pPic;		// アイコン
	std::string sTitle;	// タイトル
	std::string sText;	// 文字
	Vector2 vPos;		// 場所
	float fScale;		// 拡大縮小
	float iAlpha;		// 透明度

	PICTURE_DATA();
};

class BasePicture
{
public:
	BasePicture();
	virtual ~BasePicture();

	// アイコン関連
	// virtual void UpdateIcon();
	void RenderIcon(int iX, int iY);
	void RenderIconNonPurchase(int iX, int iY);	//	未購入版アイコン
	void RenderIconInfo(int iX, int iY);

	// ★かなり質の良い画像を読み込むので選択したときに画像をロードさせる
	// 一括で読み込むと大変なことになるはず。
	virtual void InitExe() = 0;
	virtual void RereaseExe(); // 見終わった画像はその都度解放する
	void UpdateExe();
	void CtrlExe(int iDevice);	// 画像操作
	void RenderExe();
	void RenderExeInfo(int iX, int iY);

	//virtual void AnLock();	//　ロック解除

	// アクセサ
	PICTURE_ICON_DATA GetPictureIcon() { return m_tagPictureIcon; }	// イラストアイコンの構造体
	PICTURE_DATA GetPicture() { return m_tagPicture; }				// イラストの構造体
																	
	void SetIconPos(Vector2 vPos) { m_tagPictureIcon.vPos = vPos; }	// 閲覧用のポジションの設定
	bool isRock() { return m_bRock; }

	int GetPrice() { return m_iPrice; }
protected:

	PICTURE_ICON_DATA	m_tagPictureIcon;	// アイコンの構造体
	PICTURE_DATA		m_tagPicture;		// 開いた時のイラスト


	bool m_bRock;			// 封印されてるロック　(お金で買えない・サムネが鍵マークに)
	int m_iPrice;			// 値段
};

//+---------------------------------
//	アイル―のイラスト
//+---------------------------------
class AirouPicture :public BasePicture
{
public:
	AirouPicture();
	~AirouPicture() {};

	// ★かなり質の良い画像を読み込むので選択したときに画像をロードさせる
	void InitExe();

private:

};

//+---------------------------------
//	棒人間のイラスト
//+---------------------------------
class TekiPicture :public BasePicture
{
public:
	TekiPicture();
	~TekiPicture() {};

	// ★かなり質の良い画像を読み込むので選択したときに画像をロードさせる
	void InitExe();

private:

};

//+---------------------------------
//	∵のイラスト
//+---------------------------------
class NazenarabaPicture :public BasePicture
{
public:
	NazenarabaPicture();
	~NazenarabaPicture() {};

	// ★かなり質の良い画像を読み込むので選択したときに画像をロードさせる
	void InitExe();

private:

};

//+---------------------------------
//	アラミタマのイラスト
//+---------------------------------
class AramitamaPicture :public BasePicture
{
public:
	AramitamaPicture();
	~AramitamaPicture() {};

	// ★かなり質の良い画像を読み込むので選択したときに画像をロードさせる
	void InitExe();

private:

};

//+---------------------------------
//	兄貴のイラスト
//+---------------------------------
class AnikiPicture :public BasePicture
{
public:
	AnikiPicture();
	~AnikiPicture() {};

	// ★かなり質の良い画像を読み込むので選択したときに画像をロードさせる
	void InitExe();

private:

};

//+---------------------------------
//	一枚絵のイラスト
//+---------------------------------
class RestPicture :public BasePicture
{
public:
	RestPicture();
	~RestPicture() {};

	// ★かなり質の良い画像を読み込むので選択したときに画像をロードさせる
	void InitExe();

private:

};

class GatheringPicture :public BasePicture
{
public:
	GatheringPicture();
	~GatheringPicture() {};

	// ★かなり質の良い画像を読み込むので選択したときに画像をロードさせる
	void InitExe();

private:

};

class StylishPicture :public BasePicture
{
public:
	StylishPicture();
	~StylishPicture() {};

	// ★かなり質の良い画像を読み込むので選択したときに画像をロードさせる
	void InitExe();

private:

};

class CoolPicture :public BasePicture
{
public:
	CoolPicture();
	~CoolPicture() {};

	// ★かなり質の良い画像を読み込むので選択したときに画像をロードさせる
	void InitExe();

private:

};

class AlleyPicture :public BasePicture
{
public:
	AlleyPicture();
	~AlleyPicture() {};

	// ★かなり質の良い画像を読み込むので選択したときに画像をロードさせる
	void InitExe();

private:

};

class SadPicture :public BasePicture
{
public:
	SadPicture();
	~SadPicture() {};

	// ★かなり質の良い画像を読み込むので選択したときに画像をロードさせる
	void InitExe();

private:

};

class SexyPicture :public BasePicture
{
public:
	SexyPicture();
	~SexyPicture() {};

	// ★かなり質の良い画像を読み込むので選択したときに画像をロードさせる
	void InitExe();

private:

};

class SeaPicture :public BasePicture
{
public:
	SeaPicture();
	~SeaPicture() {};

	// ★かなり質の良い画像を読み込むので選択したときに画像をロードさせる
	void InitExe();

private:

};


class WinTekiPicture :public BasePicture
{
public:
	WinTekiPicture();
	~WinTekiPicture() {};

	// ★かなり質の良い画像を読み込むので選択したときに画像をロードさせる
	void InitExe();

private:

};


class WinAnikiPicture :public BasePicture
{
public:
	WinAnikiPicture();
	~WinAnikiPicture() {};

	// ★かなり質の良い画像を読み込むので選択したときに画像をロードさせる
	void InitExe();

private:

};


class Manatu1Picture :public BasePicture
{
public:
	Manatu1Picture();
	~Manatu1Picture() {};

	// ★かなり質の良い画像を読み込むので選択したときに画像をロードさせる
	void InitExe();

private:

};


class SuikaPicture :public BasePicture
{
public:
	SuikaPicture();
	~SuikaPicture() {};

	// ★かなり質の良い画像を読み込むので選択したときに画像をロードさせる
	void InitExe();

private:

};


class MahuyuPicture :public BasePicture
{
public:
	MahuyuPicture();
	~MahuyuPicture() {};

	// ★かなり質の良い画像を読み込むので選択したときに画像をロードさせる
	void InitExe();

private:

};


class ToirePicture :public BasePicture
{
public:
	ToirePicture();
	~ToirePicture() {};

	// ★かなり質の良い画像を読み込むので選択したときに画像をロードさせる
	void InitExe();

private:

};

class Manatu2Picture :public BasePicture
{
public:
	Manatu2Picture();
	~Manatu2Picture() {};

	// ★かなり質の良い画像を読み込むので選択したときに画像をロードさせる
	void InitExe();

private:

};

