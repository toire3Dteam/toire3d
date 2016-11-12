#pragma once

class LoadSceneThreadManager
{
public:
	//------------------------------------------------------
	//	���̂̎擾
	//------------------------------------------------------
	static LoadSceneThreadManager *GetInstance(){ static LoadSceneThreadManager i; return &i; }

	//------------------------------------------------------
	//	�V�[���쐬
	//------------------------------------------------------
	void Initialize(BaseScene *NewScene){ if (m_pLoadSceneThread){ delete m_pLoadSceneThread; }m_pLoadSceneThread = new LoadSceneThread(NewScene); m_bAction = true; }

	//------------------------------------------------------
	//	���
	//------------------------------------------------------
	void Release(){ SAFE_DELETE(m_pLoadSceneThread); m_bAction = false; }
	~LoadSceneThreadManager(){ Release(); }

	//------------------------------------------------------
	//	�Q�b�^�[
	//------------------------------------------------------
	BaseScene *GetNewScene(){ return m_pLoadSceneThread->GetNewScene(); }
	bool isInitializeEnd(){ return m_pLoadSceneThread->isInitializeEnd(); }
	bool isAction(){ return m_bAction; }

private:
	class LoadSceneThread
	{
	public:

		//------------------------------------------------------
		//	�X���b�h�֌W
		//------------------------------------------------------
		static void ThreadFunc(void *arg);
		static bool bThread;

		//------------------------------------------------------
		//	�������ƊJ��
		//------------------------------------------------------
		LoadSceneThread(BaseScene *NewScene);
		~LoadSceneThread();

		//------------------------------------------------------
		//	�Q�b�^�[
		//------------------------------------------------------
		BaseScene *GetNewScene(){ return m_pNewScene; }
		bool isInitializeEnd(){ return !bThread; }

	private:
		BaseScene*	m_pNewScene;
	};

	LoadSceneThread *m_pLoadSceneThread;
	bool m_bAction;

	// �V���O���g���̍�@
	LoadSceneThreadManager():m_pLoadSceneThread(nullptr), m_bAction(false){}
	LoadSceneThreadManager(const LoadSceneThreadManager&){}
	LoadSceneThreadManager& operator=(const LoadSceneThreadManager&){}
};


#define LoadSceneThreadMgr (LoadSceneThreadManager::GetInstance())