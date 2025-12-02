#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 objectColor; // Used for tinting (Green for working, Red for broken)

void main()
{
    vec4 texColor = texture(texture1, TexCoord);
    // Mix the texture color with the object color. 
    // If objectColor is white (1,1,1), it shows the texture as is.
    // If objectColor is red (1,0,0), it tints it red.
    FragColor = texColor * vec4(objectColor, 1.0);
}
