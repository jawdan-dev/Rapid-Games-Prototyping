if (m_activeElement == id) {
	var filtered = Game_filterLine(keyboard_string);
	while (string_length(filtered) > 20) 
		filtered = string_delete(filtered, string_length(filtered) - 1, 1);
	filtered = string_trim(filtered, [ " " ]);
	
	if (filtered != m_text) {
		m_text = filtered;
		event_user(0);
		keyboard_string = m_text;
	}
	
	if (keyboard_check_pressed(vk_enter)) {
		with (obj_gui_solve_set) {
			event_user(0);	
		}
	}
}


m_disabled = !m_canInteractFunc();
if (!visible || !m_hoverable || !GUI_canInteract(id)) {
	if (m_hoveredElement == id) 
		m_hoveredElement = noone;
	return;
}

if (m_hoveredElement <= 0) // IDK WHY ITS NOT 'noone' BUT OKAY
	m_hoveredElement = id;
