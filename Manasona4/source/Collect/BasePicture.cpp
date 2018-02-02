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
//	ぶんぶんのイラスト
//+---------------------------------
AirouPicture::AirouPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Airou.png");
	m_tagPictureIcon.sTitle = "【キャラクターイラスト】ぶんぶん";
	m_tagPictureIcon.sText = "ぶんぶんのキャラクターイラスト";

	// 隠しトロフィーフラグ
	m_bRock = false;

	// 値段
	m_iPrice = 1000;
}

// 実行後の画像の初期化
void AirouPicture::InitExe()
{
	m_tagPicture.pPic=new tdn2DObj("Data/UI/Collect/Picture/Airou.png");
	m_tagPicture.sTitle = "【キャラクターイラスト】ぶんぶん";
	m_tagPicture.sText = "ぶんぶんのキャラクターイラスト";
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
	m_iPrice = 1000;
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
	m_iPrice = 1000;
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
	m_iPrice = 1000;
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
	m_iPrice = 1000;
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
	m_iPrice = 1000;
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
	m_iPrice = 1000;
}

// 実行後の画像の初期化
void CoolPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Cool.png");
	m_tagPicture.sTitle = "【スペシャルイラスト】攻殻機動隊っぽく立ってみた";
	m_tagPicture.sText = "ハイカラですね。";
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
	m_iPrice = 1000;
}

// 実行後の画像の初期化
void AlleyPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Alley.png");
	m_tagPicture.sTitle = "【スペシャルイラスト】路地裏";
	m_tagPicture.sText = "ぶんぶんは平然と裏切る。";
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
	m_iPrice = 1000;
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
	m_iPrice = 1000;
}

// 実行後の画像の初期化
void SexyPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Sexy.png");
	m_tagPicture.sTitle = "【スペシャルイラスト】目覚めし者";
	m_tagPicture.sText = "セクシーに愛された封印されし魂";
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
	m_iPrice = 1000;
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


//+---------------------------------
//	封印されし魂のイラスト
//+---------------------------------
AramitamaPicture::AramitamaPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Aramitama.png");
	m_tagPictureIcon.sTitle = "【キャラクターイラスト】封印されし魂";
	m_tagPictureIcon.sText = "封印されし魂のキャラクターイラスト";

	// 隠しトロフィーフラグ
	m_bRock = false;

	// 値段
	m_iPrice = 1000;
}

void AramitamaPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Aramitama.png");
	m_tagPicture.sTitle = "【キャラクターイラスト】封印されし魂";
	m_tagPicture.sText = "封印されし魂のキャラクターイラスト";
	m_tagPicture.vPos.x = -100.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}


//+---------------------------------
//	兄貴のイラスト
//+---------------------------------
AnikiPicture::AnikiPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Aniki.png");
	m_tagPictureIcon.sTitle = "【キャラクターイラスト】兄貴";
	m_tagPictureIcon.sText = "兄貴のキャラクターイラスト";

	// 隠しトロフィーフラグ
	m_bRock = false;

	// 値段
	m_iPrice = 1000;

}

void AnikiPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Aniki.png");
	m_tagPicture.sTitle = "【キャラクターイラスト】兄貴";
	m_tagPicture.sText = "兄貴のキャラクターイラスト";
	m_tagPicture.vPos.x = -200.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;

}

//+---------------------------------
//	リザルトテキのイラスト
//+---------------------------------
WinTekiPicture::WinTekiPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/WinTeki.png");
	m_tagPictureIcon.sTitle = "【リザルトイラスト】テキ";
	m_tagPictureIcon.sText = "リザルト画面のイラストです。";

	// 隠しトロフィーフラグ
	m_bRock = false;

	// 値段
	m_iPrice = 1000;

}

void WinTekiPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/WinTeki.png");
	m_tagPicture.sTitle = "【リザルトイラスト】テキ";
	m_tagPicture.sText = "リング場で相手をKOさせたイメージ。";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}

//+---------------------------------
//	リザルト兄貴のイラスト
//+---------------------------------
WinAnikiPicture::WinAnikiPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/WinAniki.png");
	m_tagPictureIcon.sTitle = "【リザルトイラスト】兄貴";
	m_tagPictureIcon.sText = "リザルト画面のイラストです。";

	// 隠しトロフィーフラグ
	m_bRock = false;

	// 値段
	m_iPrice = 1000;
}

void WinAnikiPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/WinAniki.png");
	m_tagPicture.sTitle = "【リザルトイラスト】兄貴";
	m_tagPicture.sText = "歪みねぇ一枚絵。";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}

//+---------------------------------
//	闘魂2のイラスト
//+---------------------------------
Manatu2Picture::Manatu2Picture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Manatu2.png");
	m_tagPictureIcon.sTitle = "【ゲームイラスト】闘魂のファイト";
	m_tagPictureIcon.sText = "ゲームのイラストです。";

	// 隠しトロフィーフラグ
	m_bRock = false;

	// 値段
	m_iPrice = 1000;
}

void Manatu2Picture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Manatu2.png");
	m_tagPicture.sTitle = "【ゲームイラスト】闘魂のファイト";
	m_tagPicture.sText = "3D化したわっしょいたち\n";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}

//+---------------------------------
//	トイレのイラスト
//+---------------------------------
ToirePicture::ToirePicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Toire.png");
	m_tagPictureIcon.sTitle = "【ゲームイラスト】トイレアクション";
	m_tagPictureIcon.sText = "ゲームのイラストです。";

	// 隠しトロフィーフラグ
	m_bRock = false;

	// 値段
	m_iPrice = 1000;
}

void ToirePicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Toire.png");
	m_tagPicture.sTitle = "【ゲームイラスト】トイレアクション";
	m_tagPicture.sText = "2年生の時に制作した作品\n2人以上トイレに駆け込むと大爆発が起きる、\n";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}

//+---------------------------------
//	真冬のイラスト
//+---------------------------------
MahuyuPicture::MahuyuPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Mahuyu.png");
	m_tagPictureIcon.sTitle = "【ゲームイラスト】真冬の夜の";
	m_tagPictureIcon.sText = "ゲームのイラストです。";

	// 隠しトロフィーフラグ
	m_bRock = false;

	// 値段
	m_iPrice = 1000;
}

void MahuyuPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Mahuyu.png");
	m_tagPicture.sTitle = "【ゲームイラスト】真冬の夜の";
	m_tagPicture.sText = "弾幕\n";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}

//+---------------------------------
//	スイカのイラスト
//+---------------------------------
SuikaPicture::SuikaPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Suika.png");
	m_tagPictureIcon.sTitle = "【ゲームイラスト】スイカ";
	m_tagPictureIcon.sText = "ゲームのイラストです。";

	// 隠しトロフィーフラグ
	m_bRock = false;

	// 値段
	m_iPrice = 1000;
}

void SuikaPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Suika.png");
	m_tagPicture.sTitle = "【ゲームイラスト】スイカ";
	m_tagPicture.sText = "1年生の時に制作した作品\nスイカを相手に投げつける対戦ゲーム";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}

//+---------------------------------
//	闘魂1のイラスト
//+---------------------------------
Manatu1Picture::Manatu1Picture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Manatu1.png");
	m_tagPictureIcon.sTitle = "【ゲームイラスト】最初に作ったゲーム";
	m_tagPictureIcon.sText = "ゲームのイラストです。";

	// 隠しトロフィーフラグ
	m_bRock = false;

	// 値段
	m_iPrice = 1000;
}

void Manatu1Picture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Manatu1.png");
	m_tagPicture.sTitle = "【ゲームイラスト】最初に作ったゲーム";
	m_tagPicture.sText = "頑張って作った記憶が思い出がよみがえる、懐かしの作品。";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}
