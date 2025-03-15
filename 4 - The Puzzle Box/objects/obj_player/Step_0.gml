if (!m_enabled) return;

var dx = keyboard_check(ord("D")) - keyboard_check(ord("A"));
var dy = keyboard_check(ord("S")) - keyboard_check(ord("W"));

if (dx != 0 || dy != 0) {
	var delta = getDeltaTime();
	x += dx * m_movementSpeed * delta;
	y += dy * m_movementSpeed * delta;
	
	m_networkUpdateData = true;
}