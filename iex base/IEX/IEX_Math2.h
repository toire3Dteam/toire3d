
#ifndef IEX_MATH2_H
#define IEX_MATH2_H

struct Quaternion;
struct Vector3;

void Quaternion_rotate(Quaternion* out, Vector3 axis, // ���K��
	float radian); // -�� ~ ��

#endif // !IEX_MATH2_H
