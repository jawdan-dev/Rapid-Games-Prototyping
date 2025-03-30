shader_type spatial;
render_mode ensure_correct_normals;

uniform sampler2D m_lightTexture;
uniform vec4 darkColor : source_color;
uniform float gridSize = 4.0;

varying vec3 v_color;
varying vec3 v_darkColor;

void vertex() {
	float type = clamp(UV.x * 0.25, 0, 1);
	v_color = texture(m_lightTexture, vec2(type, 0.5f)).rgb;
	v_darkColor = v_color * darkColor.rgb;
}

void fragment() {
	vec4 world = INV_VIEW_MATRIX * vec4(VERTEX, 1.0);
	vec3 pos = world.xyz;
	
	float gridScale = 1.0f / gridSize;
	float checkPos = 
		floor(pos.x * gridScale) + 
		floor(pos.y * gridScale) + 
		floor(pos.z * gridScale);
	
	vec3 col = (mod(checkPos, 2.0f) < 1.0f) ? v_color : v_darkColor;
	
	ROUGHNESS = 0.0f;
	ALBEDO = col;
}