#ifndef SPRITEBATCH_H_
#define SPRITEBATCH_H_

#include <glm\glm.hpp>

typedef int GLint;
typedef unsigned int GLuint;
typedef unsigned short GLushort;

struct TextureRegion;
class Texture;

class SpriteBatch
{
public:
	/** Batched rendering object used to efficiently draw textures in 2D
		- int size: max amount of draw calls per begin/end */	//(using the same texture)
	SpriteBatch(int size);
	~SpriteBatch();

	/** Prepare the batch for rendering, must have a matching end() call */
	void begin();
	void setProjectionMatrix(glm::mat4 matrix);

	void draw(Texture& t, float x, float y);
	void draw(Texture& t, float x, float y, float w, float h);
	/** Draws texture rotated around its center */
	void draw(Texture& t, float x, float y, float w, float h, float angleDeg);
	/** Draws texture rotated around the given origin, origin is relative to x/y */
	void draw(Texture& t, float x, float y, float w, float h, float rotOriginX, float rotOriginY, float angleDeg);

	void draw(TextureRegion& r, float x, float y);
	void draw(TextureRegion& r, float x, float y, float w, float h);
	void draw(TextureRegion& r, float x, float y, float w, float h, float angleDeg);
	void draw(TextureRegion& r, float x, float y, float w, float h, float rotOriginX, float rotOriginY, float angleDeg);

	/** Ends the drawing of more textures and flush the batch */
	void end();
private:
	/** Do error checks and texture swapping, avoids code duplication */
	void preDraw(Texture& texture);
	/** Render everything drawn so far, must be done before swapping texture */
	void flush();
	/** Swaps the current active texture */
	void swapTexture(Texture& texture);

	void drawUnrotated(Texture& t, float x, float y, float w, float h, float u, float v, float u2, float v2);
	void drawRotated(Texture& t, float x, float y, float w, float h, float rotOriginX, float rotOriginY, float angleDeg, float u, float v, float u2, float v2);

	GLuint shader;
	GLuint vao;
	GLuint vertexBuffer;
	GLuint indexBuffer;

	GLint mvp; // model view projection matrix uniform

	bool drawing;	//wether the batch is drawing or not

	float* vertices;	//vertex buffer array
	GLushort* indices;	//index buffer array

	int size;	// max amount of draw calls
	int bufferByteSize; //size of vertex buffer in bytes
	int drawCalls;	// amount of draw calls used since begin()

	GLuint lastTexture;	//texture used by last draw call.
	int idx;	//last unused vertex index
	bool blendEnabled;
};

#endif //SPRITEBATCH_H_