event_inherited();


var cx = x - m_lastPosX;
var cy = y - m_lastPosY;

if (cx != 0 || cy != 0) {
	if (cx != 0) m_flipped = cx > 0;
	sprite_index = spr_player_walking;	
} else {
	sprite_index = spr_player_idle;	
}

m_lastPosX = x;
m_lastPosY = y;