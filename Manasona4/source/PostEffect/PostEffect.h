#pragma once
#include "TDNLIB.h"

/*
	ポストエフェクト
*/

class PostEffect
{
public:
	// いつもの
	static PostEffect& GetInstance();
	static void Release();
	virtual ~PostEffect();

	void Update();

	// スクリーン投影
	void RadialBigin();
	void RadialEnd();
	void RadialRender();
	void SetRadialBlur(Vector2 pos,float power);

	void BloomBigin();
	void BloomEnd();
	void BloomRender();
	void Ctrl();

	// 
	void BlueUpdate();


private:
	static PostEffect* pInstance;
	// 封印
	PostEffect();
	PostEffect(const PostEffect&) {}
	PostEffect& operator=(const PostEffect&) {}



private:// 変数

	// スクリーン
	tdn2DObj* m_radialScreen;
	tdn2DObj* m_postBlurScreen;
	tdn2DObj* m_bloomScreen;

	// 最終的に画面に映すサーフェイス
	Surface* m_backbuffer;

	// ブラ―
	float m_blurValue;
	float m_bluePower;


};


#define PostEffectMgr  (PostEffect::GetInstance())