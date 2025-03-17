function Network_getUUID() {
	static m_uuidCounter = 0;
	return ++m_uuidCounter;
}

function Network_sendPacket(_category, _data) {
	if (obj_server.m_isHosting) {
		Network_sendPacketDirect(-1, _category, _data, true, false);
		return;
	}
	Network_sendPacketDirect(obj_server.m_serverSocket, _category, _data, false, false);
}

function Network_sendPacketDirect(targetSocket, category, data, broadcast, ignoreTarget) {
	if (targetSocket < 0 && !broadcast) return;
	
	var packet = {
		m_category: category, 
		m_data: data,
		m_time: obj_server.m_time,
	};
	
	var json = json_stringify(packet);
	var buffer = buffer_create(string_length(json), buffer_fixed, 1);
	buffer_write(buffer, buffer_text, json);
	var bufferSize = buffer_tell(buffer);
	
	if (broadcast) {
		with (obj_server) {
			var len = array_length(m_clientSockets);
			for (var i = 0; i < len; i++) {
				if (ignoreTarget && m_clientSockets[i] == targetSocket) continue;
				
				//network_send_packet(m_clientSockets[i], buffer, bufferSize);
				network_send_packet(m_clientSockets[i], buffer, bufferSize);

				show_debug_message("Sending {2} data to socket {0} of size {1}", m_clientSockets[i], bufferSize, category);
			}
		}
	} else {
		show_debug_message("Sending {2} data to socket {0} of size {1}", targetSocket, bufferSize, category);
		network_send_packet(targetSocket, buffer, bufferSize);
	}	
	
	buffer_delete(buffer);
}

function Network_handlePacket(_socket, _category, _data, _time) {
	m_lastPacket = _data;
	
	show_debug_message("recieved packet type {0} @ ({2}) with data {1}", _category, _data, floor(_time * 100) / 100);
	
	switch (_category) {
		case "_onConnect": {
			if (!obj_server.m_isHosting) break;
			
			// Player connected to us.
			
			var wordMapping = [];
			var keys = ds_map_keys_to_array(obj_game_manager.m_wordMapping);
			var keyCount = array_length(keys);
			for (var i = 0; i < keyCount; i++) {
				wordMapping[i] = {
					m_key: keys[i],
					m_word: ds_map_find_value(obj_game_manager.m_wordMapping, keys[i]),
				};
			}
			
			
			Network_sendPacketDirect(_socket, "wordMapping", {
				m_wordChoices: obj_game_manager.m_wordChoices,
				m_wordMapping: wordMapping,			
			}, false, false);
		} break;
		case "_onDisconnect": {
			if (!obj_server.m_isHosting) break;
			
			// Player disconnected from us.
			
			if (!ds_map_exists(obj_server.m_playerUUIDs, _socket)) break;
			var uuid = ds_map_find_value(obj_server.m_playerUUIDs, _socket);
			Network_destroyIntance(uuid);
		} break;
		
		case "wordMapping": {
			if (obj_server.m_isHosting) break;
			if (!structHasFields(_data, [ "m_wordChoices", "m_wordMapping" ])) break;
			
			obj_game_manager.m_wordChoices = _data.m_wordChoices;
			
			ds_map_clear(obj_game_manager.m_wordMapping);
			var len = array_length(_data.m_wordMapping);
			for (var i = 0; i < len; i++) {
				ds_map_add(obj_game_manager.m_wordMapping, _data.m_wordMapping[i].m_key, _data.m_wordMapping[i].m_word);
			}
		} break;
		
		case "wordUpdate": {
			if (obj_server.m_isHosting) {
				if (!structHasFields(_data, [ "m_key", "m_word", "m_amount" ])) break;
				
				Game_addWordStatistic(_data.m_key, _data.m_word, _data.m_amount);
			} else {
				if (!structHasFields(_data, [ "m_key" ])) break;
				if (!structHasFields(_data, [ "m_word" ])) {
					// delete.
					if (ds_map_exists(obj_game_manager.m_wordMapping, _data.m_key)) {
						ds_map_delete(obj_game_manager.m_wordMapping, _data.m_key)
					}
					break;
				}
				
				var value = ds_map_find_value(obj_game_manager.m_wordMapping, _data.m_key)
				if (is_undefined(value)) {
					ds_map_add(obj_game_manager.m_wordMapping, _data.m_key, _data.m_word);
				} else if (value !=  _data.m_word) {
					ds_map_replace(obj_game_manager.m_wordMapping, _data.m_key, _data.m_word);
				}
			}
		} break;
		
		case "where": {
			// Send all NETWORK OBJECTS to client.
			with (obj_network_syncable) {
				var data = {
					m_assetName: object_get_name(object_index),
					m_data: Network_getData(id),
				};
				
				// This is happening too soon.
				// A 'request other players' thing would be better.
				Network_sendPacketDirect(_socket, "create", data, false, false);
			}
		} break;
		
		
		case "create": {
			if (!structHasFields(_data, [ "m_assetName", "m_data" ])) break;
			
			var assetID = asset_get_index(_data.m_assetName);
			
			var isPlayer = assetID == obj_player;
			if (isPlayer)
				assetID = obj_network_player;
			
			if (!object_is_ancestor(assetID, obj_network_syncable)) break;
			
			if (obj_server.m_isHosting) {
				if (!structHasFields(_data, [ "m_localID" ])) break;

				// todo: setup perms
				var sx = 0, sy = 0, sd = 0;
				if (structHasFields(_data.m_data, [ "x" ])) sx = _data.m_data.x;
				if (structHasFields(_data.m_data, [ "y" ])) sy = _data.m_data.y;
				if (structHasFields(_data.m_data, [ "depth" ])) sd = _data.m_data.depth;
				var instance = instance_create_depth(sx, sy, sd, assetID);
				
				Network_setData(instance, _data.m_data);
				
				var replyData = {
					m_assetName: _data.m_assetName,
					m_data: Network_getData(instance)
				};
				
				if (isPlayer) {
					// Associate play UUID to socket.
					ds_map_add(obj_server.m_playerUUIDs, _socket, replyData.m_data.m_networkID)
				}
				
				// Let everyone else know????
				Network_sendPacketDirect(_socket, "create", replyData, true, true);
				
				// Update client.
				variable_struct_set(replyData, "m_localID", _data.m_localID);
				Network_sendPacketDirect(_socket, "create", replyData, false, false);
				
				break;
			} 
			
			
			if (!structHasFields(_data.m_data, [ "m_networkID" ])) break;
				
			var instance = noone;
			if (variable_struct_exists(_data, "m_localID")) {
				instance = _data.m_localID;
			} else {
				if (Network_getInstance(_data.m_data.m_networkID) != noone) break; 
				
				// Make the instance.
				var sx = 0, sy = 0, sd = 0;
				if (structHasFields(_data.m_data, [ "x" ])) sx = _data.m_data.x;
				if (structHasFields(_data.m_data, [ "y" ])) sy = _data.m_data.y;
				if (structHasFields(_data.m_data, [ "depth" ])) sd = _data.m_data.depth;
					
				obj_server.m_spawningSyncable = true;
				instance = instance_create_depth(sx, sy, sd, assetID);
				obj_server.m_spawningSyncable = false;
			}
			Network_setData(instance, _data.m_data);
				
			instance.m_enabled = true;
			ds_map_add(obj_server.m_networkSyncables, _data.m_data.m_networkID, instance);
		} break;
		
		case "destroy": {
			if (!structHasFields(_data, [ "m_networkID" ])) break;
						
			Network_destroyIntance(_data.m_networkID);
		} break;
		
		
		case "update": {
			if (!structHasFields(_data, [ "m_networkID" ])) break;
			
			var instance = Network_getInstance(_data.m_networkID);
			if (instance == noone) break;
			
			// TODO: Check perms.
				
			// Coolio.
			Network_setData(instance, _data);
			
			// Let everyone else know.
			if (obj_server.m_isHosting)
				Network_sendPacketDirect(_socket, "update", _data, true, true);
		} break;
	}
}

function Network_assignInstance(_instanceID, _x, _y) {
	if (obj_server.m_isHosting) return;
	
	var data = {
		m_localID: _instanceID,
		m_assetName: object_get_name(_instanceID.object_index),
		m_data: Network_getData(_instanceID),
	}
	Network_sendPacket("create", data);
}

function Network_getInstance(_networkID) {
	if (!ds_map_exists(obj_server.m_networkSyncables, _networkID)) 
		return noone;
	return ds_map_find_value(obj_server.m_networkSyncables, _networkID);
}
function Network_destroyIntance(_networkID) {
	var instance = Network_getInstance(_networkID);
	if (instance == noone) return;
	
	ds_map_delete(obj_server.m_networkSyncables, _networkID);
	instance_destroy(instance);
	
	if (!obj_server.m_isHosting) return;
	
	Network_sendPacketDirect(-1, "destory", { m_networkID: _networkID }, true, false);
}

function Network_getData(_id) {
	var data = {};

	with (_id) {
		if (!object_is_ancestor(object_index, obj_network_syncable)) return;
		
		variable_struct_set(data, "m_networkID", m_networkID);
		
		var len = array_length(m_packetFilter);
		for (var i = 0; i < len; i++) {
			if (!variable_instance_exists(id, m_packetFilter[i])) 
				continue;
				
			variable_struct_set(
				data, 
				m_packetFilter[i], 
				variable_instance_get(id, m_packetFilter[i])
			);
		}
	}
	
	return data;
}

function Network_setData(_id, _data) {
	with (_id) {
		if (!object_is_ancestor(object_index, obj_network_syncable)) return;
		
		var names = struct_get_names(_data);	
		var len = array_length(names);
		for (var i = 0; i < len; i++) {
			if (!variable_instance_exists(id, names[i])) continue;
			if (obj_server.m_isHosting && names[i] == "m_networkID") continue;
				
			variable_instance_set(id, names[i], variable_struct_get(_data, names[i]));
		}
		
		event_user(14);
	}
}
