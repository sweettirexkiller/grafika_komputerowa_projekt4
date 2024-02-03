#shader vertex
#version 330 core
		
layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec2 aTexCoord;
		
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	gl_Position = projection * view * model *  aPosition;
	TexCoord = aTexCoord;
};

#shader fragment
#version 330 core
out vec4 FragColor;


uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 objectColor;
uniform vec3 lightColor;
  
in vec2 TexCoord;

uniform sampler2D outTexture;

void main()
{
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
} 