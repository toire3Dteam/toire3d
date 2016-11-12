#include "OverDriveStage.h"
#include "DeferredEx\DeferredEx.h"

//+------------------------------
//	�I�[�o�[�h���C�u�p�̃X�e�[�W
//+------------------------------

OverDriveStage::OverDriveStage()
{
	// �w�i
	m_pBG = new tdn2DObj("Data/Stage/OverDrive/OverDriveBG.png");
	m_pSpeedLine= new SpeedLineBlueEffect();
	m_pSpeedLine->ActionRoop(Vector3(0, 0, 300), 2.0f, 2.0f, Vector3(PI / 8, PI / 4 , 0), Vector3(PI / 8, PI / 4, 0));


	
	// 
	m_pScreen = new tdn2DObj(1280, 720, TDN2D::RENDERTARGET);
	if (FAILED(tdnSystem::GetDevice()->CreateDepthStencilSurface(1280, 720, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pStencilSurface, NULL))) 
	{
		MessageBox(tdnSystem::GetWindow(), "�[�x�o�b�t�@���쐬�ł��Ȃ�����", "ERROR", MB_OK);
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
	// �܂��͍��̃T�[�t�F�C�X��ۑ�
	tdnSystem::GetDevice()->GetRenderTarget(0, &save);

	// ���݂̃X�e���V���o�b�t�@���ꎞ�ۊ�
	tdnSystem::GetDevice()->GetDepthStencilSurface(&saveZ);

	// �V���h�E�}�b�v�p�X�e���V���o�b�t�@�ɐ؂�ւ�
	tdnSystem::GetDevice()->SetDepthStencilSurface(m_pStencilSurface);

	m_pScreen->RenderTarget(0); //�؂�ւ�

	//�@���������̉�ʃN���A
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);

	{
		m_pBG->Render(0, 0,RS::COPY_NOZ);

		m_pSpeedLine->Render();
	}

	// ���Еt��
	tdnSystem::GetDevice()->SetRenderTarget(0, save);//�����_�[�^�[�Q�b�g�̕���	
	tdnSystem::GetDevice()->SetDepthStencilSurface(saveZ); //�X�e���V���o�b�t�@�̕���

}

