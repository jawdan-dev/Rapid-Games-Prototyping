with (obj_gui_solve_text) {
	var words = Game_getLineWords(m_text);
	var word = words[m_wordLookIndex];

	if (string_length(word) != string_length(obj_gui_solve_textInput.m_text)) return;

	ds_map_add(m_customMap, word, obj_gui_solve_textInput.m_text);

	m_wordLookIndex++;
	if (m_wordLookIndex >= array_length(words)) 
		m_wordLookIndex = 0
}