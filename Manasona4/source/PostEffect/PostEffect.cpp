#include "PostEffect.h"
#include "system\System.h"


PostEffect* PostEffect::pInstance = nullptr;

/***************************/
//	シングルトン
/***************************/
PostEffect & PostEffect::GetInstance()
{
	if (!pInstance)
	{
		pInstance = new PostEffect();
	}
	// TODO: return ステートメントをここに挿入します
	return *pInstance;
}

void PostEffect::Release()
{
	SAFE_DELETE(pInstance);
}


/***************************/
//	初期化・解放
/***************************/
PostEffect::PostEffect()
{
	// FinalPassSurface
	m_radialScreen = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::RENDERTARGET);
	m_postBlurScreen = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::RENDERTARGET);
	m_bloomScreen = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::RENDERTARGET);

	// BackBuffer
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_backbuffer);

	m_blurValue = 2.0f;

	m_bluePower = 0.0f;
}

PostEffect::~PostEffect()
{
	SAFE_DELETE(m_radialScreen);
	SAFE_DELETE(m_postBlurScreen);
	SAFE_DELETE(m_bloomScreen);

}

/***************************/
//	実装内容
/***************************/
void PostEffect::RadialBigin()
{
	m_radialScreen->RenderTarget();
}

void PostEffect::RadialEnd()
{
	// レンダーターゲット切り替え
	tdnSystem::GetDevice()->SetRenderTarget(0, m_backbuffer);

}
void PostEffect::RadialRender()
{
	m_radialScreen->Render(0, 0, shader2D, "RadialBlur");
}

// 放射ブラ―セット
void PostEffect::SetRadialBlur(Vector2 pos, float power)
{
	// ブラ―
	m_bluePower = power;
	shader2D->SetValue("BluePower", m_bluePower);

	// 場所
	shader2D->SetValue("CenterX", pos.x);
	shader2D->SetValue("CenterY", pos.y);
}

void PostEffect::BloomBigin()
{
	m_postBlurScreen->RenderTarget();
}

void PostEffect::BloomEnd()
{

	m_postBlurScreen->Render(0, 0, shader2D, "XBlur");

	// ブルーム用
	// レンダーターゲット切り替え
	m_bloomScreen->RenderTarget();
	m_postBlurScreen->Render(0, 0, shader2D, "YBlur");

	tdnSystem::GetDevice()->SetRenderTarget(0, m_backbuffer);

}


void PostEffect::Update()
{
	Ctrl();
	BlueUpdate();

	shader2D->SetValue("BlurValue", m_blurValue);
}

void PostEffect::BloomRender()
{
	
	m_bloomScreen->Render(0, 0, shader2D, "post");

	//tdnText::Draw(1000, 600, 0xff00ffff, "パワー%.2f", m_blurValue);

}

void PostEffect::Ctrl()
{
	//if (KeyBoard(KB_ENTER))
	//{
	//	if (KeyBoard(KB_Z))
	//	{
	//		m_blurValue += 0.05f;
	//	}
	//	if (KeyBoard(KB_X))
	//	{
	//		m_blurValue -= 0.05f;
	//	}

	//	if (KeyBoard(KB_C))
	//	{
	//		m_bluePower = 15.0f;
	//	}

	//
	//}

}

// ブラ―
void PostEffect::BlueUpdate()
{

	m_bluePower -= 1.0f;
	m_bluePower = Math::Clamp(m_bluePower, 0.0f, 30.0f);

	// ブラ―
	shader2D->SetValue("BluePower", m_bluePower);
		
}

