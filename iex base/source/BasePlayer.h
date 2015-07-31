#ifndef CHARABASE_H_
#define CHARABASE_H_

//******************************************************************************
//
//
//		Player基本クラス
//
//
//******************************************************************************

class BasePlayer
{
protected:

	int hp;					/*	ひっとぽいんとかいふくするならきずぐすりとほうぎょくで	*/
	int mode;				/*	Update関数のswitch分岐で使う(stateパターン)			*/
	int process;			/*	Update関数で呼び出す関数の中で使う(modeの中のstate)	*/
	int attack_again_time;	/*	攻撃に必要な待ち時間(遠距離はすぐ撃てるし近距離は少し時間がかかる)*/

	/*	定数のように使う(アタックアゲインタイムに必要な時間)	*/
	int NEAR_AGAIN;
	int FAR_AGAIN;
	int CHARA_AGAIN;

	float scale;		/*	スケール値	*/
	float speed;		/*	スピード値	*/

	// 多分使わない
	//float max_speed;	/*	最高速度	*/
	//float accel;		/*	加速度	if( speed < max_speed ) speed += accel		*/

	float gravity;		/*	重力		*/

	Vector3 pos;		/*	座標	*/
	Vector3 angle;		/*	向き	*/
	Vector3 move;		/*	移動値	*/

	bool died;			/*	死んだかどうか				*/
	bool isJump;		/* 2段ジャンプ防止用(必要？)	*/
	bool isLand;		/* 地面についているか			*/


	bool invincible;	/* 無敵状態	*/
	int invincible_time;/* 無敵時間	*/

	iex3DObj2 *obj;

	virtual void Near_attack();				/*	近距離攻撃				*/
	virtual void Far_attack();				/*	遠距離攻撃				*/
	virtual void Jump();					/*	ジャンプ				*/
	virtual void Chara_attack() = 0;		/*	キャラ固有				*/
	virtual void Special_attack() = 0;		/*	キャラ必殺技			*/
	virtual void Burst();					/*	バースト発動			*/
	virtual void Move();					/*	移動					*/
	virtual void Die();						/*	死ぬ					*/
	virtual void Control(int pad);			/*	キー入力系はここで		*/

	void Compute_move();					/*	移動量とか重力に従って座標を更新	*/


	//=====================================================================
	// 定数
	// angle.yにそれぞれ代入してください
	const float LEFT_ANGLE = 270.0f * .0174532925f;
	const float RIGHT_ANGLE = 90.0f * .0174532925f;

public:

	//===========================================
	//	初期化と解放
	BasePlayer(){}
	void Initialize(int pad);		// 座標とかstate情報とかキャラで共通している情報の初期化( 全部初期化設定するのがめんどくさい人用 )
	void Respawn(int pad);			//	リスポーン(復活)

	virtual ~BasePlayer(){}


	//===========================================
	//	更新と描画
	virtual void Update(int pad);
	virtual void Render();



	//===========================================
	//	ゲッター・セッター
	void Damage(int value)
	{
		hp -= value;
	}

	void Set_invincible()
	{
		invincible = true;
		invincible_time = 120;
	}
	void Set_invincible(int frame)
	{
		invincible = true;
		invincible_time = frame;
	}

	// 座標
	void Get_pos(Vector3 &out){ out = pos; }
	Vector3 Get_pos(){ return pos; }
	void Set_pos(const Vector3 &p){ pos = p; }



	// objの各ベクトル
	void Get_obj_front(Vector3 &out){ out = Vector3(obj->TransMatrix._31, obj->TransMatrix._32, obj->TransMatrix._33); }
	Vector3 Get_obj_front(){ return Vector3(obj->TransMatrix._31, obj->TransMatrix._32, obj->TransMatrix._33); }

	void Get_obj_up(Vector3 &out){ out = Vector3(obj->TransMatrix._21, obj->TransMatrix._22, obj->TransMatrix._23); }
	Vector3 Get_obj_up(){ return Vector3(obj->TransMatrix._21, obj->TransMatrix._22, obj->TransMatrix._23); }


	// アングル
	void Get_angle(Vector3 &out){ out = angle; }
	Vector3 Get_angle(){ return angle; }
	void Set_angle(const Vector3 &a){ angle = a; }


	// モード(state)
	int Get_mode(){ return mode; }
	void Set_mode(int num)
	{
		mode = num;
		process = 0;
	}


	// モードの中のモード
	int Get_process(){ return process; }
	void Set_process(int p){ process = p; }


	// 生死
	bool isDied(){ return died; }
	void Set_die(bool d){ died = d; }


	// 速度
	float Get_speed(){ return speed; }
	void Set_speed(float s){ speed = s; }


	// モーション
	void Set_motion(int num){ if (obj->GetMotion(0) != num) obj->SetMotion(0, num); }


	//======================================
	//	定数(こんなにいらないかも)
	enum PLAYER_MODE
	{
		WAIT,			/*	待機			*/
		MOVE,			/*	移動			*/
		NEAR_ATTACK,	/*	近距離			*/
		FAR_ATTACK,		/*	遠距離			*/
		CHARA_ATTACK,	/*	キャラ固有		*/
		JUMP,			/*	ジャンプ		*/
		BURST,			/*	バースト中		*/
		SPECIAL_ATTACK,	/*	必殺技			*/
		GETTING_ITEM,	/*	アイテム取得状態*/
		DIE				/*	死にきった		*/
	};
};

#endif