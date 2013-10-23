#ifndef CONFIG_H_
#define CONFIG_H_

// Note: since each uniform buffer may at most be 64kb, care must be taken that the grid resolution doesnt exceed this
//       e.g. 1920x1200 wont do with 16x16.
#define LIGHT_GRID_TILE_DIM_X 64
#define LIGHT_GRID_TILE_DIM_Y 64
// used for clustered forward
#define LIGHT_GRID_MAX_DIM_Z 256 

// Max screen size of 1920x1080
#define LIGHT_GRID_MAX_DIM_X ((1920 + LIGHT_GRID_TILE_DIM_X - 1) / LIGHT_GRID_TILE_DIM_X)
#define LIGHT_GRID_MAX_DIM_Y ((1080 + LIGHT_GRID_TILE_DIM_Y - 1) / LIGHT_GRID_TILE_DIM_Y)

// the maximum number if PointLights supported, this is limited by constant buffer size, commonly
// this is 64kb, but AMD only seem to allow 2048 PointLights...
#define NUM_POSSIBLE_LIGHTS (1024)

// Must be a power of two, configure to suit hardware/wishes, also limited at runtime 
// by glGet GL_MAX_COLOR_TEXTURE_SAMPLES. The smaller value is used.
// Note that 16x appears to be supported using super sampling on NVidia GPUs. For higher, CSAA should really be used.
#define MAX_ALLOWED_MSAA_SAMPLES 8


#define DEFAULT_SHADING_MODEL_STRING "SHADING_MODEL_DEFAULT"

#endif  // CONFIG_H_