#ifndef AIROU_H_
#define AIROU_H_

class Airou : public BasePlayer
{
private:

	void Move();					// �ړ��l���g���č��W�X�V������d�͉�������B
	void Control(int pad);			// �L�[���͌n�͂����ŁB
	void Die();

	void Burst();
	void Near_attack();
	void Chara_attack();
	void Special_attack();

public:
	Airou(){}
	Airou(int pad);
	~Airou();

	// �X�V�ƕ`��
	void Update(int pad);
	void Render();


};

#endif