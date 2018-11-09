#include "Mesh.h"

#define VERTICES 0
#define TEXCOORDS 1
#define NORMALS 2

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
	TransformationMat = Mat4::ScaleMat(Scale) * Mat4::TranslationMat(Position) * Rotation.GetMatrix();
}

const Transform Lerp(const Transform & From, const Transform & To, float progress)
{
	Transform Lerped = Transform();
	Lerped.Position = Lerp(From.Position, To.Position, progress);
	Lerped.Rotation = Lerp(From.Rotation, To.Rotation, progress);
	Lerped.Scale = Lerp(From.Scale, To.Scale, progress);
	return Lerped;
}

void Transform::UpdateTransformationMatrix()
{
	TransformationMat = Mat4::ScaleMat(Scale) * Mat4::TranslationMat(Position) * Rotation.GetMatrix();
}

Mat4 Transform::GetTransformationMatrix()
{
	return TransformationMat;
}

/////////////
//	Mesh
/////////////

Mesh::Mesh() :
	transform(), startTransform(), endTransform()
{
}

Mesh::Mesh(Transform newTransform) :
	transform(newTransform)
{
}

Mesh::Mesh(Transform newTransform, Transform newStartTransform, Transform newEndTransform) :
	transform(newTransform), startTransform(newStartTransform), endTransform(newEndTransform)
{
}

void Mesh::SetAnimationProgress(float progress)
{
	transform = Lerp(startTransform, endTransform, progress);
}

Mat4 Mesh::GetTransformationMatrix()
{
	return transform.GetTransformationMatrix();
}

void Mesh::CreateBufferObjects()
{
	GLuint VboVertices, VboTexcoords, VboNormals;

	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	{
		glGenBuffers(1, &VboId);

		glBindBuffer(GL_ARRAY_BUFFER, VboId);
		{
			glGenBuffers(1, &VboVertices);
			glBindBuffer(GL_ARRAY_BUFFER, VboVertices);
			glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vec3), &Vertices[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(VERTICES);
			glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), 0);

			if (TexcoordsLoaded)
			{
				glGenBuffers(1, &VboTexcoords);
				glBindBuffer(GL_ARRAY_BUFFER, VboTexcoords);
				glBufferData(GL_ARRAY_BUFFER, Texcoords.size() * sizeof(Vec2), &Texcoords[0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(TEXCOORDS);
				glVertexAttribPointer(TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), 0);
			}
			if (NormalsLoaded)
			{
				glGenBuffers(1, &VboNormals);
				glBindBuffer(GL_ARRAY_BUFFER, VboNormals);
				glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(Vec3), &Normals[0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(NORMALS);
				glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), 0);
			}
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VboVertices);
	glDeleteBuffers(1, &VboTexcoords);
	glDeleteBuffers(1, &VboNormals);
}

void Mesh::DestroyBufferObjects()
{
	glBindVertexArray(VaoId);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(TEXCOORDS);
	glDisableVertexAttribArray(NORMALS);
	glDeleteBuffers(1, &VboId);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::Draw()
{
	glBindVertexArray(VaoId);
	glDrawArrays(GL_TRIANGLES, (GLuint)Vertices.size(), 3);
	glBindVertexArray(0);
}
