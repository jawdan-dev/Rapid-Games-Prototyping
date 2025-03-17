if (!m_enabled) return;

if (obj_game_manager.m_inPuzzle) return;

var dx = keyboard_check(ord("D")) - keyboard_check(ord("A"));
var dy = keyboard_check(ord("S")) - keyboard_check(ord("W"));

if (dx != 0 || dy != 0) {
	{ // Normalize input.
		var len = sqrt((dx * dx) + (dy * dy));
		dx /= len; dy /= len;
	}
	
	var delta = getDeltaTime();
	
	var tx = x + (dx * m_movementSpeed * delta);
	var ty = y + (dy * m_movementSpeed * delta);
	
	var finalPos = Movement_moveTo(x, y, tx, ty, 5, 2);
	var moved = finalPos.x != x || finalPos.y != y;
	var xChange = floor(finalPos.x) - floor(x);
	
	x = finalPos.x;
	y = finalPos.y;
	m_drawX = floor(x);
	m_drawY = floor(y);
	
	m_networkUpdateData = true;
	
	if (moved) {
		if (xChange != 0) m_flipped = xChange > 0;
		sprite_index = spr_player_walking;	
	} else {
		sprite_index = spr_player_idle;	
	}
} else {
	sprite_index = spr_player_idle;	
}

var interactableList = ds_list_create();
var interactableCount = collision_circle_list(x, y, m_interactableRange, obj_interactable, false, true, interactableList, false);

if (interactableCount > 0) {
	m_closestInteractable = noone;	
	
	var bestDist = infinity;
	for (var i = 0; i < interactableCount; i++) {
		var obj = ds_list_find_value(interactableList, i);
		
		with (obj) {
			var ox = x + (sprite_width * 0.5), oy = y + (sprite_height * 0.5);
			var cx = ox - other.x, cy = oy - other.y;
			var dist = (cx * cx) + (cy * cy);
			
			if (bestDist <= dist) break;
			if (!m_interactableEnabled) break;
			
			bestDist = dist;
			other.m_closestInteractable = id;
		}
	}
} else {
	m_closestInteractable = noone;	
}
ds_list_destroy(interactableList);

if (m_closestInteractable != noone && keyboard_check_released(ord("E"))) {
	obj_game_manager.m_inPuzzle = true;
	obj_game_manager.m_puzzle = m_closestInteractable.m_puzzle;
	obj_gui_solve_textInput.m_text = "";
	obj_gui_solve_textInput.m_possibleOptions = [];
	ds_map_clear(obj_gui_solve_text.m_customMap);

	var words = Game_getLineWords(obj_game_manager.m_puzzle);
	var wordCount = array_length(words);
	
	var firstNotFound = undefined;
	for (var i = 0; i < wordCount; i++) {
		if (!is_undefined(Game_getWordMapping(words[i]))) continue;
		firstNotFound = i;
		break;
	}
	
	if (!is_undefined(firstNotFound)) 
		obj_gui_solve_text.m_wordLookIndex = firstNotFound 
	else 
		obj_gui_solve_text.m_wordLookIndex = 0;
	
}
