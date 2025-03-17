function Vector2(_x, _y) constructor {
	x = _x;
    y = _y;
	
	function equal(_other) {
		return x == _other.x && y == _other.y;
    }
    function add(_other) {
		return new Vector2(x + _other.x, y + _other.y);
    }
    function subtract(_other) {
		return new Vector2(x - _other.x, y - _other.y);
    }
    function multiply(_other) {
		return new Vector2(x * _other.x, y * _other.y);
    }
    function divide(_other) {
		return new Vector2(x / _other.x, y / _other.y);
    }
    function dot(_other) {
		return (x * _other.x) + (y * _other.y);
    }
    function length() {
		return sqrt(self.dot(self));
    }
    function normal() {
		var length = self.length();
		return self.divide(new Vector2(length, length));
    }
    function angle() {
		var normal = self.normal();
		return arctan2(normal.x, normal.y);
    }
    function interpolate(_other, _t) {
		return new Vector2(
			lerp(x, _other.x, _t),
			lerp(y, _other.y, _t)
		);
    }
	function manhattan() {
		return abs(x) + abs(y);	
	}
}