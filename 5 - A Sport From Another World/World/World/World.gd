extends Node
class_name World;

static var s_worldInstance : World;

@export var m_worldSize : Vector3i = Vector3i(10, 4, 10);
@export var m_chunkSize : Vector3i = Vector3i(10, 10, 10);

var m_chunks : Dictionary;
@onready var m_chunkPrefab = preload("res://World/Chunk/Chunk.tscn")

var m_defaultFill : float = 1.0;
var m_defaultTerrain : Chunk.TerrainType = Chunk.TerrainType.Sky;

var m_updateQueue : Array[Chunk] = [];

func _ready() -> void:
	s_worldInstance = self;
	
	$"-X".position = Vector3.ZERO;
	$"-Y".position = $"-X".position;
	$"-Z".position = $"-X".position;
	$"+X".position = Vector3(m_worldSize * m_chunkSize);
	$"+Y".position = $"+X".position;
	$"+Z".position = $"+X".position;
	
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

func changeSphere(center : Vector3, radius : float, type : Chunk.TerrainType) -> void:
	var rad : Vector3i = Vector3i.ONE * int(ceil(radius + 1));
	var minPos : Vector3i = Vector3i(center) - rad;
	var maxPos : Vector3i = Vector3i(center) + rad;
	minPos -= minPos % m_chunkSize;
	maxPos -= maxPos % m_chunkSize;
	
	var v : Vector3i;
	for x : int in range(minPos.x, maxPos.x + 1, m_chunkSize.x):
		v.x = x;
		for y : int in range(minPos.y, maxPos.y + 1, m_chunkSize.y):
			v.y = y;
			for z : int in range(minPos.z, maxPos.z + 1, m_chunkSize.z):	
				v.z = z;
				
				var chunk : Chunk = m_chunks.get(v) as Chunk;
				if (chunk == null): continue;
				
				chunk.changeSphere(center, radius, type);

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
