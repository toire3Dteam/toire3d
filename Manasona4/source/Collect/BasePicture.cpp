#include "BasePicture.h"



//+------------------------------
//	基本のコレクション用のイラスト
//+------------------------------

// アイコン用のデータの初期化
PICTURE_ICON_DATA::PICTURE_ICON_DATA()
{
	pIcon = nullptr;
	sTitle = "なし";
	sText = "なし";
	vPos = VECTOR2_ZERO;
	iAlpha = 1.0f;
}

// 画像用のデータの初期化
PICTURE_DATA::PICTURE_DATA()
{
	pPic = nullptr;
	sTitle = "なし";
	sText = "なし";
	vPos = VECTOR2_ZERO;
	fScale = 1.0f;
	iAlpha = 1.0f;

}

//+-------------------------
//	初期化・解放
//+-------------------------
BasePicture::BasePicture()
{

	m_bRock = false;
	m_iPrice = 9999;
}

BasePicture::~BasePicture()
{
	SAFE_DELETE(m_tagPictureIcon.pIcon);
}

//+-------------------------
//	更新・描画	(アイコン)
//+-------------------------
//void BasePicture::UpdateIcon()
//{
//
//
//}

void BasePicture::RenderIcon(int iX, int iY)
{
	// アイコン
	m_tagPictureIcon.pIcon->Render(
		(int)m_tagPictureIcon.vPos.x + iX,
		(int)m_tagPictureIcon.vPos.y + iY);

}

//	未購入版アイコン
void BasePicture::RenderIconNonPurchase(int iX, int iY)
{
	// 暗く
	m_tagPictureIcon.pIcon->SetARGB(0xff777777);
	
	// アイコン
	m_tagPictureIcon.pIcon->Render(
		(int)m_tagPictureIcon.vPos.x + iX,
		(int)m_tagPictureIcon.vPos.y + iY);

	// 元に戻す
	m_tagPictureIcon.pIcon->SetARGB(0xffffffff);

}

void BasePicture::RenderIconInfo(int iX, int iY)
{
	// タイトル
	tdnFont::RenderString(m_tagPictureIcon.sTitle.c_str(), "HGｺﾞｼｯｸE",
		22, iX, iY, 0xffffffff, RS::COPY);
	// 説明
	tdnFont::RenderString(m_tagPictureIcon.sText.c_str(), "HGｺﾞｼｯｸE",
		19, iX, iY + 30, 0xffffffff, RS::COPY);

}

//void BasePicture::InitExe()
//{
//
//}

// 画像解放
void BasePicture::RereaseExe()
{
	SAFE_DELETE(m_tagPicture.pPic);
}

//+-------------------------
//	更新・描画	(起動したとき)
//+-------------------------
void BasePicture::UpdateExe()
{

}

void BasePicture::CtrlExe(int iDevice)
{


	// スティック誤差用
	static const float l_fAbj(0.001f);

	// 左スティックの取得
	float l_iLX, l_iLY;
	tdnInput::GetAxisXYf(&l_iLX, &l_iLY, iDevice);

	if (l_iLX > 0.05f - l_fAbj)
	{
		m_tagPicture.vPos.x += l_iLX * 5;
	}
	else if (l_iLX < -0.05f + l_fAbj)
	{
		m_tagPicture.vPos.x += l_iLX * 5;
	}
	if (l_iLY > 0.05f - l_fAbj)
	{
		m_tagPicture.vPos.y += l_iLY * 5;
	}
	else if (l_iLY < -0.05f + l_fAbj)
	{
		m_tagPicture.vPos.y += l_iLY * 5;
	}

	// 十字キーで移動
	if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, iDevice))
	{
		m_tagPicture.vPos.x += 5;
	}
	else if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, iDevice))
	{
		m_tagPicture.vPos.x -= 5;
	}
	if (tdnInput::KeyGet(KEYCODE::KEY_UP, iDevice))
	{
		m_tagPicture.vPos.y -= 5;
	}
	else if (tdnInput::KeyGet(KEYCODE::KEY_DOWN, iDevice))
	{
		m_tagPicture.vPos.y += 5;
	}


	// 拡大
	// 右スティックの取得
	float l_iRX, l_iRY;
	tdnInput::GetAxisXY2f(&l_iRX, &l_iRY, iDevice);

	if (l_iRY > 0.05f - l_fAbj)
	{
		m_tagPicture.fScale -= 0.01f;
	}
	else if (l_iRY < -0.05f + l_fAbj)
	{
		m_tagPicture.fScale += 0.01f;
	}

	// スケールがマイナスや拡大しすぎないようにクランプ
	m_tagPicture.fScale = Math::Clamp(m_tagPicture.fScale, 0.5f, 1.5f);

}

void BasePicture::RenderExe()
{
	// 画像の設定
	m_tagPicture.pPic->SetScale(m_tagPicture.fScale);

	// 鑑賞用
	m_tagPicture.pPic->Render(
		(int)m_tagPicture.vPos.x,
		(int)m_tagPicture.vPos.y);
}

void BasePicture::RenderExeInfo(int iX, int iY)
{
	// タイトル
	tdnFont::RenderString(m_tagPicture.sTitle.c_str(), "HGｺﾞｼｯｸE",
		22, iX, iY, 0xffffffff, RS::COPY);
	// 説明
	tdnFont::RenderString(m_tagPicture.sText.c_str(), "HGｺﾞｼｯｸE",
		18, iX+8, iY + 32, 0xffffffff, RS::COPY);
}


//+---------------------------------
//	アイル―のイラスト
//+---------------------------------
AirouPicture::AirouPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Airou.png");
	m_tagPictureIcon.sTitle = "【キャラクターイラスト】アイル―";
	m_tagPictureIcon.sText = "アイル―のキャラクターイラスト";

	// 隠しトロフィーフラグ
	m_bRock = false;

	// 値段
	m_iPrice = 10;
}

// 実行後の画像の初期化
void AirouPicture::InitExe()
{
	m_tagPicture.pPic=new tdn2DObj("Data/UI/Collect/Picture/Airou.png");
	m_tagPicture.sTitle = "【キャラクターイラスト】アイル―";
	m_tagPicture.sText = "アイル―のキャラクターイラスト";
	m_tagPicture.vPos.x = -300.0f;
	m_tagPicture.vPos.y = -200.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}
