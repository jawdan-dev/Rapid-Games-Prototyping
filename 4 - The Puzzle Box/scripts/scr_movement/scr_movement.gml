// Script assets have changed for v2.3.0 see
// https://help.yoyogames.com/hc/en-us/articles/360005277377 for more information
function Movement_moveTo(_x, _y, _tx, _ty, _width = 0, _height = 0){
	var finalPos = { x: _x, y: _y };
	
	var fx = floor(_x), fy = floor(_y);
	var ftx = floor(_tx), fty = floor(_ty);
	var dfx = sign(ftx - fx), dfy = sign(fty - fy);
	
	
	var w = _width * dfx, h = _height * dfy;
	fx += w; ftx += w;
	fy += h; fty += h;
	
	// Handle x movement.
	var layerID = layer_get_id("Room");
	var tilemapID = layer_tilemap_get_id(layerID);
	
	var hitWall = false
	for (var i = fx; i != ftx; i += dfx) {
		var tileIDs = [
			tilemap_get_at_pixel(tilemapID, i + dfx, floor(finalPos.y)),
			tilemap_get_at_pixel(tilemapID, i + dfx, floor(finalPos.y) - _height),
			tilemap_get_at_pixel(tilemapID, i + dfx, floor(finalPos.y) + _height),
		];
		
		var hitSolid = false;
		var len = array_length(tileIDs);
		for (var j = 0; j < len; j++) {
			if (!Movement_isSolidRoomTile(tileIDs[j])) continue;
			hitSolid = true;
			break;
		}
		if (!hitSolid) continue;
		
		hitWall = true;
		finalPos.x = i - w;
		break;
	}
	if (!hitWall) finalPos.x = _tx;
	
	hitWall = false;
	for (var i = fy; i != fty; i += dfy) {
		var tileIDs = [
			tilemap_get_at_pixel(tilemapID, floor(finalPos.x), i + dfy),
			tilemap_get_at_pixel(tilemapID, floor(finalPos.x) - _width, i + dfy),
			tilemap_get_at_pixel(tilemapID, floor(finalPos.x) + _width, i + dfy),
		];
		
		var hitSolid = false;
		var len = array_length(tileIDs);
		for (var j = 0; j < len; j++) {
			if (!Movement_isSolidRoomTile(tileIDs[j])) continue;
			hitSolid = true;
			break;
		}
		if (!hitSolid) continue;
		
		hitWall = true;
		finalPos.y = i - h;
		break;
	}
	if (!hitWall) finalPos.y = _ty;
	
	return finalPos;
}

function Movement_isSolidRoomTile(_tileID) {
	switch (_tileID) {
		case 2:
		case 3:
		case 4:
		case 5: //
		case 8:
		case 9:
		case 10:
		case 11:
		case 14:
		case 15:
		case 16:
		case 17:
		case 20:
		case 21:
		case 22:
		case 23: 
		return true;
	}
	return false;
}