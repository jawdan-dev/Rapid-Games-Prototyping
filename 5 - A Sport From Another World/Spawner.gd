extends Node3D

const m_projectile = preload("res://Projectile/Projectile.tscn");

var m_cooldown : float = 0;


func _process(delta: float) -> void:
	m_cooldown -= delta;
	if (m_cooldown <= 0.0):
		m_cooldown = 0.1;
		
		var projectile : Node3D = m_projectile.instantiate();
		projectile.position = global_position;
		
		var angle : float = randf_range(0, TAU);
		projectile.linear_velocity = Vector3(sin(angle), randf_range(0, 3), cos(angle)).normalized() * 20;
		projectile.rotate_y(angle);
		
		add_child(projectile);
