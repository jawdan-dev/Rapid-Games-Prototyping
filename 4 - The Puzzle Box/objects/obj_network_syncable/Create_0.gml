m_networkID = -1;
m_enabled = false;
//
m_networkUpdateData = false;
m_networkCooldownMax = 0.05;
m_networkCooldown = m_networkCooldownMax;
m_lastNetworkRecieveTime = 0;

// Config
m_packetFilter = [];
m_serverSide = true;
//
event_user(15)
//

show_debug_message("scene start check");
if (!obj_server.m_sceneStarted && obj_server.m_isHosting != m_serverSide) {
	instance_destroy();
	show_debug_message("destroyed instance....");
	return;
}


if (!obj_server.m_isHosting) {	
	// Ask for an id..
	if (!obj_server.m_spawningSyncable)
		Network_assignInstance(id, x, y);
} else {
	m_networkID = Network_getUUID();
	m_enabled = true;	
	ds_map_add(obj_server.m_networkSyncables, m_networkID, id);
}