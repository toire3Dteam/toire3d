#include "TDNLIB.h"


/************************/
//		tdnFont
/************************/

/****** �O���錾 *******/

//	�Ǘ��p�p�����[�^									
bool tdnFont::m_IsInitialized = false;
tdnFont::CacheDesc		tdnFont::m_CacheDesc = { 0 }; //�L���b�V���̐F�X

/***********************/


/***********************************************/
//		�������E���
/***********************************************/
void	tdnFont::Initialize()
{
	//	�ʏ핶���L���b�V���̏�����
	{
		//	���łɊm�ۂ���Ă���̈悪����ꍇ�͔j��
		SAFE_DELETE_ARRAY(m_CacheDesc.textureCacheList);	//	�����e�N�X�`���̃��X�g
		SAFE_DELETE_ARRAY(m_CacheDesc.referenceDataList);	//	�L���b�V���̏ƍ��p�f�[�^���X�g

															//	�L���b�V����ۑ�����̈���m��
		m_CacheDesc.textureCacheList = new tdn2DObj[CacheQty];  // �����e�N�X�`���̃��X�g
		m_CacheDesc.referenceDataList = new	RefData[CacheQty];	// �L���b�V���̏ƍ��p�f�[�^�̃��X�g

																//	�L���b�V����������
		for (UINT i = 0; i < CacheQty; i++)
		{
			m_CacheDesc.referenceDataList[i].isEnable = false;// �L���t���O��false
		}

		m_CacheDesc.nextUseCacheNum = 0;//	���Ɏg�p����L���b�V���̔ԍ�
	}

	//	�������t���O�𗧂Ă�
	m_IsInitialized = true;

}

void	tdnFont::Release()
{
	//	�ʏ�L���b�V��
	{
		SAFE_DELETE_ARRAY(m_CacheDesc.textureCacheList);	//	�����e�N�X�`���̃��X�g
		SAFE_DELETE_ARRAY(m_CacheDesc.referenceDataList);	//	�L���b�V���̏ƍ��p�f�[�^���X�g
		m_CacheDesc.nextUseCacheNum = 0;
	}

	//	�������t���O�����Z�b�g
	m_IsInitialized = false;
}

/***********************************************/
//		�����`��
/***********************************************/

//������`��
void	tdnFont::RenderString(LPCSTR string, LPCSTR fontName, int fontSize, int drawX, int  drawY, DWORD color, DWORD RenderFlag)
{
	//	�ϐ��̗p��
	int	x = drawX;
	int	y = drawY;
	UINT	myByte = 0;
	//	�I�[�����܂Ń��[�v
	for (UINT i = 0; string[i] != '\0'; i += myByte)
	{
		//	�����̃o�C�g���𒲂ׂ�	
		myByte = _mbclen((BYTE*)&string[i]);

		//	������`��
		int	drawCoordX = x;
		int	drawCoordY = y;
		Vector2	drawedSize = RenderCharacter(&string[i], fontName, fontSize, drawCoordX, drawCoordY, color, RenderFlag);

		//	�`����W�����炷
		x += drawedSize.x;
	}
}


// �ꕶ���`��
Vector2	tdnFont::RenderCharacter(LPCSTR character, LPCSTR fontName, int fontSize, int drawX, int  drawY, DWORD color, DWORD RenderFlag)
{
	//	�����e�N�X�`���Ǘ��p�����[�^������������Ă��邩�`�F�b�N
	if (!m_IsInitialized)	Initialize();

	//	�ϐ��̗p��
	tdn2DObj*	pImage;
	UINT	createSize;
	UINT	byteSize = _mbclen((BYTE*)character);

	//	���ƍ������r���đ傫���ق�����Ƀe�N�X�`�����쐬
	int		createSizeW = (int)((float)fontSize);
	int		createSizeH = (int)((float)fontSize);
	if (createSizeW > createSizeH)	createSize = createSizeW;
	else							createSize = createSizeH;

	//	�g�p����e�N�X�`���̃A�h���X[�z��ԍ�]���Z�b�g
	pImage = &m_CacheDesc.textureCacheList[SearchCache(character, createSize, fontName)];

	//	�󔒃`�F�b�N
	bool	draw(true);
	
	switch (byteSize) 
	{
	case	1:
		if (character[0] == ' ') {	draw = false;	}
		break;
	case	2:
	{
		char	buffer[3] = { character[0], character[1], '\0' };
		if (strcmp(buffer, "�@") == 0) {
			draw = false;
		}
	}
	break;
	}

	//	�`��
	if (draw)
	{
		pImage->SetARGB(color);
		pImage->Render(drawX, drawY, pImage->GetWidth(), pImage->GetHeight(),
			0, 0, pImage->GetWidth(), pImage->GetHeight(), RenderFlag);
	}

	//	�`�悵�������̃T�C�Y��Ԃ�
	return	Vector2((float)pImage->GetWidth(), (float)pImage->GetHeight());
}


//  �Ȉ�
void tdnFont::RenderFont2D(LPCSTR _String, int _FontSize, float _DrawX, float _DrawY, DWORD col)
{
	LPSTR	c_FontName = "HGS�n�p�p�߯�ߑ�";

	RenderString(_String, c_FontName, _FontSize,
		_DrawX, _DrawY, col, RS::COPY);
}

/****************************************/
//	 2DObj���當�������T�|�[�g�֐�
/****************************************/
UINT	tdnFont::SearchCache(LPCSTR chara, UINT size, LPCSTR fontName)
{
	//	�g�p�\�ȃL���b�V����T��
	UINT	sizeAllow = 0;							//	���e�T�C�Y��
	UINT	byteSize = _mbclen((BYTE*)chara);	//	�o�C�g��
												//	�L���b�V���������[�v
	for (UINT cNum = 0; cNum < CacheQty; cNum++) {
		RefData&	refData = m_CacheDesc.referenceDataList[cNum];
		if (!refData.isEnable)									continue;
		//	�o�C�g������
		if (refData.byte != byteSize)							continue;
		//	��������i�o�C�g���ɉ����ĕ���j
		if (byteSize != 2) {
			if (refData.chara[0] != chara[0])					continue;
		}
		else {
			if (refData.chara[0] != chara[0])					continue;
			if (refData.chara[1] != chara[1])					continue;
		}
		//	�T�C�Y����
		if (refData.size - sizeAllow > size)					continue;
		if (refData.size + sizeAllow < size)					continue;
		//	�t�H���g����
		if (strlen(refData.fontName) == 0) {
			if (strlen(fontName) != 0)						continue;
		}
		else {
			if (strlen(fontName) == 0)						continue;
			if (strcmp(refData.fontName, fontName) != 0)	continue;
		}
		//	������N���A
		return	cNum;
	}

	//	�V���������e�N�X�`�����쐬
	UINT	useNum = m_CacheDesc.nextUseCacheNum;
	m_CacheDesc.textureCacheList[useNum].LoadFontTexture(chara, size, fontName);
	//	�ƍ��p�p�����[�^���X�V
	RefData&	refData = m_CacheDesc.referenceDataList[useNum];
	refData.chara[0] = chara[0];
	refData.chara[1] = chara[1];
	refData.size = size;
	refData.byte = byteSize;
	refData.isEnable = true;
	refData.fontName = fontName;

	//	���ɏ���������L���b�V���̔ԍ����X�V
	if (++m_CacheDesc.nextUseCacheNum == CacheQty)	m_CacheDesc.nextUseCacheNum = 0;

	//	�V�����e�N�X�`�����쐬�����ԍ���Ԃ�
	return	useNum;
}
