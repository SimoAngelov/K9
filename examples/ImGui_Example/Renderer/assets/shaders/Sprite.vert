// Request GLSL 3.3
#version 330

// uniforms for world transform and view-proj
uniform mat4 u_WorldTransform;
uniform mat4 u_ViewProj;

// Attribute 0 is position, 1 is tex coords.
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

// Any vertex outputs (other than position)
out vec2 v_fragTexCoord;

void main()
{
	// Convert position to homogeneous coordinates
	vec4 pos = vec4(a_Position, 1.0);

	// Transform to position world space, then clip space
	gl_Position = u_ViewProj * u_WorldTransform * pos;

	// Pass along the texture coordinate to frag shader
	v_fragTexCoord = a_TexCoord;
}
