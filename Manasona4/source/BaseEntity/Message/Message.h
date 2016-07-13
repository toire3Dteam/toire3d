#pragma once
#include "TDNLIB.h"
#include "../Entity/EntityEnum.h"

// メッセージタイプ
enum MESSAGE_TYPE
{
	EFFECT_CAMERA,	// エフェクトカメラ発動メッセージ
};

enum class PERSON_TYPE;	// 前方宣言
// EX_INFO用の構造体
struct RIPPLE_INFO
{
	PERSON_TYPE type;
	Vector3 pos;
	float size;
};

// プレイヤーInfo
struct PLAYER_INFO
{
	Vector3 pos;
	ENTITY_ID id;
};

/* エフェクトカメラ発動時に送る時の構造体 */
struct EFFECT_CAMERA_INFO
{
	int scriptID;			// カメラスクリプトのID
	Vector3 *pOrgPos;		// エフェクトカメラの原点の座標(nullptrならVector3(0,0,0))
	EFFECT_CAMERA_INFO() :scriptID(0), pOrgPos(nullptr){}
};

// イントロ発動メッセージ送信のときにいれる構造体
struct INTRO_CAMERA_INFO
{
	Vector3 start_person_pos;	// うわさを流す人間の座標
};

/*
	メッセージはシンプルな列挙型。
	内容はなんでもあり。
	
	・誰が
	・誰に
	・メッセージの列挙
	・メッセージの追加情報(Void*)
	・ディレイ用
*/

/*電報構造体*/
struct Message
{
	// このTelegramを送ったエンティティ
	ENTITY_ID          Sender;

	// このTelegramを受け取るエンティティ
	ENTITY_ID          Receiver;

	// メッセージの内容。　（列挙型に設定されているもの）
	int          Msg;

	// メッセージの追加情報 (キャラクターが動く→ [追加情報] Xに２移動 )
	// 追加情報を事前に知ることが不可能なのでVoid*で提供
	void*        ExtraInfo;

	// メッセージの遅延時間
	double       DispatchTime;

	Message() :DispatchTime(-1),
		Sender(ENTITY_ID::ID_ERROR),
		Receiver(ENTITY_ID::ID_ERROR),
		Msg(-1)
	{}

	Message(double time,
		ENTITY_ID    sender,
		ENTITY_ID    receiver,
		int    msg,
		void*  info = NULL) : DispatchTime(time),
		Sender(sender),
		Receiver(receiver),
		Msg(msg),
		ExtraInfo(info)
	{}

};