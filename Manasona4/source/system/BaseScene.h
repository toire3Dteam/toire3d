#pragma once

/*****************************/
//		�x�[�X�V�[��
/*****************************/
class BaseScene
{
public:
	//	�����E���
	BaseScene() :m_bLoad(false), m_fLoadPercentage(0){ memset(m_LoadComment, '\0', sizeof(m_LoadComment)); }
	virtual ~BaseScene(){ }
	//	������
	virtual bool Initialize(){ return true; }
	//	�X�V�E�`��
	virtual void Update(){}
	virtual void Render(){}

	bool m_bLoad;				// sceneLoading��2�d�ɓǂݍ��ނ̂ŁA1��ǂݏI������炱���true�ɂ���2��ǂݍ��܂Ȃ��悤�ɂ���
	float m_fLoadPercentage;	// ���[�h�̊���(0�`1)	���[�h�V�[���̃��[�h�o�[�Ŏg��
	char m_LoadComment[256];	// �������[�h���Ă��邩�̃R�����g
};

