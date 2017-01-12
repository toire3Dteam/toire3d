#include "SelectUI.h"
#include "SelectUIState.h"


static int adjustY = +32;//24
static float g_scale = 0.9f;


SelectUI::SelectUI(SIDE side, ENTITY_ID id, int DeviceID, bool bAI) :BaseGameEntity(id)
{
	// 遅延タイマー
	m_iWaitFrame = 0;

	// AI
	m_bAI = bAI;

	// ★UIが右か左サイドなのかを受け取る　演出が変わるの先に取得
	m_eSide = side;

	// ★操作するコントローラーのデバイス
	m_iDeviceID = DeviceID;

	// キャラの選択番号初期化
	m_iSelectCharacterNo = 0;
	m_iSelectPartnerNo = 0;

	// キャラクターの画像
	for (int i = 0; i < (int)CHARACTER::END; i++)
	{
		switch ((CHARACTER)i)
		{
		case CHARACTER::AIROU:
			m_tagCharaPic[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Character/airou.png");
			m_tagCharaPicRip[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Character/airou.png");
			m_tagCharaInfo[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Info/airou.png");
			m_tagCharaName[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/name/airou.png");
			m_pCharaIconRip[i] = new tdn2DAnim("Data/UI/CharacterSelect/icon/airou.png");

			break;
		case CHARACTER::TEKI:
			m_tagCharaPic[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Character/teki.png");
			m_tagCharaPicRip[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Character/teki.png");
			m_tagCharaInfo[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Info/teki.png");
			m_tagCharaName[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/name/teki.png");
			m_pCharaIconRip[i] = new tdn2DAnim("Data/UI/CharacterSelect/icon/teki.png");

			break;
		case CHARACTER::NAZENARA:
			m_tagCharaPic[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Character/nazenaraba.png");
			m_tagCharaPicRip[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Character/nazenaraba.png");
			m_tagCharaInfo[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Info/nazenaraba.png");
			m_tagCharaName[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/name/nazenaraba.png");
			m_pCharaIconRip[i] = new tdn2DAnim("Data/UI/CharacterSelect/icon/nazenaraba.png");

			break;

		case CHARACTER::ARAMITAMA:
			m_tagCharaPic[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Character/nazenaraba.png");
			m_tagCharaPicRip[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Character/nazenaraba.png");
			m_tagCharaInfo[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Info/nazenaraba.png");
			m_tagCharaName[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/name/nazenaraba.png");
			m_pCharaIconRip[i] = new tdn2DAnim("Data/UI/CharacterSelect/icon/nazenaraba.png");

			break;
		default:
			MyAssert(0, "そんなデータはない");
			break;
		}

		// 仮
		m_tagCharaName[i].pPic->SetScale(g_scale);
		//m_tagCharaPic[i].pPic->SetScale(g_scale);
	}
	InitCharcterPos();

	// パートナーの画像
	for (int i = 0; i < (int)PARTNER::END; i++)
	{
		switch ((PARTNER)i)
		{
		case PARTNER::MOKOI:
			m_tagPartnerPic[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Partner/Mokoi.png");
			m_tagPartnerPicRip[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Partner/Mokoi.png");
			m_tagPartnerInfo[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Info/mokoi.png");
			m_tagPartnerName[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Name/mokoi.png");
			m_tagPartneSirclePic[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Partner/MokoiSircle.png");

			break;
		case PARTNER::MAYA:
			m_tagPartnerPic[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Partner/Maya.png");
			m_tagPartnerPicRip[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Partner/Maya.png");
			m_tagPartnerInfo[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Info/maya.png");
			m_tagPartnerName[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Name/maya.png");
			m_tagPartneSirclePic[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Partner/MayaSircle.png");

			break;
		default:
			MyAssert(0, "そんなデータはない");
			break;
		}

		// 仮
		m_tagPartnerName[i].pPic->SetScale(g_scale);
	}

	InitPartnerPos();
	
	// その他
	m_tagInfoBox.pPic = new tdn2DAnim("DATA/UI/CharacterSelect/Info/InfoBox.png");
	
	m_tagNamePlate.pPic = new tdn2DAnim("DATA/UI/CharacterSelect/name/namePlate.png");
	m_tagAndPlate.pPic = new tdn2DAnim("DATA/UI/CharacterSelect/name/andPlate.png");
	m_tagPartnerPlate.pPic = new tdn2DAnim("DATA/UI/CharacterSelect/name/PartnerPlate.png");
	m_tagPartnerSirclePlate.pPic = new tdn2DAnim("DATA/UI/CharacterSelect/Partner/PartnerSircleFont.png");
	m_fPartnerPlateAngle = 0.0f;
	
	// 仮
	m_tagNamePlate.pPic->SetScale(g_scale);
	m_tagAndPlate.pPic->SetScale(g_scale);
	m_tagPartnerPlate.pPic->SetScale(g_scale);
	


	m_tagInfoBox.iY = 354 + adjustY - 16;
	m_tagNamePlate.iY = 200 + adjustY;
	m_tagAndPlate.iY = 200 + adjustY;
	m_tagPartnerPlate.iY = 200 + adjustY;
	m_tagPartnerSirclePlate.iY = 38 + adjustY;

	if (m_eSide == SIDE::LEFT)
	{
		m_tagInfoBox.iX = 0;

		m_tagNamePlate.iX = 0;

		m_tagAndPlate.iX = 0;

		m_tagPartnerPlate.iX = 0;

		m_tagPartnerSirclePlate.iX = 315 ;
	

		m_tagInfoBox.pPic->OrderMoveAppeared(14, m_tagInfoBox.iX - 384, m_tagInfoBox.iY);

		m_tagNamePlate.pPic->OrderMoveAppeared(14, m_tagNamePlate.iX -384, m_tagNamePlate.iY);
		m_tagAndPlate.pPic->OrderMoveAppeared(8, m_tagAndPlate.iX -384, m_tagAndPlate.iY);
		m_tagPartnerPlate.pPic->OrderMoveAppeared(8, m_tagPartnerPlate.iX -384, m_tagPartnerPlate.iY);
		m_tagPartnerSirclePlate.pPic->OrderMoveAppeared(12, m_tagPartnerSirclePlate.iX - 512, m_tagPartnerSirclePlate.iY);
	}
	else
	{
		m_tagInfoBox.iX = 899;

		m_tagNamePlate.iX = 890;

		m_tagAndPlate.iX = 890;

		m_tagPartnerPlate.iX = 890;

		m_tagPartnerSirclePlate.iX = 709 ;

		m_tagInfoBox.pPic->OrderMoveAppeared(14, m_tagInfoBox.iX + 384, m_tagInfoBox.iY);

		m_tagNamePlate.pPic->OrderMoveAppeared(14, m_tagNamePlate.iX + 384, m_tagNamePlate.iY);
		m_tagAndPlate.pPic->OrderMoveAppeared(8, m_tagAndPlate.iX + 384, m_tagAndPlate.iY);
		m_tagPartnerPlate.pPic->OrderMoveAppeared(8, m_tagPartnerPlate.iX + 384, m_tagPartnerPlate.iY);
		m_tagPartnerSirclePlate.pPic->OrderMoveAppeared(12, m_tagPartnerSirclePlate.iX + 512, m_tagPartnerSirclePlate.iY);

	}

	// 選択カーソル
	if (m_bAI == true)
	{
		m_pSelect.pPic = new tdn2DAnim("Data/UI/CharacterSelect/Icon/selectcpu.png");
	}
	else if (m_eSide == SIDE::LEFT)
	{
		m_pSelect.pPic = new tdn2DAnim("Data/UI/CharacterSelect/Icon/select1p.png");
		
	}else 
	{
		m_pSelect.pPic = new tdn2DAnim("Data/UI/CharacterSelect/Icon/select2p.png");
	}
	m_pSelect.pPic->OrderShake(INT_MAX - 1 , 0, 8, 8);
	m_pSelect.pPic->Action();
	m_pSelect.iX = 0;
	m_pSelect.iY = 0;

	// OKの文字
	m_tagOKfont.pPic = new tdn2DAnim("Data/UI/CharacterSelect/Info/OK.png");
	m_tagOKfont.pPic->OrderGrow(14, 0.0f, 1.0f / 14.0f);
	m_tagOKfont.iY = 354 + adjustY - 16;
	if (m_eSide == SIDE::LEFT)
	{
		m_tagOKfont.iX = 0;
	}
	else
	{
		m_tagOKfont.iX = 899;
	}

	m_bOK = false;

	/* ステートマシン初期化 */
	m_pStateMachine = new StateMachine<SelectUI>(this);
	m_pStateMachine->SetCurrentState(SelectUIState::Intro::GetInstance());	// ステートの設定


}

void SelectUI::InitCharcterPos()
{
	// キャラクターの画像
	for (int i = 0; i < (int)CHARACTER::END; i++)
	{

		m_tagCharaPic[i].iY = -96;
		m_tagCharaPicRip[i].iY = -96;
		m_tagCharaInfo[i].iY = 354 + adjustY - 16;
		m_tagCharaName[i].iY = 200 + adjustY;

		// ★[パートナー]左サイドだったら
		if (m_eSide == SIDE::LEFT)
		{
			// ポジション設定
			// キャラ絵
			m_tagCharaPic[i].iX = 0;

			// キャラ絵波紋
			m_tagCharaPicRip[i].iX = 0;

			// キャラの説明絵
			m_tagCharaInfo[i].iX = 0;

			// キャラの名前
			m_tagCharaName[i].iX = 0;

			// アニメーション
			// 立ち絵
			m_tagCharaPic[i].pPic->OrderMoveAppeared(12, m_tagCharaPic[i].iX - 384, m_tagCharaPic[i].iY);
		}
		else // 右サイドだったら 
		{

			// ポジション設定
			// キャラ絵
			m_tagCharaPic[i].iX = 899-64;
			m_tagCharaPic[i].pPic->SetTurnOver(true);

			// キャラ絵波紋
			m_tagCharaPicRip[i].iX = 899 - 64;
			m_tagCharaPicRip[i].pPic->SetTurnOver(true);
			
			// キャラの説明絵
			m_tagCharaInfo[i].iX = 899;

			// キャラの名前
			m_tagCharaName[i].iX = 890;

			// アニメーション
			// 立ち絵
			m_tagCharaPic[i].pPic->OrderMoveAppeared(12, m_tagCharaPic[i].iX + 384, m_tagCharaPic[i].iY);

		}

		// 共通アニメ
		// 波紋
		m_tagCharaPicRip[i].pPic->OrderRipple(14, 1.0f, 0.01f);

		// 説明
		m_tagCharaInfo[i].pPic->OrderMoveAppeared(12, m_tagCharaInfo[i].iX + 384 / 12, m_tagCharaInfo[i].iY);

		// 名前
		m_tagCharaName[i].pPic->OrderMoveAppeared(8, m_tagCharaName[i].iX - 100, m_tagCharaName[i].iY + 10);

		// アイコンの波紋用
		m_pCharaIconRip[i]->OrderRipple(14, 1.0f, 0.02f);
	}
}

void SelectUI::InitPartnerPos()
{
	// パートナーの画像
	for (int i = 0; i < (int)PARTNER::END; i++)
	{


		m_tagPartnerPic[i].iY = 265 + adjustY;
		m_tagPartnerPicRip[i].iY = 265 + adjustY;
		m_tagPartnerInfo[i].iY = 354 + adjustY - 16;
		m_tagPartnerName[i].iY = 200 + adjustY;
		m_tagPartneSirclePic[i].iY = 38 + adjustY;

		// ★[パートナー]アニメーション
		// 左サイドだったら
		if (m_eSide == SIDE::LEFT)
		{
			m_tagPartnerPic[i].iX = 127;

			m_tagPartnerPicRip[i].iX = 127;

			m_tagPartnerInfo[i].iX = 0;

			m_tagPartnerName[i].iX = 0;
	
			m_tagPartneSirclePic[i].iX = 315;

			m_tagPartneSirclePic[i].pPic->OrderMoveAppeared(12, m_tagPartneSirclePic[i].iX - 512, m_tagPartneSirclePic[i].iY);

		}
		else // 右サイドだったら 
		{
			m_tagPartnerPic[i].iX = 1024;
			
			m_tagPartnerPicRip[i].iX = 1024;

			m_tagPartnerInfo[i].iX = 899;

			m_tagPartnerName[i].iX = 890;

			m_tagPartneSirclePic[i].iX = 709;

			m_tagPartneSirclePic[i].pPic->OrderMoveAppeared(12, m_tagPartneSirclePic[i].iX + 512, m_tagPartneSirclePic[i].iY);

		}

		// 共通アニメーション
		m_tagPartnerPic[i].pPic->OrderMoveAppeared(8, m_tagPartnerPic[i].iX - 32, m_tagPartnerPic[i].iY);

		// 波紋
		m_tagPartnerPicRip[i].pPic->OrderRipple(14, 1.0f, 0.015f);

		// 説明
		m_tagPartnerInfo[i].pPic->OrderMoveAppeared(12, m_tagPartnerInfo[i].iX + 384 / 12, m_tagPartnerInfo[i].iY);

		// 名前
		m_tagPartnerName[i].pPic->OrderMoveAppeared(8, m_tagPartnerName[i].iX - 100, m_tagPartnerName[i].iY + 10);

	
	}
}

SelectUI::~SelectUI()
{
	FOR((int)CHARACTER::END)
	{
		SAFE_DELETE(m_tagCharaPic[i].pPic);
		SAFE_DELETE(m_tagCharaPicRip[i].pPic);
		SAFE_DELETE(m_pCharaIconRip[i]);
		SAFE_DELETE(m_tagCharaInfo[i].pPic);
		SAFE_DELETE(m_tagCharaName[i].pPic);

	}

	FOR((int)PARTNER::END)
	{
		SAFE_DELETE(m_tagPartnerPic[i].pPic);
		SAFE_DELETE(m_tagPartnerPicRip[i].pPic);
		SAFE_DELETE(m_tagPartnerInfo[i].pPic);
		SAFE_DELETE(m_tagPartnerName[i].pPic);
		SAFE_DELETE(m_tagPartneSirclePic[i].pPic);
	}

	SAFE_DELETE(m_tagInfoBox.pPic);
	SAFE_DELETE(m_tagNamePlate.pPic);
	SAFE_DELETE(m_tagAndPlate.pPic);
	SAFE_DELETE(m_tagPartnerPlate.pPic);
	SAFE_DELETE(m_tagPartnerSirclePlate.pPic);

	SAFE_DELETE(m_pStateMachine);

	SAFE_DELETE(m_pSelect.pPic);
	SAFE_DELETE(m_tagOKfont.pPic);
}

void SelectUI::Update(bool bControl)
{
	// キャラクター系の更新
	for (int i = 0; i < (int)CHARACTER::END; i++)
	{
		m_tagCharaPic[i].pPic->Update();
		m_tagCharaPicRip[i].pPic->Update();
		m_tagCharaInfo[i].pPic->Update();
		m_tagCharaName[i].pPic->Update();
		m_pCharaIconRip[i]->Update();
	}

	// パートナー系の更新
	for (int i = 0; i < (int)PARTNER::END; i++)
	{	
		m_tagPartnerPic[i].pPic->Update();
		m_tagPartnerPicRip[i].pPic->Update();
		m_tagPartnerInfo[i].pPic->Update();
		m_tagPartnerName[i].pPic->Update();
		m_tagPartneSirclePic[i].pPic->Update();

	}

	// インフォボックス
	m_tagInfoBox.pPic->Update();

	// 名前系
	m_tagNamePlate.pPic->Update();
	m_tagAndPlate.pPic->Update();
	m_tagPartnerPlate.pPic->Update();
	
	if (SIDE::LEFT == m_eSide)
	{
		m_fPartnerPlateAngle += 0.002f;
	}else
	{
		m_fPartnerPlateAngle -= 0.002f;
	}
	m_tagPartnerSirclePlate.pPic->SetAngle(m_fPartnerPlateAngle);
	m_tagPartnerSirclePlate.pPic->Update();
	
	// セレクト更新
	m_pSelect.pPic->Update();

	// OK
	m_tagOKfont.pPic->Update();

	// ★ステートマシン更新
	if (bControl == true)
	{
		m_pStateMachine->Update();
	}

	return;

}

void SelectUI::Render()
{	

	// 説明用ボックス
	m_tagInfoBox.pPic->Render(m_tagInfoBox.iX, m_tagInfoBox.iY);

	// 説明
	m_tagCharaInfo[m_iSelectCharacterNo].pPic->Render(m_tagCharaInfo[m_iSelectCharacterNo].iX, m_tagCharaInfo[m_iSelectCharacterNo].iY);
	
	// パートナー説明
	m_tagPartnerInfo[m_iSelectPartnerNo].pPic->Render(m_tagPartnerInfo[m_iSelectPartnerNo].iX, m_tagPartnerInfo[m_iSelectPartnerNo].iY);

	// 名前プレート
	if (GetFSM()->isInState(*SelectUIState::SecondStep::GetInstance()) ||
		GetFSM()->isInState(*SelectUIState::SecondToOKStep::GetInstance()))
	{		
		m_tagNamePlate.pPic->SetARGB(0xaa666666);
		m_tagNamePlate.pPic->Render(m_tagNamePlate.iX, m_tagNamePlate.iY);
	}
	else
	{
		m_tagNamePlate.pPic->Render(m_tagNamePlate.iX, m_tagNamePlate.iY);
	}

	// 名前
	if (GetFSM()->isInState(*SelectUIState::SecondStep::GetInstance()))
	{
		m_tagCharaName[m_iSelectCharacterNo].pPic->SetARGB(0xaa666666);
		m_tagCharaName[m_iSelectCharacterNo].pPic->Render(m_tagCharaName[m_iSelectCharacterNo].iX, m_tagCharaName[m_iSelectCharacterNo].iY);
	}else
	{
		m_tagCharaName[m_iSelectCharacterNo].pPic->Render(m_tagCharaName[m_iSelectCharacterNo].iX, m_tagCharaName[m_iSelectCharacterNo].iY);

	}
	// パートナー選択プレート1
	m_tagPartnerPlate.pPic->Render(m_tagPartnerPlate.iX, m_tagPartnerPlate.iY);

	// パートナー選択した時の絵
	m_tagPartnerPic[m_iSelectPartnerNo].pPic->Render(m_tagPartnerPic[m_iSelectPartnerNo].iX, m_tagPartnerPic[m_iSelectPartnerNo].iY);

	// パートナー選択した時の絵の波紋
	m_tagPartnerPicRip[m_iSelectPartnerNo].pPic->Render(m_tagPartnerPicRip[m_iSelectPartnerNo].iX, m_tagPartnerPicRip[m_iSelectPartnerNo].iY, RS::ADD);
	m_tagPartnerPicRip[m_iSelectPartnerNo].pPic->Render(m_tagPartnerPicRip[m_iSelectPartnerNo].iX, m_tagPartnerPicRip[m_iSelectPartnerNo].iY, RS::ADD);

	// パートナーの最終プレート
	m_tagPartnerSirclePlate.pPic->Render(m_tagPartnerSirclePlate.iX, m_tagPartnerSirclePlate.iY);

	// パートナー選択した時の最後の絵
	m_tagPartneSirclePic[m_iSelectPartnerNo].pPic->Render(m_tagPartneSirclePic[m_iSelectPartnerNo].iX, m_tagPartneSirclePic[m_iSelectPartnerNo].iY);

	// パートナーの名前プレート
	m_tagAndPlate.pPic->Render(m_tagAndPlate.iX, m_tagAndPlate.iY);

	// パートナーの名前
	m_tagPartnerName[m_iSelectPartnerNo].pPic->Render(m_tagPartnerName[m_iSelectPartnerNo].iX, m_tagPartnerName[m_iSelectPartnerNo].iY);

	// アイコン
	m_pCharaIconRip[m_iSelectCharacterNo]->Render(384 + (m_iSelectCharacterNo * 88), 482, RS::ADD);
	if (GetFSM()->isInState(*SelectUIState::FirstStep::GetInstance()))
	{
	
		m_pSelect.pPic->SetScale(0.9f);	// 少し小さく
		m_pSelect.pPic->SetARGB(0xffffffff);
		if (m_eSide== SIDE::LEFT)
		{

			m_pSelect.pPic->Render(384 + (m_iSelectCharacterNo * 88) - 2, 482);
		}
		else
		{

			m_pSelect.pPic->Render(384 + (m_iSelectCharacterNo * 88) + 42, 482);
		}

	}else if (GetFSM()->isInState(*SelectUIState::MiddleStep::GetInstance()))
	{
		m_pSelect.pPic->SetARGB(0xcc777777);
		if (m_eSide == SIDE::LEFT)
		{
			m_pSelect.pPic->Render(384 + (m_iSelectCharacterNo * 88) - 2, 482);

		}else
		{
			m_pSelect.pPic->Render(384 + (m_iSelectCharacterNo * 88) + 42, 482);
		}
	}
	else if (GetFSM()->isInState(*SelectUIState::SecondStep::GetInstance()))
	{
		m_pSelect.pPic->SetScale(1.0f);	// 元に戻す

		m_pSelect.pPic->SetARGB(0xffffffff);
		m_pSelect.pPic->Render(m_tagPartnerPlate.iX+88, m_tagPartnerPlate.iY + 76);
	}
	else if (GetFSM()->isInState(*SelectUIState::SecondToOKStep::GetInstance()))
	{
		m_pSelect.pPic->SetARGB(0xff888888);
		m_pSelect.pPic->Render(m_tagPartnerPlate.iX + 88, m_tagPartnerPlate.iY + 76);
	}


	// OK!
	m_tagOKfont.pPic->Render(m_tagOKfont.iX, m_tagOKfont.iY);

	// ★ここにステートマシン描画
	m_pStateMachine->Render();


}

void SelectUI::RenderCharacter()
{
	// 立ち絵
	if (GetFSM()->isInState(*SelectUIState::SecondStep::GetInstance()) ||
		GetFSM()->isInState(*SelectUIState::SecondToOKStep::GetInstance()))
	{
		m_tagCharaPic[m_iSelectCharacterNo].pPic->SetARGB(0x77555555);
		m_tagCharaPic[m_iSelectCharacterNo].pPic->Render(m_tagCharaPic[m_iSelectCharacterNo].iX, m_tagCharaPic[m_iSelectCharacterNo].iY);
	}
	else
	{
		m_tagCharaPic[m_iSelectCharacterNo].pPic->Render(m_tagCharaPic[m_iSelectCharacterNo].iX, m_tagCharaPic[m_iSelectCharacterNo].iY);
	}

	// 立ち絵波紋
	m_tagCharaPicRip[m_iSelectCharacterNo].pPic->Render(m_tagCharaPicRip[m_iSelectCharacterNo].iX, m_tagCharaPicRip[m_iSelectCharacterNo].iY, RS::ADD);
	m_tagCharaPicRip[m_iSelectCharacterNo].pPic->Render(m_tagCharaPicRip[m_iSelectCharacterNo].iX, m_tagCharaPicRip[m_iSelectCharacterNo].iY, RS::ADD);

}

// キャラ関連
void SelectUI::ActionChara()
{
	m_tagCharaPic[m_iSelectCharacterNo].pPic->Action();
	m_tagCharaInfo[m_iSelectCharacterNo].pPic->Action();
	m_tagCharaName[m_iSelectCharacterNo].pPic->Action();
}
// キャラ関連プレート
void SelectUI::ActionCharaPlate()
{
	m_tagInfoBox.pPic->Action(6);
	m_tagNamePlate.pPic->Action(12);
}

void SelectUI::ActionFirstStep()
{
	// 色の明るさを戻す
	//m_tagCharaPic[m_iSelectCharacterNo].pPic->SetARGB(0xffffffff);


}

void SelectUI::StopFirstStep()
{
	//m_tagNamePlate.pPic->Stop();
	
	// キャラクター系の色の暗く
	// 色の明るさを戻す
	//m_tagCharaPic[m_iSelectCharacterNo].pPic->SetARGB(0xff666666);

}
// パートナー関連
void SelectUI::ActionPartner() 
{
	m_tagPartnerPic[m_iSelectPartnerNo].pPic->Action();
	m_tagPartnerInfo[m_iSelectPartnerNo].pPic->Action();
	m_tagPartnerName[m_iSelectPartnerNo].pPic->Action();
}
// パートナー関連プレート
void SelectUI::ActionSecondStep()
{
	// まずはキャラ説明を止める
	m_tagCharaInfo[m_iSelectCharacterNo].pPic->Stop();


	m_tagAndPlate.pPic->Action();
	m_tagPartnerPlate.pPic->Action();
}

void SelectUI::BackSecondStep()
{
	m_tagAndPlate.pPic->Stop();
	m_tagPartnerPlate.pPic->Stop();
	m_tagPartnerInfo[m_iSelectPartnerNo].pPic->Stop();
	m_tagPartnerName[m_iSelectPartnerNo].pPic->Stop();
	m_tagPartnerPic[m_iSelectPartnerNo].pPic->Stop();
}

void SelectUI::ActionOKStep()
{
	m_tagPartnerPlate.pPic->Stop();
	m_tagPartnerInfo[m_iSelectPartnerNo].pPic->Stop();
	m_tagPartnerPic[m_iSelectPartnerNo].pPic->Stop();

	// アニメーション
	m_tagCharaPic[m_iSelectCharacterNo].pPic->Action();
	m_tagPartneSirclePic[m_iSelectPartnerNo].pPic->Action();
	m_tagPartnerSirclePlate.pPic->Action();

	m_tagOKfont.pPic->Action();

}

void SelectUI::BackOKStep()
{
	m_tagPartneSirclePic[m_iSelectPartnerNo].pPic->Stop();
	m_tagPartnerSirclePlate.pPic->Stop();
	m_tagOKfont.pPic->Stop();
}

void SelectUI::MiddleAction()
{
	// 立ち絵とアイコンに 波紋演出
	m_tagCharaPicRip[m_iSelectCharacterNo].pPic->Action();
	m_pCharaIconRip[m_iSelectCharacterNo]->Action();
}

void SelectUI::SecondToOKAction()
{
	// パートナーのアイコンに 波紋演出
	m_tagPartnerPicRip[m_iSelectPartnerNo].pPic->Action();

}
