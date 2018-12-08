#include "Mesh.h"
#include "Math/Vector.h"

#define VERTICES 0
#define TEXCOORDS 1
#define NORMALS 2

void Mesh::CreateBufferObjects()
{
	GLuint VboTexcoords, VboNormals;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	{
		glGenBuffers(2, VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		{
			glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vec3), &vertexData[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(VERTICES);
			glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), 0);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[1]);
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertexIdx), &vertexIdx[0], GL_STATIC_DRAW);
		}

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
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VboTexcoords);
	glDeleteBuffers(1, &VboNormals);
}

void Mesh::DestroyBufferObjects()
{
	glBindVertexArray(VAO);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(TEXCOORDS);
	glDisableVertexAttribArray(NORMALS);
	glDeleteBuffers(2, VBO);
	glDeleteVertexArrays(1, &VAO);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::Draw()
{
	glBindVertexArray(VAO);
	GLuint size = (GLuint)vertexIdx.size();
	if (VerticesPerFace == 3)
	{
		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, (GLvoid*)0);
	}
	/*else if (VerticesPerFace == 4)
	{
		for (int i = 0; i < size/4; i+=4)
		{
			glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4);
		}
	}*/
	glBindVertexArray(0);
}
