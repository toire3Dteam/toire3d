#ifndef EFFECT_H_
#define EFFECT_H_

class Effect
{
private:
	iexParticle *particle;


public:

	void Initialize();
	void Release();
	void Update();
	void Render();

	void Effect1(const Vector3 &pos);		// �����G�t�F�N�g 
	void Effect2(const Vector3 &pos);		// ���G�t�F�N�g
	void Effect3(const Vector3 &pos);		// �R���オ��G�t�F�N�g

};


#endif