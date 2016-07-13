
#include "AI\NPC\PersonManager.h"

//　ひとの種類
#include "AI\NPC\BasePerson\BasePerson.h"
#include "../../Sound/SoundManager.h"
#include "FieldPlayer\FieldPlayer.h"
#include "SoccerBall\SoccerBall.h"


// 宣言
PersonManager* PersonManager::pInstance = nullptr;

int PersonManager::m_IDcount = 0;

/***********/
//	実体取得
/***********/
PersonManager & PersonManager::GetInstance()
{
	if (!pInstance)
	{
		pInstance = new PersonManager();
	}

	// TODO: return ステートメントをここに挿入します
	return *pInstance;
}

/***********/
//	解放
/***********/
void PersonManager::Release()
{
	if (pInstance)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}


// 初期化
PersonManager::PersonManager():
	BaseGameEntity(ENTITY_ID::PERSON_MNG)// ★人のマネージャー用のＩＤ番号を渡す
{
	m_IDcount = 0;
}


// 解放
PersonManager::~PersonManager()
{
	// 全部消して
	for (auto it : m_PersonData)
	{
		SAFE_DELETE(it);
	}

	//　データを空に
	m_PersonData.clear();
}

// キャラ追加
ENTITY_ID PersonManager::AddPerson(Vector3 pos, bool isAi)
{
	// 人
	BasePerson* data;

	ENTITY_ID id = (ENTITY_ID)(GAME_ID::id_person_first + m_IDcount);

	data = new FieldPlayer(id, isAi);

	// ここで設定を変更
	data->SetPos(pos);

	// 配列に加える
	m_PersonData.push_back(data);

	++m_IDcount;// 次の人の番号へ

	// 設置した人のIDを返す
	return id;
}

void PersonManager::Reset()
{
	// カウントを止める
	m_IDcount = 0;
}

void PersonManager::Update()
{
	if (m_PersonData.empty())return;// 空だったらリターン

	// 人たち更新
	for (auto it : m_PersonData)
	{
		it->Update();
	}
}

void PersonManager::Render()
{
	if (m_PersonData.empty())return;// 空だったらリターン


								   // 人たち描画
	for (auto it : m_PersonData)
	{

		it->RangeRender();

		it->Render();


	}
	// 人たちのＵＩ描画
	for (auto it : m_PersonData)
	{

		it->UIRender();
	}
}

// メッセージ
bool  PersonManager::HandleMessage(const Message& msg)
{
	// まあこのMGRにステートマシンはさすがにいいかな・・・
	// ということでここで直にメッセージに対する処理を

	switch (msg.Msg)
	{
	case MESSAGE_TYPE::TARGET_PLAYERS:
	{
		// オリジナル構造体のポインタ―型で受け取るぜ！
		PLAYER_INFO* exInfo = (PLAYER_INFO*)msg.ExtraInfo;

		// 近いプレイヤーを検索
		//Vector3 nearPos = NearPlayerPosSearch(exInfo);
		BasePerson* nearPlayer = NearPlayerSearch(exInfo);

		//int a = 114;
		//int* pa = &a;

		// 送信 
		MsgMgr->Dispatch(
			MSG_NO_DELAY,
			ENTITY_ID::PERSON_MNG,
			exInfo->id,
			SEND_PLAYER_POS,
			(void*)nearPlayer	// [追記情報]　※ポインタ―を渡すときは＆を抜く
			);

		return true;// [上手くメッセージが届いた!]
		break;
	}
	case MESSAGE_TYPE::SOCCER_BALL_NEAR_PLAYER:
	{
		// オリジナル構造体のポインタ―型で受け取るぜ！
		PLAYER_INFO* exInfo = (PLAYER_INFO*)msg.ExtraInfo;

		// 近いプレイヤーを検索
		ENTITY_ID nearID = SoccerNearPlayerSearch(exInfo);

		// 送信 
		MsgMgr->Dispatch(
			MSG_NO_DELAY,
			ENTITY_ID::PERSON_MNG,
			exInfo->id,
			SEND_NEAR_PLAYER,
			(void*)&nearID	// [追記情報]　※ポインタ―を渡すときは＆を抜く
			);
	}
		break;
	default:
		MyAssert(0, "そんな命令は受け付けてないです");
		break;
	}


	return false;// [上手くメッセージが届かなかった]
}

BasePerson * PersonManager::GetPerson(int no)
{
	if (m_PersonData.empty())
	{
		MyAssert(0, "データが空");
	}

	// エラー処理
	if (no >= (int)m_PersonData.size())
	{
		MyAssert(0, "指定した場所にデータがない");
	}

	int i(0);
	m_it = m_PersonData.begin();
	while (i != no)
	{
		m_it++;
		i++;
	}

	return (*m_it);

}

// 近いキャラクターを探す
Vector3 PersonManager::NearPlayerPosSearch(PLAYER_INFO* info)
{
	float nearPlayer = FLT_MAX;
	Vector3 nearPos = VECTOR_ZERO;
	// 人たち
	for (auto it : m_PersonData)
	{
		//if (it->GetPos())
		if (it->GetID() == info->id)continue;
		
		float len = Math::Length(it->GetPos(), info->pos);
		if (len <= nearPlayer)
		{
			nearPlayer = len;
			nearPos = it->GetPos();
		}


	}

	return  nearPos;

}

BasePerson* PersonManager::NearPlayerSearch(PLAYER_INFO* info)
{
	float nearPlayerlen = FLT_MAX;
	BasePerson* nearPlayer = NULL;
	BasePerson* myPlayer;
	// 人たち
	for (auto it : m_PersonData)
	{
		//if (it->GetPos())
		if (it->GetID() == info->id)
		{
			myPlayer = it;
			continue;
		}
		if (it->m_LifePoint >= 3)continue;// 死んでるやつはなし

		float len = Math::Length(it->GetPos(), info->pos);
		if (len <= nearPlayerlen)
		{
			nearPlayerlen = len;
			nearPlayer = it;
		}

	}

	if (nearPlayer==NULL)
	{
		//MyAssert(0, "誰もいなくなった");
		return myPlayer;
	}
	return nearPlayer;
}

// サッカーボールと
ENTITY_ID PersonManager::SoccerNearPlayerSearch(PLAYER_INFO * info)
{
	float nearPlayer = FLT_MAX;
	ENTITY_ID nearID;
	// 人たち
	for (auto it : m_PersonData)
	{
		if (it->m_LifePoint >= 3)continue;// 死んでるやつはなし

		float len = Math::Length(it->GetPos(), SOCCER_BALL.GetPos());
		if (len <= nearPlayer)
		{
			nearPlayer = len;
			nearID = it->GetID();
		}

	}

	return nearID;
}
