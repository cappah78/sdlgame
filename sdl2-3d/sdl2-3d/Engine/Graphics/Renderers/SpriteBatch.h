#ifndef SPRITEBATCH_H_
#define SPRITEBATCH_H_

#include <glm\glm.hpp>
#include <vector>
#include "../GL/VertexBuffer.h"

typedef int GLint;
typedef unsigned int GLuint;
typedef unsigned short GLushort;

struct TextureRegion;
class Texture;

/** 
Renderer batching draw calls/vertex data to efficiently draw textures on the xy plane, in the (0, 0, -1) direction
*/
class SpriteBatch
{
public:
	/** 
	- int size: max amount of draw calls per begin/end 
	*/
	SpriteBatch(int size);
	~SpriteBatch();

	/** Prepare the batch for rendering, must have a matching end() call */
	void begin();
	void setProjectionMatrix(glm::mat4 matrix);

	void draw(const Texture& t, float x, float y);
	void draw(const Texture& t, float x, float y, float w, float h);
	/** Draws texture rotated around its center */
	void draw(const Texture& t, float x, float y, float w, float h, float angleDeg);
	/** Draws texture rotated around the given origin, origin is relative to x/y */
	void draw(const Texture& t, float x, float y, float w, float h, float rotOriginX, float rotOriginY, float angleDeg);

	void draw(const TextureRegion& r, float x, float y);
	void draw(const TextureRegion& r, float x, float y, float w, float h);
	void draw(const TextureRegion& r, float x, float y, float w, float h, float angleDeg);
	void draw(const TextureRegion& r, float x, float y, float w, float h, float rotOriginX, float rotOriginY, float angleDeg);

	/** Ends the drawing of more textures and flush the batch */
	void end();
private:
	/** Do error checks and texture swapping, avoids code duplication */
	void preDraw(const Texture& texture);
	/** Render everything drawn so far, must be done before swapping texture */
	void flush();
	/** Swaps the current active texture */
	void swapTexture(const Texture& texture);

	void drawUnrotated(const Texture& t, float x, float y, float w, float h, float u, float v, float u2, float v2);
	void drawRotated(const Texture& t, float x, float y, float w, float h, float rotOriginX, float rotOriginY, float angleDeg, float u, float v, float u2, float v2);

	GLuint m_shader;
	GLuint m_vao;

	VertexBuffer m_verticeBuffer;
	VertexBuffer m_indiceBuffer;

	std::vector<float> m_verticeData;

	GLint m_mvpLoc; // model view projection matrix uniform

	bool m_drawing;	//wether the batch is drawing or not

	int m_size;	// max amount of draw calls
	int m_drawCalls;	// amount of draw calls used since begin()

	GLuint m_lastTexture;	//texture used by last draw call.
	bool m_blendEnabled;
};

#endif //SPRITEBATCH_H_