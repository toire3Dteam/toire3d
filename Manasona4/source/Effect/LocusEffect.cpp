#include "LocusEffect.h"

//+------------------------
//	剣の残像
//+------------------------
LocusEffect::LocusEffect(const char* fileName, const int frameMax, const int separateNum)
{
	m_pReferencePosL = nullptr;
	m_pReferencePosR = nullptr;

	m_pTex = new tdn2DObj(fileName);

	m_iFrameMax = frameMax;
	m_iSeparate = separateNum;

	// 軌跡の最大数取得
	m_iLocusMax = (m_iFrameMax - 1)*(m_iSeparate - 1) + m_iFrameMax;

	// 軌跡の最大数分配列を生成
	m_pPosL = new Vector3[m_iLocusMax];
	m_pPosR = new Vector3[m_iLocusMax];
	memset(m_pPosL, 0, sizeof(m_pPosL));
	memset(m_pPosR, 0, sizeof(m_pPosR));

	m_pTU = new float[m_iLocusMax];

	// ↑で生成したTUの配列に等間隔にUV座標を指定
	for (int i = 0; i < m_iLocusMax; i++)
	{
		m_pTU[i] = i*(1.0f / (float(m_iLocusMax) - 1.0f));
	}

	// 初期設定
	m_lCol = 0xffffffff;
	m_bAction = false;
}

LocusEffect::~LocusEffect()
{
	SAFE_DELETE(m_pTex);
	SAFE_DELETE_ARRAY(m_pPosL);
	SAFE_DELETE_ARRAY(m_pPosR);
	SAFE_DELETE_ARRAY(m_pTU);

}

void LocusEffect::Update(const Vector3& posL, const Vector3& posR)
{
	if (m_bAction == false)return;

	// 軌跡分下り順で回し
	// 配列に格納されている座標を最後尾にずらしていく
	for (int i = (m_iLocusMax - 1); i > 0; i--)
	{
		m_pPosL[i] = m_pPosL[i - 1];
		m_pPosR[i] = m_pPosR[i - 1];
	}

	// 現在受け取った座標は一番手前の配列に格納
	m_pPosL[0] = posL;
	m_pPosR[0] = posR;

}

void LocusEffect::Update()
{
	if (m_bAction == false)return;

	// 軌跡分下り順で回し
	// 配列に格納されている座標を最後尾にずらしていく
	for (int i = (m_iLocusMax - 1); i > 0; i--)
	{
		m_pPosL[i] = m_pPosL[i - 1];
		m_pPosR[i] = m_pPosR[i - 1];
	}

	// 現在受け取った座標は一番手前の配列に格納
	m_pPosL[0] = *m_pReferencePosL;
	m_pPosR[0] = *m_pReferencePosR;
}

void LocusEffect::Render()
{
	if (m_bAction == false)return;

	// カリングOFFに
	tdnSystem::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 軌跡分ポリゴン生成
	LVERTEX vertex[4];
	for (int i = 0; i < m_iLocusMax - 1; i++)// [?]-1にしないと表示がバグる
	{
		vertex[0].x = m_pPosL[i].x;
		vertex[0].y = m_pPosL[i].y;
		vertex[0].z = m_pPosL[i].z;
		vertex[0].color = m_lCol;
		vertex[0].tu = m_pTU[i];
		vertex[0].tv = 0.0f;

		vertex[1].x = m_pPosR[i].x;
		vertex[1].y = m_pPosR[i].y;
		vertex[1].z = m_pPosR[i].z;
		vertex[1].color = m_lCol;
		vertex[1].tu = m_pTU[i];
		vertex[1].tv = 1.0f;

		vertex[2].x = m_pPosL[i + 1].x;
		vertex[2].y = m_pPosL[i + 1].y;
		vertex[2].z = m_pPosL[i + 1].z;
		vertex[2].color = m_lCol;
		vertex[2].tu = m_pTU[i + 1];
		vertex[2].tv = 0.0f;

		vertex[3].x = m_pPosR[i + 1].x;
		vertex[3].y = m_pPosR[i + 1].y;
		vertex[3].z = m_pPosR[i + 1].z;
		vertex[3].color = m_lCol;
		vertex[3].tu = m_pTU[i + 1];
		vertex[3].tv = 1.0f;

		//3D空間にポリゴン描画！
		tdnPolygon::Render3D(vertex, 2, m_pTex, RS::COPY_NOZ);

	}

	// カリング設定を戻す
	tdnSystem::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void LocusEffect::Action(const Vector3& posL, const Vector3& posR)
{
	// 全ての配列に受け取った座標で初期化
	for (int i = 0; i < m_iLocusMax; i++)
	{
		m_pPosL[i] = posL;
		m_pPosR[i] = posR;
	}

	// アクションフラグを立てる
	m_bAction = true;
}

void LocusEffect::Action(Vector3* posL, Vector3* posR)
{
	m_pReferencePosL = posL;
	m_pReferencePosR = posR;

	// 全ての配列に受け取った座標で初期化
	for (int i = 0; i < m_iLocusMax; i++)
	{
		m_pPosL[i] = *m_pReferencePosL;
		m_pPosR[i] = *m_pReferencePosR;
	}

	// アクションフラグを立てる
	m_bAction = true;
}

void LocusEffect::Stop()
{
	m_bAction = false;
}

