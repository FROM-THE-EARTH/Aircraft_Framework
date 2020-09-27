#include "Quaternion.h"

Quaternion::Quaternion(float direction, float elevation)
{
	x = std::tan((90 - elevation) * PI / 180.0f);
	y = 0;
	z = 1;
	w = 0;

	*this = normalized();
}

Quaternion Quaternion::angularVelocityApplied(const Vec3 &v)
{
	/*return {
		0.5f * (y * v.z - z * v.y + w * v.x),
		0.5f * (-x * v.z + z * v.x + w * v.y),
		0.5f * (x * v.y - y * v.x + w * v.z),
		0.5f * (-x * v.x - y * v.y - z * v.z)};*/

		
	return {
		0.5f * (-y * v.z + z * v.y + w * v.x),
		0.5f * (x * v.z - z * v.x + w * v.y),
		0.5f * (-x * v.y + y * v.x + w * v.z),
		0.5f * (-x * v.x - y * v.y - z * v.z)};
}