#include "BaseTrophy.h"
#include "Data\PlayerData.h"
#include "Data\SelectData.h"
#include "../Sound/SoundManager.h"

//+------------------------------
//	基本のトロフィー
//+------------------------------

// トロフィーデータの初期化
TROPHY_DATA::TROPHY_DATA()
{
	pIcon = nullptr;
	sTitle = "なし";
	sText = "なし";
	vPos = VECTOR2_ZERO;
	iAlpha = 1.0f;
}

// 初期化
BaseTrophy::BaseTrophy()
{
	m_bAction = false;
	m_iActiveFrame = 0;

	m_pPlate = new tdn2DObj("Data/Trophy/Plate.png");
	m_pIconRip = new tdn2DAnim("Data/Trophy/Rip.png");
	m_pIconRip->OrderRipple(42, 1.0f, 0.02f);

	m_iDelayFrame = 0;

	m_vRoomPos = VECTOR2_ZERO;

	//m_bRock = true;
	//m_pRockIcon = new tdn2DObj("Data/Trophy/Rock.png");

	m_bHide = false;
}

BaseTrophy::~BaseTrophy()
{
	SAFE_DELETE(m_tagTrophy.pIcon);
	SAFE_DELETE(m_pPlate);
	SAFE_DELETE(m_pIconRip);
	//SAFE_DELETE(m_pRockIcon);
}

// 更新・描画
void BaseTrophy::Update()
{
	// 演出が始まっていなかったら飛ばす
	if (m_bAction == false)return;

	// ディレイがあるなら
	if (m_iDelayFrame > 0) m_iDelayFrame--;
	if (m_iDelayFrame > 0)return;

	m_iActiveFrame++;// 演出フレーム更新

	if (m_iActiveFrame >= 240)
	{
		m_bAction = false;
	}

	enum STEP
	{
		INTRO = 12,
		END = 220
	};

	// 最初
	if (m_iActiveFrame <= INTRO)
	{
		m_tagTrophy.iAlpha = (float)m_iActiveFrame / (float)INTRO;
		m_tagTrophy.vPos.x += (float)400 / (float)INTRO;
	}
	// 下がる
	else if (m_iActiveFrame >= END)
	{
		m_tagTrophy.iAlpha += 0.2f;
		m_tagTrophy.vPos.x -= 40;
	}
	else
	{
		if (m_pIconRip->GetAction()->IsEnd() == true || 
			m_pIconRip->GetAction()->IsAction() == false)
		{
			m_pIconRip->Action();
		}

	}

	// アルファ
	m_tagTrophy.iAlpha = Math::Clamp(m_tagTrophy.iAlpha, 0.0f, 1.0f);

	// 波紋
	m_pIconRip->Update();

}

void BaseTrophy::Render()
{
	// 演出が始まっていなかったら飛ばす
	if (m_bAction == false)return;

	// ディレイがあるなら
	if (m_iDelayFrame > 0)return;

	int l_iX = (int)m_tagTrophy.vPos.x;
	int l_iY = (int)m_tagTrophy.vPos.y;
	int l_iAlpha = (int)(255 * m_tagTrophy.iAlpha);
	DWORD l_dCol = ARGB(l_iAlpha, 255, 255, 255);

	// プレート
	m_pPlate->SetAlpha(l_iAlpha);
	m_pPlate->Render(l_iX, l_iY);

	// アイコン
	m_tagTrophy.pIcon->SetAlpha(l_iAlpha);
	m_tagTrophy.pIcon->Render(l_iX + 10, l_iY + 4 , 64, 64, 0, 0, 128, 128);

	// タイトル
	//tdnFont::RenderString(m_tagTrophy.sTitle.c_str(), "HGｺﾞｼｯｸE",
	//	22, l_iX + 72, l_iY + 6 , 0xffffffff, RS::COPY);
	tdnFont::RenderString("トロフィーを手に入れました！", "HGｺﾞｼｯｸE",
		22, l_iX + 82, l_iY + 6, l_dCol, RS::COPY);

	//// 文字描画
	//tdnFont::RenderString(m_tagTrophy.sText.c_str(), "HGｺﾞｼｯｸE",
	//	19, l_iX + 72, l_iY +36, 0xffffffff, RS::COPY);
	tdnFont::RenderString(m_tagTrophy.sTitle.c_str(), "HGｺﾞｼｯｸE",
		19, l_iX + 82, l_iY + 36, l_dCol, RS::COPY);

	// 波紋
	m_pIconRip->Render(l_iX + 10, l_iY + 4, 64, 64, 0, 0, 128, 128, RS::ADD);

}

// 
void BaseTrophy::Action(int iDelayFrame)
{
	m_bAction = true;
	m_iActiveFrame = 0;

	// 初期位置
	m_tagTrophy.vPos.x = -400;
	m_tagTrophy.vPos.y = 48;
	m_tagTrophy.iAlpha = 0.0f;

	// 波紋 途中から描画したいので
	m_pIconRip->Stop();

	m_iDelayFrame = iDelayFrame;

	// SEの再生
	se->Play("トロフィー");
}

// 演出を切る
void BaseTrophy::Stop()
{
	m_bAction = false;
	m_iActiveFrame = 0;

}

//	閲覧用
void BaseTrophy::RenderRoom(int iX, int iY/*, bool bRock*/)
{
	//　ロック中なら
	//if (bRock)
	//{
	//	//m_pRockIcon->SetScale(0.5f);
	//	//m_pRockIcon->Render((int)m_vRoomPos.x + iX, (int)m_vRoomPos.y + iY);
	//
	//}else
	//{
		// アイコン
		m_tagTrophy.pIcon->SetAlpha(255);
		m_tagTrophy.pIcon->Render((int)m_vRoomPos.x + iX, (int)m_vRoomPos.y + iY);
	//}

}

// タイトル・説明文字
void BaseTrophy::RenderInfo(int iX, int iY)
{
	// 隠しトロフィーなら
	//if (m_bHide)
	//{

	//}
	//else
	{	
		// タイトル
		tdnFont::RenderString(m_tagTrophy.sTitle.c_str(), "HGｺﾞｼｯｸE",
			22, iX, iY, 0xffffffff, RS::COPY);
		// 説明
		tdnFont::RenderString(m_tagTrophy.sText.c_str(), "HGｺﾞｼｯｸE",
			19, iX, iY + 30, 0xffffffff, RS::COPY);

	}


}


//+---------------------------------
//	初プレイのトロフィー
//+---------------------------------

FirstTrophy::FirstTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/first.png");
	m_tagTrophy.sTitle = "闘魂のファイト開幕";
	m_tagTrophy.sText = "初めてゲームをプレイした";

	// 隠しトロフィーフラグ
	m_bHide = false;
}

//+---------------------------------
//	初めて対戦をした
//+---------------------------------
FirstBattleTrophy::FirstBattleTrophy()
{	
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/Battle.png");
	m_tagTrophy.sTitle = "初対戦";
	m_tagTrophy.sText = "初めて対戦した。";

	// 隠しトロフィーフラグ
	m_bHide = false;
}


//+---------------------------------
//	大きいダメージを出した
//+---------------------------------
BigDamageTrophy::BigDamageTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/power.png");
	m_tagTrophy.sTitle = "強力なコンボ";
	m_tagTrophy.sText = "一回のコンボで5000ダメージ以上与えた。";

	// 隠しトロフィーフラグ
	m_bHide = false;
}


//+---------------------------------
//	最大HPで勝利した
//+---------------------------------
MaxHpFinishTrophy::MaxHpFinishTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/hart.png");
	m_tagTrophy.sTitle = "パーフェクト勝利";
	m_tagTrophy.sText = "ダメージを受けずに勝利した。";

	// 隠しトロフィーフラグ
	m_bHide = false;
}


//+---------------------------------
//	早く勝利した
//+---------------------------------
SpeedFinishTrophy::SpeedFinishTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/timer.png");
	m_tagTrophy.sTitle = "瞬殺";
	m_tagTrophy.sText = "20秒以内で勝利した。";

	// 隠しトロフィーフラグ
	m_bHide = false;
}

//+---------------------------------
//	トレーニングに連続時間こもった
//+---------------------------------
TrainingTimeTrophy::TrainingTimeTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/Plactice.png");
	m_tagTrophy.sTitle = "努力家";
	m_tagTrophy.sText = "連続で5分以上トレーニングルームで練習した。";

	// 隠しトロフィーフラグ
	m_bHide = false;
}

//+---------------------------------
//	最高難易度でCOMを倒した
//+---------------------------------
PowerfulEnemyWinTrophy::PowerfulEnemyWinTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/medal.png");
	m_tagTrophy.sTitle = "COMマスター";
	m_tagTrophy.sText = "最高難易度のCOMに勝った。";

	// 隠しトロフィーフラグ
	m_bHide = false;
}

//+---------------------------------
//	SSランクで倒した
//+---------------------------------
SSRankWinTrophy::SSRankWinTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/star.png");
	m_tagTrophy.sTitle = "完全勝利";
	m_tagTrophy.sText = "SSランクで勝利した。";

	// 隠しトロフィーフラグ
	m_bHide = false;
}

//+---------------------------------
//	最高難易度でCOM+SSランクを倒した
//+---------------------------------
PowerfulEnemySSRankWinTrophy::PowerfulEnemySSRankWinTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/starMedal.png");
	m_tagTrophy.sTitle = "闘魂を極めし者";
	m_tagTrophy.sText = "最高難易度のCOMにSSランクで勝利した。";

	// 隠しトロフィーフラグ
	m_bHide = false;
}

//+---------------------------------
//	コンテンツをたくさん解放した
//+---------------------------------
BuyManyContentTrophy::BuyManyContentTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/item.png");
	m_tagTrophy.sTitle = "お金で買えない価値がある";
	m_tagTrophy.sText = "10種類以上コンテンツを解放した。";

	// 隠しトロフィーフラグ
	m_bHide = false;
}

//+---------------------------------
//	チャレンジモードでタスクをクリア
//+---------------------------------
ChallengeTrophy::ChallengeTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/kusuri.png");
	m_tagTrophy.sTitle = "コンボマスター";
	m_tagTrophy.sText = "チャレンジモードで最高難易度のタスクをクリアした。";

	// 隠しトロフィーフラグ
	m_bHide = false;
}

//+---------------------------------
//	アッタカイボム
//+---------------------------------
AttakaiBomTrophy::AttakaiBomTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/bom.png");
	m_tagTrophy.sTitle = "大爆発エンド";
	m_tagTrophy.sText = "HPが0の相手にアッタカイの爆弾を爆発させた。";

	// 隠しトロフィーフラグ
	m_bHide = false;

}

//+---------------------------------
//	ぶんぶんで100コンボ
//+---------------------------------
AirouChallengeTrophy::AirouChallengeTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/airou.png");
	m_tagTrophy.sTitle = "おしおき";
	m_tagTrophy.sText = "ぶんぶんで100コンボした。";

	// 隠しトロフィーフラグ
	m_bHide = false;

}

//+---------------------------------
//	テキでアサルトダイブ
//+---------------------------------
TekiChallengeTrophy::TekiChallengeTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/Teki.png");
	m_tagTrophy.sTitle = "大根おろし";
	m_tagTrophy.sText = "テキでアサルトダイブを超上空から当てた。";

	// 隠しトロフィーフラグ
	m_bHide = false;

}

//+---------------------------------
//	10000ダメージ
//+---------------------------------
BigBigDamageTrophy::BigBigDamageTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/power2.png");
	m_tagTrophy.sTitle = "北斗なコンボ";
	m_tagTrophy.sText = "一回のコンボで10000ダメージ以上与えた。";

	// 隠しトロフィーフラグ
	m_bHide = false;

}



//+---------------------------------
//	全てのトロフィーを集めた
//+---------------------------------
CompleteTrophy::CompleteTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/full.png");
	m_tagTrophy.sTitle = "闘魂の覇者";
	m_tagTrophy.sText = "全てのトロフィーを入手した。";

	// 隠しトロフィーフラグ
	m_bHide = false;
}
