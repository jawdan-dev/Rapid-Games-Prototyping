function isDebugView() {
	return keyboard_check(vk_f4);
}

function getDeltaTime(){
	return delta_time * 0.0000001;
}

function structHasFields(_struct, _fields) {
	var len = array_length(_fields);
	for (var i = 0; i < len; i++) {
		if (variable_struct_exists(_struct, _fields[i])) continue;
		return false;	
	}
	return true;
}
function stringHasCharacter(_str, _c) {
	var len = string_length(_str);
	for (var i = 0; i < len; i++)
		if (string_char_at(_str, i) == _c)
			return true;
	return false;
}


function moveTowards(_x, _y, _tx, _ty, _moveAmount) {
	var rx = _x, ry = _y;
	
	if (rx != _tx || ry != _ty) {
		var cx = _tx - _x, cy = _ty - _y;
		var len = sqrt((cx * cx) + (cy * cy))
		
		if (len > _moveAmount) {
			var nx = cx / len, ny = cy / len;
			
			rx += nx * _moveAmount;		
			ry += ny * _moveAmount;
		} else {
			rx = _tx;	
			ry = _ty;
		}		
	}
	
	return {
		x: rx,
		y: ry,
	};
}