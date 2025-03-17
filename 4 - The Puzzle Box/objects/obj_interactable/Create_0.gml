event_inherited();

{ // Setup collision.
	var tileSize = 16;
	var sx = floor(x / tileSize), sy = floor(y / tileSize);
	var ex = floor((x + sprite_width - 1) / tileSize), ey = floor((y + sprite_height - 1) / tileSize);

	var layerID = layer_get_id("Room");
	var tilemapID = layer_tilemap_get_id(layerID);
	for (var ix = sx; ix <= ex; ix++) {
		for (var iy = sy; iy <= ey; iy++) {
			tilemap_set(tilemapID, 5, ix, iy);
		}
	}
}

depth = -y;

m_isHovered = false;
m_interactableEnabled = true;