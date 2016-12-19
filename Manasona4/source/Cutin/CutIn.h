#pragma once
#include "TDNLIB.h"
#include "BaseEntity\Entity\BaseGameEntity.h"

//+--------------------
//	�J�b�g�C�����o�^�C�v
//+--------------------
namespace CutInType
{
	class Base
	{
	public:
		Base::Base() { m_bAction = false; }
		virtual Base::~Base() {}

		virtual void Update() = 0;
		virtual void Render() = 0;	
		virtual  void Action() { m_bAction = true; };

	protected:
		//�@�t���O
		bool m_bAction;

		// �C���X�g�̍\����
		struct Pic
		{
			tdn2DAnim* pic;
			Vector2 pos;
			Vector2 uv;
			float rotate;

			void PicClean()
			{
				pic = nullptr;
				pos.x = pos.y = 0.0f;
				uv.x = uv.y = 0.0f;
				rotate = 0.0f;
			}
		};

	};

	// �A�C���\�J�b�g�C��
	class Airou :public Base
	{
	public:
		Airou();
		~Airou();

		void Update();
		void Render();
		void Action();

	private:
		Pic m_pPic;
		Pic m_pPicShink;
		
		Pic m_pPicBackRip;
		Pic m_pPicBackRip2;

		tdn2DObj* m_pBackCol;
	};

	// �Ă��J�b�g�C��
	class Teki :public Base
	{
	public:
		Teki();
		~Teki();

		void Update();
		void Render();
		void Action();

	private:
		Pic m_pPic;
		Pic m_pPicShink;

		Pic m_pPicBackRip;
		Pic m_pPicBackRip2;

		tdn2DObj* m_pBackCol;
	};

	// �Ȃ��Ȃ�J�b�g�C��
	class Nazenara :public Base
	{
	public:
		Nazenara();
		~Nazenara();

		void Update();
		void Render();
		void Action();

	private:
		Pic m_pPic;
		Pic m_pPicShink;

		Pic m_pPicBackRip;
		Pic m_pPicBackRip2;

		tdn2DObj* m_pBackCol;
	};

}




//+--------------------
//	�J�b�g�C��
//+--------------------

enum class CUTIN_TYPE_NAME
{
	AIROU, TEKI, NAZENARABA, ARRAY_END
};

class CutIn :public BaseGameEntity
{
private:
	// �V���O���g���̍�@
	CutIn();
	~CutIn();
	CutIn(const CutIn&);
	CutIn &operator=(const CutIn&) {}

	// 1�����Ȃ�����
	static CutIn *m_pInstance;
public:
	// ���̎擾
	static CutIn *GetInstance()
	{
		if (!m_pInstance) m_pInstance = new CutIn();
		return m_pInstance;
	}
	static void Rerease() {	SAFE_DELETE(m_pInstance);}

	void Update();
	void Render();

	void Action(CUTIN_TYPE_NAME);

	// �L�������Ɍp���������ꂼ��̉��o��t����
	virtual void PeformanceUpdate() {};
	virtual void PeformanceRender() {};

	bool  HandleMessage(const Message& msg);

private:
	
	// �J�b�g�C���p�̃����_�[�^�[�Q�b�g
	tdn2DObj* m_pScreen;
	float m_fRate;
	float m_fScale;
	int m_iFrame;
	bool m_bActionFlag;
	bool m_bRenderFlag;

	// �A���t�@�l�̒l
	int m_iAlphaNearFrame;
	int m_iAlphaFarFrame;
	
	// �I��̒l
	int m_iEndFrame;

	CutInType::Base* m_pCutinType[(int)CUTIN_TYPE_NAME::ARRAY_END];
	CUTIN_TYPE_NAME	m_eSelectType;

	// ����ۑ�
	ENTITY_ID m_eSaveID;

};

// ���̉�
#define CutInMgr (CutIn::GetInstance())
