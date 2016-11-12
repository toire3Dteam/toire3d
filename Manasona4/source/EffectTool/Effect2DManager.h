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
	// �V���O���g��
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

	// �o��
	bool OutputPng( char* fileName,const int interval);
	bool OutputPngLine( char* fileName, const int interval,int WidthFrameNum);


	// �A�N�Z�T
	int GetNodeMAX() { return (int)m_pEffectNode.size(); }
	
	Effect2D* GetEffectNode(int no) 
	{ 
		if (no >= (int)m_pEffectNode.size())MyAssert(0,"�T�C�Y�ȏ�");
		return  m_pEffectNode[no];
	}	
	
	Effect2D* GetEffectNode()
	{
		if (GetNodeMAX() == 0)MyAssert(0, "�m�[�h�����݂��Ȃ�");
		return  m_pEffectNode[m_iSelectNodeNo];
	}

	void SetMaxFrame(const int maxFrame) { m_iMaxEffectFrame = maxFrame; }

	// �I�����Ă�m�[�h
	int m_iSelectNodeNo;



public:	
	// �t���[��
	int m_iEffectFrame;
	int m_iMaxEffectFrame;

	// �m�[�h
	std::vector<Effect2D*> m_pEffectNode;

	// �Z�[�u�ɕK�v�ȃf�[�^
	//int saveData;
	std::ofstream m_saveData;	// �ŏI�o��

	// �ۑ��p�X�N���[��
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