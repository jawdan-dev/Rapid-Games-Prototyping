// Script assets have changed for v2.3.0 see
// https://help.yoyogames.com/hc/en-us/articles/360005277377 for more information
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