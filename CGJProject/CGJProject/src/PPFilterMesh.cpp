#include "PPFilterMesh.h"


PPFilterMesh::PPFilterMesh(GLuint v_coord_id) :
	v_coord(v_coord_id)
{
}

void PPFilterMesh::CreateBufferObjects()
{
	GLfloat fbo_vertices[] = {
		-1, -1,
		1, -1,
		-1,  1,
		1,  1,
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	{
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(fbo_vertices) * 8, fbo_vertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(v_coord);
			glVertexAttribPointer(
				v_coord,			// attribute
				2,                  // number of elements per vertex, here (x,y)
				GL_FLOAT,           // the type of each element
				GL_FALSE,           // take our values as-is
				0,                  // no extra data between each position
				0
			);
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void PPFilterMesh::DestroyBufferObjects()
{
	glDisableVertexAttribArray(v_coord);
	glDeleteVertexArrays(1, &VAO);
	glBindVertexArray(0);
}

void PPFilterMesh::Draw()
{
	//glEnableVertexAttribArray(v_coord);
	glBindVertexArray(VAO);
	//glVertexAttribPointer(
	//	v_coord,			// attribute
	//	2,                  // number of elements per vertex, here (x,y)
	//	GL_FLOAT,           // the type of each element
	//	GL_FALSE,           // take our values as-is
	//	0,                  // no extra data between each position
	//	0                   // offset of first element
	//);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//glDisableVertexAttribArray(v_coord);
	glBindVertexArray(0);
}
