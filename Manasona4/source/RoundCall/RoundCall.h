#pragma once
#include "TDNLIB.h"
#include "../BaseEntity/Entity/EntityEnum.h"

//+-------------------------
//	���E���h�R�[��
//+-------------------------

//enum CALL_TYPE
//{
//
//};

// �x�[�X���E���h�R�[��
class BaseRoundCall
{
public:
	BaseRoundCall() 
	{
		m_bActionFlag = false;
	};
	virtual ~BaseRoundCall()
	{

	}

	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Action(){}
	virtual void Action(int roundnum){}
	virtual void Action(ENTITY_ID WinnerID){}

protected:		
	// �A�N�V�����t���O
	bool m_bActionFlag;

};

// �ŏ��̃��E���h�R�[��
class FirstRoundCall :public BaseRoundCall
{
public:
	FirstRoundCall();
	~FirstRoundCall();

	void FrameMove();
	void Update();
	void Render();
	void Action();
	void Action(int roundnum) {  Action(); };



protected:
	// ���E���h�i���o�[
	tdn2DAnim* m_pNumber;
	

	// �C���X�g�̍\����
	struct Pic
	{
		tdn2DAnim* pic;
		Vector2 pos;
		Vector2 uv;
		float rotate;
		float scale;
		void PicClean()
		{
			pic = nullptr;
			pos.x = pos.y = 0.0f;
			uv.x = uv.y = 0.0f;
			rotate = 0.0f;
			scale = 1.0f;
		}
	};

	// ���E���h���[��
	//tdn2DAnim* m_pRollBig;
	//float m_fRollBigAngle;
	Pic m_pRollBig;

	//tdn2DAnim* m_pRollMini;
	//float m_fRollMiniAngle;
	Pic m_pRollMini;

	enum { ROLL_START = 12, ROLL_END = 112 };

	Pic m_pBlackLine;

	Pic m_pFirstFont;

	// ���̃����O
	tdn2DAnim* m_pLightRing;

	// ���̐�
	tdn2DAnim* m_pLightLine;

	// 
	int m_iFrame;


};


// ���x���R�[��
class RebelCall :public BaseRoundCall
{
public:
	RebelCall();
	~RebelCall();

	void FrameMove();
	void Update();
	void Render();
	void Action();
	void Action(int roundNum);
	void MaskScreen();


protected:


	// �C���X�g�̍\����
	struct Pic
	{
		tdn2DAnim* pic;
		Vector2 pos;
		Vector2 uv;
		float rotate;
		float scale;
		void PicClean()
		{
			pic = nullptr;
			pos.x = pos.y = 0.0f;
			uv.x = uv.y = 0.0f;
			rotate = 0.0f;
			scale = 1.0f;
		}
	};

	// Rebel_Circle
	Pic m_pCenterCircle;

	Pic m_pBG;

	enum { REBEL_START = 12, REBEL_END = 119 };

	// Pic
	// ������
	Pic m_pBlackLineL;
	Pic m_pBlackLineR;

	Pic m_pFirstFont;

	// ���̃����O
	tdn2DAnim* m_pLightRing;

	// ���̐�
	tdn2DAnim* m_pLightLine;

	// ���E���h�i���o�[
	tdn2DAnim* m_pNumber;
	// ���E���h�̐���
	int m_iRoundNumber;

	// �}�X�N�p�����_�[�^�[�Q�b�g
	tdn2DObj* m_pMaskScreen;
	tdn2DObj* m_pMaskPic;

	// 
	int m_iFrame;
	


};



// �A�N�V�����R�[��
class ActionCall :public BaseRoundCall
{
public:
	ActionCall();
	~ActionCall();

	void FrameMove();
	void Update();
	void Render();
	void Action();
	void Action(int roundnum) {  Action(); };

protected:


	// �C���X�g�̍\����
	struct Pic
	{
		tdn2DAnim* pic;
		Vector2 pos;
		Vector2 uv;
		float rotate;
		float scale;
		void PicClean()
		{
			pic = nullptr;
			pos.x = pos.y = 0.0f;
			uv.x = uv.y = 0.0f;
			rotate = 0.0f;
			scale = 1.0f;
		}
	};
	
	Pic m_pBG;
	Pic m_pBGRip;

	enum { ACTION_START = 1, ACTION_END = 34, END_FRAME = 42 };

	Pic m_pFirstFont;

	// ���̃����O
	tdn2DAnim* m_pLightRing;

	// ���̐�
	tdn2DAnim* m_pLightLine;

	// 
	int m_iFrame;



};



// �t�B�j�b�V���R�[��
class FinishCall :public BaseRoundCall
{
public:
	FinishCall();
	~FinishCall();

	void FrameMove();
	void Update();
	void Render();
	void Action(ENTITY_ID WinnerID);

protected:

	// �C���X�g�̍\����
	struct Pic
	{
		tdn2DAnim* pic;
		Vector2 pos;
		Vector2 uv;
		float rotate;
		float scale;
		void PicClean()
		{
			pic = nullptr;
			pos.x = pos.y = 0.0f;
			uv.x = uv.y = 0.0f;
			rotate = 0.0f;
			scale = 1.0f;
		}
	};

	enum { FINISH_START = 8, FINISH_END = 138, END_FRAME = 150 };

	Pic m_pBG;
	Pic m_pBGRip;
	Pic m_pFont;

	// ���̐�
	tdn2DAnim* m_pLightLine;
	
	// 
	int m_iFrame;

	// ���������ID
	ENTITY_ID m_WinnerID;
};



// �I�[�o�[�h���C�u�t�B�j�b�V���R�[��
class OverDriveFinishCall :public BaseRoundCall
{
public:
	OverDriveFinishCall();
	~OverDriveFinishCall();

	void FrameMove();
	void Update();
	void Render();
	void Action(ENTITY_ID WinnerID);

protected:

	// �C���X�g�̍\����
	struct Pic
	{
		tdn2DAnim* pic;
		Vector2 pos;
		Vector2 uv;
		float rotate;
		float scale;
		void PicClean()
		{
			pic = nullptr;
			pos.x = pos.y = 0.0f;
			uv.x = uv.y = 0.0f;
			rotate = 0.0f;
			scale = 1.0f;
		}
	};

	enum { OVER_FINISH_START = 18, OVER_FINISH_END = 138, OVER_END_FRAME = 150 };

	Pic m_pBG;
	Pic m_pBGRip;
	Pic m_pFont;

	// ���̐�
	tdn2DAnim* m_pLightLine;

	Pic m_pSircle;

	// 
	int m_iFrame;

	// ���������ID
	ENTITY_ID m_WinnerID;
};

// �^�C���A�b�v�R�[��
class TimeUpCall :public BaseRoundCall
{
public:
	TimeUpCall();
	~TimeUpCall();

	void FrameMove();
	void Update();
	void Render();
	void Action(ENTITY_ID WinnerID);

protected:

	// �C���X�g�̍\����
	struct Pic
	{
		tdn2DAnim* pic;
		Vector2 pos;
		Vector2 uv;
		float rotate;
		float scale;
		void PicClean()
		{
			pic = nullptr;
			pos.x = pos.y = 0.0f;
			uv.x = uv.y = 0.0f;
			rotate = 0.0f;
			scale = 1.0f;
		}
	};

	enum { OVER_FINISH_START = 18, OVER_FINISH_END = 138, OVER_END_FRAME = 150 };

	Pic m_pBG;
	Pic m_pBGRip;
	Pic m_pFont;

	// ���̐�
	tdn2DAnim* m_pLightLine;

	Pic m_pSircle;

	// 
	int m_iFrame;

	// ���������ID
	ENTITY_ID m_WinnerID;
};


// �h���[�R�[��
class DrawCall :public BaseRoundCall
{
public:
	DrawCall();
	~DrawCall();

	void FrameMove();
	void Update();
	void Render();
	void Action();

protected:

	// �C���X�g�̍\����
	struct Pic
	{
		tdn2DAnim* pic;
		Vector2 pos;
		Vector2 uv;
		float rotate;
		float scale;
		void PicClean()
		{
			pic = nullptr;
			pos.x = pos.y = 0.0f;
			uv.x = uv.y = 0.0f;
			rotate = 0.0f;
			scale = 1.0f;
		}
	};

	enum { OVER_FINISH_START = 18, OVER_FINISH_END = 138, OVER_END_FRAME = 150 };

	Pic m_pBG;
	Pic m_pBGRip;
	Pic m_pFont;

	// ���̐�
	tdn2DAnim* m_pLightLine;

	Pic m_pSircle;

	// 
	int m_iFrame;
};