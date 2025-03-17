m_sceneStarted = true;
show_debug_message("room start");

if (room == rm_game)
	Network_sendPacket("where", {});
	
		
if (m_isHosting) {
	camera_destroy(view_camera[0]);
	view_camera[0] = camera_create_view(0, 0, room_width, room_height);
}
