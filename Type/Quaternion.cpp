#include "Quaternion.h"

Quaternion::Quaternion(float direction, float elevation)
{

	const float yaw = direction * PI / 180.0f;
	const float pitch = -elevation * PI / 180.0f;
	const float roll = 0.0f;

	const float
		cy = cos(yaw * 0.5f),
		sy = sin(yaw * 0.5f),
		cp = cos(pitch * 0.5f),
		sp = sin(pitch * 0.5f),
		cr = cos(roll * 0.5f),
		sr = sin(roll * 0.5f);

	x = cy * cp * sr - sy * sp * cr;
	y = sy * cp * sr + cy * sp * cr;
	z = sy * cp * cr - cy * sp * sr;
	w = cy * cp * cr + sy * sp * sr;
}

Quaternion Quaternion::angularVelocityApplied(const Vec3 &v)
{
	return {
		0.5f * (y * v.z - z * v.y + w * v.x),
		0.5f * (-x * v.z + z * v.x + w * v.y),
		0.5f * (x * v.y - y * v.x + w * v.z),
		0.5f * (-x * v.x - y * v.y - z * v.z)};
}