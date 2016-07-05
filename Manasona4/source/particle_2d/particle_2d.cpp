
#include "TDNLIB.h"
#include "particle_2d.h"


//*****************************************************************************
//		パーティクル管理
//*****************************************************************************
Particle_2d *Particle2dManager::m_pParticle2d = nullptr;
/* 基本関数 */
void Particle2dManager::Initialize(char *filename, int nParticles, int uindex, int vindex){ if (!m_pParticle2d){ m_pParticle2d = new Particle_2d; }m_pParticle2d->Initialize(filename, nParticles, uindex, vindex); }
void Particle2dManager::Release(){ SAFE_DELETE(m_pParticle2d); }
void Particle2dManager::Update(){ assert(m_pParticle2d); m_pParticle2d->Update(); }
void Particle2dManager::Render(){ assert(m_pParticle2d); m_pParticle2d->Render(); }


/* ここからパーティクルセットの関数 */
//*****************************************************************************
//		きらきら(移動なし)
//*****************************************************************************
void Particle2dManager::Effect_KiraKira(const Vector2 &pos, const Vector2 &range, float scale, float ScaleFluctuation, int LoopCount, int EndFrame)
{
	FOR(LoopCount)
		m_pParticle2d->Set(3, 0, 1.0f, EndFrame, .0f, (int)(EndFrame * 0.65f), 0.75f, pos + Vector2((float)tdnRandom::Get((int)-range.x, (int)range.x), (float)tdnRandom::Get((int)-range.y, (int)range.y)), Vector2(0, 0), Vector2(0, 0), 1.0f, 1.0f, 1.0f, scale + tdnRandom::Get(-ScaleFluctuation, ScaleFluctuation), RS::ADD);
}

//*****************************************************************************
//		砂煙
//*****************************************************************************
void Particle2dManager::Effect_SandCloud(const Vector2 &pos)
{
	m_pParticle2d->Set(15, 0, 0x08191919, 90, 0x00000000, 25, 0x0cffffff,
		pos + Vector2((rand() % 25) - 12.0f, (rand() % 25) - 12.0f),
		Vector2((rand() % 5 - 2.5f)*0.5f, (rand() % 2 + .0f) * -0.5f),
		Vector2(0, -.001f),
		.01f, 1.01f, 32.0f, RS::COPY);
}

//*****************************************************************************
//		リアル羊のオーラ
//*****************************************************************************
void Particle2dManager::Effect_RealSheep(const Vector2 &pos)
{
	m_pParticle2d->Set(15, 0, 0x80550055, 45, 0x00000000, 25, 0x0c110011,
		pos + Vector2((rand() % 25) - 12.0f, (rand() % 25) - 12.0f),
		Vector2((rand() % 4 - 1.5f)*0.5f, (rand() % 3 + .0f) * -0.5f),
		Vector2(-.1f, -.001f),
		-.05f, 1.05f, 64.0f, RS::COPY);
}

//*****************************************************************************
//		火の粉的な
//*****************************************************************************
void Particle2dManager::Effect_Hinoko(const Vector2 &pos)
{
	const Vector2 move = Vector2((rand() % 20 - 10.0f)*0.5f, (rand() % 5 + 1.0f) * -0.5f);
	m_pParticle2d->Set(9, 0, 0xffffffff, 90, 0x00000000, 45, 0x0cffffff,
		pos + Vector2((float)tdnRandom::Get(-15, 15), (float)tdnRandom::Get(-15, 15)),
		move,
		Vector2(-move.x * 0.01f, -move.y * 0.00001f),
		(rand() % 10) * .01f, 1.0f, 12.0f - (rand() % 12), RS::COPY);
}

//*****************************************************************************
//		デブ煙
//*****************************************************************************
void Particle2dManager::Effect_FatSmoke(const Vector2 &pos, float scale, int LoopCount)
{
	FOR(LoopCount)
	{
		m_pParticle2d->Set(15, 0, 0x88ffffff, 75, 0x01ffffff, 25, 0x0cffffff,
			pos + Vector2((rand() % 129) - 64.0f, (rand() % 129) - 64.0f),
			Vector2((rand() % 5 - 2.5f)*0.5f, (rand() % 5 + .0f) * -0.5f),
			Vector2(0, -.001f),
			.01f, 1.01f, scale, RS::COPY);
	}
}

//*****************************************************************************
//		アンリミテッド
//*****************************************************************************
void Particle2dManager::Effect_Unlimited(const Vector2 &pos)
{
	const Vector2 move = Vector2((rand() % 3 - 1.5f)*0.5f, (rand() % 2 + .0f) * -0.5f);
	m_pParticle2d->Set(8, 0, 0x80ffffff, 45, 0x00000000, 20, 0x0cffffff,
		pos + Vector2((rand() % 25) - 12.0f, (rand() % 41) - 20.0f),
		Vector2((rand() % 4 - 1.5f)*0.5f, (rand() % 3 + .0f) * -0.5f),
		move*0.1f,
		.01f, 1.02f, 64.0f - (rand() % 10), RS::SUB);
}

//*****************************************************************************
//		肉湯気
//*****************************************************************************
void Particle2dManager::Effect_NikuSmoke(const Vector2 &pos)
{
	m_pParticle2d->Set(15, 0, 0x08191919, 90, 0x00000000, 25, 0x0cffffff,
		pos + Vector2((rand() % 25) - 12.0f, (rand() % 25) - 12.0f),
		Vector2((rand() % 5 - 2.5f)*0.5f, (rand() % 5 + .0f) * -0.5f),
		Vector2(0, -.001f),
		.01f, 1.01f, 32.0f, RS::COPY);
}



//*****************************************************************************
//		セット
//*****************************************************************************
void Particle_2d_data::Set(LPPARTICLE_2d pd)
{
	memcpy(&pdata, pd, sizeof(PARTICLE_2d));
	CurFrame = 0;
	CurAlpha = 0;
	bFlags = 1;
}

//*****************************************************************************
//		更新
//*****************************************************************************
void Particle_2d_data::Update()
{
	float	work;
	int	a1, r1, g1, b1, a2, r2, g2, b2;

	if (bFlags == 0) return;

	//	情報更新
	pdata.pos += pdata.move;
	pdata.move += pdata.power;
	pdata.angle += pdata.rotate;
	pdata.scale *= pdata.stretch;

	//	カラー計算
	if (CurFrame < pdata.aFrame) CurAlpha = .0f;
	else {
		if (CurFrame < pdata.mFrame){
			work = (CurFrame - pdata.aFrame) / (float)(pdata.mFrame - pdata.aFrame);
			a1 = (pdata.aColor >> 24);
			r1 = ((pdata.aColor >> 16) & 0xFF);
			g1 = ((pdata.aColor >> 8) & 0xFF);
			b1 = ((pdata.aColor) & 0xFF);
		}
		else {
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
inline bool Particle_2d_data::SetVertex(TLVERTEX* v, int uindex, int vindex)
{
	bool bRender = false;

	bRender = true;

	for (int i = 0; i < 4; i++)
	{
		v[i].sz = 0;
		v[i].rhw = 1.0f;
	}


	// 原点から拡大
	float scale_2 = pdata.scale * 0.5f;
	v[0].sx = v[2].sx = (float)-scale_2;
	v[1].sx = v[3].sx = (float)scale_2;
	v[0].sy = v[1].sy = (float)-scale_2;
	v[2].sy = v[3].sy = (float)scale_2;

	//v[0].sx = -32;
	//v[0].sy = -32;

	//v[1].sx = 32;
	//v[1].sy = -32;

	//v[2].sx = -32;
	//v[2].sy = 32;

	//v[3].sx = 32;
	//v[3].sy = 32;

	// 原点から回転
	for (DWORD i = 0; i < 4; i++){
		const float xrot = v[i].sx;
		const float yrot = v[i].sy;
		v[i].sx = xrot * cos(pdata.angle) + yrot * sin(pdata.angle);
		v[i].sy = -xrot * sin(pdata.angle) + yrot * cos(pdata.angle);
	}
	
	// 移動
	v[0].sx += pdata.pos.x;
	v[2].sx += pdata.pos.x;
	v[1].sx += pdata.pos.x;
	v[3].sx += pdata.pos.x;	
	v[0].sy += pdata.pos.y;
	v[1].sy += pdata.pos.y;
	v[2].sy += pdata.pos.y;
	v[3].sy += pdata.pos.y;
	//v[0].sx = pdata.pos.x - scale_2;
	//v[0].sy = pdata.pos.y - scale_2;
	//
	//v[1].sx = pdata.pos.x + scale_2;
	//v[1].sy = pdata.pos.y - scale_2;
	//
	//v[2].sx = pdata.pos.x - scale_2;
	//v[2].sy = pdata.pos.y + scale_2;
	//
	//v[3].sx = pdata.pos.x + scale_2;
	//v[3].sy = pdata.pos.y + scale_2;

	

	if (bRender == false) return false;
	//	タイプ設定
	v[0].tu = v[2].tu = (float)(pdata.type % uindex) * (1.0f / uindex) + 0.001f;
	v[1].tu = v[3].tu = v[0].tu + (1.0f / uindex);
	v[0].tv = v[1].tv = (float)(pdata.type / vindex) * (1.0f / vindex) + 0.001f;
	v[3].tv = v[2].tv = v[0].tv + (1.0f / vindex);

	//	色設定
	v[0].color = v[1].color = v[2].color = v[3].color = ((DWORD)(CurAlpha*255.0f) << 24) | ((DWORD)(r*255.0f) << 16) | ((DWORD)(g*255.0f) << 8) | ((DWORD)(b*255.0f));

	return true;
}

//------------------------------------------------------
//		固定機能描画
//------------------------------------------------------
void	Particle_2d_data::Render(int uindex, int vindex)
{
	TLVERTEX	v[4];

	if (SetVertex(v, uindex, vindex) == false) return;
	//	合成設定
	if (pdata.flag == RS::SUB) tdnSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	else                      tdnSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//	レンダリング
	
	tdnSystem::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(TLVERTEX));
}


// 移動
void Particle_2d_data::MovePos(const Vector2 &move)
{
	pdata.pos += move;
}

///
//
///

void Particle_2d::Initialize(char* filename, int nParticles, int uindex, int vindex)
{
	this->uindex = (uindex <= 0) ? 1 : uindex;
	this->vindex = (vindex <= 0) ? 1 : vindex;

	//	パーティクルバッファ確保
	this->nParticles = nParticles;
	this->ParticleData = new Particle_2d_data[nParticles];
	//	全リセット
	Reset();
	//	テクスチャ設定
	this->lpTexture = new tdn2DObj(filename);
}

//------------------------------------------------------
//		解放
//------------------------------------------------------
void Particle_2d::Release()
{
	delete[] ParticleData;
	ParticleData = nullptr;
	delete lpTexture;
	lpTexture = nullptr;
}

//*****************************************************************************
//		データ設定
//*****************************************************************************
//------------------------------------------------------
//		構造体指定
//------------------------------------------------------
void Particle_2d::Set(LPPARTICLE_2d pd)
{
	for (int i = 0; i<nParticles; i++){
		if (ParticleData[i].bActive()) continue;
		ParticleData[i].Set(pd);
		break;
	}
}

//------------------------------------------------------
//		データ個別指定
//------------------------------------------------------
void Particle_2d::Set(int type, int aFrame, COLOR aColor, int eFrame, COLOR eColor, int mFrame, COLOR mColor,
	const Vector2 &Pos, const Vector2 &Move, const Vector2 &Power, float rotate, float stretch, float scale, u8 flag)
{
	PARTICLE_2d pd;

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
	pd.angle = .0f;

	pd.flag = flag;

	Set(&pd);
}

//------------------------------------------------------
//		データ個別指定
//------------------------------------------------------
void	Particle_2d::Set(int type, int aFrame, float aAlpha, int eFrame, float eAlpha, int mFrame, float mAlpha,
	const Vector2 &Pos, const Vector2 &Move, const Vector2 &Power,
	float r, float g, float b, float scale, u8 flag)
{
	COLOR	color;
	DWORD	aa, ea, ma;

	aa = ((DWORD)(aAlpha*255.0f) << 24);
	ea = ((DWORD)(eAlpha*255.0f) << 24);
	ma = ((DWORD)(mAlpha*255.0f) << 24);
	color = ((DWORD)(r*255.0f) << 16) | ((DWORD)(g*255.0f) << 8) | (DWORD)(b*255.0f);
	Set(type, aFrame, aa | color, eFrame, ea | color, mFrame, ma | color, Pos, Move, Power, .0f, 1.0f, scale, flag);
}

//*****************************************************************************
//		更新
//*****************************************************************************
void Particle_2d::Update()
{
	for (int i = 0; i<nParticles; i++){
		if (ParticleData[i].bActive() == FALSE) continue;
		ParticleData[i].Update();
	}
}

//*****************************************************************************
//		リセット
//*****************************************************************************
void Particle_2d::Reset()
{
	for (int i = 0; i < nParticles; i++)
	{
		if (ParticleData[i].bActive() == FALSE) continue;
		ParticleData[i].SetFlag(0);
	}
}
void Particle_2d::Reset(int num)
{
	nParticles = num;
	delete[] ParticleData;
	ParticleData = new Particle_2d_data[nParticles];

	Reset();
}

//*****************************************************************************
//		描画
//*****************************************************************************
//------------------------------------------------------
//		固定機能描画
//------------------------------------------------------
void Particle_2d::Render()
{
	int		i;
	float	scale = -1.0f;

	tdnRenderState::Set(RS::ADD, NULL, lpTexture->GetTexture());
	tdnSystem::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	tdnSystem::GetDevice()->SetFVF(D3DFVF_TLVERTEX);
	//	アルファブレンド設定
	tdnSystem::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	tdnSystem::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	tdnSystem::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	for (i = 0; i<nParticles; i++){
		//	パーティクルレンダリング
		if (ParticleData[i].bActive()) ParticleData[i].Render(uindex, vindex);
	}

	tdnSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	tdnSystem::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}



// パーティクル全体を移動
void Particle_2d::MovePos(const Vector2 &move)
{
	for (int i = 0; i<nParticles; i++){
		if (ParticleData[i].bActive() == FALSE) continue;
		ParticleData[i].MovePos(move);
	}
}