extends Node3D

@export var m_cooldownMax : float = 0.1;
var m_cooldown : float = 0;


func _process(delta: float) -> void:
	if (m_cooldown > 0.0):
		m_cooldown -= delta;
		return;
	m_cooldown = m_cooldownMax;
		
	var angle : float = randf_range(0, TAU);
	Projectile.Spawn(
		global_position,
		Vector3(sin(angle), randf_range(0, 3), cos(angle)).normalized() * 20.0,
		[ Chunk.TerrainType.Grass, Chunk.TerrainType.Dirt, Chunk.TerrainType.Stone ].pick_random(),
		randf_range(1, 3)
	);
