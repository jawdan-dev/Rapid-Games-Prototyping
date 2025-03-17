if (!m_enabled) return;

draw_sprite_ext(sprite_index, image_index, m_drawX, m_drawY, m_flipped ? -1 : 1, 1, 0, c_white, 1.0);
event_inherited();