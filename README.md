Simple high performance work in progress Minecraft like Voxel Renderer with lua scripting.

Should be instantly compileable/runnable with VS2013/12 and perhaps older versions.

Random screenshot of last finished feature:
https://dl.dropboxusercontent.com/u/18555381/Permanent/GravelBlockScripting.png
https://dl.dropboxusercontent.com/u/18555381/Permanent/aowithtextures.png


To run the game as a standalone, copy the sdl2-3d/Assets folder to the Release (or Debug) folder.

<b> Design </b>

The engine will be using a data oriented design, meaning a lot of focus is around making the data easily iterateable with as few cache misses as possible.

All game objects will be Data Driven, e.g. fully created and configurable through scripts and other files without recompiling the engine.

The renderer will be fully opengl based and should be easy to swap out for another render implementation.

<b> Layout </b>

The game loop primarily takes place in GameScreen.cpp, which initializes and renders the VoxelWorld, camera and other things like the skybox. The GameScreen holds a WorldRenderer which can render VoxelWorlds, this makes it easy to swap out renderers without needing to integrate render logic in the VoxelWorld.

VoxelWorld manages block types and properties of every type, contains all the Lua high level lua bindings like World.setBlock(id, x, y, z), and holds a ChunkManager which loads/unloads chunks and allows easy retrieval of specific chunks.

ChunkManager has a LuaChunkGenerator which will be in charge of generating any new chunks created for the first time using a Lua script to be easily modifiable.

<b> Performance and Rendering </b>

Chunks are 16x16x16 blocks to have a good balance between render calls, time to rebuild a chunk's vertices, and culling.

Chunks hold a list of BlockID's(ushort) and Colors (rgba8888 = 32bits), and an expandable list of bytes which is able to efficiently contain any per-block data needed for logic. Block logic will be done through Lua Scripts, the data will only be stored on the c++ side per chunk in the data list, being efficiently packed in memory so all block scripts can done in one pass while reading the block specific properties once and pushing them to lua.


WorldRenderer handles the high level rendering, iterating chunks, blocks, faces and vertices to calculate Ambient Occlusion and smooth lighting. It uses an instance of the lower level renderer VoxelRenderer which is able to add faces to a container VoxelRenderer::Chunk which holds the render data (VAO and Buffers) of a single chunk. All chunks are sharing a single array of colors, indices and vertex positions to build the chunk with faces before pushing the data to the GPU.

The renderer uses a minimal amount of memory, every vertex only using having 2 integers.
The shader (voxelshader.vert/frag), uses

layout(location = 0) in uint in_point;

to contain the block's x/y/z in the first 15 bits of the integer, and 10 bits for the Texture ID which is used to sample the texture from a 3d Texture (OpenGL Texture Array), ID's are managed by a TextureManager to make it easy to retrieve texturesID's cpu side. Vertices are translated by the x/y/z (unpacked from the integer), multiplied by the mvp matrix which is already offset by the Chunk offset cpu side.

Color uses an rgba8888(32 bit integer, Color8888.cpp) buffer, which gets normalized to float for the shader. The alpha component will indicate the light level and ambient occlusion tint, while rgb can be used to tint the block with a color.

The texture coordinate buffer is a static buffer shared by all chunks.

<b> WIP: </b>
- Scriptable triggers for block updates:

Scriptable triggerable events with a simple evaluation parsed and checked in c++
to reduce amount of lua calls significantly.

<b> TODO: </b>

- Vertex based mincraft like lighting.
- Actual chunk generation (currently using a single call to a lua method to build the world)
- More Lua bindings between c++/lua (to script things like controls and interactions)
- Higher detail Ambient Occlusion like Cube World, sampling from a larger radius.
- Chunk saving/loading
- More efficient block update system

<b> Done: </b>

- Processing blocks through scripts with configurable values for every block
- Basic World/Chunk layout
- Basic high efficienty voxel renderer
- Basic Ambient occlusion calculations for voxels
- Basic Lua bindings to setblock and create new block types from scripts.
- Learn OpenGL/GLSL/C++



