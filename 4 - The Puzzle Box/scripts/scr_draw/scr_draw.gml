function draw_panel(_sprite, _x, _y, _width, _height, _state = 0, _scale = 1.0) {
	var tw = sprite_get_width(_sprite) / 3, th = sprite_get_height(_sprite) / 3;
	var tws = tw * _scale, ths = th * _scale;
	var right = _x + _width, bottom = _y + _height;
	
	draw_sprite_part_ext(_sprite, _state, 0, 0, tw, th, _x, _y, _scale, _scale, c_white, 1.0);
	draw_sprite_part_ext(_sprite, _state, tw * 2, th * 2, tw, th, right - tws, bottom - ths, _scale, _scale, c_white, 1.0);
	draw_sprite_part_ext(_sprite, _state, tw * 2, 0, tw, th, right - tws, _y, _scale, _scale, c_white, 1.0);
	draw_sprite_part_ext(_sprite, _state, 0, th * 2, tw, th, _x, bottom - ths, _scale, _scale, c_white, 1.0);

	var ow = _width - (tws * 2), oh = _height - (ths * 2);
	var ox = _x + tws, oy = _y + ths;
	var iw = ow / tw , ih = oh / th;

	draw_sprite_part_ext(_sprite, _state, tw, th, tw, th, ox, oy, iw, ih, c_white, 1.0);
	draw_sprite_part_ext(_sprite, _state, tw, 0, tw, th, ox, _y, iw, _scale, c_white, 1.0);
	draw_sprite_part_ext(_sprite, _state, tw, th * 2, tw, th, ox, bottom - ths, iw, _scale, c_white, 1.0);
	draw_sprite_part_ext(_sprite, _state, 0, th, tw, th, _x, oy, _scale, ih, c_white, 1.);
	draw_sprite_part_ext(_sprite, _state, tw * 2, th, tw,th, right - tws, oy, _scale, ih, c_white, 1.);
}

function draw_text_bound(_x, _y, _width, _height, _text, _scale = 1.0, _color = make_color_rgb(246, 214, 189)) {
	if (isDebugView()) draw_rectangle(_x, _y, _x + _width, _y + _height, true);
	
	_scale *= 0.4;
	var sw = _width / _scale , sh = _height / _scale;
	var textWidth = string_width_ext(_text, -1, sw) * _scale;
	var textHeight = string_height_ext(_text, -1, sw) * _scale;
	
	var cx = _x + (_width * 0.5), cy = _y + (_height * 0.5);
	draw_text_ext_transformed_color(cx, cy, _text, -1, sw, _scale, _scale, 0, _color, _color, _color, _color, 1.0);
}
function draw_text_bound_getOffset(_x, _y, _width, _height, _text, _index, _scale = 1.0, _color = make_color_rgb(246, 214, 189)) {
	_scale *= 0.4;
	var sw = _width / _scale , sh = _height / _scale;
	var textWidth = string_width_ext(_text, -1, sw) * _scale;
	var textHeight = string_height_ext(_text, -1, sw) * _scale;
	
	var leftText = string_copy(_text, 0, _index);
	var leftWidth =  string_width_ext(leftText, -1, sw) * _scale;
	
	
	var cx = _x + (_width * 0.5), cy = _y + (_height * 0.5);
	
	return {
		x: cx - (textWidth * 0.5) + leftWidth,
		y: cy,
	};
}