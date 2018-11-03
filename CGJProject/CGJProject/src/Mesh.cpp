#include "Mesh.h"

/////////////
//	Transform
/////////////
Transform::Transform()
{
	Position = Vec3(0.f);
	Rotation = Quat(1.f, 0.f, 0.f, 0.f);
	Scale = Vec3(1.f);
}

Transform::Transform(const Vec3 & Pos, const Quat & Rot, const Vec3 & Scl) :
	Position(Pos), Rotation(Rot), Scale(Scl)
{
}

const Transform Lerp(const Transform & From, const Transform & To, float progress)
{
	Transform Lerped = Transform();
	Lerped.Position = Lerp(From.Position, To.Position, progress);
	Lerped.Rotation = Lerp(From.Rotation, To.Rotation, progress);
	Lerped.Scale = Lerp(From.Scale, To.Scale, progress);
	return Lerped;
}

/////////////
//	Mesh
/////////////
void Mesh::SetAnimationProgress(float progress)
{
	transform = Lerp(startTransform, endTransform, progress);
}

Mat4 Mesh::GetTransformationMatrix()
{
	return Mat4::ScaleMat(transform.Scale) * Mat4::TranslationMat(transform.Position) * transform.Rotation.GetMatrix();
}
