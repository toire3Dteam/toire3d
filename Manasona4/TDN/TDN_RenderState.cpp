#include	"TDNLIB.h"


/********************************************/
//	tdnRenderState
//	�f�o�C�X�̃����_�����O �X�e�[�g���`����B
//	
/********************************************/


// �����_�[�X�e�[�g������
void tdnRenderState::Initialize()
{
	//	�|���S���`��ݒ�
	tdnSystem::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// �J�����O�̐ݒ�@������ŃJ�����O����
	tdnSystem::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);				// �h��Ԃ����[�h	�ʂ�h��Ԃ�
	tdnSystem::GetDevice()->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);			// �V�F�[�f�B���O���[�h�@�O�[���[(���`���)		
																						   
	//	�A���t�@�u�����h�ݒ�															
	tdnSystem::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h�ɂ�铧������L���ɂ���	 
	tdnSystem::GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);				// �s�N�Z�����Ƃ̃A���t�@�e�X�g��L���ɂ���
	tdnSystem::GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);				// �A���t�@�l����Ƃ���s�N�Z���̎󂯎���󂯎�苑�ۂ�������@�f�t�H���g��
	tdnSystem::GetDevice()->SetRenderState(D3DRS_ALPHAREF, 0);							// D3DRS_ALPHATESTENABLE���L���ɐݒ肳��Ă���ꍇ�ɁAD3DRS_ALPHAFUNC���瑗�����r�֐����g��
																						// �s�N�Z�����e�X�g���邽�߂̊�A���t�@�l���w��
	//	�u�����h�ݒ� �e�N�X�`���u�����f�B���O																	 
	tdnSystem::GetDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);	// (?)�J���[ �u�����f�B���O�����@�f�t�H���g�l��D3DTOP_MODULATE
	tdnSystem::GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT);	// (?) �ŏ��̃J���[�̈����B�f�t�H���g�̃p�����[�^�[�� D3DTA_TEXTURE �ł��B
	tdnSystem::GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);	// (?) �f�t�H���g�̃p�����[�^�[�� D3DTA_CURRENT 
	tdnSystem::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// D3DTSS_ALPHAOP ���g�����ꍇ�ɂ́���ɃA���t�@�����ɉe����^���܂��B 
	tdnSystem::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);	// (?)�Ӗ��s���B�ȏ�
	tdnSystem::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);	// (?)
																						   
	//	�e�N�X�`���ݒ�																	    
	tdnSystem::GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);		// �~�b�v�}�b�v�@NONE->�~�b�v�}�b�v�����𖳌��ɂ���B���X�^���C�U�͑���Ɋg��t�B���^���g��
	tdnSystem::GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// �k���t�B���^ D3DTEXF_LINEAR->�o�C���j�A���
	tdnSystem::GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// �g��t�B���^ D3DTEXF_LINEAR->�o�C���j�A���
	//tdnSystem::GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	//tdnSystem::GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

	tdnSystem::GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);		// u���W�Ŏg���e�N�X�`���A�h���V���O WRAP->�J��Ԃ� CLAMP->�L�΂�
	tdnSystem::GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);		// v���W�Ŏg���e�N�X�`���A�h���V���O WRAP->�J��Ԃ� 
																						   
	//	���C�e�B���O�ݒ�																   
	//tdnLight::SetAmbient(0x20202020);												   
	tdnSystem::GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);						// Direct3D �̃��C�e�B���O��L���ɂ���ɂ� TRUE
																						   
	//	�X�N���[���ݒ�																	    
	tdnSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);						// z�o�b�t�@�����O��L���ɂ���ɂ� D3DZB_TRUE
	tdnSystem::GetDevice()->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);			// FALSE �̏ꍇ�́A�����̃T���v���͂��ׂē����T���v�����O�l (�s�N�Z���̒��S�ŃT���v�����O���ꂽ�l) �ŋL�q����āA�}���`�T���v�� �o�b�t�@�ɑ΂����A���`�G�C���A�V���O �����_�����O���\�ɂȂ�

	tdnSystem::GetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);				// ���_�@���̎������K����L���ɂ���ɂ� TRUE

}

/****************************/
//	�����_�����O�X�e�[�g
/****************************/
void tdnRenderState::Set(DWORD state, D3DMATERIAL9* lpMaterial, Texture2D* lpTexture)
{
	switch (state)
	{

	case RS::COPY:
		tdnSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// ���� = �]���� + �]����
		tdnSystem::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// �������Ă�����̂��ǂꂭ�炢������邩
		tdnSystem::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ��ɕ`���Ă������̂����̔��Ε����߂ĕ`��
		tdnSystem::GetDevice()->SetRenderState(D3DRS_FOGENABLE, FALSE);					// �t�H�O�u�����f�B���O�𖳌��B�V�F�[�_�[�Ńt�H�O���΂悭�ˁH
		tdnSystem::GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);					// Direct3D �̃��C�e�B���O��L���ɂ���
		tdnSystem::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				// �[�x�o�b�t�@�ւ̏������݂�L���ɂ���
		break;
	case RS::COPY_NOZ:// 2D�ɓK�p�����3D�̉��ɕ`��ł�����
		tdnSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_FOGENABLE, FALSE);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);				// �[�x�o�b�t�@�ւ̏������݂�L���ɂ���
		break;
	case RS::ADD:	//	���Z����
		tdnSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// ��ɕ`���Ă��镨�̂��S�͕`��
		tdnSystem::GetDevice()->SetRenderState(D3DRS_FOGENABLE, FALSE);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		break;

	case RS::SUB:
		tdnSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);	// �]���悩��]���������Z�����B���� = �]���� - �]����
		tdnSystem::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// ��ɕ`���Ă��镨�̂��S�͕`��
		tdnSystem::GetDevice()->SetRenderState(D3DRS_FOGENABLE, FALSE);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		break;

	case RS::MUL:
		tdnSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);			// �����Ȃ�
		tdnSystem::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);		// �O�ɏ����Ă����ɐF���|����
		tdnSystem::GetDevice()->SetRenderState(D3DRS_FOGENABLE, FALSE);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		break;

	case RS::NEGA:
		tdnSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_FOGENABLE, FALSE);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		break;

	case RS::INVERT:	/*	���]		*/
		tdnSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_SUBTRACT);	// ���ʂ͓]��������]���悪���Z�����B���� = �]���� - �]����
		tdnSystem::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_FOGENABLE, FALSE);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		break;
	}

	//	�}�e���A���ݒ�
	if (lpMaterial) tdnSystem::GetDevice()->SetMaterial(lpMaterial);
	//	�e�N�X�`���ݒ� �e�N�X�`�����f�o�C�X�̃X�e�[�W�Ɋ��蓖�Ă�B
	tdnSystem::GetDevice()->SetTexture(0, lpTexture);
}

//	�T���v���[�X�e�[�g�@TRUE�FLINER(���)�@FLASE:POINT(�����͑���)�@
void	tdnRenderState::Filter(BOOL bFilter)
{
	if (bFilter){
		tdnSystem::GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		tdnSystem::GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	}
	else {
		tdnSystem::GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		tdnSystem::GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	}


}