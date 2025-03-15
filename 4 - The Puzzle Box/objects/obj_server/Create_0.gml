#macro c_serverAddress "127.0.0.1"
#macro c_serverPort 3000
#macro c_serverType network_socket_ws
#macro c_serverMaxClients 32


show_debug_log(true);

m_sceneStarted = false;
m_serverSocket = 0;
m_clientSockets = [];
m_networkSyncables = ds_map_create();
m_playerUUIDs = ds_map_create();
m_spawningSyncable = false;
m_time = 0


// Initialize.
if (m_isHosting) {
	show_debug_message("	> Starting server.");
	m_serverSocket = network_create_server_raw(c_serverType, c_serverPort, c_serverMaxClients);				

	if (m_serverSocket < 0) {
		show_debug_message("	> Failed to launch server. Socket already in use.");
		//throw ("	> Failed to launch server. Socket already in use.");
		
		m_isHosting = false;
		show_debug_message("	> Connecting to server.");
		m_serverSocket = network_create_socket(c_serverType);
		network_connect_raw_async(m_serverSocket, c_serverAddress, c_serverPort);
	}
		
	room_goto(rm_game);
} else {
	show_debug_message("	> Connecting to server.");
	m_serverSocket = network_create_socket(c_serverType);
	network_connect_raw_async(m_serverSocket, c_serverAddress, c_serverPort);
}

show_debug_message("	> Server socket: " + string(m_serverSocket));
window_set_caption(m_isHosting ? "Server" : "Client");