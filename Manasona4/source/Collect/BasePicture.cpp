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
	// セレクトキーで初期化
	if (tdnInput::KeyGet(KEYCODE::KEY_SELECT, iDevice)||
		tdnInput::KeyGet(KEYCODE::KEY_R3, iDevice))
	{
		m_tagPicture.vPos = VECTOR2_ZERO;
		m_tagPicture.fScale = 1.0f;
	}


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

//+---------------------------------
//	テキのイラスト
//+---------------------------------
TekiPicture::TekiPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Teki.png");
	m_tagPictureIcon.sTitle = "【キャラクターイラスト】テキ";
	m_tagPictureIcon.sText = "テキのキャラクターイラスト";

	// 隠しトロフィーフラグ
	m_bRock = false;

	// 値段
	m_iPrice = 10;
}

// 実行後の画像の初期化
void TekiPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Teki.png");
	m_tagPicture.sTitle = "【キャラクターイラスト】テキ";
	m_tagPicture.sText = "テキのキャラクターイラスト";
	m_tagPicture.vPos.x = -100.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}

//+---------------------------------
//	∵のイラスト
//+---------------------------------
NazenarabaPicture::NazenarabaPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Nazenaraba.png");
	m_tagPictureIcon.sTitle = "【キャラクターイラスト】(∵)";
	m_tagPictureIcon.sText = "(∵)のキャラクターイラスト";

	// 隠しトロフィーフラグ
	m_bRock = false;

	// 値段
	m_iPrice = 10;
}

// 実行後の画像の初期化
void NazenarabaPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Nazenaraba.png");
	m_tagPicture.sTitle = "【キャラクターイラスト】(∵)";
	m_tagPicture.sText = "(∵)のキャラクターイラスト";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}


//+---------------------------------
//	一枚絵のイラスト
//+---------------------------------
RestPicture::RestPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Rest.png");
	m_tagPictureIcon.sTitle = "【スペシャルイラスト】お昼ご飯";
	m_tagPictureIcon.sText = "特別なイラストです。";

	// 隠しトロフィーフラグ
	m_bRock = false;

	// 値段
	m_iPrice = 10;
}

// 実行後の画像の初期化
void RestPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Rest.png");
	m_tagPicture.sTitle = "【スペシャルイラスト】お昼ご飯";
	m_tagPicture.sText = "みんなでわいわいお昼ご飯。\nレジャーシートの上でこれから肉を焼こうとしている大事故一歩手前の瞬間。";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}

//+-------------------------------------
// 設定
GatheringPicture::GatheringPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Gathering.png");
	m_tagPictureIcon.sTitle = "【スペシャルイラスト】集合写真";
	m_tagPictureIcon.sText = "特別なイラストです。";

	// 隠しトロフィーフラグ
	m_bRock = false;

	// 値段
	m_iPrice = 10;
}

// 実行後の画像の初期化
void GatheringPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Gathering.png");
	m_tagPicture.sTitle = "【スペシャルイラスト】集合写真";
	m_tagPicture.sText = "みんな写真に写りたすぎて後ろが映っていない、わっしょいな写真。";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}

//+-------------------------------------
// 設定
StylishPicture::StylishPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Stylish.png");
	m_tagPictureIcon.sTitle = "【スペシャルイラスト】スタイリッシュ";
	m_tagPictureIcon.sText = "特別なイラストです。";

	// 隠しトロフィーフラグ
	m_bRock = false;

	// 値段
	m_iPrice = 10;
}

// 実行後の画像の初期化
void StylishPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Stylish.png");
	m_tagPicture.sTitle = "【スペシャルイラスト】スタイリッシュ";
	m_tagPicture.sText = "各々秘めている思いがある。";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}

//+-------------------------------------
// 設定
CoolPicture::CoolPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Cool.png");
	m_tagPictureIcon.sTitle = "【スペシャルイラスト】攻殻機動隊っぽく立ってみた";
	m_tagPictureIcon.sText = "特別なイラストです。";

	// 隠しトロフィーフラグ
	m_bRock = false;

	// 値段
	m_iPrice = 10;
}

// 実行後の画像の初期化
void CoolPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Cool.png");
	m_tagPicture.sTitle = "【スペシャルイラスト】攻殻機動隊っぽく立ってみた";
	m_tagPicture.sText = "イケメン";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}

//+-------------------------------------
// 設定
AlleyPicture::AlleyPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Alley.png");
	m_tagPictureIcon.sTitle = "【スペシャルイラスト】路地裏";
	m_tagPictureIcon.sText = "特別なイラストです。";

	// 隠しトロフィーフラグ
	m_bRock = false;

	// 値段
	m_iPrice = 10;
}

// 実行後の画像の初期化
void AlleyPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Alley.png");
	m_tagPicture.sTitle = "【スペシャルイラスト】路地裏";
	m_tagPicture.sText = "アイル―は平然と裏切る。";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}

//+-------------------------------------
// 設定
SadPicture::SadPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Sad.png");
	m_tagPictureIcon.sTitle = "【スペシャルイラスト】雨";
	m_tagPictureIcon.sText = "特別なイラストです。";

	// 隠しトロフィーフラグ
	m_bRock = false;

	// 値段
	m_iPrice = 10;
}

// 実行後の画像の初期化
void SadPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Sad.png");
	m_tagPicture.sTitle = "【スペシャルイラスト】雨";
	m_tagPicture.sText = "かなしそう。";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}

//+-------------------------------------
// 設定
SexyPicture::SexyPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Sexy.png");
	m_tagPictureIcon.sTitle = "【スペシャルイラスト】目覚めし者";
	m_tagPictureIcon.sText = "特別なイラストです。";

	// 隠しトロフィーフラグ
	m_bRock = false;

	// 値段
	m_iPrice = 10;
}

// 実行後の画像の初期化
void SexyPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Sexy.png");
	m_tagPicture.sTitle = "【スペシャルイラスト】目覚めし者";
	m_tagPicture.sText = "セクシーに愛されたアラミタマ";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}

//+-------------------------------------
// 設定
SeaPicture::SeaPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Sea.png");
	m_tagPictureIcon.sTitle = "【スペシャルイラスト】海";
	m_tagPictureIcon.sText = "特別なイラストです。";

	// 隠しトロフィーフラグ
	m_bRock = false;

	// 値段
	m_iPrice = 10;
}

// 実行後の画像の初期化
void SeaPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Sea.png");
	m_tagPicture.sTitle = "【スペシャルイラスト】海";
	m_tagPicture.sText = "奈良県には海がない。";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}
