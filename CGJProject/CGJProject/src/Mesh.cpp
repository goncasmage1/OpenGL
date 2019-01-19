#include "Mesh.h"

//N�o me posso esquecer de eliminar os buffers das tangentes e bitangentes

#define VERTICES 0
#define TEXCOORDS 1
#define NORMALS 2
#define TANGENT 3
#define BITANGENT 4

void Mesh::CreateBufferObjects()
{
	GLuint VboTexcoords, VboNormals, VboTangent, VboBiTangent;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	{
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
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

				if (tangentData.size() > 0 && biTangentData.size() > 0)
				{
					glGenBuffers(1, &VboTangent);
					glBindBuffer(GL_ARRAY_BUFFER, VboTangent);
					glBufferData(GL_ARRAY_BUFFER, tangentData.size() * sizeof(Vec3), &tangentData[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(TANGENT);
					glVertexAttribPointer(TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), 0);

					glGenBuffers(1, &VboBiTangent);
					glBindBuffer(GL_ARRAY_BUFFER, VboBiTangent);
					glBufferData(GL_ARRAY_BUFFER, biTangentData.size() * sizeof(Vec3), &biTangentData[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(BITANGENT);
					glVertexAttribPointer(BITANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), 0);
				}

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
	glDeleteBuffers(1, &VboTexcoords);
	glDeleteBuffers(1, &VboTangent);
	glDeleteBuffers(1, &VboBiTangent);
	glDeleteBuffers(1, &VboNormals);
}

void Mesh::DestroyBufferObjects()
{
	glBindVertexArray(VAO);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(TEXCOORDS);
	glDisableVertexAttribArray(NORMALS);
	glDisableVertexAttribArray(TANGENT);
	glDisableVertexAttribArray(BITANGENT);
	glDeleteBuffers(1, &VboVertices);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::Draw()
{
	glBindVertexArray(VAO);
	GLuint size = (GLuint)Vertices.size();
	if (VerticesPerFace == 3) glDrawArrays(GL_TRIANGLES, 0, size);
	glBindVertexArray(0);
}