#include	"TDNLIB.h"

//using namespace Math;

//*****************************************************
//		�s��֘A
//*****************************************************

//*****************************************************
//		�ϊ��s��
//*****************************************************

/**
*@brief				������Mat�Ɍ����ƕ��s�ړ��̏�������(XYZ��])
*@param[in]	Mat		�|�����킹�����ʂ̍s��	
*/
void	Math::SetTransformMatrixXYZ(Matrix *Mat, float posX, float posY, float posZ, float angleX, float angleY, float angleZ)
{
	float	sx, sy, sz, cx, cy, cz;

	sx = sinf(angleX);	sy = sinf(angleY);	sz = sinf(angleZ);
	cx = cosf(angleX);	cy = cosf(angleY);	cz = cosf(angleZ);

	Mat->_11 = (float)(cy*cz + sx*sy*sz);
	Mat->_12 = (float)(cx*sz);
	Mat->_13 = (float)(-sy*cz + sx*cy*sz);
	Mat->_14 = (float)(0.0f);

	Mat->_21 = (float)(-cy*sz + sx*sy*cz);
	Mat->_22 = (float)(cx*cz);
	Mat->_23 = (float)(sy*sz + sx*cy*cz);
	Mat->_24 = (float)(0.0f);

	Mat->_31 = (float)(cx*sy);
	Mat->_32 = (float)(-sx);
	Mat->_33 = (float)(cx*cy);
	Mat->_34 = (float)(0.0f);

	Mat->_41 = posX;
	Mat->_42 = posY;
	Mat->_43 = posZ;
	Mat->_44 = (float)(1.0f);
}

/**
*@brief				������Mat�Ɍ����ƕ��s�ړ��̏�������(ZXY��])
*@param[in]	Mat		�|�����킹�����ʂ̍s��
*/
void	Math::SetTransformMatrixZXY(Matrix *Mat, float posX, float posY, float posZ, float angleX, float angleY, float angleZ)
{
	float	sx, sy, sz, cx, cy, cz;

	sx = sinf(angleX);	sy = sinf(angleY);	sz = sinf(angleZ);
	cx = cosf(angleX);	cy = cosf(angleY);	cz = cosf(angleZ);

	Mat->_11 = cz*cy + sz*sx*sy;
	Mat->_12 = sz*cx;
	Mat->_13 = -cz*sy + sz*sx*cy;
	Mat->_14 = 0.0f;

	Mat->_21 = -sz*cy + cz*sx*sy;
	Mat->_22 = cz*cx;
	Mat->_23 = sz*sy + cz*sx*cy;
	Mat->_24 = 0.0f;

	Mat->_31 = cx*sy;
	Mat->_32 = -sx;
	Mat->_33 = cx*cy;
	Mat->_34 = 0.0f;

	Mat->_41 = posX;
	Mat->_42 = posY;
	Mat->_43 = posZ;
	Mat->_44 = 1.0f;
}

//*****************************************************************************
//		�r���[�ϊ��s��
//*****************************************************************************
//------------------------------------------------------
//	������W�n�r���[�ϊ�
//------------------------------------------------------
void Math::LookAtLH(Matrix& mat, Vector& Eye, Vector& At, Vector& Up)
{
	Vector3 zaxis;
	zaxis.x = At.x - Eye.x;
	zaxis.y = At.y - Eye.y;
	zaxis.z = At.z - Eye.z;
	Vector3 xaxis = Vector3(Up.y*zaxis.z - Up.z*zaxis.y, Up.z*zaxis.x - Up.x*zaxis.z, Up.x*zaxis.y - Up.y*zaxis.x);
	Vector3 yaxis = Vector3(zaxis.y*xaxis.z - zaxis.z*xaxis.y, zaxis.z*xaxis.x - zaxis.x*xaxis.z, zaxis.x*xaxis.y - zaxis.y*xaxis.x);
	
	xaxis.Normalize();
	yaxis.Normalize();
	zaxis.Normalize();

	mat._11 = xaxis.x;
	mat._21 = xaxis.y;
	mat._31 = xaxis.z;

	mat._12 = yaxis.x;
	mat._22 = yaxis.y;
	mat._32 = yaxis.z;

	mat._13 = zaxis.x;
	mat._23 = zaxis.y;
	mat._33 = zaxis.z;

	mat._14 = 0;
	mat._24 = 0;
	mat._34 = 0;

	mat._41 = -(xaxis.x*Eye.x + xaxis.y*Eye.y + xaxis.z*Eye.z);
	mat._42 = -(yaxis.x*Eye.x + yaxis.y*Eye.y + yaxis.z*Eye.z);
	mat._43 = -(zaxis.x*Eye.x + zaxis.y*Eye.y + zaxis.z*Eye.z);
	mat._44 = 1;
}

//*****************************************************************************
//		���e�ϊ��\��
//*****************************************************************************
//------------------------------------------------------
//	�p�[�X���e�ϊ�
//------------------------------------------------------
void Math::PerspectiveLH(Matrix& mat, float fovY, float aspect, float zn, float zf)
{
	float yScale = 1 / tanf(fovY / 2);
	float xScale = yScale / aspect;

	mat._11 = xScale;
	mat._12 = 0;
	mat._13 = 0;
	mat._14 = 0;

	mat._21 = 0;
	mat._22 = yScale;
	mat._23 = 0;
	mat._24 = 0;

	mat._31 = 0;
	mat._32 = 0;
	mat._33 = zf / (zf - zn);
	mat._34 = 1;

	mat._41 = 0;
	mat._42 = 0;
	mat._43 = (-zn*zf) / (zf - zn);
	mat._44 = 0;
}

//------------------------------------------------------
//	���s���e�ϊ�
//------------------------------------------------------
void Math::OlthoLH(Matrix& mat, float w, float h, float zn, float zf)
{
	mat._11 = 2.0f / w;
	mat._12 = 0;
	mat._13 = 0;
	mat._14 = 0;

	mat._21 = 0;
	mat._22 = 2.0f / h;
	mat._23 = 0;
	mat._24 = 0;

	mat._31 = 0;
	mat._32 = 0;
	mat._33 = 1.0f / (zf - zn);
	mat._34 = 0;

	mat._41 = 0;
	mat._42 = 0;
	mat._43 = zn / (zn - zf);
	mat._44 = 1.0f;
}





//*****************************************************************************
//
//		�N�H�[�^�j�I���֘A
//
//*****************************************************************************

//*****************************************************************************
//		�ϊ�
//*****************************************************************************
//------------------------------------------------------
//	�s��ւ̕ϊ�
//------------------------------------------------------
void Quaternion::toMatrix(Matrix& m)
{
	float s = sqrtf(x*x + y*y + z*z + w*w);
	s = 2.0f / (s * s);
	float vx = x * s;
	float vy = y * s;
	float vz = z * s;
	float wx = vx * w;
	float wy = vy * w;
	float wz = vz * w;
	float sx = x * vx;
	float sy = y * vy;
	float sz = z * vz;
	float cx = y * vz;
	float cy = z * vx;
	float cz = x * vy;

	m._11 = 1.0f - sy - sz;
	m._12 = cz + wz;
	m._13 = cy - wy;
	m._14 = 0.0f;
	m._21 = cz - wz;
	m._22 = 1.0f - sx - sz;
	m._23 = cx + wx;
	m._24 = 0.0f;
	m._31 = cy + wy;
	m._32 = cx - wx;
	m._33 = 1.0f - sx - sy;
	m._34 = 0.0f;
	m._41 = 0.0f;
	m._42 = 0.0f;
	m._43 = 0.0f;
	m._44 = 1.0f;
}

//------------------------------------------------------
//		���ʐ��`���
//------------------------------------------------------
Quaternion QuaternionSlerp(Quaternion& q, Quaternion& r, float t)
{
	//	����
	float dot = q.x*r.x + q.y*r.y + q.z*r.z + q.w*r.w;
	float ss = 1.0f - (dot * dot);
	if (ss <= 0.0f) return q;

	Quaternion Target = r;
	if (dot < 0.0f)
	{
		Target = -Target;
		dot = -dot;
	}
	//	���ϒl�␳
	if (dot > 1.0f) dot = 1.0f;

	//	��Ԋ����v�Z
	float ph = acosf(dot);
	float s = 1 / sinf(ph);
	float s1 = sinf(ph * (1.0f - t)) * s;
	float s2 = sinf(ph *  t) * s;
	//	���
	return q*s1 + Target*s2;
}

//*****************************************************
//		�����̌v�Z
//*****************************************************
float Math::Length(float x1, float y1, float x2, float y2)
{
	float x = x1 - x2;
	float y = y1 - y2;

	//  �x�N�g���̒������O�����̒藝���狁�߁A���߂��x�N�g���̒�����Ԃ�
	return sqrtf(x*x + y*y);
}

inline float Math::Length(Vector3 PosA, Vector3 PosB)
{
	//  PosA��PosB�Ƃ̃x�N�g�����v�Z
	//�i����͋��������߂邾���Ȃ̂ŕ����͊֌W�Ȃ��j
	Vector3 Vec = PosA - PosB;

	//  �x�N�g���̒������O�����̒藝���狁�߁A���߂��x�N�g���̒�����Ԃ�
	return sqrtf(Vec.x*Vec.x + Vec.y*Vec.y + Vec.z*Vec.z);
}


//********************************************************************
//						���W�ϊ�
//********************************************************************
Vector2 Math::WorldToScreen(const Vector3 &WorldPos)
{
	// 3D��2D���W�ɂ���
	//�����_��Min~Max�͈̔͂ɗ}����@
	auto Clamp = [](float val, float Min, float Max){
		return min(Max, max(val, Min));
	};
	Matrix m = matView * matProjection;

	Vector2 ret;
	ret.x = WorldPos.x * m._11 + WorldPos.y * m._21 + WorldPos.z * m._31 + 1 * m._41;
	ret.y = WorldPos.x * m._12 + WorldPos.y * m._22 + WorldPos.z * m._32 + 1 * m._42;
	float w = WorldPos.x * m._14 + WorldPos.y * m._24 + WorldPos.z * m._34 + 1 * m._44;

	if (w == 0) ret.x = ret.y = 0;
	else{
		ret.x /= w;
		ret.y /= w;
	}
	ret.x = Clamp(ret.x, -1.0f, 1.0f);
	ret.y = Clamp(ret.y, -1.0f, 1.0f);

	ret.x = (ret.x + 1) * (tdnSystem::GetScreenSize().right / 2);
	ret.y = (((ret.y * -1) + 1) * (tdnSystem::GetScreenSize().bottom / 2));

	return ret;
}

Vector3 Math::ScreenToWorld(const Vector2 &ScreenPos, float ProjectiveSpaceZ)
{
	D3DXMATRIX Viewport = {
		tdnSystem::GetScreenSize().right * 0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -tdnSystem::GetScreenSize().bottom * 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		tdnSystem::GetScreenSize().right * 0.5f, tdnSystem::GetScreenSize().bottom * 0.5f, 0.0f, 1.0f
	};
	D3DXMATRIX InverseView, InverseProjection, InverseViewport;
	D3DXMatrixInverse(&InverseView, NULL, &matView);
	D3DXMatrixInverse(&InverseProjection, NULL, &matProjection);
	D3DXMatrixInverse(&InverseViewport, NULL, &Viewport);

	D3DXVECTOR3 Position;
	D3DXVec3TransformCoord(&Position, &D3DXVECTOR3(static_cast<FLOAT>(ScreenPos.x), static_cast<FLOAT>(ScreenPos.y), ProjectiveSpaceZ),
		&(InverseViewport * InverseProjection * InverseView));

	return Vector3(Position.x, Position.y, Position.z);
}

Vector3 Math::ScreenToWorldPlate(const Vector2 &ScreenPos, Vector3 &PlateNormal, float Shift)
{
	// �X�N���[���ォ��̃v���W�F�N�V������Near��Far�����߂�
	Vector3 NearPosition = Math::ScreenToWorld(ScreenPos, 0.0f);
	Vector3 FarPosition = Math::ScreenToWorld(ScreenPos, 1.0f);

	// Near�̍��W��Far�̍��W���g���ĒP�ʃx�N�g�������
	Vector3 Direction = FarPosition - NearPosition;
	Direction.Normalize();


	/*	���ƕ��ʂɂ���_����
	AX�̒���: XB�̒��� = PA��N�̓��� : PB��N�̓���
	�����ς̓}�C�i�X�l�ɂȂ�ꍇ������̂ŁA��Βl���g���Ă��������B

	��_X = A + �x�N�g��AB * (PA��N�̓��� / (PA��N�̓��� + PB��N�̓���))
	*/

	static const float dist = 65535;	// �Ƃ肠�����ł����l(��_���Ƃ邽�߁A���܂菬�����ƕ��ʂɓ͂��Ȃ�)

	Vector3 PA = NearPosition;
	Vector3 PB = (NearPosition + Direction * dist);
	float XB = abs(Vector3Dot(PA, PlateNormal));

	float pa_n = abs(Vector3Dot(PA, PlateNormal));
	float pb_n = abs(Vector3Dot(PB, PlateNormal));

	return (NearPosition + ((NearPosition + Direction * dist) - NearPosition) * (pa_n / (pa_n + pb_n)));
}

//********************************************************************
//						�x�W�G�Ȑ�
//********************************************************************
void Math::Bezier(Vector3 *out, Vector3 pos_array[], int num_elements_array, float percentage)
{
	assert(num_elements_array > 0);

	float b = percentage;
	float a = 1 - b;

	/*				//		�Q�l����		//
	//�x�W�F�Ȑ����@�܂��@�@�ŏ��ƒ��ԁ@�@�@�@���Ɂ@�@�@�@���ԂƍŌ�
	pos->x = a*a*a* p1.x + 3 * a*a*b*p2.x + 3 * a*b*b*p2.x + b*b*b*p3.x;
	pos->y = a*a*a* p1.y + 3 * a*a*b*p2.y + 3 * a*b*b*p2.y + b*b*b*p3.y;
	pos->z = a*a*a* p1.z + 3 * a*a*b*p2.z + 3 * a*b*b*p2.z + b*b*b*p3.z;
	*/

	// 2�_�Ԃ̒����̏ꍇ�A�x�W�G�v�Z������Ƃ��������Ȃ�̂ŁA�����ɂ�钼���̌v�Z�ɂ���
	if (num_elements_array == 2)
	{
		*out = pos_array[0] * a + pos_array[1] * b;
		return;
	}

	// �n�_
	*out = pos_array[0] * (float)pow(a, num_elements_array);

	// ����
	for (int i = 1; i < num_elements_array - 1; i++)	// -1�Ȃ̂͏I�_���Ȃ�����
	{
		float mult = b;
		for (int j = 1; j < num_elements_array - 1; j++)
		{
			mult *= (j >= i) ? a : b;
		}
		*out += pos_array[i] * (num_elements_array * mult);
	}

	// �I�_
	*out += pos_array[num_elements_array - 1] * (float)pow(b, num_elements_array);
}

// ���
float Math::Clamp(float val, float Min, float Max)
{
	return min(Max, max(val, Min));
}

// �u�����h
float Math::Blend(float Rate, float MinNum, float MaxNum)
{	
	return (MaxNum*Rate) + (MinNum*(1.0f - Rate));
}


// 2D�̊G���v���W�F�N�V����
Vector2 Math::GetProjPos(Vector2 pos)
{
	Vector2 out;
	
	// �X�N���[���̍ő啝�ō��̃|�W�V����������
	out.x = pos.x / tdnSystem::GetScreenSize().right;
	out.y = pos.y / tdnSystem::GetScreenSize().bottom;

	// 0�`1.0��
	Math::Clamp(out.x, 0.0f, 1.0f);
	Math::Clamp(out.y, 0.0f, 1.0f);

	// -1.0�`1.0��
	out.x *= 2.0f;// �܂��@0~2.0��
	out.x -= 1.0f;// �����ā@-1.0~1.0��

	out.y *= 2.0f;// �܂��@0~2.0��
	out.y -= 1.0f;// �����ā@-1.0~1.0��

	return out;
}

//********************************************************************
//						Collision
//********************************************************************

//************************************************************************************
//		���Ƌ��Ƃ̓����蔻��(�������Ă���Ftrue��Ԃ��@�������Ă��Ȃ��Ffalse��Ԃ�)
//************************************************************************************
bool Collision::SphereAndSphere(Vector3 PosA, float RadiusA, Vector3 PosB, float RadiusB)
{
	//  PosA��PosB�̋��������߂�
	float Dist = Math::Length(PosA, PosB);

	//  �������A�����̕��������蔻��̔��a�{�����蔻��̔��a��菬�����Ȃ�����
	if ((RadiusA + RadiusB) > Dist){
		// �������Ă���̂� true ��Ԃ�
		return true;
	}
	// �Ⴄ�Ȃ�΁A���Ԃ�����(�������Ă��Ȃ�)�̂� false ��Ԃ�
	return false;
}
