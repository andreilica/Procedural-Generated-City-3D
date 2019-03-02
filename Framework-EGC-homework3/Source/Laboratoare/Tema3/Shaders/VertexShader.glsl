#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform int isCue;
uniform float magnitude;

out vec2 tex_coord;
out vec3 world_position;
out vec3 world_normal;

void main()
{
		 
	world_position = (Model * vec4(v_position,1)).xyz;

	world_normal = normalize( mat3(Model) * v_normal );

	tex_coord = v_texture_coord;

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);

}
