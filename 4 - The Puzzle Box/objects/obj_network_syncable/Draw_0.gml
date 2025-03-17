if (isDebugView()) {
	draw_set_color(c_black);	
	draw_text(x - sprite_get_xoffset(sprite_index), y - sprite_get_yoffset(sprite_index), string(m_networkID));
	draw_set_color(c_white);	
}



