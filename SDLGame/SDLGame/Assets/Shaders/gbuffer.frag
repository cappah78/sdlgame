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

out FSOutput out;

uniform sampler2D u_tex;

void main()	
{	
    out.worldSpacePos = worldSpacePos;	
    out.diffuse = texture(u_tex, texCoord).xyz;	
    out.normal = normalize(normal);	
    out.texCoord = vec3(texCoord, 0.0);	
};
