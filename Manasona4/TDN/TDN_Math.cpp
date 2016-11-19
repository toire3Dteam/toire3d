#include	"TDNLIB.h"

//using namespace Math;

//*****************************************************
//		行列関連
//*****************************************************

//*****************************************************
//		変換行列
//*****************************************************

/**
*@brief				引数のMatに向きと平行移動の情報を入れる(XYZ回転)
*@param[in]	Mat		掛け合わせた結果の行列	
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
*@brief				引数のMatに向きと平行移動の情報を入れる(ZXY回転)
*@param[in]	Mat		掛け合わせた結果の行列
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

void Math::SetTransMatrixFrontVec(Matrix *Mat, Vector3 &vPos, Vector3 &vFrontVec)
{
	Matrix m;
	LookAtLH(m, vPos, vPos + vFrontVec, Vector3(0, 1, 0));
	Mat->_11 = m._11;
	Mat->_12 = m._21;
	Mat->_13 = m._31;
	Mat->_21 = m._12;
	Mat->_22 = m._22;
	Mat->_23 = m._32;
	Mat->_31 = m._13;
	Mat->_32 = m._23;
	Mat->_33 = m._33;
	Mat->_41 = vPos.x;
	Mat->_42 = vPos.y;
	Mat->_43 = vPos.z;
}

//*****************************************************************************
//		ビュー変換行列
//*****************************************************************************
//------------------------------------------------------
//	左手座標系ビュー変換
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
void Math::LookAtLHRoll(Matrix& mat, Vector& Eye, Vector& At, float roll)
{
	Vector3 zaxis(At.x - Eye.x, At.y - Eye.y, At.z - Eye.z);	// カメラZ軸(座標と注視点で出せる)
	zaxis.Normalize();

	const Vector3 up(sinf(roll) * zaxis.z, cosf(roll), sinf(roll) * zaxis.x);	// 仮の上ベクトル。iexは010ベクトル(シェーダーでもやってるかな)で出しているが、この関数ではroll値(Radian)と正面ベクトルで上ベクトル出す
	Vector3 xaxis = Vector3(up.y*zaxis.z - up.z*zaxis.y, up.z*zaxis.x - up.x*zaxis.z, up.x*zaxis.y - up.y*zaxis.x);						// カメラX軸(仮の上ベクトルと正面ベクトルの外積で出す)
	Vector3 yaxis = Vector3(zaxis.y*xaxis.z - zaxis.z*xaxis.y, zaxis.z*xaxis.x - zaxis.x*xaxis.z, zaxis.x*xaxis.y - zaxis.y*xaxis.x);	// カメラY軸(上記のZとXの外積で出す)

	xaxis.Normalize();
	yaxis.Normalize();


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
//		投影変換表列
//*****************************************************************************
//------------------------------------------------------
//	パース投影変換
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
//	平行投影変換
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
//		クォータニオン関連
//
//*****************************************************************************

//*****************************************************************************
//		変換
//*****************************************************************************
//------------------------------------------------------
//	行列への変換
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
//		球面線形補間
//------------------------------------------------------
Quaternion QuaternionSlerp(Quaternion& q, Quaternion& r, float t)
{
	//	内積
	float dot = q.x*r.x + q.y*r.y + q.z*r.z + q.w*r.w;
	float ss = 1.0f - (dot * dot);
	if (ss <= 0.0f) return q;

	Quaternion Target = r;
	if (dot < 0.0f)
	{
		Target = -Target;
		dot = -dot;
	}
	//	内積値補正
	if (dot > 1.0f) dot = 1.0f;

	//	補間割合計算
	float ph = acosf(dot);
	float s = 1 / sinf(ph);
	float s1 = sinf(ph * (1.0f - t)) * s;
	float s2 = sinf(ph *  t) * s;
	//	補間
	return q*s1 + Target*s2;
}

//*****************************************************
//		距離の計算
//*****************************************************
float Math::Length(float x1, float y1, float x2, float y2)
{
	float x = x1 - x2;
	float y = y1 - y2;

	//  ベクトルの長さを三平方の定理から求め、求めたベクトルの長さを返す
	return sqrtf(x*x + y*y);
}

float Math::Length(const Vector2 &PosA, const Vector2 &PosB)
{
	const Vector2 v(PosA - PosB);
	return sqrtf(v.x * v.x + v.y * v.y);
}

float Math::Length(const Vector3 &PosA, const Vector3 &PosB)
{
	//  PosAとPosBとのベクトルを計算
	//（今回は距離を求めるだけなので方向は関係ない）
	Vector3 Vec = PosA - PosB;

	//  ベクトルの長さを三平方の定理から求め、求めたベクトルの長さを返す
	return sqrtf(Vec.x*Vec.x + Vec.y*Vec.y + Vec.z*Vec.z);
}


//********************************************************************
//						座標変換
//********************************************************************
Vector2 Math::WorldToScreen(const Vector3 &WorldPos)
{
	// 3Dを2D座標にする
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

Vector2 Math::WorldToProj(const Vector3 & WorldPos)
{
	// 3Dを2D座標にする
	Matrix m = matView * matProjection;

	Vector2 ret;
	ret.x = WorldPos.x * m._11 + WorldPos.y * m._21 + WorldPos.z * m._31 + 1 * m._41;
	ret.y = WorldPos.x * m._12 + WorldPos.y * m._22 + WorldPos.z * m._32 + 1 * m._42;
	float w = WorldPos.x * m._14 + WorldPos.y * m._24 + WorldPos.z * m._34 + 1 * m._44;

	if (w == 0) ret.x = ret.y = 0;
	else {
		ret.x /= w;
		ret.y /= w;
	}
	ret.x = Clamp(ret.x, -1.0f, 1.0f);
	ret.y = Clamp(ret.y, -1.0f, 1.0f);

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
	// スクリーン上からのプロジェクションのNearとFarを求める
	Vector3 NearPosition = Math::ScreenToWorld(ScreenPos, 0.0f);
	Vector3 FarPosition = Math::ScreenToWorld(ScreenPos, 1.0f);

	// Nearの座標とFarの座標を使って単位ベクトルを作る
	Vector3 Direction = FarPosition - NearPosition;
	Direction.Normalize();


	/*	線と平面による交点判定
	AXの長さ: XBの長さ = PAとNの内積 : PBとNの内積
	※内積はマイナス値になる場合があるので、絶対値を使ってください。

	交点X = A + ベクトルAB * (PAとNの内積 / (PAとNの内積 + PBとNの内積))
	*/

	static const float dist = 65535;	// とりあえずでかい値(交点をとるため、あまり小さいと平面に届かない)

	Vector3 PA = NearPosition;
	Vector3 PB = (NearPosition + Direction * dist);
	float XB = abs(Vector3Dot(PA, PlateNormal));

	float pa_n = abs(Vector3Dot(PA, PlateNormal));
	float pb_n = abs(Vector3Dot(PB, PlateNormal));

	return (NearPosition + ((NearPosition + Direction * dist) - NearPosition) * (pa_n / (pa_n + pb_n)));
}

//********************************************************************
//						ベジエ曲線
//********************************************************************
void Math::Bezier(Vector3 *out, Vector3 PosArray[], int NumArray, float percentage)
{
	assert(NumArray > 0);

	float b = percentage;
	float a = 1 - b;

	/*				//		参考資料		//
	//ベジェ曲線↓　まず　　最初と中間　　　　次に　　　　中間と最後
	pos->x = a*a*a* p1.x + 3 * a*a*b*p2.x + 3 * a*b*b*p2.x + b*b*b*p3.x;
	pos->y = a*a*a* p1.y + 3 * a*a*b*p2.y + 3 * a*b*b*p2.y + b*b*b*p3.y;
	pos->z = a*a*a* p1.z + 3 * a*a*b*p2.z + 3 * a*b*b*p2.z + b*b*b*p3.z;
	*/

	// 2点間の直線の場合、ベジエ計算をするとおかしくなるので、割合による直線の計算にする
	if (NumArray == 2)
	{
		*out = PosArray[0] * a + PosArray[1] * b;
		return;
	}

	// 始点
	*out = PosArray[0] * (float)pow(a, NumArray);

	// 中間
	for (int i = 1; i < NumArray - 1; i++)	// -1なのは終点を省くから
	{
		float mult = b;
		for (int j = 1; j < NumArray - 1; j++)
		{
			mult *= (j >= i) ? a : b;
		}
		*out += PosArray[i] * (NumArray * mult);
	}

	// 終点
	*out += PosArray[NumArray - 1] * (float)pow(b, NumArray);
}

void Math::Bezier(float *out, float FloatArray[], int NumArray, float percentage)
{
	assert(NumArray > 0);

	float b = percentage;
	float a = 1 - b;

	// 2点間の直線の場合、ベジエ計算をするとおかしくなるので、割合による直線の計算にする
	if (NumArray == 2)
	{
		*out = FloatArray[0] * a + FloatArray[1] * b;
		return;
	}

	// 始点
	*out = FloatArray[0] * (float)pow(a, NumArray);

	// 中間
	for (int i = 1; i < NumArray - 1; i++)	// -1なのは終点を省くから
	{
		float mult = b;
		for (int j = 1; j < NumArray - 1; j++)
		{
			mult *= (j >= i) ? a : b;
		}
		*out += FloatArray[i] * (NumArray * mult);
	}

	// 終点
	*out += FloatArray[NumArray - 1] * (float)pow(b, NumArray);
}
void Math::Bezier(ViewData *out, ViewData InfoArray[], int NumArray, float percentage)
{
	assert(NumArray > 0);

	float b = percentage;
	float a = 1 - b;

	// 2点間の直線の場合、ベジエ計算をするとおかしくなるので、割合による直線の計算にする
	if (NumArray == 2)
	{
		out->pos = InfoArray[0].pos * a + InfoArray[1].pos * b;
		out->target = InfoArray[0].target * a + InfoArray[1].target * b;
		out->roll = InfoArray[0].roll * a + InfoArray[1].roll * b;
		return;
	}

	// 始点
	out->pos = InfoArray[0].pos * (float)pow(a, NumArray);
	out->target = InfoArray[0].target * (float)pow(a, NumArray);
	out->roll = InfoArray[0].roll * (float)pow(a, NumArray);

	// 中間
	for (int i = 1; i < NumArray - 1; i++)	// -1なのは終点を省くから
	{
		float mult = b;
		for (int j = 1; j < NumArray - 1; j++)
		{
			mult *= (j >= i) ? a : b;
		}
		out->pos += InfoArray[i].pos * (NumArray * mult);
		out->target += InfoArray[i].target * (NumArray * mult);
		out->roll += InfoArray[i].roll * (NumArray * mult);
	}

	// 終点
	out->pos += InfoArray[NumArray - 1].pos * (float)pow(b, NumArray);
	out->target += InfoArray[NumArray - 1].target * (float)pow(b, NumArray);
	out->roll += InfoArray[NumArray - 1].roll * (float)pow(b, NumArray);
}

// 補間
float Math::Clamp(float val, float Min, float Max)
{
	return min(Max, max(val, Min));
}

// ブレンド
float Math::Blend(float Rate, float MinNum, float MaxNum)
{	
	return (MaxNum*Rate) + (MinNum*(1.0f - Rate));
}


// 2Dの絵をプロジェクション
Vector2 Math::GetProjPos(Vector2 pos)
{
	Vector2 out;
	
	// スクリーンの最大幅で今のポジションを割る
	out.x = pos.x / tdnSystem::GetScreenSize().right;
	out.y = pos.y / tdnSystem::GetScreenSize().bottom;

	// 0～1.0へ
	Math::Clamp(out.x, 0.0f, 1.0f);
	Math::Clamp(out.y, 0.0f, 1.0f);

	// -1.0～1.0へ
	out.x *= 2.0f;// まず　0~2.0へ
	out.x -= 1.0f;// 引いて　-1.0~1.0へ

	out.y *= 2.0f;// まず　0~2.0へ
	out.y -= 1.0f;// 引いて　-1.0~1.0へ

	return out;
}

//********************************************************************
//						Collision
//********************************************************************

//************************************************************************************
//		球と球との当たり判定(当たっている：trueを返す　当たっていない：falseを返す)
//************************************************************************************
//bool Collision::SphereAndSphere(Vector3 PosA, float RadiusA, Vector3 PosB, float RadiusB)
//{
//	//  PosAとPosBの距離を求める
//	float Dist = Math::Length(PosA, PosB);
//
//	//  もしも、距離の方が当たり判定の半径＋当たり判定の半径より小さくなったら
//	if ((RadiusA + RadiusB) > Dist){
//		// 当たっているので true を返す
//		return true;
//	}
//	// 違うならば、隙間がある(当たっていない)ので false を返す
//	return false;
//}
