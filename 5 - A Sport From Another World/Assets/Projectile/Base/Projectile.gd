extends RigidBody3D
class_name Projectile;

@export var m_lifeTime = 20.0;

@export var m_type : Chunk.TerrainType = Chunk.TerrainType.Stone :
	set(value):
		m_type = value;
		$Mesh.set_instance_shader_parameter("m_type", float(int(m_type)));
	
@export var m_size : float = 1.0 :
	set(value):
		m_size = value;
		$Collider.scale = Vector3.ONE * m_size;
		$Mesh.scale = Vector3.ONE * m_size;
		
var m_amount : float = 0;
		
func _ready() -> void:
	$Mesh.set_instance_shader_parameter("m_type", float(int(m_type)));

func _process(delta: float) -> void:
	m_lifeTime -= delta;
	if (m_lifeTime > 0): return;
	queue_free();

func onWorldHit(body: Node) -> void:
	var player : Player = body as Player;
	if (player != null): 
		if (player.m_playerIndex == m_type - Chunk.TerrainType.Player0):
			return
	
	World.s_worldInstance.changeSphere(global_position, m_size, m_type, m_amount);
	queue_free();
	
const c_projectile = preload("res://Assets/Projectile/Base/Projectile.tscn");
static func Spawn(from : Vector3, velocity : Vector3, type : Chunk.TerrainType, size : float, amount : float = 0) -> Projectile:
	var projectile : Node3D = c_projectile.instantiate()
	projectile.position = from;
	projectile.linear_velocity = velocity;
	projectile.m_size = size;
	projectile.m_type = type;
	projectile.m_amount = amount;
	return projectile;
	
