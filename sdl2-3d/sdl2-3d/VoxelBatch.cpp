#include "VoxelBatch.h"

#include <memory.h>

#include "SimpleShader.h"
#include "Material.h"
#include "Camera.h"

#include <stdio.h>
#include <iostream>

const char* CAMERA_TRANSFORM_UNIFORM_NAME = "CameraTransform";
const int CAMERA_TRANSFORM_BINDING_POINT = 0;

VoxelBatch::VoxelBatch(unsigned int sizeInFaces)
	: m_sizeInFaces(sizeInFaces)
	, m_drawing(false)
	, m_drawCalls(0)
	, m_vertexIdx(0)
	, m_texCoordIdx(0)
	, m_blendEnabled(false)
	, m_currentMaterial(NULL)
{
	SimpleShader::Context context;
	SimpleShader shader("voxelshader.vert", "voxelshader.frag", context);
	shader.link();
	m_shaderId = shader.getProgramId();
	glUseProgram(m_shaderId);

	m_indices = new GLushort[sizeInFaces * 6];
	GLushort j = 0;
	for (int i = 0; i < sizeInFaces * 6; i += 6, j += 4) {
		m_indices[i + 0] = (j + 0);
		m_indices[i + 1] = (j + 1);
		m_indices[i + 2] = (j + 2);
		m_indices[i + 3] = (j + 2);
		m_indices[i + 4] = (j + 3);
		m_indices[i + 5] = (j + 0);
	}

	m_vertices = new float[sizeInFaces * FACE_SIZE];
	memset(m_vertices, 0, sizeInFaces * FACE_SIZE * sizeof(float));

	m_texCoords = new float[sizeInFaces * 8];
	memset(m_texCoords, 0, sizeInFaces * 8 * sizeof(float));

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeInFaces * 6 * sizeof(GLushort), m_indices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeInFaces * FACE_SIZE * sizeof(float), m_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0); //position
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &m_texCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeInFaces * 8 * sizeof(float), m_texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0); //uv
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &m_cameraTransformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_cameraTransformBuffer);
	glBindBufferBase(GL_UNIFORM_BUFFER, CAMERA_TRANSFORM_BINDING_POINT, m_cameraTransformBuffer);

	GLuint cameraTransformIdx = glGetUniformBlockIndex(m_shaderId, CAMERA_TRANSFORM_UNIFORM_NAME);
	glUniformBlockBinding(m_shaderId, cameraTransformIdx, CAMERA_TRANSFORM_BINDING_POINT);
}

VoxelBatch::VoxelBatch(const VoxelBatch& copyMe)
{
	assert(false);
}

VoxelBatch::~VoxelBatch()
{
	delete[] m_indices;
	delete[] m_vertices;
	delete[] m_texCoords;
}

void VoxelBatch::begin(const Camera& camera)
{
	assert(!m_drawing && "Call end() before begin()");
	glUseProgram(m_shaderId);
	m_drawing = true;

	m_cameraTransform.VPMatrix = camera.m_combinedMatrix;
	m_cameraTransform.VMatrix = camera.m_viewMatrix;
	m_cameraTransform.PMatrix = camera.m_projectionMatrix;

	glBindBuffer(GL_UNIFORM_BUFFER, m_cameraTransformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraTransform), &m_cameraTransform, GL_STATIC_DRAW);

	glBindVertexArray(m_vao);
}

void VoxelBatch::end()
{
	assert(m_drawing && "Call begin() before end()");
	flush();
	m_drawing = false;
	glUseProgram(0);
}


void VoxelBatch::renderFace(const Face face, int x, int y, int z, const Material& material)
{
	const Material* mat = &material;
	if (m_currentMaterial == NULL)
		swapMaterial(mat);
	else if (material.getDiffuse()->m_texture.getTextureID() != m_currentMaterial->getDiffuse()->m_texture.getTextureID())
	{
		flush();
		swapMaterial(mat);
	}
	else if (m_drawCalls >= m_sizeInFaces)
		flush();

	if ((mat->m_flags & Material::DIFFUSE) == 0)	//if has no diffuse texture return.
		return;

	const glm::vec4& texCoords = mat->m_diffuse->m_texCoords;

	assert(m_drawCalls < m_sizeInFaces && "Amount of draw calls exceeded size");
	m_drawCalls++;

	//origin bottom left back corner facing towards direction of front.
	//bottom left
	glm::vec3 p1(x, y, z);
	//top left
	glm::vec3 p2(x, y, z + 1);
	//top right
	glm::vec3 p3(x + 1, y, z + 1);
	//bottom right
	glm::vec3 p4(x + 1, y, z);

	switch (face)
	{
	case TOP:
		p1.y++;
		p2.y++;
		p3.y++;
		p4.y++;
		m_texCoords[m_texCoordIdx++] = texCoords.x;
		m_texCoords[m_texCoordIdx++] = texCoords.y;
		m_texCoords[m_texCoordIdx++] = texCoords.x;
		m_texCoords[m_texCoordIdx++] = texCoords.w;
		m_texCoords[m_texCoordIdx++] = texCoords.z;
		m_texCoords[m_texCoordIdx++] = texCoords.w;
		m_texCoords[m_texCoordIdx++] = texCoords.z;
		m_texCoords[m_texCoordIdx++] = texCoords.y;
		break;
	case BOTTOM:
		p1.x++;
		p2.x++;
		p3.x--;
		p4.x--;
		m_texCoords[m_texCoordIdx++] = texCoords.z;
		m_texCoords[m_texCoordIdx++] = texCoords.w;
		m_texCoords[m_texCoordIdx++] = texCoords.z;
		m_texCoords[m_texCoordIdx++] = texCoords.y;
		m_texCoords[m_texCoordIdx++] = texCoords.x;
		m_texCoords[m_texCoordIdx++] = texCoords.y;
		m_texCoords[m_texCoordIdx++] = texCoords.x;
		m_texCoords[m_texCoordIdx++] = texCoords.w;
		break;
	case LEFT://
		p3.x--;
		p4.x--;
		p3.y++;
		p4.y++;
		m_texCoords[m_texCoordIdx++] = texCoords.z;
		m_texCoords[m_texCoordIdx++] = texCoords.y;
		m_texCoords[m_texCoordIdx++] = texCoords.x;
		m_texCoords[m_texCoordIdx++] = texCoords.y;
		m_texCoords[m_texCoordIdx++] = texCoords.x;
		m_texCoords[m_texCoordIdx++] = texCoords.w;
		m_texCoords[m_texCoordIdx++] = texCoords.z;
		m_texCoords[m_texCoordIdx++] = texCoords.w;
		break;
	case RIGHT:
		p1.x++;
		p2.x++;
		p1.y++;
		p2.y++;
		m_texCoords[m_texCoordIdx++] = texCoords.x;
		m_texCoords[m_texCoordIdx++] = texCoords.w;
		m_texCoords[m_texCoordIdx++] = texCoords.z;
		m_texCoords[m_texCoordIdx++] = texCoords.w;
		m_texCoords[m_texCoordIdx++] = texCoords.z;
		m_texCoords[m_texCoordIdx++] = texCoords.y;
		m_texCoords[m_texCoordIdx++] = texCoords.x;
		m_texCoords[m_texCoordIdx++] = texCoords.y;
		break;
	case FRONT:
		p1.z++;
		p4.z++;
		p1.y++;
		p4.y++;
		m_texCoords[m_texCoordIdx++] = texCoords.z;
		m_texCoords[m_texCoordIdx++] = texCoords.w;
		m_texCoords[m_texCoordIdx++] = texCoords.z;
		m_texCoords[m_texCoordIdx++] = texCoords.y;
		m_texCoords[m_texCoordIdx++] = texCoords.x;
		m_texCoords[m_texCoordIdx++] = texCoords.y;
		m_texCoords[m_texCoordIdx++] = texCoords.x;
		m_texCoords[m_texCoordIdx++] = texCoords.w;
		break;
	case BACK:
		p2.z--;
		p3.z--;
		p2.y++;
		p3.y++;
		m_texCoords[m_texCoordIdx++] = texCoords.x;
		m_texCoords[m_texCoordIdx++] = texCoords.y;
		m_texCoords[m_texCoordIdx++] = texCoords.x;
		m_texCoords[m_texCoordIdx++] = texCoords.w;
		m_texCoords[m_texCoordIdx++] = texCoords.z;
		m_texCoords[m_texCoordIdx++] = texCoords.w;
		m_texCoords[m_texCoordIdx++] = texCoords.z;
		m_texCoords[m_texCoordIdx++] = texCoords.y;
		break;
	}

	m_vertices[m_vertexIdx++] = p1.x;
	m_vertices[m_vertexIdx++] = p1.y;
	m_vertices[m_vertexIdx++] = p1.z;

	m_vertices[m_vertexIdx++] = p2.x;
	m_vertices[m_vertexIdx++] = p2.y;
	m_vertices[m_vertexIdx++] = p2.z;

	m_vertices[m_vertexIdx++] = p3.x;
	m_vertices[m_vertexIdx++] = p3.y;
	m_vertices[m_vertexIdx++] = p3.z;

	m_vertices[m_vertexIdx++] = p4.x;
	m_vertices[m_vertexIdx++] = p4.y;
	m_vertices[m_vertexIdx++] = p4.z;
}

void VoxelBatch::flush()
{
	if (m_drawCalls == 0)
		return;

	//std::cout << "flushan" << m_vertexIdx << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertexIdx * sizeof(float), m_vertices);

	glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_texCoordIdx * sizeof(float), m_texCoords);

	glDrawElements(GL_TRIANGLES, m_drawCalls * 6, GL_UNSIGNED_SHORT, 0);

	m_drawCalls = 0;
	m_vertexIdx = 0;
	m_texCoordIdx = 0;
}

void VoxelBatch::swapMaterial(const Material* const material)
{
	m_currentMaterial = material;
	const Texture& t = m_currentMaterial->getDiffuse()->m_texture;

	if (t.getNumComponents() == 4) //rgba
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		m_blendEnabled = true;
	}
	else if (m_blendEnabled)
	{
		glDisable(GL_BLEND);
		m_blendEnabled = false;
	}

	t.bind();
}