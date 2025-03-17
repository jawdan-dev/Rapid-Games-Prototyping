obj_game_manager.m_inPuzzle = false;

with (obj_gui_solve_text) {
	if (obj_gui_solve_text.m_text == "") return;
	
	var keys = ds_map_keys_to_array(m_customMap);
	var keyCount = array_length(keys);
	
	for (var i = 0; i < keyCount; i++) {
		Game_addWordStatistic(keys[i], ds_map_find_value(m_customMap, keys[i]), 1);
	}
	
	var lineWords = Game_getLineWords(m_text);
	var lineWordCount = array_length(lineWords);
	for (var j = 0; j < lineWordCount; j++) {
		var found = false;
		for (var i = 0; i < keyCount; i++) {
			if (keys[i] != lineWords[j]) continue;
			found = true;
			break;
		}
		if (found) continue;
		
		var mapping = Game_getWordMapping(lineWords[j]);
		if (is_undefined(mapping)) continue;
		Game_addWordStatistic(lineWords[j], mapping, 0.5);
	}
	
	
	obj_player.m_closestInteractable.m_puzzle = "";
	obj_player.m_closestInteractable.m_networkUpdateData = true;
	obj_game_manager.m_puzzleQueue[array_length(obj_game_manager.m_puzzleQueue)] = obj_player.m_closestInteractable;
}