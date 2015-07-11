
#include "IEX_Input2.h"

float Key_axis_normalize(float deadzone, int key_axis)
{
	if (-deadzone < key_axis && key_axis < deadzone)
		return 0.0f;

	return key_axis * 0.001f;
}
