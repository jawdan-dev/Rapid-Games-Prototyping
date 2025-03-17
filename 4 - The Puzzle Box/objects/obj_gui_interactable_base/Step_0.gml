m_disabled = !m_canInteractFunc();
if (!visible || !m_hoverable || !GUI_canInteract(id)) {
	if (m_hoveredElement == id) 
		m_hoveredElement = noone;
	return;
}

if (m_hoveredElement <= 0) // IDK WHY ITS NOT 'noone' BUT OKAY
	m_hoveredElement = id;