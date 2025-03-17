if (m_puzzle != "") {
	m_lerp = max(m_lerp - getDeltaTime() * 2, 0);
} else {
	m_lerp = min(m_lerp + getDeltaTime() * 2, 1);
}
m_interactableEnabled = m_lerp <= 0;

if (m_puzzle == "" && obj_server.m_isHosting && m_lerp >= 1) {
	var found = false;
	var len = array_length(obj_game_manager.m_puzzleQueue);
	for (var i = 0; i < len; i++) {
		if (obj_game_manager.m_puzzleQueue[i] != id) continue;
		found = true;
		break;
	}
	
	if (!found)
		obj_game_manager.m_puzzleQueue[len] = id;
}

