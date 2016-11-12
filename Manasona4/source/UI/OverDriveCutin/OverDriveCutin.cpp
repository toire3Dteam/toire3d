#include "OverDriveCutin.h"

//+------------------------------
//	�I�[�o�[�h���C�u�̃J�b�g�C��
//+------------------------------

OverDriveCutin::OverDriveCutin(BasePlayer* pPlayer)
{
	m_bRender = false;
	m_iFrame = 0;

	m_pBG = new tdn2DObj("Data/UI/Game/OverdriveCutin/BG.png");
	m_pPlayerData = pPlayer;

	m_pScreen = new tdn2DObj(1280, 720, TDN2D::RENDERTARGET);
	m_pScreenMask = new tdn2DObj("Data/UI/Game/OverdriveCutin/CutinMask.png");

	m_bTurnOverFlag = false;
	m_fMaskRate = 0.0f;
	m_fMaskPow = 1.0f / 8.0f;

	m_vPos.x = 0.0f;
	m_vPos.y = 0.0f;

	m_pFont1.pPic = new tdn2DAnim("Data/UI/Game/OverdriveCutin/font1.png");
	m_pFont1.pPic->OrderMoveAppeared(8, -400, +200);

	m_pFont2.pPic = new tdn2DAnim("Data/UI/Game/OverdriveCutin/font2.png");
	m_pFont2.pPic->OrderMoveAppeared(8, -400, +200);

	m_pFont3.pPic = new tdn2DAnim("Data/UI/Game/OverdriveCutin/font3.png");
	m_pFont3.pPic->OrderMoveAppeared(8, -400, +200);

	m_pPostLight = new tdn2DObj("Data/UI/Game/OverdriveCutin/PostLight.png");
	

	m_pFont1.pPic->SetScale(1.5f);
	m_pFont2.pPic->SetScale(1.5f);
	m_pFont3.pPic->SetScale(1.5f);


	//if (FAILED(tdnSystem::GetDevice()->CreateDepthStencilSurface(1280, 720, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pStencilSurface, NULL)))
	//{
	//	MessageBox(tdnSystem::GetWindow(), "�[�x�o�b�t�@���쐬�ł��Ȃ�����", "ERROR", MB_OK);
	//}

}

OverDriveCutin::~OverDriveCutin()
{
	SAFE_DELETE(m_pBG);
	SAFE_DELETE(m_pScreen);
	SAFE_DELETE(m_pScreenMask);

	SAFE_DELETE(m_pFont1.pPic);
	SAFE_DELETE(m_pFont2.pPic);
	SAFE_DELETE(m_pFont3.pPic);

	SAFE_DELETE(m_pPostLight);

	//SAFE_RELEASE(m_pStencilSurface);

}

void OverDriveCutin::Update()
{
	if (m_bRender == false)return;

	enum 
	{
		PERFORMANCE_END_FRAME = 70,
		MASK_CLOSE_FRAME = 60
	};

	// �p�t�H�[�}���X�t���[���ȏ�o�߂���ƏI��
	m_iFrame++;
	if (m_iFrame>= PERFORMANCE_END_FRAME)
	{
		m_iFrame = 0;
		m_bRender = false;
	}

	if (m_iFrame < MASK_CLOSE_FRAME)
	{
		// �}�X�N���[�g
		m_fMaskRate += m_fMaskPow;
	}
	else
	{
		// �}�X�N���[�g
		m_fMaskRate -= m_fMaskPow;
	}

	m_fMaskRate = min(0.99f, m_fMaskRate);
	m_fMaskRate = max(-0.1f, m_fMaskRate);

	// 
	m_pPlayerData->GetCutinPic()->Update();
	m_vPos.x += 0.5f;	// �ꏊ�X���C�h���Ă���
	m_vPos.y += -0.25f;

	m_pFont1.pPic->Update();
	m_pFont1.vPos.x += 0.5f;
	m_pFont1.vPos.y += -0.5f;

	m_pFont2.pPic->Update();
	m_pFont2.vPos.x += 1;
	m_pFont2.vPos.y += -1;

	m_pFont3.pPic->Update();
	m_pFont3.vPos.x += 1.5f;
	m_pFont3.vPos.y += -1.5f;

}

void OverDriveCutin::Render()
{
	if (m_bRender == false)return;

	Surface* save;
	Surface* saveZ;

	// �܂��͍��̃T�[�t�F�C�X��ۑ�
	tdnSystem::GetDevice()->GetRenderTarget(0, &save);

	// ���݂̃X�e���V���o�b�t�@���ꎞ�ۊ�
	tdnSystem::GetDevice()->GetDepthStencilSurface(&saveZ);
	
	// Z�o�b�t�@��؂�ւ�
	//tdnSystem::GetDevice()->SetDepthStencilSurface(m_pStencilSurface);

	m_pScreen->RenderTarget(0); //�؂�ւ�

	//��ʃN���A
	//tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);

	{
		m_pBG->Render(0, 0);
		
		// �t�H���g
		m_pFont1.pPic->SetTurnOver(m_bTurnOverFlag);
		m_pFont2.pPic->SetTurnOver(m_bTurnOverFlag);
		m_pFont3.pPic->SetTurnOver(m_bTurnOverFlag);
		if (m_bTurnOverFlag)
		{
			m_pFont1.pPic->SetAngle(1.57f);
			m_pFont2.pPic->SetAngle(1.57f);
			m_pFont3.pPic->SetAngle(1.57f);
		}
		else
		{
			m_pFont1.pPic->SetAngle(0.0f);
			m_pFont2.pPic->SetAngle(0.0f);
			m_pFont3.pPic->SetAngle(0.0f);
		}

		m_pFont1.pPic->Render((int)m_pFont1.vPos.x, (int)m_pFont1.vPos.y);
		m_pFont2.pPic->Render((int)m_pFont2.vPos.x, (int)m_pFont2.vPos.y);
		m_pFont3.pPic->Render((int)m_pFont3.vPos.x, (int)m_pFont3.vPos.y);
		
		// �L�����N�^�[
		m_pPlayerData->GetCutinPic()->Render((int)m_vPos.x, (int)m_vPos.y);

		// �|�X�g���C�g
		m_pPostLight->Render(0, 0);

		//tdnFont::RenderString(m_pPlayerData->GetName(), "���C���I", 128, 200, 200, 0xffffffff, RS::COPY);

	}
	
	tdnSystem::GetDevice()->SetRenderTarget(0, save);//�����_�[�^�[�Q�b�g�̕���
	//tdnSystem::GetDevice()->SetDepthStencilSurface(saveZ); //�X�e���V���o�b�t�@�̕���

	// �}�X�N����
	shader2D->SetValue("MaskTex", m_pScreenMask);
	shader2D->SetValue("g_fMaskEdgeRate", m_fMaskRate);

	// �X�N���[��
	m_pScreen->SetTurnOver(m_bTurnOverFlag);
	m_pScreen->Render(0, 0, 1280 , 720 , 0, 0, 1280, 720,shader2D, "MaskEdge");

}

void OverDriveCutin::Action()
{
	// �`��
	m_bRender = true;
	
	// �t���[��������
	m_iFrame = 0;


	if (m_pPlayerData->GetDir()==DIR::LEFT)
	{
		m_bTurnOverFlag = true;
	}
	else
	{
		m_bTurnOverFlag = false;
	}

	// �}�X�N������
	m_fMaskRate = 0.0f;

	// �A�N�V����
	m_pPlayerData->GetCutinPic()->Action();
	m_vPos = VECTOR2_ZERO;
	m_vPos.x = -100;

	m_pFont1.pPic->Action();
	m_pFont2.pPic->Action();
	m_pFont3.pPic->Action();

	m_pFont1.vPos = VECTOR2_ZERO;
	m_pFont2.vPos = VECTOR2_ZERO;
	m_pFont3.vPos = VECTOR2_ZERO;

}
