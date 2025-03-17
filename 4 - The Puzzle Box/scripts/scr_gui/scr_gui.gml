function GUI_isHovered(_id) {
	return m_hoveredElement == _id;	
}
function GUI_isActive(_id) {
	return m_activeElement == _id;
}
function GUI_canInteract(_id) {
	return !_id.m_disabled && _id.m_canInteractFunc(); // && !Animator_isActive() 
}


function GUI_registerElement(_layerName, _id) {
	if (!variable_global_exists("m_guiDict")) 
		global.m_guiDict = ds_map_create(); 

	var arr = ds_map_find_value(global.m_guiDict, _layerName);
	if (arr == undefined) {
		arr = [ _id ];	
		ds_map_add(global.m_guiDict, _layerName, arr);	
	} else {
		arr[array_length(arr)] = _id;
	}
	
	_id.depth = -9999;
	_id.visible = false;
	_id.m_guiLayer = _layerName;
}
function GUI_unregisterElement(_id) {
	if (!variable_global_exists("m_guiDict") || !variable_instance_exists(_id, "m_guiLayer")) return;
	
	var arr = ds_map_find_value(global.m_guiDict, _id.m_guiLayer);
	if (arr == undefined) return;
	
	var len = array_length(arr);
	for (var i = 0; i < len; i++) {
		if (arr[i] == _id) {
			array_delete(arr, i, 1);
			break;
		}
	}
	if (array_length(arr) == 0)
		ds_map_delete(global.m_guiDict, _id.m_guiLayer)

}

function GUI_setVisibility(_layerName, _visible) {
	if (!variable_global_exists("m_guiDict")) return;
	var elements = ds_map_find_value(global.m_guiDict, _layerName);
	if (elements == undefined) return;
	
	var len = array_length(elements);
	for (var i = 0; i < len; i++) {
		elements[i].visible = _visible;
	}
}