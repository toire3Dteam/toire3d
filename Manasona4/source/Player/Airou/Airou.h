#pragma once

class Airou : public BasePlayer
{
public:
	Airou(int deviceID, bool bAI);

	/****************************/
	//	�L�����N�^�[�ŗL�X�L��
	/****************************/
	void SkillUpdate();//

private:
	void InitActionDatas();	// �������z�֐��I�[�o�[���C�h
};

