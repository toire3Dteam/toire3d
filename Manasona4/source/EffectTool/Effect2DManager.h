#pragma once
#include "TDNLIB.h"
#include "Effect2D.h"

class Effect2D;

class Effect2DManager
{
private:
	Effect2DManager();
	~Effect2DManager();
	Effect2DManager(const Effect2DManager&) {}
	Effect2DManager& operator=(const Effect2DManager&) {}
	static Effect2DManager* m_pInstance;

public:
	// シングルトン
	static Effect2DManager& GetInstance();
	static void Release();

	void Update();
	void Render();
	void AddNode();
	void EraseNode(int no);
	void EraseNode();
	bool Save(const char* fileName);
	bool Load(const char* fileName);
	void ChangeScreenSize(const int SizeX, const int SizeY);
	
	void ChangeScreenSize256();
	void ChangeScreenSize512();

	// 出力
	bool OutputPng( char* fileName,const int interval);
	bool OutputPngLine( char* fileName, const int interval,int WidthFrameNum);


	// アクセサ
	int GetNodeMAX() { return (int)m_pEffectNode.size(); }
	
	Effect2D* GetEffectNode(int no) 
	{ 
		if (no >= (int)m_pEffectNode.size())MyAssert(0,"サイズ以上");
		return  m_pEffectNode[no];
	}	
	
	Effect2D* GetEffectNode()
	{
		if (GetNodeMAX() == 0)MyAssert(0, "ノードが存在しない");
		return  m_pEffectNode[m_iSelectNodeNo];
	}

	void SetMaxFrame(const int maxFrame) { m_iMaxEffectFrame = maxFrame; }

	// 選択してるノード
	int m_iSelectNodeNo;



public:	
	// フレーム
	int m_iEffectFrame;
	int m_iMaxEffectFrame;

	// ノード
	std::vector<Effect2D*> m_pEffectNode;

	// セーブに必要なデータ
	//int saveData;
	std::ofstream m_saveData;	// 最終出力

	// 保存用スクリーン
	tdn2DObj* m_pScreen;
	Surface*  m_pStencilSurface;
	int m_iScreenWidth;
	int m_iScreenHeight;

	tdn2DObj* m_pLineScreen;
	Surface*  m_pLineStencilSurface;

public:
	void CalcFrame();
	void CalcSelectNo();

	
};

#define Effect2DMgr	(Effect2DManager::GetInstance())