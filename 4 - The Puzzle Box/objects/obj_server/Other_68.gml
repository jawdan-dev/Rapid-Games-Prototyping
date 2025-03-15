networkEventType = ds_map_find_value(async_load, "type");

switch (networkEventType) {
	case network_type_non_blocking_connect:
	case network_type_connect: {
		if (!m_isHosting) { 
			room_goto(rm_game);
			return;
		}
		
		var socket = ds_map_find_value(async_load, "socket");
		m_clientSockets[array_length(m_clientSockets)] = socket;
		
		Network_handlePacket(socket, "_onConnect", {}, 0);
	} break;
	
	case network_type_disconnect: {
		if (!m_isHosting) {
			room_goto(rm_server);
		}
		
		var socket = ds_map_find_value(async_load, "socket");
		
		var len = array_length(m_clientSockets);
		for (var i = 0; i < len; i++) {
			if (m_clientSockets[i] != socket) continue;	
			
			array_delete(m_clientSockets, i, 1);
			break;
		}
		
		Network_handlePacket(socket, "_onDisconnect", {}, 0);
	} break;
	
	case network_type_data: {
		var buffer = ds_map_find_value(async_load, "buffer");
		var socket = ds_map_find_value(async_load, "id");
		
		buffer_seek(buffer, buffer_seek_start, 0);
		var bufferString = buffer_read(buffer, buffer_text);
		
		var packetData = {}
		try {
			var packetData = json_parse(bufferString);
		} catch(_err) { 
			show_debug_message("Failed to parse JSON packet: {0}", bufferString);
			break;
		}
		
		if (!variable_struct_exists(packetData, "m_category")) break;
		var category = variable_struct_get(packetData, "m_category");
		if (!is_string(category)) break;
		
		if (!variable_struct_exists(packetData, "m_data")) break;
		var data = variable_struct_get(packetData, "m_data");
		if (!is_struct(data)) break;
		
		if (!variable_struct_exists(packetData, "m_time")) break;
		var time = variable_struct_get(packetData, "m_time");
		if (!is_numeric(time)) break;
		
		Network_handlePacket(socket, category, data, time);
	} break;
}