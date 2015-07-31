#ifndef CAMERA_H_
#define CAMERA_H_

class Camera
{
private:

	iexView *view;
	Vector3	pos, target;
	Vector3 ipos, itarget;	//���z�̈ʒu

	Vector3 angle;

	Vector3 p_pos;

	float dist;	//����
	int mode;

	void Fix_camera();			/*	�Œ�	*/
	void Debug_camera();		/*	�f�o�b�O�p	*/

	//�J�����؂�ւ�
	void Change_Camera(int m){
		if (mode != m)mode = m;
	}

public:

	enum{
		FIX
	};

	Camera();
	~Camera();

	/*�Q�b�^�[�ƃZ�b�^�[*/

	void Set_mode(int m){ mode = m; }
	int Get_mode(){ return mode; }

	void Set_pos(const Vector3 &p){ pos = p; }
	void Get_pos(Vector3 &out){ out = pos; }
	Vector3 Get_pos(){ return pos; }

	void Set_angle(const Vector3 &a){ angle = a; }
	void Get_angle(Vector3 &out){ out = angle; }
	Vector3 Get_angle(){ return angle; }

	void Update();
	void Render();
};

extern Camera * camera;

#endif