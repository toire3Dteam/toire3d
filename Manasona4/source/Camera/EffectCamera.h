#pragma once

//*****************************************************************************************************************************
//
//		カメラクラス
//
//*****************************************************************************************************************************

class TimeLineCamera
{
public:
	int ID;			// 自分のパターン番号
	int NumPattern;	// データの持ってる数
	struct Pattern
	{
		int EndFrame;			// タイムライン上での終了フレーム
		Vector3 *pPosArray;		// 座標(開始と終わりだけでなく、中間とかも作るので可変長)
		Vector3 *pTargetArray;	// 注視点(同じく可変長)
		int NumPoints;		// 上記可変長の要素数
		bool fix;
		Pattern() :pPosArray(nullptr), pTargetArray(nullptr){}
		~Pattern(){ if (pPosArray){ delete[] pPosArray; }if (pTargetArray){ delete[] pTargetArray; } }
	}*pPatterns;

	TimeLineCamera() :pPatterns(nullptr), NumPattern(0){}
	~TimeLineCamera(){ if (pPatterns)delete[] pPatterns; }
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
	float m_CurrentFrame;	// 現在のフレーム数
	std::vector<TimeLineCamera*> m_list;

	//------------------------------------------------------
	//	制御するカメラさん
	//------------------------------------------------------
	Camera *m_pCamera;

	void GetTimeLineCameraPos(Vector3 *out_pos, Vector3 *out_target);

public:

	//------------------------------------------------------
	//	初期化・解放
	//------------------------------------------------------
	EffectCamera(Camera *me);
	~EffectCamera();

	//------------------------------------------------------
	//	更新
	//------------------------------------------------------
	void Update();

	//------------------------------------------------------
	//	行動セット
	//------------------------------------------------------
	void Start(int id);
};