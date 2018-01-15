#version 330 core
in vec2 textureC;
out vec4 colour;

//Text contents
uniform sampler2D text;
//Colour of text
uniform vec3 textColour;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, textureC).r);
    colour = vec4(textColour, 1.0) * sampled;
}  