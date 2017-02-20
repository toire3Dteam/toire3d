#pragma once
#include "TDNLIB.h"
#include "Data\SelectData.h"
#include "BaseEntity\Entity\BaseGameEntity.h"
#include "BaseEntity/State/StateMachine.h"
#include "BattleLoadingState.h"

//+---------------------------
//	�o�g���O���[�h���
//+---------------------------

// �X���C�h�p
struct SlideDesc
{
	tdn2DAnim*	pPic;
	float		fScale;
	Vector2		vStartPos;
	Vector2		vPos;
	Vector2		vMove;
	float		fAlpha;
	float		fAngle;
	bool		bEnd;

	SlideDesc::SlideDesc()
	{
		pPic = nullptr;
		fScale = 1.0f;
		vStartPos = VECTOR2_ZERO;
		vPos = VECTOR2_ZERO;
		vMove= VECTOR2_ZERO;
		fAlpha = 1.0f;
		fAngle = 0.0f;
		bEnd = false;
	}

	// �X�V
	void Update();
	void Render(RS eRS = RS::COPY);
	void RenderUV(int iw,int ih,RS eRS = RS::COPY);
	void RenderUV(int iw, int ih, tdnShader* pShader, char* name);
	void RenderSP();

	void Action(int iDelayTimer = 0);
	// �����ɂ��Ă���
	void End() { bEnd = true; }		 
	// ���W���Z
	void AddPos(Vector2 vAdd) { vPos += vAdd; };
};


// �X���C�h�L�����p
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

	// �X�V
	void Update();
	void Render();
	void RenderName();

	void Action(int iDelayTimer = 0);
	// �����ɂ��Ă���
	void End() { bEnd = true; }

};

// �Ō�̃L�����p
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

	// �X�V
	void Update();
	void Render();

	void Action(int iDelayTimer = 0);
	void End() { bEnd = true; }

};


class BattleLoading :public BaseGameEntity
{
private:
	// �V���O���g���̍�@
	BattleLoading();
	~BattleLoading();
	 BattleLoading(const BattleLoading&);
	 BattleLoading &operator=(const BattleLoading&) {}

	 // 1�����Ȃ�����
	 static BattleLoading *m_pInstance;
public:
	// ���̎擾
	static BattleLoading *GetInstance()
	{
		if (!m_pInstance) m_pInstance = new BattleLoading;
		return m_pInstance;
	}


	// �������E���
	void Initialize();
	static void Rerease() 
	{
		SAFE_DELETE(m_pInstance);
	}
	// �X�V�E�`��
	void Update();
	void Render();
	void MaskRender();
	void MaskRender(FinalCharaDesc pFinalChara);
	void MaskRenderLightParticle(SlideDesc pSlide);
	
	// �����s���Œ���̕������X�V
	void LoadingCircleUpdate();

	//------------------------
	//	�A�N�Z�T�[
	//------------------------
	StateMachine<BattleLoading> *GetFSM() { return m_pStateMachine; }
	// ���b�Z�[�W��M(BaseEntity�ɂ��I�[�o�[���C�h)
	bool  HandleMessage(const Message& msg) { return m_pStateMachine->HandleMessage(msg); }


	// �摜�Q
	enum IMAGE
	{
		BLACK_LINE,
		BLUE_RING,
		LOADING_CIRCLE,
		I_INTRO_LIGHT,
		S_UP_FRAME,
		S_DOWN_FRAME,
		S_VS,
		S_1PLAYER,
		S_2PLAYER,
		S_1PBACK,
		S_2PBACK,
		S_1PBACK_SL,
		S_2PBACK_SL,
		F_BACK,
		F_VERSUS,
		F_FRAME,
		F_FLASH,
		F_FLASH_BACK,
		F_BLACK_CIRCLE,
		F_CIRCLE_1,
		F_CIRCLE_2,
		F_LIGHT_PARTICLE,
		ARRAY_END
	};

	bool isEnd() { return m_bEnd; }
	void SetEndFlag(bool bEnd) { m_bEnd = bEnd; }
	
	void SetSkipFlag(bool bSkip) { m_bSkip = bSkip; }

private:
	//------------------------------------------------------
	//	�����o�ϐ�
	//------------------------------------------------------
	bool m_bEnd;	// VS���o�I��������ǂ���

	// �X�e�[�g�}�V��
	StateMachine<BattleLoading> *m_pStateMachine;

	// �X���C�h�p�L�����N�^�[�f�[�^
	SlideCharaDesc m_tagSlideChara1P[(int)CHARACTER::END];
	SlideCharaDesc m_tagSlideChara2P[(int)CHARACTER::END];
		
	SlideDesc m_pImages[IMAGE::ARRAY_END];
	
	// �Ō�̃L�����N�^�[�f�[�^
	FinalCharaDesc m_tagFinalChara1P[(int)CHARACTER::END];
	FinalCharaDesc m_tagFinalChara2P[(int)CHARACTER::END];

	// �}�X�N
	tdn2DObj* m_pMask1P;
	tdn2DObj* m_pMask2P;
	tdn2DObj* m_pMaskSurface;
	tdn2DObj* m_pMaskLightParticle;

	tdn2DObj* m_pIntroBlackBack;
	tdn2DObj* m_pMaskIntroBlackBack;

	// �V�[���p�̃t���[��
	int m_iSceneFrame;

	Vector2 m_vCirclePos;

	float m_fFadeRate;

	bool m_bSkip;

	//+------------------------------------
	// �X�e�[�g���t�����h�N���X��
	//+------------------------------------
	friend class BattleLoadingState::Intro;
	friend class BattleLoadingState::Slide1P;
	friend class BattleLoadingState::Slide2P;
	friend class BattleLoadingState::SlideEnd;
	friend class BattleLoadingState::FinalStep;
	friend class BattleLoadingState::FadeChangeStep;

};

// ���̉�
#define BattleLoadInst (BattleLoading::GetInstance())