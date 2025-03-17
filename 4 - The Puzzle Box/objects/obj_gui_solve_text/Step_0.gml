m_disabled = !m_canInteractFunc();
m_text = obj_game_manager.m_puzzle;

if (!visible || !m_hoverable || !GUI_canInteract(id)) {
	if (m_hoveredElement == id) 
		m_hoveredElement = noone;
	return;
}

if (m_hoveredElement <= 0) // IDK WHY ITS NOT 'noone' BUT OKAY
	m_hoveredElement = id;
	