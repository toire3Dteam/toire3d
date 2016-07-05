#pragma once

/*****************************/
//		�x�[�X�V�[��
/*****************************/
class BaseScene
{
public:
	//	�����E���
	BaseScene() :m_bLoad(false), m_fLoadPercentage(0){}
	virtual ~BaseScene(){ }
	//	������
	virtual bool Initialize(){ return true; }
	virtual bool Initialize(BaseScene *newScene){ return true; }	// ��sceneLoading��p
	//	�X�V�E�`��
	virtual bool Update(){ return false; }
	virtual void Render(){}

	bool m_bLoad;				// sceneLoading��2�d�ɓǂݍ��ނ̂ŁA1��ǂݏI������炱���true�ɂ���2��ǂݍ��܂Ȃ��悤�ɂ���
	float m_fLoadPercentage;	// ���[�h�̊���(0�`1)	���[�h�V�[���̃��[�h�o�[�Ŏg��
};

