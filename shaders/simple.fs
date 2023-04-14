#version 330 core
out vec4 FragColour;  
in vec3 colour;
in vec2 textureCoordinate;

uniform sampler2D texture0;

void main()
{
//    FragColour = vec4(colour, 1.0);
    FragColour = texture(texture0, textureCoordinate) * vec4(colour, 1.0);  
//    FragColour = texture(texture0, textureCoordinate);
//	FragColour = vec4(1,1,1,1);
}