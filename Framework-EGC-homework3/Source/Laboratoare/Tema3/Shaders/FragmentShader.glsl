#version 330

uniform sampler2D texture;

struct SpotLight{
	vec3 light_direction;
	vec3 light_position;
	float material_kd;
	float material_ka;
	float material_ks;
	int material_shininess;
	float attenuation_factor;
	float cutoff_angle;
};

in vec3 world_position;
in vec3 world_normal;
in vec2 tex_coord;

uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ka;
uniform float material_ks;
uniform int material_shininess;
uniform float attenuation_factor;

uniform SpotLight spots[4];

layout(location = 0) out vec4 out_color;

void main()
{
	vec4 colorAfterTexture = texture2D(texture, tex_coord);

	vec3 L = normalize( light_position - world_position );
	vec3 V = normalize( eye_position - world_position );
	vec3 H = normalize( L + V );

	vec4 ambient_light = material_ka * colorAfterTexture;
	float diffuse_light = material_kd * max(dot(world_normal, L), 0);
	float specular_light = material_ks *  pow ( max(dot(world_normal, H), 0), material_shininess);

	if(diffuse_light <= 0){
		specular_light = 0;
	}

	float distanceToLight = length(light_position - world_position);
	float attenuation = 1.0 / (1.0 + attenuation_factor * pow(distanceToLight, 2));

	vec4 light_color = ambient_light + attenuation * (diffuse_light + specular_light);
	
	for(int i = 0; i < 4; i++){
		vec3 L = normalize( spots[i].light_position - world_position );
		vec3 V = normalize( eye_position - world_position );
		vec3 H = normalize( L + V );

		vec4 ambient_light = spots[i].material_ka * colorAfterTexture;
		float diffuse_light = spots[i].material_kd * max(dot(world_normal, L), 0);
		float specular_light = spots[i].material_ks *  pow ( max(dot(world_normal, H), 0), spots[i].material_shininess);

		if(diffuse_light <= 0){
			specular_light = 0;
		}

		float distanceToLight = length(spots[i].light_position - world_position);
		float attenuation = 1.0 / (1.0 + spots[i].attenuation_factor * pow(distanceToLight, 2));

	
		float cut_off = radians(spots[i].cutoff_angle);
		float spot_light = dot(-L, spots[i].light_direction);
		if (spot_light > cos(cut_off))
		{
			attenuation = (spot_light - cos(cut_off))/(1 - cos(cut_off));
			attenuation = pow(attenuation, 2);
		}else{
			attenuation = 0;
		}

		vec4 partial_color = ambient_light + attenuation * (diffuse_light + specular_light); 

		light_color += partial_color;
	}

	out_color = light_color;
}