#include "OverDriveStage.h"
#include "DeferredEx\DeferredEx.h"

//+------------------------------
//	オーバードライブ用のステージ
//+------------------------------

OverDriveStage::OverDriveStage()
{
	// 背景
	m_pBG = new tdn2DObj("Data/Stage/OverDrive/OverDriveBG.png");
	m_pSpeedLine= new SpeedLineBlueEffect();
	m_pSpeedLine->ActionRoop(Vector3(0, 0, 300), 2.0f, 2.0f, Vector3(PI / 8, PI / 4 , 0), Vector3(PI / 8, PI / 4, 0));


	
	// 
	m_pScreen = new tdn2DObj(1280, 720, TDN2D::RENDERTARGET);
	if (FAILED(tdnSystem::GetDevice()->CreateDepthStencilSurface(1280, 720, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pStencilSurface, NULL))) 
	{
		MessageBox(tdnSystem::GetWindow(), "深度バッファが作成できなかった", "ERROR", MB_OK);
	}

}

OverDriveStage::~OverDriveStage()
{
	SAFE_DELETE(m_pBG);
	SAFE_DELETE(m_pSpeedLine);
	SAFE_DELETE(m_pScreen);
	SAFE_RELEASE(m_pStencilSurface);
}


void OverDriveStage::Update()
{


	m_pSpeedLine->Update();
}

void OverDriveStage::Render()
{
	Surface* save;
	Surface* saveZ;
	// まずは今のサーフェイスを保存
	tdnSystem::GetDevice()->GetRenderTarget(0, &save);

	// 現在のステンシルバッファを一時保管
	tdnSystem::GetDevice()->GetDepthStencilSurface(&saveZ);

	// シャドウマップ用ステンシルバッファに切り替え
	tdnSystem::GetDevice()->SetDepthStencilSurface(m_pStencilSurface);

	m_pScreen->RenderTarget(0); //切り替え

	//　こっち側の画面クリア
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);

	{
		m_pBG->Render(0, 0,RS::COPY_NOZ);

		m_pSpeedLine->Render();
	}

	// お片付け
	tdnSystem::GetDevice()->SetRenderTarget(0, save);//レンダーターゲットの復元	
	tdnSystem::GetDevice()->SetDepthStencilSurface(saveZ); //ステンシルバッファの復元

}

