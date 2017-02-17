#pragma once
#include "TDNLIB.h"
#include "Data\SelectData.h"
#include "BaseEntity\Entity\BaseGameEntity.h"
#include "BaseEntity/State/StateMachine.h"
#include "BattleLoadingState.h"

//+---------------------------
//	バトル前ロード画面
//+---------------------------

// スライド用
struct SlideDesc
{
	tdn2DAnim*	pPic;
	float		fScale;
	Vector2		vStartPos;
	Vector2		vPos;
	Vector2		vMove;
	float		fAlpha;
	bool		bEnd;

	SlideDesc::SlideDesc()
	{
		pPic = nullptr;
		fScale = 1.0f;
		vStartPos = VECTOR2_ZERO;
		vPos = VECTOR2_ZERO;
		vMove= VECTOR2_ZERO;
		fAlpha = 1.0f;
		bEnd = false;
	}

	// 更新
	void Update();
	void Render(RS eRS = RS::COPY);
	void RenderSP();

	void Action(int iDelayTimer = 0);
	// 透明にしていく
	void End() { bEnd = true; }		 
	// 座標加算
	void AddPos(Vector2 vAdd) { vPos += vAdd; };
};


// スライドキャラ用
struct SlideCharaDesc
{
	tdn2DAnim*	pPic;
	tdn2DAnim*	pNamePic;
	float		fScale;
	Vector2		vStartPos;
	Vector2		vPos;
	Vector2		vMove;
	float		fAlpha;
	bool		bEnd;

	SlideCharaDesc::SlideCharaDesc()
	{
		pPic = nullptr;
		pNamePic = nullptr;
		fScale = 1.0f;
		vStartPos = VECTOR2_ZERO;
		vPos = VECTOR2_ZERO;
		vMove = VECTOR2_ZERO;
		fAlpha = 1.0f;
		bEnd = false;
	}

	// 更新
	void Update();
	void Render();
	void RenderName();

	void Action(int iDelayTimer = 0);
	// 透明にしていく
	void End() { bEnd = true; }

};

// 最後のキャラ用
struct FinalCharaDesc
{
	tdn2DAnim*	pPic;
	float		fScale;
	Vector2		vStartPos;
	Vector2		vPos;
	Vector2		vMove;
	bool		bEnd;

	FinalCharaDesc::FinalCharaDesc()
	{
		pPic = nullptr;
		fScale = 1.0f;
		vStartPos = VECTOR2_ZERO;
		vPos = VECTOR2_ZERO;
		vMove = VECTOR2_ZERO;
		bEnd = false;
	}

	// 更新
	void Update();
	void Render();

	void Action(int iDelayTimer = 0);
	void End() { bEnd = true; }

};


class BattleLoading :public BaseGameEntity
{
private:
	// シングルトンの作法
	BattleLoading();
	~BattleLoading();
	 BattleLoading(const BattleLoading&);
	 BattleLoading &operator=(const BattleLoading&) {}

	 // 1つしかない実体
	 static BattleLoading *m_pInstance;
public:
	// 実体取得
	static BattleLoading *GetInstance()
	{
		if (!m_pInstance) m_pInstance = new BattleLoading;
		return m_pInstance;
	}
	static void Rerease() 
	{
		SAFE_DELETE(m_pInstance);
	}

	// 更新・描画
	void Update();
	void Render();
	void MaskRender();
	void MaskRender(FinalCharaDesc pFinalChara);

	//------------------------
	//	アクセサー
	//------------------------
	StateMachine<BattleLoading> *GetFSM() { return m_pStateMachine; }
	// メッセージ受信(BaseEntityによるオーバーライド)
	bool  HandleMessage(const Message& msg) { return m_pStateMachine->HandleMessage(msg); }


	// 画像群
	enum IMAGE
	{
		BLACK_LINE,
		S_UP_FRAME,
		S_DOWN_FRAME,
		S_VS,
		S_1PLAYER,
		S_2PLAYER,
		S_1PBACK,
		S_2PBACK,
		F_BACK,
		F_VERSUS,
		F_FRAME,
		F_FLASH,
		F_FLASH_BACK,
		F_BLACK_CIRCLE,
		ARRAY_END
	};

private:
	// ステートマシン
	StateMachine<BattleLoading> *m_pStateMachine;

	// スライド用キャラクターデータ
	SlideCharaDesc m_tagSlideChara1P[(int)CHARACTER::END];
	SlideCharaDesc m_tagSlideChara2P[(int)CHARACTER::END];
		
	SlideDesc m_pImages[IMAGE::ARRAY_END];
	
	// 最後のキャラクターデータ
	FinalCharaDesc m_tagFinalChara1P[(int)CHARACTER::END];
	FinalCharaDesc m_tagFinalChara2P[(int)CHARACTER::END];

	// マスク
	tdn2DObj* m_pMask1P;
	tdn2DObj* m_pMask2P;
	tdn2DObj* m_pMaskSurface;

	// シーン用のフレーム
	int m_iSceneFrame;

	Vector2 m_vCirclePos;

	//+------------------------------------
	// ステートをフレンドクラスへ
	//+------------------------------------
	friend class BattleLoadingState::Intro;
	friend class BattleLoadingState::Slide1P;
	friend class BattleLoadingState::Slide2P;
	friend class BattleLoadingState::SlideEnd;
	friend class BattleLoadingState::FinalStep;
	friend class BattleLoadingState::FadeChangeStep;

};

// 実体化
#define BattleLoadInst (BattleLoading::GetInstance())