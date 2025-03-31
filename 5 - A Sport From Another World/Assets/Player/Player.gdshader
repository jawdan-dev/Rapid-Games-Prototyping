shader_type spatial;

instance uniform vec4 i_color;
void fragment() {
	ALBEDO = i_color.rgb;
}