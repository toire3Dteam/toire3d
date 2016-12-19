#pragma once
#include "TDNLIB.h"
#include "BaseEntity\Entity\BaseGameEntity.h"

//+--------------------
//	カットイン演出タイプ
//+--------------------
namespace CutInType
{
	class Base
	{
	public:
		Base::Base() { m_bAction = false; }
		virtual Base::~Base() {}

		virtual void Update() = 0;
		virtual void Render() = 0;	
		virtual  void Action() { m_bAction = true; };

	protected:
		//　フラグ
		bool m_bAction;

		// イラストの構造体
		struct Pic
		{
			tdn2DAnim* pic;
			Vector2 pos;
			Vector2 uv;
			float rotate;

			void PicClean()
			{
				pic = nullptr;
				pos.x = pos.y = 0.0f;
				uv.x = uv.y = 0.0f;
				rotate = 0.0f;
			}
		};

	};

	// アイル―カットイン
	class Airou :public Base
	{
	public:
		Airou();
		~Airou();

		void Update();
		void Render();
		void Action();

	private:
		Pic m_pPic;
		Pic m_pPicShink;
		
		Pic m_pPicBackRip;
		Pic m_pPicBackRip2;

		tdn2DObj* m_pBackCol;
	};

	// てきカットイン
	class Teki :public Base
	{
	public:
		Teki();
		~Teki();

		void Update();
		void Render();
		void Action();

	private:
		Pic m_pPic;
		Pic m_pPicShink;

		Pic m_pPicBackRip;
		Pic m_pPicBackRip2;

		tdn2DObj* m_pBackCol;
	};

	// なぜならカットイン
	class Nazenara :public Base
	{
	public:
		Nazenara();
		~Nazenara();

		void Update();
		void Render();
		void Action();

	private:
		Pic m_pPic;
		Pic m_pPicShink;

		Pic m_pPicBackRip;
		Pic m_pPicBackRip2;

		tdn2DObj* m_pBackCol;
	};

}




//+--------------------
//	カットイン
//+--------------------

enum class CUTIN_TYPE_NAME
{
	AIROU, TEKI, NAZENARABA, ARRAY_END
};

class CutIn :public BaseGameEntity
{
private:
	// シングルトンの作法
	CutIn();
	~CutIn();
	CutIn(const CutIn&);
	CutIn &operator=(const CutIn&) {}

	// 1つしかない実体
	static CutIn *m_pInstance;
public:
	// 実体取得
	static CutIn *GetInstance()
	{
		if (!m_pInstance) m_pInstance = new CutIn();
		return m_pInstance;
	}
	static void Rerease() {	SAFE_DELETE(m_pInstance);}

	void Update();
	void Render();

	void Action(CUTIN_TYPE_NAME);

	// キャラ毎に継承させそれぞれの演出を付ける
	virtual void PeformanceUpdate() {};
	virtual void PeformanceRender() {};

	bool  HandleMessage(const Message& msg);

private:
	
	// カットイン用のレンダーターゲット
	tdn2DObj* m_pScreen;
	float m_fRate;
	float m_fScale;
	int m_iFrame;
	bool m_bActionFlag;
	bool m_bRenderFlag;

	// アルファ値の値
	int m_iAlphaNearFrame;
	int m_iAlphaFarFrame;
	
	// 終りの値
	int m_iEndFrame;

	CutInType::Base* m_pCutinType[(int)CUTIN_TYPE_NAME::ARRAY_END];
	CUTIN_TYPE_NAME	m_eSelectType;

	// 受取先保存
	ENTITY_ID m_eSaveID;

};

// 実体化
#define CutInMgr (CutIn::GetInstance())
