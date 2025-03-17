var imageIndex = GUI_isHovered(id) | (GUI_isActive(id) << 1);
if (m_disabled) imageIndex = 4;

draw_panel(sprite_index, x, y, sprite_width, sprite_height, imageIndex);

var border = 5;
draw_text_bound(x + border, y + border, sprite_width - (border * 2), sprite_height - (border * 2), m_text, m_textScale, make_color_rgb(250, 245, 216));