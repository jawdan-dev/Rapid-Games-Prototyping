event_inherited()

var yOffset = ease_in(m_lerp) * -50;

draw_sprite_ext(spr_rosetta_fragments, m_fragmentIndex, x, y + yOffset, 1, 1, 0, c_white, 1.0 - m_lerp);
if (m_interactableEnabled && m_isHovered) draw_sprite_ext(spr_rosetta_fragments_highlight, m_fragmentIndex, x, y + yOffset, 1, 1, 0, c_white, 1.0 - m_lerp);