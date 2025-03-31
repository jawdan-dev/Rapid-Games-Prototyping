shader_type canvas_item;

instance uniform float i_healthPercentage = 1.0f;
instance uniform vec4 i_color : source_color;
uniform float u_pulseRate = 3.0f;
uniform float u_pulseDistance = 0.1f;
uniform float u_startRadius = 0.2f;
uniform float u_healthRange = 0.7f;
uniform float u_border = 0.49;

void fragment() {
	vec2 centerChange = UV.xy - vec2(0.5, 0.5);
	float border = 
		(abs(centerChange.x) >= u_border || 
		abs(centerChange.y) >= u_border) ? 1.0f : 0.0f;

	if (i_healthPercentage >= 1.0f) {
		if (border < 1.0f) discard;
		COLOR.a = border;
	} else {
		float pulseDist = u_pulseDistance;
		float pulse = sin(TIME * u_pulseRate) * pulseDist;
		
		float amount = length(centerChange) * inversesqrt(u_startRadius);
		float healthPercentage = 1.0f - ((1.0f - i_healthPercentage) * u_healthRange);
		float health = max(0, amount - (healthPercentage) - pulse);

		COLOR.a = max(border, (health * health) * i_color.a);
	}
	COLOR.rgb = i_color.rgb;
}