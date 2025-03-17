var imageIndex = GUI_isHovered(id) | (GUI_isActive(id) << 1);
if (m_disabled || true) imageIndex = 4;

draw_panel(sprite_index, x, y, sprite_width, sprite_height, imageIndex);

if (m_text != "") {
	
	
	var selectedRemaining = m_wordLookIndex;
	var selectedStart = 0;
	var selectedEnd = 0;
	
	var drawText = "";
	if (isDebugView()) {
		drawText = m_text;
	} else {
		var filteredWord = "";
		var len = string_length(m_text);
		for (var i = 1; i <= len; i++) {
			var char = string_char_at(m_text, i);
			if (char == " " || !stringHasCharacter(VALID_CHARACTERS, char)) {
				if (char == "'") {
					filteredWord += char;
					continue;
				} else if (string_length(filteredWord) > 0) {
					if (selectedRemaining == 0) {
						selectedStart = string_length(drawText);
					}
					
					var doublecheck = Game_filterLine(filteredWord)
					var wordMap = ds_map_find_value(m_customMap, string_lower(doublecheck));
					if (is_undefined(wordMap)) 
						wordMap = Game_getWordMapping(doublecheck)
					
					if (is_undefined(wordMap)) {
						var strlen = string_length(filteredWord);
						for (var j = 0; j < strlen; j++) {
							if (string_char_at(filteredWord, j + 1) == "'") drawText += "'"
							else drawText += "*";
						}
					} else {
						drawText += (string_lower(doublecheck) == wordMap) ? filteredWord : wordMap;	
					}
					
					if (selectedRemaining == 0) {
						selectedEnd = string_length(drawText);
					}
					filteredWord = "";
					selectedRemaining--;
				}
				drawText += char;
				continue;
			}
			filteredWord += char;
		}
		
		if (string_length(filteredWord) > 0) {
			if (selectedRemaining == 0) {
				selectedStart = string_length(drawText);
			}
					
			var doublecheck = Game_filterLine(filteredWord)
			var wordMap = ds_map_find_value(m_customMap, string_lower(doublecheck));
			if (is_undefined(wordMap)) 
				wordMap = Game_getWordMapping(doublecheck)
					
			if (is_undefined(wordMap)) {
				var strlen = string_length(filteredWord);
				for (var j = 0; j < strlen; j++) {
					if (string_char_at(filteredWord, j + 1) == "'") drawText += "'"
					else drawText += "*";
				}
			} else {
				drawText += (string_lower(doublecheck) == wordMap) ? filteredWord : wordMap;	
			}
					
			if (selectedRemaining == 0) {
				selectedEnd = string_length(drawText);
			}
		}
	}
	
	var border = 5;
	draw_text_bound(x + border, y + border, sprite_width - (border * 2), sprite_height - (border * 2), drawText, m_textScale, make_color_rgb(250, 245, 216));
	
	var underline = "";
	for (var i = selectedStart; i < selectedEnd; i++) underline += "_";
	
	var _start = draw_text_bound_getOffset(x + border, y + border, sprite_width - (border * 2), sprite_height - (border * 2), drawText, selectedStart, m_textScale, make_color_rgb(250, 245, 216));
	var _end = draw_text_bound_getOffset(x + border, y + border, sprite_width - (border * 2), sprite_height - (border * 2), drawText, selectedEnd, m_textScale, make_color_rgb(250, 245, 216));
	
	draw_text_bound(_start.x, y + border + 2, _end.x -_start.x, sprite_height - (border * 2), underline, m_textScale, make_color_rgb(250, 245, 216));
}