if (obj_server.m_isHosting) {
	if (array_length(m_puzzleQueue) > 0) {
		var puzzle = Game_getNextPuzzleIterative(m_puzzleSearchState, 50);
		if (!is_undefined(puzzle)) {
			var target = m_puzzleQueue[0];
			array_delete(m_puzzleQueue, 0, 1);
			
			with (target) {
				m_puzzle = puzzle;
				m_fragmentIndex = random(sprite_get_number(spr_rosetta_fragments)) - 1;
				m_networkUpdateData = true;
			}
		}
	}
	return;
}


if (m_inPuzzle && obj_player.m_closestInteractable != noone && obj_player.m_closestInteractable.m_puzzle == "")
	m_inPuzzle = false;
	
GUI_setVisibility("Solve", m_inPuzzle); 

