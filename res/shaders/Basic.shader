#shader vertex
#version 330 core
		
layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;
		
out vec4 vertexColor;
out vec2 TexCoord;

void main()
{
	gl_Position = aPosition;
	vertexColor = aColor;
	TexCoord = aTexCoord;
};

#shader fragment
#version 330 core
out vec4 FragColor;
  
in vec4 vertexColor;
in vec2 TexCoord;

uniform sampler2D outTexture;

void main()
{
    FragColor = texture(outTexture, TexCoord);
} 