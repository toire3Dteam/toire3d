
class RID_2DObj : public iex2DObj
{
public:
	//iex2DObj(){};
	////	�t�@�C������쐬
	//iex2DObj(char* filename);
	////	�쐬
	//iex2DObj(u32 width, u32 height, u8 flag);
	RID_2DObj() :iex2DObj(){};
	//	�t�@�C������쐬
	RID_2DObj(char* filename) : iex2DObj(filename){};
	//	�쐬
	RID_2DObj(u32 width, u32 height, u8 flag) : iex2DObj(width, height, flag){};

	~RID_2DObj(){};
	
	void rot_render(float angle);
	void rot_render(float angle, iexShader* shader, char* tech);
	void rot_render(float angle, s32 DstX, s32 DstY, s32 DstW, s32 DstH, s32 SrcX, s32 SrcY, s32 width, s32 height, u32 dwFlags = RS_COPY, COLOR color = 0xFFFFFFFF, float z = .0f);
	void rot_render(float angle, s32 DstX, s32 DstY, s32 DstW, s32 DstH, s32 SrcX, s32 SrcY, s32 width, s32 height, iexShader* shader, char* tech, COLOR color = 0xFFFFFFFF, float z = .0f);
private:

};





class RID_2Drot : public Scene
{
public:
	iexView *view;
	RID_2DObj *turn;

	float angle;

	RID_2Drot();
	~RID_2Drot();

	//	������
	bool Initialize();
	//	�X�V�E�`��
	void Update();	//	�X�V
	void Render();	//	�`��
private:

};
