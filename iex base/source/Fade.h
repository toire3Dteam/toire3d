#ifndef FADE_H_
#define	FADE_H_

//=======================================================================
//						�t�F�[�h�N���X
//=======================================================================
//	���ϐ����Ƃ����Ԃ�Ǝv���̂ŁA�Ϗ��Ŏg���Ă�������
/*	�~	*/
/*	class sceneMain : public Scene, Fade{
					�`*/


/*	��	*/
/*	class sceneMain : public Scene{
	private:
		Fade *fade;
					�`*/

class Fade
{
public:
	int mode;				/*	�t�F�[�h�C���A�t�F�[�h�A�E�g�A�������Ȃ�	*/
	int speed;				/*	�t�F�[�h���鑬�x	*/
	BYTE limit;				/*	�ǂ��܂Ńt�F�[�h���邩(0�`255�͈̔�)	*/
	BYTE alpha;				/*	���l(0�`255)	*/
	DWORD color;			/*	�J���[	*/

	enum FADE_FLAG{ FADE_STOP, FADE_IN, FADE_OUT };

	//	������
	Fade();
	void Initialize();


	// �ݒ�
	void Set(int Mode, int Speed, BYTE Limit, BYTE Alpha, DWORD Color = 0x00000000);	/*	�t���ݒ��	*/
	void Set(int Mode, int Speed, BYTE Limit);											/*	�r���Œǉ��������Ƃ���	*/
	void Set(int Mode, int Speed);														/*	�ȈՔ�	*/


	//	�X�V�E�`��
	void Update();
	void Render();

	bool isFadeStop(){ return (mode == FADE_STOP); }
};

#endif // !_FADE_H_
