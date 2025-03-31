extends Node3D
class_name ProjectileManager;

static var s_instance : ProjectileManager;


func _ready() -> void:
	s_instance = self;
