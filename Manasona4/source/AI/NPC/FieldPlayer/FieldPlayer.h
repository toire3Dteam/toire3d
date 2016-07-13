#pragma once
#include "TDNLIB.h"
#include "AI\NPC\BasePerson\BasePerson.h"
#include "AI\FieldGraph\FieldGraph.h"

struct TASK
{
	Vector3 pos;
	FIELD_STATE fieldState;
};

class FieldPlayer :public BasePerson
{
public:
	FieldPlayer(ENTITY_ID id, bool isAi = false);
	~FieldPlayer();

	void Update();
	void Ctrl();
	void AI();

	void Render();

	bool HandleMessage(const Message& msg);

	// アクセサ
	// ★ステートマシンのアクセサを作る
	StateMachine<FieldPlayer>* GetFSM()const { return m_pStateMachine; }

	FieldGraph* GetGraph() { return m_graph; }

	std::stack<TASK> m_task;
	std::stack<TASK> GetTaskPos() { return m_task; };

	Vector3 GetTargetPos() { return m_targetPos; }
	void SetTargetPos(Vector3 p) { m_targetPos = p; }

	void taskSetting();

	bool isHave;// 持ってる
	FieldNode* m_goalNode;
	Vector3 m_goalPos;

	bool isAI;// AIか？

	BasePerson* m_nearPlayer;
	

	int m_invincibleFlame;

	int m_PadNum;
private:
	// ★ステートマシン
	StateMachine<FieldPlayer>* m_pStateMachine;

	FieldGraph* m_graph;// グラフ探索
	
	Vector3 m_targetPos;

	tdn2DObj* m_gage;
	tdn2DObj* m_life;
	
						// 移動のタスク
	
};
