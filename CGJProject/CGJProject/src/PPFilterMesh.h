#pragma once

#include "Mesh.h"

class PPFilterMesh : public Mesh {

protected:

	GLuint v_coord;

public:

	PPFilterMesh(GLuint v_coord_id);

	virtual void CreateBufferObjects() override;
	virtual void DestroyBufferObjects() override;

	virtual void Draw() override;

};