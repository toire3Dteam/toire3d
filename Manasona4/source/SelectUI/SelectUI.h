#pragma once
#include "TDNLIB.h"
// �G���e�B�e�B�֘A�̃C���N���[�h
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"

// �Z���N�g�V�[���ŃL���������X�e�[�W�Ȃǂ̏��
#include "../Data/SelectData.h"

// �t�����h�p
#include "SelectUIState.h"


//+--------------------------
//	�L�����N�^�[�Z���N�gUI
//+--------------------------
struct SelectPicture
{
	tdn2DAnim* pPic;
	int iX, iY;
};

class SelectUI : public BaseGameEntity
{
public:
	SelectUI(SIDE side, ENTITY_ID id,int DeviceID);
	void InitCharcterPos();
	void InitPartnerPos();
	~SelectUI();

	//����
	void Update();
	//�`��
	void Render();
	
	void ActionChara();
	void ActionCharaPlate();

	void ActionFirstStep();
	void StopFirstStep();

	void ActionPartner();
	void ActionSecondStep();
	void BackSecondStep();

	void ActionOKStep();
	void BackOKStep();

	void MiddleAction();

	// �X�e�[�g�}�V���擾
	StateMachine<SelectUI> *GetFSM() { return m_pStateMachine; }

	// ���b�Z�[�W��M(BaseEntity�ɂ��I�[�o�[���C�h)
	bool  HandleMessage(const Message& msg) { return m_pStateMachine->HandleMessage(msg); }

	int GetSelectCharacter()
	{
		return m_iSelectCharacterNo;
	}

	int GetSelectPartner()
	{
		return m_iSelectPartnerNo;
	}

private:

	StateMachine<SelectUI> *m_pStateMachine;	// �X�e�[�g�}�V��

	// ���삷��R���g���[���[�̃f�o�C�X
	int m_iDeviceID;

	// �L�����N�^�[�Z���N�g�E�p�[�g�i�[�Z���N�g�ԍ�
	int m_iSelectCharacterNo;
	int m_iSelectPartnerNo;

	// �L�����N�^�[�T�C�h
	SIDE m_eSide;

	tdn2DAnim* m_pCharaIconRip[(int)CHARACTER::END];// �L�����N�^�[���̃A�C�R���̔g��

	SelectPicture m_tagCharaPic[(int)CHARACTER::END];// �L�����N�^�[���̉摜
	SelectPicture m_tagCharaPicRip[(int)CHARACTER::END];// �L�����N�^�[���̔g��p�摜

	SelectPicture m_tagCharaInfo[(int)CHARACTER::END];// �L�����N�^�[���̐���
	SelectPicture m_tagCharaName[(int)CHARACTER::END];// �L�����N�^�[���̖��O

	SelectPicture m_tagPartnerPic[(int)PARTNER::END];// �p�[�g�i�[���̉摜
	SelectPicture m_tagPartnerInfo[(int)PARTNER::END];// �p�[�g�i�[���̐���
	SelectPicture m_tagPartnerName[(int)PARTNER::END];// �p�[�g�i�[���̖��O
	SelectPicture m_tagPartneSirclePic[(int)PARTNER::END];// �p�[�g�i�[���̍Ō�̉摜

	// �ЂƂ������Ă����
	SelectPicture m_tagInfoBox;

	// ���O�n
	SelectPicture m_tagNamePlate;
	SelectPicture m_tagAndPlate;
	SelectPicture m_tagPartnerPlate;
	SelectPicture m_tagPartnerSirclePlate;
	float m_fPartnerPlateAngle;

	// �I���J�[�\��
	SelectPicture m_pSelect;

	// �^�C�}�[
	int m_iWaitFrame;

	// �X�e�[�g�}�V���͒����G���悤��
	// �t�����h
	friend class SelectUIState::Intro;
	friend class SelectUIState::FirstStep;
	friend class SelectUIState::MiddleStep;
	friend class SelectUIState::SecondStep;
	friend class SelectUIState::SecondToOKStep;
	friend class SelectUIState::OKStep;

};
