#pragma once

#include "../BaseEntity/Message/Message.h"

//*****************************************************************************************************************************
//
//		カメラクラス
//
//*****************************************************************************************************************************

class TimeLineCamera
{
public:
	struct ShakeEvent
	{
		SHAKE_CAMERA_INFO ShakeInfo;
		int TriggerFrame;
		ShakeEvent &operator = (const ShakeEvent &event){ ShakeInfo = event.ShakeInfo; TriggerFrame = event.TriggerFrame; return *this; }
	}*pShakeEventArray;
	int NumShakeEvent;
	void PartitioningShakeEvent(int num)
	{
		NumShakeEvent = num;

		// 既存のバッファの開放
		if (pShakeEventArray) { delete[] pShakeEventArray; }

		if (num == 0)
		{
			pShakeEventArray = nullptr;
			return;
		}

		//配列確保
		pShakeEventArray = new ShakeEvent[num];
	}

	int ID;			// 自分のパターン番号
	int NumPattern;	// データの持ってる数
	class Pattern
	{
	public:
		int EndFrame;			// タイムライン上での終了フレーム
		//Vector3 *pPosArray;		// 座標(開始と終わりだけでなく、中間とかも作るので可変長)
		//Vector3 *pTargetArray;	// 注視点(同じく可変長)
		//float *pRollArray;		// Roll値(同じく)
		ViewData *pViewDataArray;
		int NumPoints;		// 上記可変長の要素数
		float FrameSpeed;		// フレーム加算速度
		bool fix;
		Pattern() :pViewDataArray(nullptr){}
		~Pattern(){ ClearArrays(); }
		void InstanceArrays(){ ClearArrays(); pViewDataArray = new ViewData[NumPoints]; }
	private:
		void ClearArrays(){ if (pViewDataArray){ delete[] pViewDataArray; } }
	}*pPatterns;

	TimeLineCamera() :pPatterns(nullptr), NumPattern(0), NumShakeEvent(0), pShakeEventArray(nullptr){}
	~TimeLineCamera()
	{
		if (pPatterns){delete[] pPatterns;} 
		if(pShakeEventArray)
		{
			delete[] pShakeEventArray;
		}
	}
};

class Camera;

class EffectCamera
{
private:
	//------------------------------------------------------
	//	スクリプト関係
	//------------------------------------------------------
	int m_CurrentScriptID;	// 今再生しているカメラスクリプトID(m_listの添え字の役割)
	int m_PatternCursor;		// パターンのカーソル(PatternCursor >= NumPattern)だったら終了
	int m_EventFrame;	// 振動とか用のイベント時間
	int m_EventCursor;	// イベントのカーソル
	float m_CurrentFrame;	// 現在のフレーム数
	bool m_bAction;			// アクションフラグ
	Vector3 m_vOrgPos;
	std::vector<TimeLineCamera*> m_list;

	//------------------------------------------------------
	//	制御するカメラさん
	//------------------------------------------------------
	//Camera *m_pCamera;
	ViewData *m_pViewData;

	void GetTimeLineViewData(ViewData *out);

public:

	//------------------------------------------------------
	//	初期化・解放
	//------------------------------------------------------
	EffectCamera(ViewData *pViewData, LPCSTR filename, const Vector3 &OrgPos=VECTOR_ZERO);
	~EffectCamera();

	//------------------------------------------------------
	//	更新
	//------------------------------------------------------
	void Update();

	//------------------------------------------------------
	//	行動セット
	//------------------------------------------------------
	void Start(int id);

	bool isAction(){ return m_bAction; }
	int GetEventTime(){ return m_EventFrame; }
};