#include "PlayerManager.h"
#include "Airou/Airou.h"
#include "Nazenara/Nazenara.h"
#include "Teki/Teki.h"
#include "Aramitama/Aramitama.h"
#include "Aniki/Aniki.h"
#include "Stage/Stage.h"
#include "../BaseEntity/Message/MessageDispatcher.h"
#include "../Stand/Stand.h"
#include "../Number/Number.h"
#include "../Collision/Collision.h"
#include "Data\PlayerData.h"
#include "Data\SelectData.h"
#include "UI\GameUI.h"


// 実体の宣言
PlayerManager *PlayerManager::pInstance = nullptr;

PlayerManager::PlayerManager() :BaseGameEntity(ENTITY_ID::PLAYER_MGR),	// エンティティID登録
m_pStage(nullptr), m_NumPlayer(1), m_pPlayers(nullptr),
m_bHeaveHoDriveOverFlow(false)
{
	m_PointAteam = 0;
	m_PointBteam = 0;

	m_OverDriveDim = 255;
}
PlayerManager::PlayerManager(const PlayerManager&) :BaseGameEntity(ENTITY_ID::PLAYER_MGR){}

void PlayerManager::Initialize(int NumPlayer, Stage::Base *pStage, SideData SideDatas[(int)SIDE::ARRAY_MAX], int iRoundNum)
{
	m_NumPlayer = NumPlayer;
	m_pStage = pStage;

	// プレイヤ確保
	m_pPlayers = new BasePlayer*[NumPlayer];

	FOR(NumPlayer)
	{
		// (TODO)チームを今は仮で振り分け!! 2016/10.04日　左か右をTeamで変えてる
		SIDE side;
		if (i % 2 == 0)
		{
			side = SIDE::LEFT;
		}
		else
		{
			side = SIDE::RIGHT;
		}

		// キャラクターで分岐する
		switch (SideDatas[(int)side].eCharacter)
		{
		case CHARACTER::AIROU:
			m_pPlayers[i] = new Airou(side, SideDatas[(int)side]);
			break;
		case CHARACTER::TEKI:
			m_pPlayers[i] = new Teki(side, SideDatas[(int)side]);
			break;
		case CHARACTER::NAZENARA:
			m_pPlayers[i] = new Nazenara(side, SideDatas[(int)side]);
			break;
		case CHARACTER::ARAMITAMA:
			m_pPlayers[i] = new Aramitama(side, SideDatas[(int)side]);
			break;
		case CHARACTER::ANIKI:
			m_pPlayers[i] = new Aniki(side, SideDatas[(int)side]);
			break;
		default:
			m_pPlayers[i] = new Airou(side, SideDatas[(int)side]);
			//MyAssert(false,"そんなキャラクターはいない")
			break;
		}
		m_pPlayers[i]->InitActionDatas();		// ★攻撃情報を各キャラに初期化させる
		m_pPlayers[i]->InitMotionDatas();		// 各キャラごとのモーション番号
	}

	// キャラが被ってたら2Pの色を変える
	if (SideDatas[(int)SIDE::LEFT].eCharacter== SideDatas[(int)SIDE::RIGHT].eCharacter)
	{
		m_pPlayers[(int)SIDE::RIGHT]->ChangeColor();
	}


	// プレイヤーが2人という前提のコード
	assert(NumPlayer == 2);
	m_pPlayers[0]->SetTargetPlayer(m_pPlayers[1]);
	m_pPlayers[1]->SetTargetPlayer(m_pPlayers[0]);

	if (iRoundNum <= 1)
	{
		m_pPlayers[0]->SetLastOneWin();
		m_pPlayers[1]->SetLastOneWin();
	}

	FOR(NumPlayer)
	{
		// (TODO)チームを今は仮で振り分け!! 2016/10.04日　左か右をTeamで変えてる
		SIDE side;
		if (i % 2 == 0)
		{
			side = SIDE::LEFT;
		}
		else
		{
			side = SIDE::RIGHT;
		}

		// AI初期化 なぜここで初期化するというと、初期化の中でプレイヤーを参照することがあり、全員の初期化が完了した後に呼び出したいから
		m_pPlayers[i]->InitAI(SideDatas[(int)side].eAIType);
	}

	m_pDamageInfoPlate = new tdn2DObj("Data/UI/Game/Training/DamageInfoPlate.png");


}

PlayerManager::~PlayerManager()
{
	// ダブルポインターの開放
	FOR(m_NumPlayer)
	{
		SAFE_DELETE(m_pPlayers[i]);
	}
	delete[] m_pPlayers;

	SAFE_DELETE(m_pDamageInfoPlate);
}

void PlayerManager::Update(PLAYER_UPDATE flag)
{	
	// 誰かが1More覚醒していたら全員の動きを止める
	FOR(m_NumPlayer)
	{
		if (m_pPlayers[i]->isGameTimerStopFlag() == true)
		{
			m_pPlayers[i]->Update(flag);			// モーションとか移動値の作成とか、基本的な更新。
			
			//m_pStage->Collision(m_pPlayers[i], m_pPlayers[i]->GetMoveAddress());	// ステージとの判定で、move値をどうこういじった後に
			//m_pPlayers[i]->UpdatePos();			// 座標にmove値を足す更新

			// ●暗転処理
			m_OverDriveDim = max(m_OverDriveDim - 0.1f , 0.25f);

			return; // ほかのキャラクターの所へは断じて通さない
		}
		
	}
	// 誰も覚醒していなかったので通常通り更新
	
	// ●暗転解放処理
	m_OverDriveDim = min(m_OverDriveDim + 0.1f, 1.0f);
	
	

	/* プレイヤーたち更新 */
	FOR(m_NumPlayer)
	{
		if(m_bHeaveHoDriveOverFlow)m_pPlayers[i]->UpdateDrive();			// モーションとか移動値の作成とか、基本的な更新。
		else m_pPlayers[i]->Update(flag);
	}

	// チームポイント計算
	//CalcTeamPoint();
}

// トレーニング・練習の時のみ更新
void PlayerManager::UpdateTraining()
{
	// (仮)
	//PlayerDataMgr->m_TrainingData.iHpRecovery = (int)HP_RECOVERY_TYPE::AUTO_RECOVERY;
	//PlayerDataMgr->m_TrainingData.iHp1P = 50;
	//PlayerDataMgr->m_TrainingData.iHp2P = 75;
	//PlayerDataMgr->m_TrainingData.iSpGage = 50;
	//PlayerDataMgr->m_TrainingData.iPartnerRecovery= (int)PARTNER_RECOVERY_TYPE::MAX;
	//PlayerDataMgr->m_TrainingData.iInfo = (int)TRAINING_INFO_TYPE::DAMEGE;

	// AIに対しての命令
	FOR(m_NumPlayer)
	{
		if (m_pPlayers[i]->isAI() == true)
		{
			m_pPlayers[i]->GetAI()->UpdateTraining();
		}
	}

	// 自動回復かどうか
	if (SelectDataMgr->Get()->tagTrainingDatas.eHpRecovery == HP_RECOVERY_TYPE::AUTO_RECOVERY)
	{

		FOR(m_NumPlayer)
		{
			// 硬直時間がなければ回復
			if (m_pPlayers[i]->GetRecoveryFrame() <= 0)
			{

				m_pPlayers[i]->AddHP(100);

			}
		}

	}


	// 設定した最大HPより多かったら減らす
	float fHpPersent1P = (float)(SelectDataMgr->Get()->tagTrainingDatas.iHp1P) / (float)(100);
	float fHpPersent2P = (float)(SelectDataMgr->Get()->tagTrainingDatas.iHp2P) / (float)(100);
	float fTrainingMaxHp1P = m_pPlayers[(int)SIDE::LEFT]->GetMaxHP()*fHpPersent1P;
	float fTrainingMaxHp2P = m_pPlayers[(int)SIDE::RIGHT]->GetMaxHP()*fHpPersent2P;

	// 左の最大HP
	if (m_pPlayers[(int)SIDE::LEFT]->GetHP() > m_pPlayers[(int)SIDE::LEFT]->GetMaxHP()*fHpPersent1P)
	{
		m_pPlayers[(int)SIDE::LEFT]->SetHP((int)fTrainingMaxHp1P);
	};
	// 右の最大HP
	if (m_pPlayers[(int)SIDE::RIGHT]->GetHP() > m_pPlayers[(int)SIDE::RIGHT]->GetMaxHP()*fHpPersent2P)
	{
		m_pPlayers[(int)SIDE::RIGHT]->SetHP((int)fTrainingMaxHp2P);
	};


	// 設定したSPゲージより少なかったら設定した値に
	// お互いの硬直が解けたら
	if (m_pPlayers[(int)SIDE::LEFT]->GetRecoveryFrame() <= 0 &&
		m_pPlayers[(int)SIDE::RIGHT]->GetRecoveryFrame() <= 0)
	{
		FOR(m_NumPlayer)
		{
			// 自分が攻撃してないとき + パートナーも攻撃していない
			if (m_pPlayers[i]->isAttackState() == false && 
				m_pPlayers[i]->GetStand()->isActive() == false)
			{
				if (m_pPlayers[i]->GetOverDriveGage() < SelectDataMgr->Get()->tagTrainingDatas.iSpGage)
				{
					m_pPlayers[i]->SetOverDriveGage((float)SelectDataMgr->Get()->tagTrainingDatas.iSpGage);
				}
			}
		}
	};
	

	// パートナーゲージをMAX固定にしていたら
	if (SelectDataMgr->Get()->tagTrainingDatas.ePartnerRecovery == PARTNER_RECOVERY_TYPE::MAX)
	{
		// お互いの硬直が解けたら
		if (m_pPlayers[(int)SIDE::LEFT]->GetRecoveryFrame() <= 0 &&
			m_pPlayers[(int)SIDE::RIGHT]->GetRecoveryFrame() <= 0)
		{
			FOR(m_NumPlayer)
			{
				// 自分が攻撃してないとき + パートナーも攻撃していない
				if (m_pPlayers[i]->isAttackState() == false &&
					m_pPlayers[i]->GetStand()->isActive() == false)
				{	
					if (m_pPlayers[i]->GetStand()->GetStandGage() < m_pPlayers[i]->GetStand()->GetStandGageMAX())
					{
						m_pPlayers[i]->GetStand()->SetStandGage(m_pPlayers[i]->GetStand()->GetStandGageMAX());
					}
				}
			}
		};
	}


	// キャラクター個別の設定
	FOR(m_NumPlayer)
	{
		// もしアラミタマだったら
		if (m_pPlayers[i]->GetCharacterType() == CHARACTER::ARAMITAMA)
		{
			AramitamaDesk l_tagDesk;
			l_tagDesk.iWassyoiGauge = 0;
			// 常にわっしょいゲージ最大なら
			if (SelectDataMgr->Get()->tagTrainingDatas.eWassyoiType == WASSYOI_TYPE::MAX)
			{
				l_tagDesk.iWassyoiGauge = Aramitama::c_WASSYOIGAUGE_MAX;
				m_pPlayers[i]->SetCharacterData((void*)&l_tagDesk);
			}


		}
	}



}

void PlayerManager::UpdateHit()
{

}

void PlayerManager::UpdatePos()
{
	/* 位置を確定 */
	FOR(m_NumPlayer)
	{
		// 相手が時を止めてたらスルーする
		if (m_pPlayers[i]->GetTargetPlayer()->isGameTimerStopFlag()) continue;

		m_pStage->Collision(m_pPlayers[i], m_pPlayers[i]->GetMoveAddress());	// ステージとの判定で、move値をどうこういじった後に
		m_pPlayers[i]->UpdatePos();			// 座標にmove値を足す更新
	}
}

void PlayerManager::Render()
{
	// プレイヤーたち描画
	FOR(m_NumPlayer)
	{
		if(m_bHeaveHoDriveOverFlow)
			m_pPlayers[i]->RenderDrive();
		else
		{
			m_pPlayers[i]->Render();
#ifdef _DEBUG
			m_pPlayers[i]->RenderDebug();
#endif
		}
	}
}

void PlayerManager::Render(tdnShader* shader, char* name)
{
	// プレイヤーたち描画
	FOR(m_NumPlayer) m_pPlayers[i]->Render(shader, name);
}

void PlayerManager::RenderShadow()
{
	// プレイヤーの影描画用
	FOR(m_NumPlayer) m_pPlayers[i]->RenderShadow();

}

void PlayerManager::RenderDeferred()
{
	// プレイヤーたち描画
	FOR(m_NumPlayer) m_pPlayers[i]->RenderDeferred();

}

void PlayerManager::RenderUI()
{
	// プレイヤーのUI描画
	FOR(m_NumPlayer) m_pPlayers[i]->RenderUI();
}

void PlayerManager::RenderTraining()
{
	// ゴリ君
	SIDE eOrderSide = SelectDataMgr->Get()->eOrderTrainingSide;
	SIDE eTargetSide = m_pPlayers[(int)eOrderSide]->GetTargetPlayer()->GetSide();

	// ダメージフォント配置
	int iFontX = 200;
	int iFontY = 120;
	if (SelectDataMgr->Get()->eOrderTrainingSide == SIDE::RIGHT)
	{
		iFontX = 750;
		iFontY = 120;
	}


	// 全ての情報を描画
	if (SelectDataMgr->Get()->tagTrainingDatas.eTrainingInfo == TRAINING_INFO_TYPE::ALL)
	{
		// コマンド覆歴
		m_pPlayers[(int)eOrderSide]->RenderCommandFrame(10, 625);

		// ダメージのインフォメーションの裏に添えるプレート
		m_pDamageInfoPlate->Render(iFontX - 16, iFontY - 8);

		std::string pStr = "なし";
		// 一発ダメージ
		pStr = "ダメージ　　　　　      ";
		pStr += std::to_string(GameUIMgr->GetComboUI(eTargetSide)->GetNowDamage());
		tdnFont::RenderString(pStr.c_str(), "HGS創英角ｺﾞｼｯｸUB", 24, iFontX + 2, iFontY + 2 , 0xaa000000, RS::COPY);
		tdnFont::RenderString(pStr.c_str(), "HGS創英角ｺﾞｼｯｸUB", 24, iFontX, iFontY, 0xffeeeeee, RS::COPY);
			
		// コンボダメージ
		pStr = "コンボダメージ 　　     ";
		pStr += std::to_string(GameUIMgr->GetComboUI(eTargetSide)->GetComboDamage());
		iFontY += 40;
		tdnFont::RenderString(pStr.c_str(), "HGS創英角ｺﾞｼｯｸUB", 24, iFontX + 2, iFontY + 2, 0xaa000000, RS::COPY);
		tdnFont::RenderString(pStr.c_str(), "HGS創英角ｺﾞｼｯｸUB", 24, iFontX, iFontY, 0xffeeeeee, RS::COPY);

		// 最大ダメージ
		pStr = "最大コンボダメージ      ";
		pStr += std::to_string(GameUIMgr->GetComboUI(eTargetSide)->GetMaxDamage());
		iFontY += 40;
		tdnFont::RenderString(pStr.c_str(), "HGS創英角ｺﾞｼｯｸUB", 24, iFontX + 2, iFontY + 2, 0xaa000000, RS::COPY);
		tdnFont::RenderString(pStr.c_str(), "HGS創英角ｺﾞｼｯｸUB", 24, iFontX, iFontY, 0xffeeeeee, RS::COPY);

	}
	else if (SelectDataMgr->Get()->tagTrainingDatas.eTrainingInfo == TRAINING_INFO_TYPE::COMMAND)
	{
		// コマンド覆歴
		m_pPlayers[(int)eOrderSide]->RenderCommandFrame(10, 625);

	}
	else if (SelectDataMgr->Get()->tagTrainingDatas.eTrainingInfo == TRAINING_INFO_TYPE::DAMEGE)
	{

		// ダメージのインフォメーションの裏に添えるプレート
		m_pDamageInfoPlate->Render(iFontX - 16, iFontY - 8);

		std::string pStr = "なし";
		// 一発ダメージ
		pStr = "ダメージ　　　　　      ";
		pStr += std::to_string(GameUIMgr->GetComboUI(eTargetSide)->GetNowDamage());
		tdnFont::RenderString(pStr.c_str(), "HGS創英角ｺﾞｼｯｸUB", 24, iFontX + 2, iFontY + 2, 0xaa000000, RS::COPY);
		tdnFont::RenderString(pStr.c_str(), "HGS創英角ｺﾞｼｯｸUB", 24, iFontX, iFontY, 0xffeeeeee, RS::COPY);

		// コンボダメージ
		pStr = "コンボダメージ 　　     ";
		pStr += std::to_string(GameUIMgr->GetComboUI(eTargetSide)->GetComboDamage());
		iFontY += 40;
		tdnFont::RenderString(pStr.c_str(), "HGS創英角ｺﾞｼｯｸUB", 24, iFontX + 2, iFontY + 2, 0xaa000000, RS::COPY);
		tdnFont::RenderString(pStr.c_str(), "HGS創英角ｺﾞｼｯｸUB", 24, iFontX, iFontY, 0xffeeeeee, RS::COPY);

		// 最大ダメージ
		pStr = "最大コンボダメージ      ";
		pStr += std::to_string(GameUIMgr->GetComboUI(eTargetSide)->GetMaxDamage());
		iFontY += 40;
		tdnFont::RenderString(pStr.c_str(), "HGS創英角ｺﾞｼｯｸUB", 24, iFontX + 2, iFontY + 2, 0xaa000000, RS::COPY);
		tdnFont::RenderString(pStr.c_str(), "HGS創英角ｺﾞｼｯｸUB", 24, iFontX, iFontY, 0xffeeeeee, RS::COPY);

	}


}

//void PlayerManager::SendHitMessage(BasePlayer *pAttackPlayer, BasePlayer *pDamagePlayer, HIT_DAMAGE_INFO *pHitDamageInfo)
//{
//	/* メッセージ送信 */
//
//	// ダメージ受けたよメッセージ
//	MsgMgr->Dispatch(0, pAttackPlayer->GetID(), pDamagePlayer->GetID(), MESSAGE_TYPE::HIT_DAMAGE, pHitDamageInfo);
//
//	// ダメージを与えるやつに対してヒットストップをかける
//	pAttackPlayer->SetHitStopFrame(pHitDamageInfo->iHitStopFrame);
//	//pDamagePlayer->SetHitStopFrame(pHitDamageInfo->iHitStopFrame);
//}

bool PlayerManager::HandleMessage(const Message &msg)
{
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::HEAVE_HO_OVER_DRIVE_HIT:
		m_bHeaveHoDriveOverFlow = true;
		break;
	}
	return false;
}

// チームポイント計算
void PlayerManager::CalcTeamPoint()
{
	m_PointAteam = 0;
	m_PointBteam = 0;
	
	// プレイヤーたち描画
	FOR(m_NumPlayer)
	{
		if (m_pPlayers[i]->GetSide()==SIDE::LEFT)
		{
			m_PointAteam += m_pPlayers[i]->GetScore();
		}
		else
		{
			m_PointBteam += m_pPlayers[i]->GetScore();
		}	
	}


}

//void PlayerManager::UpdatePlayerTargetDir(BasePlayer *my, BasePlayer *you)
//{
//	const Vector3 v(you->GetPos() - my->GetPos());
//	if (v.x > 0)
//	{
//		my->SetTargetDir(DIR::RIGHT);
//		you->SetTargetDir(DIR::LEFT);
//	}
//	else if (v.x < 0)
//	{
//		my->SetTargetDir(DIR::LEFT);
//		you->SetTargetDir(DIR::RIGHT);
//	}
//}