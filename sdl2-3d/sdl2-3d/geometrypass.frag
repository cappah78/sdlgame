#version 330 core

in vec3 worldSpacePos;
in vec2 texCoord;
in vec3 normal;

struct FSOutput
{ 
    vec3 worldSpacePos; 
    vec3 diffuse; 
    vec3 normal; 
    vec3 texCoord; 
};

out FSOutput fsOut;

uniform sampler2D gColorMap;

void main()	
{	
    fsOut.worldSpacePos = worldSpacePos;	
    fsOut.diffuse = texture(gColorMap, texCoord).xyz;	
    fsOut.normal = normalize(normal);	
    fsOut.texCoord = vec3(texCoord, 0.0);	
};
