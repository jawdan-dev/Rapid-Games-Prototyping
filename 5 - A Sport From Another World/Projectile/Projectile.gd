extends RigidBody3D

func onWorldHit(body: Node) -> void:
	World.s_worldInstance.changeSphere(global_position, randf_range(1, 3), Chunk.TerrainType.Stone);
	queue_free();
