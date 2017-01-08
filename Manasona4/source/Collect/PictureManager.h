#pragma once
#include "BaseEntity\Entity\BaseGameEntity.h"
#include "BasePicture.h"

//+--------------------------
//	ピクチャーの管理クラス
//+--------------------------

// ピクチャーの種類
enum class PICTURE_TYPE
{
	AIROU,				//	アイル―
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

	//// 1つしかない実体
	//static PictureManager *m_pInstance;

public:
	//// 実体取得
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

	// 全データさよなら					
	void AllReset();

	// BaseGameEntiryサブクラスはメッセージを使って通信する
	bool  HandleMessage(const Message& msg);

	// アクセサ

	// イラストのアイコンの中身
	PICTURE_ICON_DATA GetPictureIconData(int iPictureType) { return m_pPicture[iPictureType]->GetPictureIcon(); }
	// イラストの中身
	PICTURE_DATA GetPictureData(int iPictureType) { return m_pPicture[iPictureType]->GetPicture(); }

	BasePicture* GetPicture(int iPictureType) { return m_pPicture[iPictureType]; };

	// イラスト最大数
	int GetMaxPictureNum() { return (int)PICTURE_TYPE::ARRAY_END; };
	// イラスト所持数
	int GetPictureOwned();

	PICTURE_TYPE GetSelectType() { return m_eSelectPicture; }
	void SetSelectType(PICTURE_TYPE eSelectType) { m_eSelectPicture = eSelectType; }

private:

	// イラスト達
	BasePicture* m_pPicture[(int)PICTURE_TYPE::ARRAY_END];

	// 選択しているイラスト
	PICTURE_TYPE m_eSelectPicture;

	// ロックアイコン
	tdn2DObj* m_pRockIcon;

};

// 実体化
//#define PictureMgr (PictureManager::GetInstance())