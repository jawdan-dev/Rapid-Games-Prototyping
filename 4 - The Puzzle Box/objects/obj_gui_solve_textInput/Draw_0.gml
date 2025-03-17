var db = 40;
var ws = (320 - (db * 2)) / sprite_get_width(spr_rosetta_fragments);
draw_sprite_ext(spr_rosetta_fragments, obj_player.m_closestInteractable.m_fragmentIndex, x - m_cameraOffset.x + db, (y - m_cameraOffset.y) + 90, ws, ws, 0, c_white, 1.0);

event_inherited();

var border = 4;

var split = 3;

var drawCount = 15;
var columnCount = drawCount / split;

var possibleItemHeight = 12;
var possibleHeight = (possibleItemHeight * columnCount) + (border * 2);
draw_panel(sprite_index, x, y + sprite_height, sprite_width, possibleHeight, 4);


var len = array_length(m_possibleOptions);
var sWidth = (sprite_width - (border * 2)) / split;
var xOffset = border, iOffset = 0;

for (var i = 0; i < len; i++) {
	if (i > 0 && i mod columnCount == 0) { 
		xOffset += sWidth;
		iOffset -= columnCount;
	}
	draw_text_bound(x + xOffset, y + sprite_height + ((i + iOffset) * possibleItemHeight) + border, sWidth, possibleItemHeight, m_possibleOptions[i]);
}