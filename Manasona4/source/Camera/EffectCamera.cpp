#include "TDNLIB.h"
#include "camera.h"
#include "EffectCamera.h"


//*****************************************************************************************************************************
//
//		超かっこいいカメラクラス
//
//*****************************************************************************************************************************

//=============================================================================================
//		初	期	化
EffectCamera::EffectCamera(Camera *me) :m_pCamera(me), m_CurrentScriptID(0), m_CurrentFrame(0), m_PatternCursor(0)
{
	std::ifstream ifs("DATA/Camera/script.txt");
	MyAssert(ifs, "カメラスクリプト入ってないよ！");

	while (!ifs.eof())
	{
		TimeLineCamera *set = new TimeLineCamera;

		char skip[128];	// 読み飛ばし用変数

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

			// 座標と注視点のベジエ点の個数
			ifs >> skip;
			ifs >> set->pPatterns[i].NumPoints;
			assert(set->pPatterns[i].NumPoints > 0);
			set->pPatterns[i].pPosArray = new Vector3[set->pPatterns[i].NumPoints];
			set->pPatterns[i].pTargetArray = new Vector3[set->pPatterns[i].NumPoints];

			for (int j = 0; j < set->pPatterns[i].NumPoints; j++)
			{
				// 座標
				ifs >> set->pPatterns[i].pPosArray[j].x;
				ifs >> set->pPatterns[i].pPosArray[j].y;
				ifs >> set->pPatterns[i].pPosArray[j].z;

				// 注視点
				ifs >> set->pPatterns[i].pTargetArray[j].x;
				ifs >> set->pPatterns[i].pTargetArray[j].y;
				ifs >> set->pPatterns[i].pTargetArray[j].z;
			}

			// 固定かどうか
			set->pPatterns[i].fix = (set->pPatterns[i].NumPoints == 1);
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
	// カメラフレーム更新+パターン内での終了フレーム到達したら
	if (++m_CurrentFrame > m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].EndFrame)
	{
		m_CurrentFrame = 0;

		// 次のパターンへ移行、もうパターンが無かったら終了
		if (++m_PatternCursor >= m_list[m_CurrentScriptID]->NumPattern)
		{
			m_pCamera->OffEffectCamera();	// スクリプト解除
			return;
		}
	}

	// カメラ座標更新
	Vector3 pos, target;
	GetTimeLineCameraPos(&pos, &target);

	// プレイヤー座標系にする
	Rot2D(m_pCamera->m_angle.y, &pos.z, &pos.x);
	pos += m_pCamera->m_OrgPos;
	target += m_pCamera->m_OrgPos;

	// カメラにセット
	m_pCamera->m_pos = pos;
	m_pCamera->m_target = target;
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
}
//
//=============================================================================================



void EffectCamera::GetTimeLineCameraPos(Vector3 *out_pos, Vector3 *out_target)
{
	if (m_PatternCursor == -1) return;
	if (m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].fix)
	{
		*out_pos = m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].pPosArray[0];
		*out_target = m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].pTargetArray[0];
		return;
	}

	// ベジエ計算関数に丸投げ
	Math::Bezier(
		out_pos,																	// 受け皿
		m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].pPosArray,					// 始点、中間、終点が入ってる座標配列
		m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].NumPoints,					// の要素数
		m_CurrentFrame / m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].EndFrame	// 終了フレーム
		);

	Math::Bezier(
		out_target,																	// 受け皿
		m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].pTargetArray,				// 始点、中間、終点が入ってる座標配列
		m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].NumPoints,					// の要素数
		m_CurrentFrame / m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].EndFrame	// 終了フレーム
		);
}