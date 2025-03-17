with (obj_gui_solve_text) {
	m_wordLookIndex++;
	var words = Game_getLineWords(m_text);
	if (m_wordLookIndex >= array_length(words)) 
		m_wordLookIndex = 0
}