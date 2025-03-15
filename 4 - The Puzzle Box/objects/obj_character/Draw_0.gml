if (!m_enabled) return;

draw_sprite(sprite_index, image_index, m_drawX, m_drawY);
draw_text(m_drawX, m_drawY, string(m_networkID));