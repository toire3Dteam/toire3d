#pragma once
#include "BaseEntity\Entity\BaseGameEntity.h"
#include "BasePicture.h"

//+--------------------------
//	�s�N�`���[�̊Ǘ��N���X
//+--------------------------

// �s�N�`���[�̎��
enum class PICTURE_TYPE
{
	AIROU,				//	�A�C���\
						_6,
						_7, _8, _9, _10, _11, _12,
						_13, _14, _15, _16, _17, _18,
	_26,
	_27, _28, _29, _210, _211, _212,
	_213, _214, _215, _216, _217, _218,
	ARRAY_END
};

class PictureManager : public BaseGameEntity
{
//private:
	//PictureManager();
	//~PictureManager();
	//PictureManager(const PictureManager&);
	//PictureManager &operator=(const PictureManager&) {}

	//// 1�����Ȃ�����
	//static PictureManager *m_pInstance;

public:
	//// ���̎擾
	//static PictureManager *GetInstance()
	//{
	//	if (!m_pInstance) m_pInstance = new PictureManager();
	//	return m_pInstance;
	//}
	//static void Rerease() {
	//	SAFE_DELETE(m_pInstance);
	//}

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