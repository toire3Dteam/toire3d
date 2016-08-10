#include "TDNLIB.h"
#include "Particle.h"


//*****************************************************************************
//	ペルソナ発動した瞬間
//*****************************************************************************
void ParticleManager::EffectPersonaTrigger(const Vector3 &pos)
{
	Vector3 Pos, Move, Power;

	FOR(20)
	{
		Pos.x = pos.x + rand() % 11 - 5.0f;
		Pos.y = pos.y + (float)(rand() % 3);
		Pos.z = pos.z + rand() % 11 - 5.0f;
		const int M = 8;
		Move.x = (rand() % M - (M / 2))*0.25f;
		Move.y = (rand() % 3 + 2) * 0.1f;
		Move.z = (rand() % M - (M / 2))*0.25f;
		Power.x = -Move.x*.01f;
		Power.y = 0;
		Power.z = -Move.z*.01f;
	
		// パーティクルセット
		Set(6, 0, .0f, 60, .0f, 20, .9f, Pos, Move, Power, 1.0f, 1.0f, 1.0f, 0, 1.0f, rand() % 2 + 1.5f, RS::ADD);
	}
}

//*****************************************************************************
//	ペルソナ持続オーラ
//*****************************************************************************
void ParticleManager::EffectPersonaAura(const Vector3 &pos)
{
	Vector3 Pos, Move, Power;

	Pos.x = pos.x + rand() % 26 - 12.0f;
	Pos.y = pos.y + (float)(rand() % 10);
	Pos.z = pos.z + rand() % 26 - 12.0f;
	const int M = 2;
	Move.x = (rand() % M - (M / 2))*0.01f;
	Move.y = (rand() % 2 + 3) * 0.1f;
	Move.z = (rand() % M - (M / 2))*0.01f;
	Power.x = 0;
	Power.y = 0.0f;
	Power.z = 0;

	const int type = (rand() % 2) ? 5 : 6;

	// パーティクルセット
	Set2(type, 0, .1f, 40, .0f, 10, .75f, Pos, Move, Power, .75f, .75f, .75f, 0, rand() % 314 * 0.01f, 1.0f, 1.0f, RS::ADD);
}

//*****************************************************************************
//	フィニッシュヒット
//*****************************************************************************
void ParticleManager::EffectFinish(const Vector3 &pos)
{
	Vector3 Pos, Move, Power;

	FOR(20)
	{
		Pos.x = pos.x + rand() % 11 - 5.0f;
		Pos.y = pos.y + (float)(rand() % 3);
		Pos.z = pos.z + rand() % 11 - 5.0f;
		const int M = 16;
		Move.x = (rand() % M - (M / 2))*0.25f;
		Move.y = (rand() % 3 + 7) * 0.1f;
		Move.z = (rand() % M - (M / 2))*0.25f;
		Power.x = 0;
		Power.y = 0;
		Power.z = 0;
		// 赤●
		Set(7, 0, 1.0f, 60, .0f, 40, .9f, Pos, Move, Power, 1.0f, 1.0f, 1.0f, 0, 1.0f, rand() % 2 + 1.5f, RS::COPY);

		// 粒
		for (int j = 0; j < 2; j++)
		{
			Move.x = rand() % 9 - 4.0f;
			Move.y = rand() % 9 - 4.0f;
			Move.z = rand() % 9 - 4.0f;
			Set(1, 0, 1.0f, 50, .0f, 30, .75f, pos, Move, Power, rand() % 9 * .1f, rand() % 9 * .1f, rand() % 9 * .1f, 0, 1.0f, rand() % 2 + 1.5f, RS::ADD);
		}

		Pos.x = pos.x + rand() % 6 - 2.0f;
		Pos.y = pos.y + (float)(rand() % 3);
		Pos.z = pos.z + rand() % 6 - 2.0f;
		Move.Zero();
		// かみなり
		Set2(11, 0, .75f, 20, .0f, 12, .9f, Pos, Move, Power, 1.0f, 1.0f, 1.0f, 0, rand() % 314 * .01f, 1.01f, rand() % 10 + 1.5f, RS::ADD);
	}
}

//*****************************************************************************
//	吹っ飛んだ時の煙
//*****************************************************************************
void ParticleManager::EffectFlySmoke(const Vector3 &pos)
{
	Vector3 Pos, Move, Power;

	Pos.x = pos.x + rand() % 6 - 2.0f;
	Pos.y = pos.y + (float)(rand() % 3);
	Pos.z = pos.z + rand() % 6 - 2.0f;
	Move.Zero();
	Power.Zero();
	Set2(4, 0, .75f, 50, .0f, 10, 1.0f, Pos, Move, Power, 1.0f, 1.0f, 1.0f, 0, rand() % 314 * .01f, 1.0f, rand() % 2 + 2.0f, RS::COPY);
}

//**************************************************************************************************
//
//		パーティクル管理クラス
//
//**************************************************************************************************

int				ParticleManager::m_NumParticles;	//	パーティクル最大数
ParticleData	*ParticleManager::m_pParticleDatas;	//	パーティクルデータ
tdn2DObj*       ParticleManager::m_pTexture;	//	テクスチャ

Matrix			ParticleManager::CurViewProjection;

//*****************************************************************************
//	初期化・解放
//*****************************************************************************
//------------------------------------------------------
//		初期化
//------------------------------------------------------
void ParticleManager::Initialize(char* filename, int NumParticles)
{
	//	パーティクルバッファ確保
	m_NumParticles = NumParticles;
	m_pParticleDatas = new ParticleData[NumParticles];
	//	全リセット
	Reset();
	//	テクスチャ設定
	m_pTexture = new tdn2DObj(filename);
}

//------------------------------------------------------
//		解放
//------------------------------------------------------
void ParticleManager::Release()
{
	delete[] m_pParticleDatas;
	delete m_pTexture;
}

//*****************************************************************************
//		データ設定
//*****************************************************************************
//------------------------------------------------------
//		構造体指定
//------------------------------------------------------
void ParticleManager::Set(LPPARTICLE pd)
{
	// 空きデータ検索
	FOR(m_NumParticles)
	{
		// 空きデータだったら、設定して抜ける
		if (!m_pParticleDatas[i].isActive())
		{
			m_pParticleDatas[i].Set(pd);
			break;
		}
	}
}

//------------------------------------------------------
//		データ個別指定
//------------------------------------------------------
void ParticleManager::Set(int type, int aFrame, COLOR aColor, int eFrame, COLOR eColor, int mFrame, COLOR mColor, const Vector3 &Pos, const Vector3 &Move, const Vector3 &Power, float rotate, float angle, float stretch, float scale, u8 flag)
{
	PARTICLE pd;

	pd.type = type;
	pd.aFrame = aFrame;
	pd.aColor = aColor;
	pd.eFrame = eFrame;
	pd.eColor = eColor;
	pd.mFrame = mFrame;
	pd.mColor = mColor;

	pd.pos = Pos;
	pd.move = Move;
	pd.power = Power;
	pd.rotate = rotate;
	pd.stretch = stretch;

	pd.scale = scale;
	pd.angle = angle;

	pd.flag = flag;

	Set(&pd);
}

void	ParticleManager::Set(int type, int aFrame, float aAlpha, int eFrame, float eAlpha, int mFrame, float mAlpha, const Vector3 &Pos, const Vector3 &Move, const Vector3 &Power, float r, float g, float b, float rotate, float stretch, float scale, u8 flag)
{
	Set2(type, aFrame, aAlpha, eFrame, eAlpha, mFrame, mAlpha, Pos, Move, Power, r, g, b, rotate, .0f, stretch, scale, flag);
}

void	ParticleManager::Set2(int type, int aFrame, float aAlpha, int eFrame, float eAlpha, int mFrame, float mAlpha, const Vector3 &Pos, const Vector3 &Move, const Vector3 &Power, float r, float g, float b, float rotate, float angle, float stretch, float scale, u8 flag)
{
	COLOR	color;
	DWORD	aa, ea, ma;

	aa = ((DWORD)(aAlpha*255.0f) << 24);
	ea = ((DWORD)(eAlpha*255.0f) << 24);
	ma = ((DWORD)(mAlpha*255.0f) << 24);
	color = ((DWORD)(r*255.0f) << 16) | ((DWORD)(g*255.0f) << 8) | (DWORD)(b*255.0f);
	Set(type, aFrame, aa | color, eFrame, ea | color, mFrame, ma | color, Pos, Move, Power, rotate, angle, stretch, scale, flag);
}

//*****************************************************************************
//		更新
//*****************************************************************************
void ParticleManager::Update()
{
	FOR(m_NumParticles) if (m_pParticleDatas[i].isActive()) m_pParticleDatas[i].Update();	// セットされてたら更新
}

//*****************************************************************************
//		リセット
//*****************************************************************************
void ParticleManager::Reset()
{
	for (int i = 0; i<m_NumParticles; i++) if (m_pParticleDatas[i].isActive()) m_pParticleDatas[i].SetFlag(0);	// 全員セットフラグをオフにする
}

//*****************************************************************************
//		描画
//*****************************************************************************
//------------------------------------------------------
//		固定機能描画
//------------------------------------------------------
void ParticleManager::Render()
{
	CurViewProjection = matView * matProjection;

	tdnRenderState::Set(RS::ADD, NULL, m_pTexture->GetTexture());
	tdnSystem::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	tdnSystem::GetDevice()->SetFVF(D3DFVF_TLVERTEX);
	// アルファブレンド設定
	tdnSystem::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	tdnSystem::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	tdnSystem::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	// パーティクルレンダリング
	FOR(m_NumParticles) if (m_pParticleDatas[i].isActive()) m_pParticleDatas[i].Render();

	tdnSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	tdnSystem::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//------------------------------------------------------
//		シェーダー描画
//------------------------------------------------------
void ParticleManager::Render(tdnShader* shader, char* name)
{
	//int		i;
	//float	scale = -1.0f;
	//
	//CurViewProjection = matView * matProjection;
	//
	//shader->SetTexture(m_pTexture->GetTexture());
	//iexRenderState::Set(RS_ADD, NULL, lpTexture->GetTexture());
	//
	//iexSystem::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//iexSystem::Device->SetFVF(D3DFVF_TLVERTEX);
	////	アルファブレンド設定
	//iexSystem::Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//iexSystem::Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//iexSystem::Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//
	//for (i = 0; i<m_NumParticles; i++){
	//	//	パーティクルレンダリング
	//	if (ParticleData[i].isActive()) ParticleData[i].Render(shader, name);
	//}
	//
	//iexSystem::Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//iexSystem::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}






//*****************************************************************************
//
//		パーティクル単体
//
//*****************************************************************************

//*****************************************************************************
//		セット
//*****************************************************************************
void ParticleData::Set(LPPARTICLE pd)
{
	memcpy(&pdata, pd, sizeof(PARTICLE));
	CurFrame = 0;
	CurAlpha = 0;
	bFlags = 1;
}

//*****************************************************************************
//		更新
//*****************************************************************************
void ParticleData::Update()
{
	// アクティブじゃなかったら、でていけぇ！
	if (!isActive()) return;

	float	work;
	int	a1, r1, g1, b1, a2, r2, g2, b2;

	//	情報更新
	pdata.pos += pdata.move;

	//if (pdata.isWork){
	//	if (pdata.Pos.y < pdata.work){
	//		pdata.Pos.y = pdata.work;
	//		pdata.Move.y = -pdata.Move.y*pdata.Reflection;
	//	}
	//}
	//
	//if (pdata.isDeceleration){
	//
	//	pdata.Move *= pdata.deceleration;
	//}

	pdata.move += pdata.power;
	pdata.angle += pdata.rotate;
	pdata.scale *= pdata.stretch;

	//	カラー計算
	if (CurFrame < pdata.aFrame) CurAlpha = .0f;
	else
	{
		if (CurFrame < pdata.mFrame)
		{
			work = (CurFrame - pdata.aFrame) / (float)(pdata.mFrame - pdata.aFrame);
			a1 = (pdata.aColor >> 24);
			r1 = ((pdata.aColor >> 16) & 0xFF);
			g1 = ((pdata.aColor >> 8) & 0xFF);
			b1 = ((pdata.aColor) & 0xFF);
		}
		else
		{
			work = (pdata.eFrame - CurFrame) / (float)(pdata.eFrame - pdata.mFrame);
			a1 = (pdata.eColor >> 24);
			r1 = ((pdata.eColor >> 16) & 0xFF);
			g1 = ((pdata.eColor >> 8) & 0xFF);
			b1 = ((pdata.eColor) & 0xFF);
		}

		a2 = (pdata.mColor >> 24);
		r2 = ((pdata.mColor >> 16) & 0xFF);
		g2 = ((pdata.mColor >> 8) & 0xFF);
		b2 = ((pdata.mColor) & 0xFF);
		//	カラー設定
		CurAlpha = ((a2 - a1)*work + a1) / 255.0f;
		r = ((r2 - r1)*work + r1) / 255.0f;
		g = ((g2 - g1)*work + g1) / 255.0f;
		b = ((b2 - b1)*work + b1) / 255.0f;
	}

	//	フレーム進行
	CurFrame++;
	if (CurFrame >= pdata.eFrame) bFlags = 0;
}

//*****************************************************************************
//		描画
//*****************************************************************************
//------------------------------------------------------
//		ポリゴン生成
//------------------------------------------------------
inline bool ParticleData::SetVertex(TLVERTEX* v)
{
	D3DVIEWPORT9	vp;
	tdnSystem::GetDevice()->GetViewport(&vp);

	Matrix mat2 = ParticleManager::CurViewProjection;

	float z1 = pdata.pos.x * matView._13 + pdata.pos.y * matView._23 + pdata.pos.z * matView._33 + matView._43;

	//	位置変換
	Vector3	pos;
	pos.z = pdata.pos.x * mat2._13 + pdata.pos.y * mat2._23 + pdata.pos.z * mat2._33 + mat2._43;
	if (pos.z <= 0) return false;

	pos.x = pdata.pos.x * mat2._11 + pdata.pos.y * mat2._21 + pdata.pos.z * mat2._31 + mat2._41;
	pos.y = pdata.pos.x * mat2._12 + pdata.pos.y * mat2._22 + pdata.pos.z * mat2._32 + mat2._42;

	float pw = pdata.pos.x * mat2._14 + pdata.pos.y * mat2._24 + pdata.pos.z * mat2._34 + mat2._44;

	float	x1, x2, y1, y2, s, c;
	s = sinf(pdata.angle) * pdata.scale;
	c = cosf(pdata.angle) * pdata.scale;
	//	頂点設定
	x1 = s - c;
	x2 = c + s;
	y1 = c + s;
	y2 = c - s;
	//	投影サイズ計算
	float px, py, px2, py2;
	px = x1*matProjection._11 + y1*matProjection._21 + z1*matProjection._31 + matProjection._41;
	py = x1*matProjection._12 + y1*matProjection._22 + z1*matProjection._32 + matProjection._42;
	px2 = x2*matProjection._11 + y2*matProjection._21 + z1*matProjection._31 + matProjection._41;
	py2 = x2*matProjection._12 + y2*matProjection._22 + z1*matProjection._32 + matProjection._42;

	v[0].sx = (pos.x + px) / pw;
	v[0].sy = (pos.y + py) / pw;
	v[1].sx = (pos.x + px2) / pw;
	v[1].sy = (pos.y + py2) / pw;
	v[2].sx = (pos.x - px2) / pw;
	v[2].sy = (pos.y - py2) / pw;
	v[3].sx = (pos.x - px) / pw;
	v[3].sy = (pos.y - py) / pw;

	v[0].sz = v[1].sz = v[2].sz = v[3].sz = pos.z / pw;
	v[0].rhw = v[1].rhw = v[2].rhw = v[3].rhw = 1.0f;

	int w = vp.Width;
	int h = vp.Height;

	bool bRender = false;
	for (int i = 0; i<4; i++)
	{
		if (v[i].sx > -1.0f && v[i].sx < 1.0f) bRender = true;
		if (v[i].sy > -1.0f && v[i].sy < 1.0f) bRender = true;
		v[i].sx = (v[i].sx + 1.0f) * w / 2 + vp.X;
		v[i].sy = (-v[i].sy + 1.0f) * h / 2 + vp.Y;
	}

	if (bRender == false) return false;
	//	タイプ設定
	v[0].tu = v[2].tu = (float)(pdata.type % 4) * 0.25f + 0.001f;
	v[1].tu = v[3].tu = v[0].tu + 0.248f;
	v[0].tv = v[1].tv = (float)(pdata.type / 4) * 0.25f + 0.001f;
	v[3].tv = v[2].tv = v[0].tv + 0.248f;

	//	色設定
	v[0].color = v[1].color = v[2].color = v[3].color = ((DWORD)(CurAlpha*255.0f) << 24) | ((DWORD)(r*255.0f) << 16) | ((DWORD)(g*255.0f) << 8) | ((DWORD)(b*255.0f));

	return true;
}

//------------------------------------------------------
//		固定機能描画
//------------------------------------------------------
void ParticleData::Render()
{
	TLVERTEX	v[4];

	if (SetVertex(v) == false) return;
	//	合成設定
	if (pdata.flag == RS::SUB) tdnSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	else                      tdnSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//	レンダリング
	tdnSystem::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(TLVERTEX));
}

//------------------------------------------------------
//		シェーダー描画
//------------------------------------------------------
void ParticleData::Render(tdnShader* shader, char* name)
{
	//if (pdata.flag != RS_SHADER){ Render(); return; }
	//
	//TLVERTEX	v[4];
	//SetVertex(v);
	//
	////	シェーダーの適用
	//u32 pass = shader->Begine(name);
	//
	//for (u32 p = 0; p<pass; p++)
	//{
	//	shader->BeginePass(p);
	//	shader->CommitChanges();
	//	//	レンダリング
	//	iexSystem::Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(TLVERTEX));
	//	shader->EndPass();
	//}
	//shader->End();
}