with (obj_player) {
	if (obj_server.m_isHosting) {
		other.m_isHovered = false;	
	} else {
		other.m_isHovered = m_closestInteractable == other.id;
	}
}
