#pragma once
#include "BaseEntity\Entity\BaseGameEntity.h"
#include "BasePicture.h"

//+--------------------------
//	ピクチャーの管理クラス
//+--------------------------

// ピクチャーの種類
enum class PICTURE_TYPE
{
	REST,				//	休憩中
	GATHERING,			//	集合写真
	STYLISH,			//	スタイリッシュ
	COOL,				//	かっこいいポーズ
	ALLEY,				//	路地
	SAD,				//	かなしい
	SEXY,				//  セクシー
	SEA,				//  海
	AIROU,				//	アイル―
	TEKI,				//	テキ
	NAZENARABA,			//	∵
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