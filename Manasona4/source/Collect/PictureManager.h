#pragma once
#include "BaseEntity\Entity\BaseGameEntity.h"
#include "BasePicture.h"

//+--------------------------
//	�s�N�`���[�̊Ǘ��N���X
//+--------------------------

// �s�N�`���[�̎��
enum class PICTURE_TYPE
{
	REST,				//	�x�e��
	GATHERING,			//	�W���ʐ^
	STYLISH,			//	�X�^�C���b�V��
	COOL,				//	�����������|�[�Y
	ALLEY,				//	�H�n
	SAD,				//	���Ȃ���
	SEXY,				//  �Z�N�V�[
	SEA,				//  �C
	AIROU,				//	�A�C���\
	TEKI,				//	�e�L
	NAZENARABA,			//	��
	ARRAY_END
};

class PictureManager : public BaseGameEntity
{
public:

	PictureManager();
	~PictureManager();

	void InitExe(PICTURE_TYPE eSelectType);
	void UpdateExe();
	void CtrlExe(int iDevice);
	void RenderExe();
	void RenderExeInfo(int iPictureType, int iX, int iY);
	void RereaseExe();
	//void RenderExeInfo(int iPictureType, int iX, int iY);

	void RenderIcon(int iPictureType, int iX, int iY);
	void RenderIconInfo(int iPictureType, int iX, int iY);

	// �S�f�[�^����Ȃ�					
	void AllReset();

	// BaseGameEntiry�T�u�N���X�̓��b�Z�[�W���g���ĒʐM����
	bool  HandleMessage(const Message& msg);

	// �A�N�Z�T

	// �C���X�g�̃A�C�R���̒��g
	PICTURE_ICON_DATA GetPictureIconData(int iPictureType) { return m_pPicture[iPictureType]->GetPictureIcon(); }
	// �C���X�g�̒��g
	PICTURE_DATA GetPictureData(int iPictureType) { return m_pPicture[iPictureType]->GetPicture(); }

	BasePicture* GetPicture(int iPictureType) { return m_pPicture[iPictureType]; };

	// �C���X�g�ő吔
	int GetMaxPictureNum() { return (int)PICTURE_TYPE::ARRAY_END; };
	// �C���X�g������
	int GetPictureOwned();

	PICTURE_TYPE GetSelectType() { return m_eSelectPicture; }
	void SetSelectType(PICTURE_TYPE eSelectType) { m_eSelectPicture = eSelectType; }

private:

	// �C���X�g�B
	BasePicture* m_pPicture[(int)PICTURE_TYPE::ARRAY_END];

	// �I�����Ă���C���X�g
	PICTURE_TYPE m_eSelectPicture;

	// ���b�N�A�C�R��
	tdn2DObj* m_pRockIcon;

};

// ���̉�
//#define PictureMgr (PictureManager::GetInstance())