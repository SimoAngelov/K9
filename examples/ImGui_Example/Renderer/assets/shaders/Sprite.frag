// Request GLSL 3.3
#version 330

// This is used for the texture sampling
uniform sampler2D u_Texture;

// Uniform tint color.
uniform vec4 u_Color;

// Tex coord a_put from vertex shader
in vec2 v_fragTexCoord;

// This corresponds to the output color to the color buffer
out vec4 outColor;

void main()
{
	// Sample color from texture
    outColor = texture(u_Texture, v_fragTexCoord) * u_Color;
	//outColor = vec4(v_fragTexCoord.x, v_fragTexCoord.y, 1.0, 1.0);
}
