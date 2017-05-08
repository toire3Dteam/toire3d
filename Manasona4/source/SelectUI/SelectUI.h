#pragma once
#include "TDNLIB.h"
// エンティティ関連のインクルード
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"

// セレクトシーンでキャラ数やらステージなどの情報
#include "../Data/SelectData.h"

// フレンド用
#include "SelectUIState.h"

#define ICON_HEIGHT  -120
#define MOVING_UP  5

//+--------------------------
//	キャラクターセレクトUI
//+--------------------------
struct SelectPicture
{
	tdn2DAnim* pPic;
	int iX, iY;
};

class SelectUI : public BaseGameEntity
{
public:
	SelectUI(SIDE side, ENTITY_ID id, int DeviceID, bool bAI);
	void InitCharcterPos();
	void InitPartnerPos();
	~SelectUI();

	// 処理
	void Update(bool bControl = true);
	// 描画
	void Render();
	// キャラクター描画
	void RenderCharacter();

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

	void SecondToOKAction();

	// ステートマシン取得
	StateMachine<SelectUI> *GetFSM() { return m_pStateMachine; }

	// メッセージ受信(BaseEntityによるオーバーライド)
	bool  HandleMessage(const Message& msg) { return m_pStateMachine->HandleMessage(msg); }

	// ★★★選択したキャラとパートナーを取得するアクセサ
	CHARACTER GetSelectCharacter()
	{
		return (CHARACTER)m_iSelectCharacterNo;
	}

	PARTNER GetSelectPartner()
	{
		return (PARTNER)m_iSelectPartnerNo;
	}

	COLOR_TYPE GetSelectCharacterColor()
	{
		return (COLOR_TYPE)m_iSelectCharacterColor;
	}

	bool IsOK() { return m_bOK; }
	bool IsAI() { return m_bAI; }

	int GetDevice() { return m_iDeviceID; }

private:
	int m_iWidthIcon; // 
	int m_iIconX;

	StateMachine<SelectUI> *m_pStateMachine;	// ステートマシン

	// 操作するコントローラーのデバイス
	int m_iDeviceID;

	// キャラクターセレクト・パートナーセレクト番号
	int m_iSelectCharacterNo;
	int m_iSelectPartnerNo;

	// キャラクターカラー
	int m_iSelectCharacterColor;

	// キャラクターサイド
	SIDE m_eSide;

	tdn2DAnim* m_pCharaIconRip[(int)CHARACTER::END];// キャラクター分のアイコンの波紋

	SelectPicture m_tagCharaPic[(int)CHARACTER::END];// キャラクター分の画像
	SelectPicture m_tagCharaPicRip[(int)CHARACTER::END];// キャラクター分の波紋用画像

	SelectPicture m_tagCharaInfo[(int)CHARACTER::END];// キャラクター分の説明
	SelectPicture m_tagCharaName[(int)CHARACTER::END];// キャラクター分の名前

	SelectPicture m_tagPartnerPic[(int)PARTNER::END];// パートナー分の画像
	SelectPicture m_tagPartnerPicRip[(int)PARTNER::END];// パートナー分の波紋
	SelectPicture m_tagPartnerInfo[(int)PARTNER::END];// パートナー分の説明
	SelectPicture m_tagPartnerName[(int)PARTNER::END];// パートナー分の名前
	SelectPicture m_tagPartneSirclePic[(int)PARTNER::END];// パートナー分の最後の画像

	// ひとつずつ持ってるもの
	SelectPicture m_tagInfoBox;

	// 名前系
	SelectPicture m_tagNamePlate;
	SelectPicture m_tagAndPlate;
	SelectPicture m_tagPartnerPlate;
	SelectPicture m_tagPartnerSirclePlate;
	float m_fPartnerPlateAngle;

	// 選択カーソル
	SelectPicture m_pSelect;

	// 最後のOK!の文字
	SelectPicture m_tagOKfont;

	// カラータイプ
	SelectPicture m_tagColorType2P;
	SelectPicture m_tagColorTypeEX;

	// タイマー
	int m_iWaitFrame;
	
	// OKフラグ
	bool m_bOK;

	// AIか
	bool m_bAI;

	//このOKふらぐでうんたら
	// ステートマシンは中も触れるように
	// フレンド
	friend class SelectUIState::Intro;
	friend class SelectUIState::FirstStep;
	friend class SelectUIState::MiddleStep;
	friend class SelectUIState::SecondStep;
	friend class SelectUIState::SecondToOKStep;
	friend class SelectUIState::OKStep;
	friend class SelectUIState::OKToSecondStep;
};
