shader_type spatial;
render_mode ensure_correct_normals;

uniform vec4 lightColor : source_color;
uniform vec4 darkColor : source_color;
uniform float gridSize = 4.0;


varying vec3 vertexPos;
void vertex() {
	vertexPos = VERTEX;
}

void fragment(){
	vec3 pos = vertexPos;
	
	float gridScale = 1.0f / gridSize;
	float checkPos = 
		floor(pos.x * gridScale) + 
		floor(pos.y * gridScale) + 
		floor(pos.z * gridScale);
	
	vec3 col = (mod(checkPos, 2.0f) < 1.0f) ? lightColor.rgb : darkColor.rgb;
	
	ROUGHNESS = 0.0f;
	ALBEDO = col;
}