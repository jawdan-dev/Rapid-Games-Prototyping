shader_type spatial;
render_mode ensure_correct_normals;

instance uniform float m_type = 1;
uniform sampler2D m_lightTexture : filter_nearest;
uniform vec4 m_darkColor : source_color;
uniform float gridSize = 4.0;

varying vec3 vertexPos;
varying vec3 v_lightColor;
varying vec3 v_darkColor;

void vertex() {
	vertexPos = VERTEX;
	
	float typeCount = 10.0f;
	float type = clamp((m_type / typeCount) + (0.5 / typeCount), 0, 1);
	v_lightColor = texture(m_lightTexture, vec2(type, 0.5f)).rgb;
	v_darkColor = v_lightColor * m_darkColor.rgb;
}

void fragment() {
	vec3 pos = vertexPos;

	float gridScale = 1.0f / gridSize;
	float checkPos =
		floor(pos.x * gridScale) +
		floor(pos.y * gridScale) +
		floor(pos.z * gridScale);

	vec3 col = (mod(checkPos, 2.0f) < 1.0f) ? v_lightColor : v_darkColor;
	ALBEDO = col;
	ROUGHNESS = 0.0f;
}