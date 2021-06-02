// Request GLSL 3.3
#version 330

// Tex coord a_put from vertex shader
in vec2 v_fragTexCoord;

// This corresponds to the output color to the color buffer
out vec4 outColor;

// This is u_sed for the texture sampling
uniform sampler2D u_Texture;

void main()
{
	// Sample color from texture
    outColor = texture(u_Texture, v_fragTexCoord);
	//outColor = vec4(v_fragTexCoord.x, v_fragTexCoord.y, 1.0, 1.0);
}
