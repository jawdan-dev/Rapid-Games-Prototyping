event_inherited();

var words = Game_getLineWords(obj_gui_solve_text.m_text);
var word = words[obj_gui_solve_text.m_wordLookIndex];
	
if (string_length(m_text) > 0) {
	m_possibleOptions = Game_getPossibleWordOptions(m_text, 15, string_length(word));
} else {
	m_possibleOptions = [];	
}