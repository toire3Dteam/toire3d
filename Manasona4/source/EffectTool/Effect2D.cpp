#include "Effect2D.h"

/************************/
//
/************************/

Effect2D::Effect2D()
{
	// 初期化
	m_pObj = nullptr;
	m_iFrame = 0;
	//m_iMaxFrame			= 60;
	m_vPos.x = m_vPos.y = 0;
	m_fScale = 1.0f;
	m_fAngle = 0.0f;
	m_sCol.Clear();

	// 
	m_sStart.Clear();
	m_sMiddle.Clear();
	m_sEnd.Clear();

	m_sStart.frame = 0;
	m_sMiddle.frame = 0;
	m_sEnd.frame = 30;

	// 仮
	//m_sStart.scale = 1;
	//m_sStart.col.a = 255;
	//m_sMiddle.scale = 1;
	//m_sMiddle.col.a = 255;
	//m_sEnd.scale = 1;
	//m_sEnd.col.a = 255;

	m_bMiddleFlag = false;

	// 情報
	//m_sName.clear();
	memset(m_sName, 0, sizeof(m_sName));
	memset(m_sListName, 0, sizeof(m_sListName));
	
	m_bRenderFlag = true;
}

Effect2D::~Effect2D()
{
	SAFE_DELETE(m_pObj);
}

void Effect2D::LoadTexture(char* fileName)
{
	// テクスチャがすでにあるなら一度解放し切り替え
	if (m_pObj != nullptr)
	{
		SAFE_DELETE(m_pObj);
	}

	m_pObj = new tdn2DObj(fileName);

	sprintf(m_sName, "%s", fileName); // 名前
	//m_sName = fileName;// 名前
}

void Effect2D::Update(int gameFrame)
{
	m_iFrame = gameFrame;
	if (m_pObj == nullptr)return;

	//FrameUpdate();
	CalcParam();

	// 更新
	m_pObj->SetARGB(m_sCol.a, m_sCol.r, m_sCol.g, m_sCol.b);
	m_pObj->SetScale(m_fScale);
	m_pObj->SetAngle(m_fAngle);
}

void Effect2D::Render()
{
	if (m_iFrame <= m_sStart.frame)	return;				// まだ出現していない
	if (m_iFrame >= m_sEnd.frame)	return;				// 寿命が終わっている

															// 何も読みこまれてなかったら■を描画
	if (m_pObj == nullptr)tdnPolygon::Rect((int)m_vPos.x, (int)m_vPos.y, 64);
	else m_pObj->Render((int)m_vPos.x - (m_pObj->GetWidth() / 2), (int)m_vPos.y - (m_pObj->GetHeight() / 2));

	if (m_bMiddleFlag == true)
	{

		// 中間より下か上か	
		if (m_iFrame <= m_sMiddle.frame)
		{
			// レート取得
			float rate = (float)m_iFrame / (float)m_sMiddle.frame;	// フレームをレートに
			rate = Math::Clamp(rate, 0.0f, 1.0f);
			tdnText::Draw(0, 0, 0xffffffff, "レート%.2f", rate);
		}
		else
		{
			// レート取得
			float rate = (float)m_iFrame / (float)m_sEnd.frame;// フレームをレートに
			rate = Math::Clamp(rate, 0.0f, 1.0f);
			tdnText::Draw(0, 0, 0xffffffff, "レート%.2f", rate);
		}
	
	}
	else
	{
		// レート取得
		float rate = (float)m_iFrame / (float)m_sEnd.frame;	// フレームをレートに
		rate = Math::Clamp(rate, 0.0f, 1.0f);
		tdnText::Draw(0, 0, 0xffffffff, "レート%.2f", rate);

	}

}


void Effect2D::Render(int screenX, int screenY)
{
	if (m_bRenderFlag ==false)return;
	
	if (m_iFrame <= m_sStart.frame)	return;				// まだ出現していない
	if (m_iFrame >= m_sEnd.frame)	return;				// 寿命が終わっている

														// 何も読みこまれてなかったら■を描画
	if (m_pObj == nullptr)
		tdnPolygon::Rect((int)m_vPos.x + screenX-16, (int)m_vPos.y + screenX-16, 32);
	else 
		m_pObj->Render((int)m_vPos.x - (m_pObj->GetWidth() / 2)+ screenX, (int)m_vPos.y - (m_pObj->GetHeight() / 2) + screenY);


	//if (m_bMiddleFlag == true)
	//{

	//	// 中間より下か上か	
	//	if (m_iFrame <= m_sMiddle.frame)
	//	{
	//		// レート取得
	//		float rate = (float)m_iFrame / (float)m_sMiddle.frame;	// フレームをレートに
	//		rate = Math::Clamp(rate, 0.0f, 1.0f);
	//		tdnText::Draw(0, 0, 0xffff00ff, "レート%.2f", rate);
	//	}
	//	else
	//	{
	//		// レート取得
	//		float rate = (float)m_iFrame / (float)m_sEnd.frame;// フレームをレートに
	//		rate = Math::Clamp(rate, 0.0f, 1.0f);
	//		tdnText::Draw(0, 50, 0xffff00ff, "レート%.2f", rate);
	//	}

	//}
	//else
	//{
	//	// レート取得
	//	float rate = (float)m_iFrame / (float)m_sEnd.frame;	// フレームをレートに
	//	rate = Math::Clamp(rate, 0.0f, 1.0f);
	//	tdnText::Draw(0, 0, 0xffff00ff, "レート%.2f", rate);

	//}
}

//void Effect2D::FrameUpdate()
//{
//	// フレーム更新
//	m_iFrame++;
//
//	// 最大まで行けば 0 にもどす
//	if (m_iFrame >= m_iMaxFrame) m_iFrame = 0;
//	
//}

void Effect2D::CalcParam()
{


	if (m_iFrame <= m_sStart.frame)	return;				// まだ出現していない
	if (m_iFrame >= m_sEnd.frame)		return;				// 寿命が終わっている

	if (m_bMiddleFlag == true)
	{
		// 中間より下か上か	
		if (m_iFrame <= m_sMiddle.frame)
		{
			// レート取得
			float rate = (float)m_iFrame / (float)m_sMiddle.frame;	// フレームをレートに
			rate = Math::Clamp(rate, 0.0f, 1.0f);

			// 拡大・回転・場所・色の補間
			m_fScale = Math::Blend(rate, (float)m_sStart.scale, (float)m_sMiddle.scale);
			m_fAngle = Math::Blend(rate, (float)m_sStart.angle, (float)m_sMiddle.angle);
			m_vPos.x = Math::Blend(rate, (float)m_sStart.pos.x, (float)m_sMiddle.pos.x);
			m_vPos.y = Math::Blend(rate, (float)m_sStart.pos.y, (float)m_sMiddle.pos.y);

			m_sCol.a = (int)Math::Blend(rate, (float)m_sStart.col.a, (float)m_sMiddle.col.a);
			m_sCol.r = (int)Math::Blend(rate, (float)m_sStart.col.r, (float)m_sMiddle.col.r);
			m_sCol.g = (int)Math::Blend(rate, (float)m_sStart.col.g, (float)m_sMiddle.col.g);
			m_sCol.b = (int)Math::Blend(rate, (float)m_sStart.col.b, (float)m_sMiddle.col.b);
		}
		else
		{
			// レート取得
			float rate = (float)(m_iFrame - m_sMiddle.frame) / (float)(m_sEnd.frame - m_sMiddle.frame);// フレームをレートに
			rate = Math::Clamp(rate, 0.0f, 1.0f);

			// 拡大・回転・場所・色の補間
			m_fScale = Math::Blend(rate, (float)m_sMiddle.scale, (float)m_sEnd.scale);
			m_fAngle = Math::Blend(rate, (float)m_sMiddle.angle, (float)m_sEnd.angle);
			m_vPos.x = Math::Blend(rate, (float)m_sMiddle.pos.x, (float)m_sEnd.pos.x);
			m_vPos.y = Math::Blend(rate, (float)m_sMiddle.pos.y, (float)m_sEnd.pos.y);

			m_sCol.a = (int)Math::Blend(rate, (float)m_sMiddle.col.a, (float)m_sEnd.col.a);
			m_sCol.r = (int)Math::Blend(rate, (float)m_sMiddle.col.r, (float)m_sEnd.col.r);
			m_sCol.g = (int)Math::Blend(rate, (float)m_sMiddle.col.g, (float)m_sEnd.col.g);
			m_sCol.b = (int)Math::Blend(rate, (float)m_sMiddle.col.b, (float)m_sEnd.col.b);
		}
	}
	else
	{
		// レート取得
		float rate = (float)m_iFrame / (float)m_sEnd.frame;	// フレームをレートに
		rate = Math::Clamp(rate, 0.0f, 1.0f);

		// 拡大・回転・場所・色の補間
		m_fScale = Math::Blend(rate, (float)m_sStart.scale, (float)m_sEnd.scale);
		m_fAngle = Math::Blend(rate, (float)m_sStart.angle, (float)m_sEnd.angle);
		m_vPos.x = Math::Blend(rate, (float)m_sStart.pos.x, (float)m_sEnd.pos.x);
		m_vPos.y = Math::Blend(rate, (float)m_sStart.pos.y, (float)m_sEnd.pos.y);

		m_sCol.a = (int)Math::Blend(rate, (float)m_sStart.col.a, (float)m_sEnd.col.a);
		m_sCol.r = (int)Math::Blend(rate, (float)m_sStart.col.r, (float)m_sEnd.col.r);
		m_sCol.g = (int)Math::Blend(rate, (float)m_sStart.col.g, (float)m_sEnd.col.g);
		m_sCol.b = (int)Math::Blend(rate, (float)m_sStart.col.b, (float)m_sEnd.col.b);

	}


	//
	//// 拡大
	//(float)m_fScale = Math::Blend(rate, (float)m_fFirstScale, (float)m_fEndScale);	// 最初と最後のスケールを補間



}
