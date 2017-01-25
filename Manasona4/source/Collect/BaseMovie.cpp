#include "BaseMovie.h"



//+------------------------------
//	基本のコレクション用のイラスト
//+------------------------------

// アイコン用のデータの初期化
MOVIE_ICON_DATA::MOVIE_ICON_DATA()
{
	pIcon = nullptr;
	sTitle = "なし";
	sText = "なし";
	vPos = VECTOR2_ZERO;
	iAlpha = 1.0f;
}

// 画像用のデータの初期化
MOVIE_DATA::MOVIE_DATA()
{
	pMovie = nullptr;
	pPic = nullptr;
	sTitle = "なし";
	sText = "なし";

}

//+-------------------------
//	初期化・解放
//+-------------------------
BaseMovie::BaseMovie()
{

	m_bRock = false;
	m_iPrice = 9999;
}

BaseMovie::~BaseMovie()
{
	SAFE_DELETE(m_tagMovieIcon.pIcon);
}

//+-------------------------
//	更新・描画	(アイコン)
//+-------------------------
//void BaseMovie::UpdateIcon()
//{
//
//
//}

void BaseMovie::RenderIcon(int iX, int iY)
{
	// アイコン
	m_tagMovieIcon.pIcon->Render(
		(int)m_tagMovieIcon.vPos.x + iX,
		(int)m_tagMovieIcon.vPos.y + iY);

}

//	未購入版アイコン
void BaseMovie::RenderIconNonPurchase(int iX, int iY)
{
	// 暗く
	m_tagMovieIcon.pIcon->SetARGB(0xff777777);
	
	// アイコン
	m_tagMovieIcon.pIcon->Render(
		(int)m_tagMovieIcon.vPos.x + iX,
		(int)m_tagMovieIcon.vPos.y + iY);

	// 元に戻す
	m_tagMovieIcon.pIcon->SetARGB(0xffffffff);

}

void BaseMovie::RenderIconInfo(int iX, int iY)
{
	// タイトル
	tdnFont::RenderString(m_tagMovieIcon.sTitle.c_str(), "HGｺﾞｼｯｸE",
		22, iX, iY, 0xffffffff, RS::COPY);
	// 説明
	tdnFont::RenderString(m_tagMovieIcon.sText.c_str(), "HGｺﾞｼｯｸE",
		19, iX, iY + 30, 0xffffffff, RS::COPY);

}

// 純粋仮想関数へ
//void BaseMovie::InitExe()
//{
//
//}

// 画像解放
void BaseMovie::RereaseExe()
{
	SAFE_DELETE(m_tagMovie.pPic);
	SAFE_DELETE(m_tagMovie.pMovie);
}

//+-------------------------
//	更新・描画	(起動したとき)
//+-------------------------
bool BaseMovie::UpdateExe()
{
	// ムービー更新(テアリング)
	m_tagMovie.pMovie->Update();

	// ムービーが最後まで来たら
	if (m_tagMovie.pMovie->isEndPos())
	{
		// ムービー止める
		m_tagMovie.pMovie->Stop();
		
		// メニューへ
		return true;
	}

	return false;
}

void BaseMovie::CtrlExe(int iDevice)
{


}

void BaseMovie::RenderExe()
{
	// 鑑賞用
	m_tagMovie.pPic->Render(0,0);
}

void BaseMovie::RenderExeInfo(int iX, int iY)
{
	// タイトル
	tdnFont::RenderString(m_tagMovie.sTitle.c_str(), "HGｺﾞｼｯｸE",
		22, iX, iY, 0xffffffff, RS::COPY);
	// 説明
	tdnFont::RenderString(m_tagMovie.sText.c_str(), "HGｺﾞｼｯｸE",
		18, iX+8, iY + 32, 0xffffffff, RS::COPY);
}

//+---------------------------------
//	今回のＯＰ
//+---------------------------------
OpMovie::OpMovie()
{
	m_tagMovieIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Op.png");
	m_tagMovieIcon.sTitle = "【ムービー】オープニング";
	m_tagMovieIcon.sText = "ムービーです。";

	// 隠しトロフィーフラグ
	m_bRock = false;

	// 値段
	m_iPrice = 10;
}

// 実行後の画像の初期化
void OpMovie::InitExe()
{
	// 動画関連初期化
	m_tagMovie.pMovie = new tdnMovie("DATA/Title/op.wmv", true, false);
	m_tagMovie.pPic = new tdn2DObj(m_tagMovie.pMovie->GetTexture());
	m_tagMovie.sTitle = "【ムービー】オープニング";
	m_tagMovie.sText = "ジョジョの奇妙な冒険のＯＰパロ。\n友達の協力のおかげでできた渾身の一品。";

	// ムービー再生
	m_tagMovie.pMovie->Play();
}

//+-------------------------------------
// 昔のＯＰ
OldOpMovie::OldOpMovie()
{
	m_tagMovieIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/OldOp.png");
	m_tagMovieIcon.sTitle = "【ムービー】前作のオープニング";
	m_tagMovieIcon.sText = "ムービーです。";

	// 隠しトロフィーフラグ
	m_bRock = false;

	// 値段
	m_iPrice = 10;
}

// 実行後の画像の初期化
void OldOpMovie::InitExe()
{
	m_tagMovie.pMovie = new tdnMovie("DATA/Movie/oldop.wmv", true, false);
	m_tagMovie.pPic = new tdn2DObj(m_tagMovie.pMovie->GetTexture());
	m_tagMovie.sTitle = "【ムービー】前作のオープニング";
	m_tagMovie.sText = "西遊記を元に作ったＯＰ、夏休み中気合で作ったなつかしの一品。";


	// ムービー再生
	m_tagMovie.pMovie->Play();
}