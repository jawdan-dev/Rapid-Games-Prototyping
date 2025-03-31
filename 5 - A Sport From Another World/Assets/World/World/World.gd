extends StaticBody3D
class_name World;

static var s_worldInstance : World;

@export var m_worldSize : Vector3i = Vector3i(10, 4, 10);
@export var m_chunkSize : Vector3i = Vector3i(10, 10, 10);

var m_chunks : Dictionary;
@onready var m_chunkPrefab = preload("res://Assets/World/Chunk/Chunk.tscn")

var m_defaultFill : float = 1.0;
var m_defaultTerrain : Chunk.TerrainType = Chunk.TerrainType.Sky;

var m_updateQueue : Array[Chunk] = [];

func _ready() -> void:
	s_worldInstance = self;
	
	var border : Vector3 = Vector3.ONE * 0.75;
	
	$"-Y".position = Vector3.ZERO;
	$"-X".position = $"-Y".position + border;
	$"-Z".position = $"-X".position;
	$"+Y".position = Vector3(m_worldSize * m_chunkSize) - border;
	$"+X".position = $"+Y".position
	$"+Z".position = $"+Y".position
	
	var v : Vector3i;
	for x in range(m_worldSize.x):
		v.x = x * m_chunkSize.x;
		for y in range(m_worldSize.y):
			v.y = y * m_chunkSize.y;
			for z in range(m_worldSize.z):	
				v.z = z * m_chunkSize.z;
				
				var chunk : Chunk = m_chunkPrefab.instantiate() as Chunk;
				if (chunk == null): continue;
				
				chunk.m_size = m_chunkSize;
				chunk.m_world = self;
				chunk.m_absolutePos = v;
				chunk.position = v;
				
				m_chunks.set(v, chunk);
				
				add_child(chunk);
	
	changeSphere(Vector3(m_worldSize * m_chunkSize) * 0.5, 20.0, Chunk.TerrainType.None);
	
	for x in range(m_worldSize.x):
		v.x = x * m_chunkSize.x;
		for y in range(m_worldSize.y):
			v.y = y * m_chunkSize.y;
			for z in range(m_worldSize.z):	
				v.z = z * m_chunkSize.z;
				
				var chunk : Chunk = m_chunks.get(v) as Chunk;
				if (chunk == null): continue;
				
				chunk.updateMesh();
				
				
var m_triggered = false;
var m_t : float = 0;
func _process(delta: float) -> void:
	m_t += delta * 0.5;
	
	if (Input.is_action_just_pressed("ui_accept")):
		m_triggered = !m_triggered;
		
	var target : Vector3 = Vector3(sin(m_t), 0, cos(m_t)) * 15;
	if (m_triggered):
		changeSphere((Vector3(m_worldSize * m_chunkSize) * 0.5) + target, 5.0, Chunk.TerrainType.Grass);
	else:
		changeSphere((Vector3(m_worldSize * m_chunkSize) * 0.5) + target, 8.0, Chunk.TerrainType.None);
	
	var startTime : float = Time.get_unix_time_from_system();
	const maxTime : float = 0.005; # 6ms
	while (!m_updateQueue.is_empty() && (Time.get_unix_time_from_system() - startTime) < maxTime):
		var chunk : Chunk = m_updateQueue.pop_front() as Chunk;
		chunk.updateMesh();

func changeSphere(center : Vector3, radius : float, type : Chunk.TerrainType, amount : float = 0, damageRate : float = 1.0) -> bool:
	var rad : Vector3i = Vector3i.ONE * int(ceil(radius + 1));
	var minPos : Vector3i = Vector3i(center) - rad;
	var maxPos : Vector3i = Vector3i(center) + rad;
	minPos -= minPos % m_chunkSize;
	maxPos -= maxPos % m_chunkSize;
	
	var updated : bool = false;
	
	var v : Vector3i;
	for x : int in range(minPos.x, maxPos.x + 1, m_chunkSize.x):
		v.x = x;
		for y : int in range(minPos.y, maxPos.y + 1, m_chunkSize.y):
			v.y = y;
			for z : int in range(minPos.z, maxPos.z + 1, m_chunkSize.z):	
				v.z = z;
				
				var chunk : Chunk = m_chunks.get(v) as Chunk;
				if (chunk == null): continue;
				
				if (chunk.changeSphere(center, radius, type, amount)):
					updated = true;
	
	var players : Array[Player] = PlayerManager.getAlivePlayers();
	for player in players:
		var change : Vector3 = player.position - center;
		var distance : float = change.length() - player.getPlayerRadius();
		if (distance > radius): continue;
		
		if (amount > 0 || (type != Chunk.TerrainType.None && amount >= 0)):
			player.digout();
		
		if (type != Chunk.TerrainType.None && player.m_playerIndex != (type - Chunk.TerrainType.Player0)):
			player.takeDamage((radius - distance) * damageRate);
	
	return updated;

func getWeightAbsolute(pos : Vector3i) -> float:
	var relativePos : Vector3i = Vector3i(pos.x % m_chunkSize.x, pos.y % m_chunkSize.y, pos.z % m_chunkSize.z)
	var chunkPos : Vector3i = Vector3i(pos.x - relativePos.x, pos.y - relativePos.y, pos.z - relativePos.z)
			
	var chunk : Chunk = m_chunks.get(chunkPos) as Chunk;
	if (chunk == null): return m_defaultFill;
	
	return chunk.getWeight(relativePos);
	
func getTypeAbsolute(pos : Vector3i) -> Chunk.TerrainType:
	var relativePos : Vector3i = Vector3i(pos.x % m_chunkSize.x, pos.y % m_chunkSize.y, pos.z % m_chunkSize.z)
	var chunkPos : Vector3i = Vector3i(pos.x - relativePos.x, pos.y - relativePos.y, pos.z - relativePos.z)
			
	var chunk : Chunk = m_chunks.get(chunkPos) as Chunk;
	if (chunk == null): return m_defaultTerrain;
	
	return chunk.getType(relativePos);
	
func updateChunks(pos : Vector3i, updateSurroundings : bool = false):
	var minPos : Vector3i = Vector3i(pos); 
	var maxPos : Vector3i = Vector3i(pos);
	if (updateSurroundings): 
		minPos -= m_chunkSize;
		#maxPos += m_chunkSize;
	
	var v : Vector3i;
	for x : int in range(minPos.x, maxPos.x + 1, m_chunkSize.x):
		v.x = x;
		for y : int in range(minPos.y, maxPos.y + 1, m_chunkSize.y):
			v.y = y;
			for z : int in range(minPos.z, maxPos.z + 1, m_chunkSize.z):	
				v.z = z;
	
				var chunk : Chunk = m_chunks.get(v) as Chunk;
				if (chunk == null || chunk.m_updated): continue;
				 
				chunk.m_updated = true;
				m_updateQueue.append(chunk);

func getSafePoint() -> Vector3: 
	var pos : Vector2 = Vector2.INF;
	var safePoint : Vector3 = Vector3.INF;
	var border : float = 5;
	
	while (!isSafePoint(safePoint)):
		pos = Vector2(
			randi_range(border, (m_worldSize.x * m_chunkSize.x) - border),	
			randi_range(border, (m_worldSize.z * m_chunkSize.z) - border),	
		);
		safePoint = getSafePointCalc(pos);
	return safePoint + (Vector3.UP * 2);


func isSafePoint(pos : Vector3) -> bool:
	if (pos == Vector3.INF): return false;
	return (pos.y + 5 < m_worldSize.y * m_chunkSize.y)
	
func getSafePointCalc(pos : Vector2) -> Vector3:
	var worldSpace : PhysicsDirectSpaceState3D = get_world_3d().direct_space_state;
	var topPos : Vector3 = Vector3(pos.x, m_worldSize.x * m_chunkSize.x, pos.y);
	var bottomPos : Vector3 = Vector3(topPos.x, 0, topPos.z);
	var query : PhysicsRayQueryParameters3D = PhysicsRayQueryParameters3D.create(topPos, bottomPos, collision_mask);
	var hit : Dictionary = worldSpace.intersect_ray(query);
	if (hit.is_empty()): return Vector3.INF;
	return hit.position;
	
	
