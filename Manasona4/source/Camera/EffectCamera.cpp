#include "TDNLIB.h"
#include "camera.h"
#include "EffectCamera.h"
#include "../BaseEntity/Message/MessageDispatcher.h"


//*****************************************************************************************************************************
//
//		超かっこいいカメラクラス
//
//*****************************************************************************************************************************

//=============================================================================================
//		初	期	化
EffectCamera::EffectCamera(ViewData *pViewData, LPCSTR filename, const Vector3 &OrgPos) :m_pViewData(pViewData), m_CurrentScriptID(0), m_CurrentFrame(0), m_PatternCursor(0), m_EventFrame(0), m_EventCursor(0), m_bAction(false), m_vOrgPos(OrgPos)
{
	std::ifstream ifs(filename);
	MyAssert(ifs, "カメラスクリプト入ってないよ！");

	while (!ifs.eof())
	{
		TimeLineCamera *set = new TimeLineCamera;


		char skip[128];	// 読み飛ばし用変数
		// スクリプト名読み飛ばし
		ifs >> skip;

		MyAssert(skip[0] == '[', "カメラスクリプトのテキストがずれてる\n%d番目付近が怪しいかも？", m_list.size());

		// スクリプトID取得
		ifs >> skip;
		ifs >> set->ID;

		// パターンの個数取得
		ifs >> skip;
		ifs >> set->NumPattern;
		assert(set->NumPattern > 0);
		set->pPatterns = new TimeLineCamera::Pattern[set->NumPattern];

		// パターン全回し
		for (int i = 0; i < set->NumPattern; i++)
		{
			// 終了フレーム取得
			ifs >> skip;
			ifs >> set->pPatterns[i].EndFrame;

			// フレーム速度取得
			ifs >> skip;
			ifs >> set->pPatterns[i].FrameSpeed;

			// 座標と注視点のベジエ点の個数
			ifs >> skip;
			ifs >> set->pPatterns[i].NumPoints;
			assert(set->pPatterns[i].NumPoints > 0);
			//set->pPatterns[i].pPosArray = new Vector3[set->pPatterns[i].NumPoints];
			//set->pPatterns[i].pTargetArray = new Vector3[set->pPatterns[i].NumPoints];
			set->pPatterns[i].InstanceArrays();	// ★ここでNuMPoints分の配列ポインタの確保を行っている

			for (int j = 0; j < set->pPatterns[i].NumPoints; j++)
			{
				// 座標
				ifs >> set->pPatterns[i].pViewDataArray[j].pos.x;
				ifs >> set->pPatterns[i].pViewDataArray[j].pos.y;
				ifs >> set->pPatterns[i].pViewDataArray[j].pos.z;

				// 注視点
				ifs >> set->pPatterns[i].pViewDataArray[j].target.x;
				ifs >> set->pPatterns[i].pViewDataArray[j].target.y;
				ifs >> set->pPatterns[i].pViewDataArray[j].target.z;

				// roll値
				ifs >> set->pPatterns[i].pViewDataArray[j].roll;
			}

			// 固定かどうか
			set->pPatterns[i].fix = (set->pPatterns[i].NumPoints == 1);
		}

	// カメラ振動イベント
	int NumShakeEvent(0);
	ifs >> skip;
	ifs >> NumShakeEvent;

	set->PartitioningShakeEvent(NumShakeEvent);

	FOR(NumShakeEvent)
	{
		// 発動フレーム
		ifs >> set->pShakeEventArray[i].TriggerFrame;
		// 振動の力
		ifs >> set->pShakeEventArray[i].ShakeInfo.ShakePower;
		// 振動時間
		ifs >> set->pShakeEventArray[i].ShakeInfo.ShakeFrame;
	}

		ifs >> skip;	// END読み飛ばし

		// リストに突っ込む
		m_list.push_back(set);
	}
}

//
//=============================================================================================


//=============================================================================================
//		開		放
EffectCamera::~EffectCamera()
{
	// リストの開放
	for (auto it : m_list) delete it;
}
//
//=============================================================================================


//=============================================================================================
//		更		新
Vector3 operator * (const Vector3 &v, const Matrix &m)
{
	Vector3 ret;
	ret.x = (v.x * m.m[0][0]) + (v.y * m.m[1][0]) + (v.z * m.m[2][0]);
	ret.y = (v.x * m.m[0][1]) + (v.y * m.m[1][1]) + (v.z * m.m[2][1]);
	ret.z = (v.x * m.m[0][2]) + (v.y * m.m[1][2]) + (v.z * m.m[2][2]);
	return ret;
}
void Rot2D(float rad, float *x, float *y)
{
	float ret_x = (*x * cosf(rad)) - (*y * sinf(rad));
	float ret_y = (*x * sinf(rad)) + (*y * cosf(rad));
	*x = ret_x;
	*y = ret_y;
}

void EffectCamera::Update()
{
	// はじく
	if (!m_bAction) return;

	// イベント時間更新
	m_EventFrame++;
	if (m_EventCursor < m_list[m_CurrentScriptID]->NumShakeEvent)
	{
		if (m_EventFrame == m_list[m_CurrentScriptID]->pShakeEventArray[m_EventCursor].TriggerFrame)
		{
			// 振動メッセージ送信
			MsgMgr->Dispatch(0, ENTITY_ID::CAMERA_MGR, ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::SHAKE_CAMERA, &m_list[m_CurrentScriptID]->pShakeEventArray[m_EventCursor].ShakeInfo);
			m_EventCursor++;
		}
	}

	// カメラフレーム更新+パターン内での終了フレーム到達したら
	if ((m_CurrentFrame += m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].FrameSpeed) > m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].EndFrame)
	{
		m_CurrentFrame = 0;

		// 次のパターンへ移行、もうパターンが無かったら終了
		if (++m_PatternCursor >= m_list[m_CurrentScriptID]->NumPattern)
		{
			//m_pCamera->OffEffectCamera();	// スクリプト解除
			m_bAction = false;
			return;
		}
	}

	// カメラ座標更新
	ViewData data;
	GetTimeLineViewData(&data);

	// プレイヤー座標系にする
	//Rot2D(m_pCamera->m_angle.y, &data.pos.z, &data.pos.x);
	data.pos += m_vOrgPos;
	data.target += m_vOrgPos;

	// カメラにセット
	*m_pViewData = data;
	//m_pCamera->Set(pos, target);
}
//
//=============================================================================================

//=============================================================================================
//		行	動	スタート
void EffectCamera::Start(int ID)
{
	MyAssert(ID >= 0 && ID < (int)m_list.size(), "エラー: 意図されないIDが設定されました。byエフェクトカメラ");
	m_CurrentScriptID = ID;
	m_PatternCursor = 0;
	m_CurrentFrame = 0;
	m_EventFrame = 0;
	m_EventCursor = 0;
	m_bAction = true;
}
//
//=============================================================================================



void EffectCamera::GetTimeLineViewData(ViewData *out)
{
	if (m_PatternCursor == -1) return;
	if (m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].fix)
	{
		*out = m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].pViewDataArray[0];
		return;
	}

	const float percentage = m_CurrentFrame / m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].EndFrame;	// 現在フレームのpercentage

	// ベジエ計算関数に丸投げ
	Math::Bezier(
		out,																	// 受け皿
		m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].pViewDataArray,					// 始点、中間、終点が入ってる座標配列
		m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].NumPoints,					// の要素数
		percentage
		);
}