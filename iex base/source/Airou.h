#ifndef AIROU_H_
#define AIROU_H_

class Airou : public BasePlayer
{
private:

	void Move();					// 移動値を使って座標更新したり重力加えたり。
	void Control(int pad);			// キー入力系はここで。
	void Die();

	void Burst();
	void Near_attack();
	void Chara_attack();
	void Special_attack();

public:
	Airou(){}
	Airou(int pad);
	~Airou();

	// 更新と描画
	void Update(int pad);
	void Render();


};

#endif