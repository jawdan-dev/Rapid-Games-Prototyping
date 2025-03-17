#macro VALID_CHARACTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789'"


function Game_getLines(){
	var fileID = file_text_open_read("./data.txt");
	
	var lines = [];
	
	while (!file_text_eof(fileID)) {
		var line = file_text_readln(fileID);
		lines[array_length(lines)] = string_replace_all(line, "\n", "");
	}
	
	return lines;
}


function Game_filterLine(_line) {
	var filteredLine = "";
	var len = string_length(_line);
	for (var i = 1; i <= len; i++) {
		if (!stringHasCharacter(VALID_CHARACTERS, string_char_at(_line, i))) 
			continue;
		filteredLine += string_char_at(_line, i);
	}
	return filteredLine;
}

function Game_getLineWords(_line) {
	return string_split(string_lower(Game_filterLine(_line)), " ", true);
}

function Game_getPossibleWords(_lines, _wordMap) {
	var len = array_length(_lines);
	for (var i = 0; i < len; i++) {
		var words = Game_getLineWords(_lines[i]);
			
		var wordsLen = array_length(words);
		for (var j = 0; j < wordsLen; j++) {
			if (ds_map_exists(_wordMap, words[j])) continue;
			ds_map_add(_wordMap, words[j], 0);
		}
	}
}

function Game_getStatistics(_lines, _statisticsMap) {
	var len = array_length(_lines);
	for (var i = 0; i < len; i++) {
		var words = Game_getLineWords(_lines[i]);
		
		var wordsLen = array_length(words);
		for (var j = 0; j < wordsLen; j++) {
			if (!ds_map_exists(_statisticsMap, words[j]))
				ds_map_add(_statisticsMap, words[j], 1);
			else 
				ds_map_replace(_statisticsMap, words[j], ds_map_find_value(_statisticsMap, words[j]) + 1);
		}
	}
}

function Game_getMostCommonWords(_statisticsMap, _count = 5) {
	var common = [];
	
	var keys = ds_map_keys_to_array(_statisticsMap);
	
	var len = array_length(keys);
	for (var i = 0; i < len; i++) {
		var count = ds_map_find_value(_statisticsMap, keys[i]);
		
		if (array_length(common) < _count) {
			common[array_length(common)] = { m_word: keys[i], m_count: count };
			continue;
		}
		
		var index = _count - 1;
		for (; index >= 0 && common[index].m_count < count; index--) {}
		index++;

		if (index < _count) {
			for (var j = _count; j > index; j--) {
				common[j] = common[j - 1];
			}
			common[index] = { m_word: keys[i], m_count: count };
		}
	}
	
	return common;
}

function Game_addWordStatistic(_key, _word, _amount = 1) {
	_key = string_lower(_key);
	_word = string_lower(_word);
	
	if (!obj_server.m_isHosting) { 
		Network_sendPacket("wordUpdate", {
			m_key: _key,
			m_word: _word,
			m_amount: _amount,
		});
		return;	
	}
	
	if (!ds_map_exists(obj_game_manager.m_wordChoiceStats, _key)) {
		ds_map_add(obj_game_manager.m_wordChoiceStats, _key, ds_map_create());
		//show_debug_message("created choice stat {0}", _key);
	}
	
	var wordStatsMap = ds_map_find_value(obj_game_manager.m_wordChoiceStats, _key);
	
	var currentAmount = ds_map_find_value(wordStatsMap, _word);
	if (is_undefined(currentAmount)) {
		ds_map_add(wordStatsMap, _word, _amount);
	} else {
		ds_map_replace(wordStatsMap, _word, currentAmount + _amount);
	}
	
	Game_updateWordMapping(_key);
}

function Game_updateWordMapping(_key) {
	_key = string_lower(_key);
	
	var wordStatsMap = ds_map_find_value(obj_game_manager.m_wordChoiceStats, _key);
	if (is_undefined(wordStatsMap)) return;
	
	var wordStatKeys = ds_map_keys_to_array(wordStatsMap);
	var wordStatKeyCount = array_length(wordStatKeys);
			
	var bestWord = undefined;
	var bestAmount = 0;
	for (var j = 0; j < wordStatKeyCount; j++) {
		var count = ds_map_find_value(wordStatsMap, wordStatKeys[j])
		if (is_undefined(count)) continue;
			
		if (bestAmount >= count) continue;
			
		bestAmount = count;
		bestWord = wordStatKeys[j]
	}
	
	
	var changed = false;
	var removed = false;
	if (is_undefined(bestWord)) {
		if (ds_map_exists(obj_game_manager.m_wordMapping, _key)) {
			ds_map_delete(obj_game_manager.m_wordMapping, _key)
			removed = true;
		}
	} else {
		var value = ds_map_find_value(obj_game_manager.m_wordMapping, _key)
		if (is_undefined(value)) {
			ds_map_add(obj_game_manager.m_wordMapping, _key, bestWord);
			changed = true;
		} else if (value != bestWord) {
			ds_map_replace(obj_game_manager.m_wordMapping, _key, bestWord);
			changed = true;
		}
	}
	
	if (changed || removed) {
		Network_sendPacketDirect(-1, "wordUpdate", {
			m_key: _key,
			m_word: bestWord,
		}, true, false);	
	}
}

function Game_getWordMapping(_word) {
	var mapping = ds_map_find_value(obj_game_manager.m_wordMapping, string_lower(_word));
	if (is_undefined(mapping)) return undefined;
	return mapping;
}


function Game_evaluateLine(_line) {
	var words = Game_getLineWords(_line);
	var wordCount = array_length(words);
	
	var unsolvedCount = 0;
	
	for (var i = 0; i < wordCount; i++) {
		if (!is_undefined(Game_getWordMapping(words[i]))) continue;
		
		var val = ds_map_find_value(obj_game_manager.m_wordStats, words[i]);
		if (is_undefined(val) || val <= 0) continue;
		unsolvedCount += 1 / val;
	}
	unsolvedCount /= wordCount;
	
	return unsolvedCount;
}

function Game_getNextPuzzleIterative(_state, _count) {
	var len = array_length(obj_game_manager.m_lines);
	
	if (!structHasFields(_state, [ "m_startIndex", "m_bestLine", "m_bestScore", "m_bestIndex" ])) {
		_state.m_startIndex = 0;	
		_state.m_bestLine = undefined;
		_state.m_bestScore = infinity;
		_state.m_bestIndex = undefined;
	}
	
	var i = _state.m_startIndex;
	var endIndex = i + _count;
	for (; i < len && i < endIndex; i++) {
		var lineScore = Game_evaluateLine(obj_game_manager.m_lines[i]);
		
		if (_state.m_bestScore <= lineScore) continue;
		
		_state.m_bestScore = lineScore;
		_state.m_bestLine = obj_game_manager.m_lines[i];
		_state.m_bestIndex = i;
	}
	
	if (i < len) {
		_state.m_startIndex = endIndex;	
		return undefined;
	}
	
	if (!is_undefined(_state.m_bestIndex))
		array_delete(obj_game_manager.m_lines, _state.m_bestIndex, 1);
	
	var bestLine = _state.m_bestLine;
	
	_state.m_startIndex = 0;	
	_state.m_bestLine = undefined;
	_state.m_bestScore = infinity;
	_state.m_bestIndex = undefined;
	
	return bestLine;
}

function Game_getPossibleWordOptions(_startText, _count, _length) {
	_startText = string_lower(_startText);
	
	var minLength = string_length(_startText);
	var len = array_length(obj_game_manager.m_wordChoices);
	
	var matched = [];
	for (var i = 0; i < len; i++) {
		if (string_length(obj_game_manager.m_wordChoices[i]) < minLength) continue;
		if (string_length(obj_game_manager.m_wordChoices[i]) != _length) continue;
			
		var failed = false;
		for (var j = 1; j <= minLength; j++) {
			if (string_char_at(obj_game_manager.m_wordChoices[i], j) == string_char_at(_startText, j)) continue;
			failed = true;
			break;
		}
		
		if (failed) continue;
		
		matched[array_length(matched)] = obj_game_manager.m_wordChoices[i];
		if (--_count <= 0) break;
	}
	return matched;
}