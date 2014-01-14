#version 330 core

#extension GL_EXT_texture_array : enable

uniform sampler2DArray texArr;

in vec3 texCoord;
in vec4 color;
in float eyeDist;

out vec4 out_color;

float computeLinearFogFactor()
{
   float factor = (eyeDist - 10.0f) /
            (100.0f - 10.0f );
   
   // Clamp in the [0,1] range
   factor = clamp( factor, 0.0, 1.0 );
            
   return factor;            
}

void main()
{
    float fogFactor = computeLinearFogFactor();
    vec4 fogColor = fogFactor * vec4(1, 1, 1, 1);

	vec4 texCol = texture2DArray(texArr, texCoord);
	vec4 blockCol = vec4((texCol.rgb + color.rgb) * color.a , texCol.a);

	out_color = mix(blockCol, fogColor,fogFactor) ;
}