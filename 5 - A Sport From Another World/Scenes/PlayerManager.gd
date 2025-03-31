extends Node
class_name PlayerManager;

static var s_instance;

var m_players : Array[Player] = [];

func _ready() -> void:
	s_instance = self;

func _process(delta: float) -> void:
	for player : Player in s_instance.m_players:
		if (!player.isDead()): continue;
		respawnPlayer(player);

static func getAllPlayers() -> Array[Player]:
	if (s_instance == null): return [];
	return s_instance.m_players;
static func getAlivePlayers() -> Array[Player]:
	if (s_instance == null): return [];
	
	var alive : Array[Player] = [];
	for player : Player in s_instance.m_players:
		if (player.isDead()): continue;
		alive.append(player);

	return alive;

static func registerPlayer(player : Player) -> void:
	if (s_instance == null || s_instance.m_players.has(player)): return;
	s_instance.m_players.append(player);
static func deregisterPlayer(player : Player) -> void:
	if (s_instance == null): return;
	s_instance.m_players.erase(player);

static func respawnPlayer(player : Player) -> void:
	player.m_currentHealth = player.m_maxHealth;
	player.m_gravity = 0;
	#player.m_currentTool = Player.Tool.None;
	player.global_position = World.s_worldInstance.getSafePoint();
	player.digout();
