#include "MenuIcon.h"
#include "system\System.h"

//+-------------------------
//	ベースメニューアイコン
//+-------------------------

// コンストラクタ・デストラクタ
BaseMenuIcon::BaseMenuIcon()
{
	//m_pFont = nullptr;			// 文字画像
	m_pIcon = nullptr;
	//m_pIcon = new tdn2DAnim("Data/UI/Menu/Icon/Icon.png");// アイコン画像
	//m_pIcon->OrderNone();
	m_fScale = 1.0f;
	m_fNextScale = 1.0f;

	m_vPos = VECTOR2_ZERO;		// 現在の座標
	m_vNextPos = VECTOR2_ZERO;	// 未来の座標

	//m_vPrevPos = VECTOR2_ZERO;	// 過去の座標
	//m_vCurrPos = VECTOR2_ZERO;	// 現在の座標
	//m_fMoveRate = 0.0f;			// 過去・現在の移動の補間の値
	//m_fMovePower = 0.1f;

	m_bSelectFlag = false;

	m_pInfo = nullptr;
	m_vInfoPos.x = 0.0f;
	m_vInfoPos.y = 592.0f;
}

BaseMenuIcon::~BaseMenuIcon()
{
	//SAFE_DELETE(m_pFont);
	SAFE_DELETE(m_pIcon);
	SAFE_DELETE(m_pInfo);
}

// 更新・描画
void BaseMenuIcon::Update()
{
	// 場所補間の更新
	//m_fMoveRate += m_fMovePower;
	//m_fMoveRate = Math::Clamp(m_fMoveRate, 0.0f, 1.0f);
	m_vPos = m_vNextPos * 0.3f + m_vPos * 0.7f;

	// 0.1以下の誤差は無くす処理
	if (0.9f >= abs(m_vPos.x - m_vNextPos.x) &&
		0.9f >= abs(m_vPos.y - m_vNextPos.y))
	{
		m_vPos = m_vNextPos;
	}

	// スケールの更新
	m_fScale = m_fNextScale* 0.3f + m_fScale* 0.7f;
	// 誤差になれば合わせる処理
	if (0.00000001f >= abs(m_fScale - m_fNextScale))m_fScale = m_fNextScale;

	//m_pFont->SetScale(m_fScale);
	m_pIcon->SetScale(m_fScale);
	//m_pFont->Update();
	m_pIcon->Update();

	m_pInfo->Update();

}

void BaseMenuIcon::Render(int addY)
{
	// 補間した場所に描画
	m_pIcon->Render((int)m_vPos.x, (int)m_vPos.y+ addY, 512, 512, m_bSelectFlag * 512, 0, 512, 512);
	//m_pFont->Render((int)m_vPos.x, (int)m_vPos.y);

	m_pInfo->Render((int)m_vInfoPos.x, (int)m_vInfoPos.y);

}

//void BaseMenuIcon::RenderGray()
//{
//	// 補間した場所に描画
//	m_pIcon->Render((int)m_vPos.x, (int)m_vPos.y, 512, 512,
//		m_bSelectFlag * 512, 0, 512, 512,shader2D,"Gray");
//	//m_pFont->Render((int)m_vPos.x, (int)m_vPos.y);
//
//	m_pInfo->Render((int)m_vInfoPos.x, (int)m_vInfoPos.y);
//}

void BaseMenuIcon::Action(int delayFrame)
{
	//m_pFont->Action(delayFrame);
	m_pIcon->OrderMoveAppeared(12, (int)m_vPos.x, (int)m_vPos.y + 512);
	m_pIcon->Action(delayFrame);

}


//+-------------------------
//	チュートリアルアイコン
//+-------------------------

TutorialIcon::TutorialIcon()
{
	m_pIcon = new tdn2DAnim("Data/UI/Menu/Icon/TutorialIcon.png");	// アイコン画像

	m_pInfo = new tdn2DAnim("Data/UI/Menu/Info/TutorialInfo.png");
	m_pInfo->OrderMoveAppeared(12, (int)m_vInfoPos.x + 128 , (int)m_vInfoPos.y);


}

TutorialIcon::~TutorialIcon()
{
}

void TutorialIcon::Update()
{
	BaseMenuIcon::Update();

}

void TutorialIcon::Render()
{
	BaseMenuIcon::Render();
	
}

//+-------------------------
//	バトルアイコン
//+-------------------------

BattleIcon::BattleIcon()
{
	m_pIcon = new tdn2DAnim("Data/UI/Menu/Icon/battleIcon.png");	// アイコン画像
	m_pIcon->OrderMoveAppeared(12, (int)m_vPos.x, (int)m_vPos.y + 128);

	m_pInfo = new tdn2DAnim("Data/UI/Menu/Info/battleInfo.png");
	m_pInfo->OrderMoveAppeared(12, (int)m_vInfoPos.x + 128, (int)m_vInfoPos.y);

}

BattleIcon::~BattleIcon()
{
}

void BattleIcon::Update()
{
	BaseMenuIcon::Update();
}

void BattleIcon::Render()
{
	BaseMenuIcon::Render();
}


//+-------------------------
//	トレーニングアイコン
//+-------------------------

TrainingIcon::TrainingIcon()
{
	m_pIcon = new tdn2DAnim("Data/UI/Menu/Icon/TrainingIcon.png");	// アイコン画像
	m_pIcon->OrderMoveAppeared(12, (int)m_vPos.x, (int)m_vPos.y + 128);

	m_pInfo = new tdn2DAnim("Data/UI/Menu/Info/TrainingInfo.png");
	m_pInfo->OrderMoveAppeared(12, (int)m_vInfoPos.x + 128, (int)m_vInfoPos.y);


}

TrainingIcon::~TrainingIcon()
{
}

void TrainingIcon::Update()
{
	BaseMenuIcon::Update();
}

void TrainingIcon::Render()
{
	BaseMenuIcon::Render();
}


//+-------------------------
//	コレクションアイコン
//+-------------------------

CollectIcon::CollectIcon()
{
	m_pIcon = new tdn2DAnim("Data/UI/Menu/Icon/CollectIcon.png");	// アイコン画像
	m_pIcon->OrderMoveAppeared(12, (int)m_vPos.x, (int)m_vPos.y + 128);

	m_pInfo = new tdn2DAnim("Data/UI/Menu/Info/CollectInfo.png");
	m_pInfo->OrderMoveAppeared(12, (int)m_vInfoPos.x + 128, (int)m_vInfoPos.y);


}

CollectIcon::~CollectIcon()
{
}

void CollectIcon::Update()
{
	BaseMenuIcon::Update();
}

void CollectIcon::Render()
{
	BaseMenuIcon::Render();
}


//+-------------------------
//	オプションアイコン
//+-------------------------

OptionIcon::OptionIcon()
{
	m_pIcon = new tdn2DAnim("Data/UI/Menu/Icon/OptionIcon.png");	// アイコン画像
	m_pIcon->OrderMoveAppeared(12, (int)m_vPos.x, (int)m_vPos.y + 128);

	m_pInfo = new tdn2DAnim("Data/UI/Menu/Info/OptionInfo.png");
	m_pInfo->OrderMoveAppeared(12, (int)m_vInfoPos.x + 128, (int)m_vInfoPos.y);


}

OptionIcon::~OptionIcon()
{
}

void OptionIcon::Update()
{
	BaseMenuIcon::Update();
}

void OptionIcon::Render()
{
	BaseMenuIcon::Render();
}
