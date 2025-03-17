draw_set_halign(fa_center);
draw_set_valign(fa_middle);
draw_set_font(fnt_maki);

m_inPuzzle = false;
m_puzzle = "";
m_puzzleQueue = [];
m_puzzleSearchState = {};

m_wordMapping = ds_map_create();
m_wordChoices = []; // TODO: Set via server.

if (obj_server.m_isHosting) {
	m_lines = Game_getLines();
	//
	m_wordStats = ds_map_create();
	Game_getStatistics(m_lines, m_wordStats);
	//
	m_wordChoiceStats = ds_map_create();
	var common = Game_getMostCommonWords(m_wordStats, 20);
	var commonLen = array_length(common);
	//
	for (var i = 0; i < commonLen; i++) {
		Game_addWordStatistic(common[i].m_word, common[i].m_word, 100);
	}
	Game_addWordStatistic("KangWoo", "KangWoo", 100);
	//
	m_wordChoices = ds_map_keys_to_array(m_wordStats);
	array_sort(m_wordChoices, true);
}

