extends CharacterBody3D
class_name Player;

#####################################################################
@export_category("Config")
@export var m_playerIndex : int = 0;
@export var m_inputDeadzone : float = 0.15;
#####################################################################
@export_group("Health")
@export var m_maxHealth : float = 20.0;
var m_currentHealth : float = 0;
@export var m_healthRegenSpeed : float = 5.0;
@export var m_regenCooldown : float = 8.0;
var m_lastHitTime : float = 0;
@export var m_digoutScale : float = 1.4;
#####################################################################
@export_group("Movement")
@export var m_movementSpeed : float = 10.0;
var m_movementInput : Vector2;
var m_jump : bool = false;
#####################################################################
@export_group("Camera")
@export var m_cameraLookSpeed : float = PI;
@export var m_cameraInternalHeight : float = 0.8;
@export var m_cameraArmHeight : float = 2;
@export var m_cameraArmLength : float = 4;
@export var m_cameraArmRadius : float = 2;
@export var m_cameraSafeDistance : float = 0.2;
var m_cameraAngle : Vector2;
var m_externalCamera : bool = false;
var m_externalCameraPressedLastFrame : bool = false;
var m_invertCamera : bool = false;
var m_invertCameraPressedLastFrame : bool = false;
#####################################################################
@export_group("Falling")
@export var m_fallThreshold : float = 11.0;
@export var m_fallDamageMultiplier : float = 0.8;
@export var m_fallSizeMultiplier : float = 0.3;
var m_gravity : float = 0;
#####################################################################
@export_category("Tools")
enum Tool {
	None, 
	Drill, Blobgun, Shotgun, 
	COUNT
};
@export var m_currentTool : Tool = Tool.None;
var m_useTool : bool = false;
var m_toolCooldown : float = 0.0;
var m_currentAnimator : Animator = null;
var m_lastAnimator : Animator = null;
var m_nextWeapon : bool = false;
var m_nextWeaponLastFrame : bool = false;
#####################################################################
@export_group("Pickaxe")
@export var m_pickaxeAnimator : Animator;
@export var m_pickaxeCooldownMax : float = 0.45;
@export var m_pickaxeDistance : float = 0.9;
@export var m_pickaxeRadius : float = 1.2;
@export var m_pickaxeDPS : float = 3;
var m_usePickaxe : bool = false;
#####################################################################
@export_group("Drill")
@export var m_drillAnimator : Animator;
@export var m_drillDistance : float = 1;
@export var m_drillRadius : float = 1.25;
@export var m_drillMovementFactorMax : float = 1.25;
@export var m_drillMovementCooldown : float = 2;
@export var m_drillMovementSpeed : float = 5.0;
@export var m_drillDPS : float = 8.0;
var m_drillMovementFactor : float = 0.0;
#####################################################################
@export_group("Blobgun")
@export var m_blobgunAnimator : Animator;
@export var m_blobgunSplashCount : int = 10;
@export var m_blobgunSplashSize : float = 0.7;
@export var m_blobgunSpeed : float = 10.0;
@export var m_blobgunSize : float = 2.0;
@export var m_blobgunCooldownMax : float = 1.45;
#####################################################################
@export_group("Shotgun")
@export var m_shotgunAnimator : Animator;
@export var m_shotgunAngleMax : float = TAU * 0.05
@export var m_shotgunPelletCount : int = 30;
@export var m_shotgunPelletSpeed : float = 30;
@export var m_shotgunPelletSize : float = 0.9;
@export var m_shotgunCooldownMax : float = 0.85;
#####################################################################

func _ready() -> void:
	PlayerManager.registerPlayer(self);
	var color : Color;
	match (m_playerIndex):
		0: color = Color8(255, 0, 68, 200);
		1: color = Color8(51, 0, 238, 200);
		2: color = Color8(255, 187, 51, 200);
		3: color = Color8(255, 68, 170, 200);
	$CanvasLayer/Health.set_instance_shader_parameter("i_color", color);
	$Mesh.set_instance_shader_parameter("i_color", color);
	
	var playerBit : int = 0b1 << (2 + m_playerIndex)
	if (m_externalCamera): playerBit = 0b0;
	var playerBits : int = 0b1111 << (2)
	$Camera.cull_mask = $Camera.cull_mask & ~playerBit;
	$CanvasLayer/SubViewportContainer/SubViewport/SeeThrough.cull_mask = $CanvasLayer/SubViewportContainer/SubViewport/SeeThrough.cull_mask & ~playerBit;
	$Mesh.layers = playerBit;
	$Mesh/Eyes.layers = playerBit;


func _notification(what):
	if (what == NOTIFICATION_PREDELETE):
		PlayerManager.deregisterPlayer(self);

func _process(delta: float) -> void:
	var deadzoneSqr : float = m_inputDeadzone * m_inputDeadzone;
	
	if (Input.is_key_pressed(KEY_1)): 
		m_playerIndex = 0;
		_ready();
	if (Input.is_key_pressed(KEY_2)): 
		m_playerIndex = 1;
		_ready();
	if (Input.is_key_pressed(KEY_3)): 
		m_playerIndex = 2;
		_ready();
	if (Input.is_key_pressed(KEY_4)): 
		m_playerIndex = 3;
		_ready();
	
	var controllerID : int = PlayerManager.getAllPlayers().find(self);
	
	#####################################################################
	
	m_movementInput = Vector2(
		Input.get_joy_axis(controllerID, JOY_AXIS_LEFT_X),
		Input.get_joy_axis(controllerID, JOY_AXIS_LEFT_Y)
	);
	if (m_movementInput.length_squared() < deadzoneSqr):
		m_movementInput = Vector2.ZERO;
	m_jump = m_jump || Input.is_joy_button_pressed(controllerID, JOY_BUTTON_A);
	
	#####################################################################
	
	var cameraInput : Vector2 = Vector2(
		Input.get_joy_axis(controllerID, JOY_AXIS_RIGHT_X),
		Input.get_joy_axis(controllerID, JOY_AXIS_RIGHT_Y)
	);
	if (m_invertCamera):
		cameraInput.y *= -1;
	
	if (cameraInput.length_squared() < deadzoneSqr):
		cameraInput = Vector2.ZERO;
	else: 
		cameraInput *= m_cameraLookSpeed * delta;
		const cameraRange : float = 0.4;
		m_cameraAngle.x = fposmod(m_cameraAngle.x - cameraInput.x, TAU);
		m_cameraAngle.y = clampf(m_cameraAngle.y + cameraInput.y, -PI * cameraRange, PI * cameraRange);
		
	if (Input.is_joy_button_pressed(controllerID, JOY_BUTTON_START)):
		if (!m_externalCameraPressedLastFrame):
			m_externalCamera = !m_externalCamera;
			_ready();
		m_externalCameraPressedLastFrame = true;
	else:
		m_externalCameraPressedLastFrame = false;
		
	if (Input.is_joy_button_pressed(controllerID, JOY_BUTTON_BACK)):
		if (!m_invertCameraPressedLastFrame):
			m_invertCamera = !m_invertCamera;
		m_invertCameraPressedLastFrame = true;
	else:
		m_invertCameraPressedLastFrame = false;
	
	#####################################################################
	
	m_useTool = Input.get_joy_axis(controllerID, JOY_AXIS_TRIGGER_RIGHT) > 0.8;
	m_usePickaxe = Input.is_joy_button_pressed(controllerID, JOY_BUTTON_RIGHT_SHOULDER);
	
	m_nextWeapon = Input.is_joy_button_pressed(controllerID, JOY_BUTTON_LEFT_SHOULDER);
	if (m_nextWeapon):
		if (!m_nextWeaponLastFrame):
			m_currentTool = (m_currentTool + 1) % Tool.COUNT;
		m_nextWeaponLastFrame = true;
	else:
		m_nextWeaponLastFrame = false;
	
	if (Input.is_joy_button_pressed(controllerID, JOY_BUTTON_B)):
		World.s_worldInstance.changeSphere(position, getPlayerRadius() * 3, getTerrainType(), -1);
	
	if (Input.is_joy_button_pressed(controllerID, JOY_BUTTON_X)):
		World.s_worldInstance.changeSphere(position - Vector3(0, 1, 0), 2, getTerrainType());
	

func _physics_process(delta: float) -> void:	
	updateCamera();
	if (isDead()): return;
	
	regenHealth(delta);
	$CanvasLayer/Health.set_instance_shader_parameter("i_healthPercentage", m_currentHealth / m_maxHealth);
	
	m_drillAnimator.visible = false;
	if (m_lastAnimator != m_currentAnimator):
		if (m_lastAnimator != null):
			m_lastAnimator.visible = false;
		m_lastAnimator = m_currentAnimator;
		if (m_currentAnimator != null):
			m_currentAnimator.visible = true;
	
	if (m_currentAnimator != null):
		if (m_currentAnimator.hasFinished()):
			m_currentAnimator = null;
	else:
		m_useTool = Tool_pickaxe(delta) && m_useTool;
		match (m_currentTool):
			Tool.Drill: Tool_drill(delta);
			Tool.Blobgun: Tool_blobgun(delta);
			Tool.Shotgun: Tool_shotgun(delta);
	
	if (m_currentAnimator != null):
		m_currentAnimator.visible = true;

	var totalMovement : Vector3 = Vector3.ZERO;
	
	var forward : Vector3 = Vector3(sin(m_cameraAngle.x), 0, cos(m_cameraAngle.x));	
	var right : Vector3 = Vector3(cos(m_cameraAngle.x), 0, -sin(m_cameraAngle.x));	
	
	totalMovement += right * m_movementInput.x * m_movementSpeed;
	totalMovement += forward * m_movementInput.y * m_movementSpeed;
	
	if (is_on_floor()):
		if (m_gravity < -m_fallThreshold):
			var damage : float = -(m_gravity + m_fallThreshold);
			damage = pow(damage, 1.2);
			takeDamage(damage * m_fallDamageMultiplier);
			World.s_worldInstance.changeSphere(position, getPlayerRadius() + (damage * m_fallSizeMultiplier), getTerrainType(), -1, 2);
		
		if (m_jump):
			m_gravity = 10.0;
		else:
			m_gravity = -0.1;
	else:
		m_gravity += -9.81 * delta;
	totalMovement.y += m_gravity;
	m_jump = false;
	
	velocity = totalMovement;
	move_and_slide();

func getPlayerRadius() -> float:
	return ($WorldCollider.shape as SphereShape3D).radius;
	
func digout() -> void:
	World.s_worldInstance.changeSphere(
		position, 
		getPlayerRadius() * m_digoutScale, 
		getTerrainType(), 
		-1
	);

func updateCamera() -> void:
	$CanvasLayer/SubViewportContainer.visible = m_currentTool == Tool.Drill;
	$Mesh.rotation = Vector3(-m_cameraAngle.y, m_cameraAngle.x, 0);
		
	if (!m_externalCamera):
		$Camera.position = Vector3(0, m_cameraInternalHeight, 0);
		$Camera.global_rotation = Vector3(-m_cameraAngle.y, m_cameraAngle.x, 0);
		$CanvasLayer/SubViewportContainer/SubViewport/SeeThrough.global_transform = $Camera.global_transform;
		return;
	
	var worldSpace : PhysicsDirectSpaceState3D = get_world_3d().direct_space_state;
	
	var heightQuery : PhysicsRayQueryParameters3D = PhysicsRayQueryParameters3D.create(global_position, global_position + (Vector3.UP * m_cameraArmHeight), collision_mask);
	var heightPos : Vector3 = safeCast(worldSpace, heightQuery, m_cameraSafeDistance);

	var cameraXZDir : Vector3 = Vector3(sin(m_cameraAngle.x), 0, cos(m_cameraAngle.x));	
	var lengthQuery : PhysicsRayQueryParameters3D = PhysicsRayQueryParameters3D.create(heightPos, heightPos + (cameraXZDir * m_cameraArmLength), collision_mask);
	var lengthPos : Vector3 = safeCast(worldSpace, lengthQuery, m_cameraSafeDistance);
	
	var cameraDir : Vector3 = Vector3(
		cos(m_cameraAngle.y) * cameraXZDir.x,
		sin(m_cameraAngle.y),
		cos(m_cameraAngle.y) * cameraXZDir.z
	);
	var cameraQuery : PhysicsRayQueryParameters3D = PhysicsRayQueryParameters3D.create(lengthPos, lengthPos + (cameraDir * m_cameraArmRadius), collision_mask);
	var cameraPos : Vector3 = safeCast(worldSpace, cameraQuery, m_cameraSafeDistance);
	
	$Camera.global_position = cameraPos;
	$Camera.global_rotation = Vector3(-m_cameraAngle.y, m_cameraAngle.x, 0);
	
	$CanvasLayer/SubViewportContainer/SubViewport/SeeThrough.global_transform = $Camera.global_transform;

func safeCast(worldSpace : PhysicsDirectSpaceState3D, query : PhysicsRayQueryParameters3D, safeDistance : float) -> Vector3:
	var rayHit : Dictionary = worldSpace.intersect_ray(query);
	if (rayHit.is_empty()): return query.to;
	
	var direction : Vector3 = query.to - query.from;
	
	var safePos : Vector3 = rayHit.position - (direction * safeDistance)
	if ((safePos - query.from).dot(direction) < 0): return query.from;
	
	return safePos;


func isDead():
	return m_currentHealth <= 0.0;

func getForward() -> Vector3:
	return Vector3(
		-cos(m_cameraAngle.y) * sin(m_cameraAngle.x),
		-sin(m_cameraAngle.y),
		-cos(m_cameraAngle.y) * cos(m_cameraAngle.x)
	);
func getRandomForward(maxAngle : float) -> Vector3:
	var angle : float = randf_range(0, TAU);
	var offset : Vector2 = Vector2(
		sin(angle), cos(angle)
	) * randf_range(0, maxAngle)
	
	return Vector3(
		-cos(m_cameraAngle.y + offset.y) * sin(m_cameraAngle.x + offset.x),
		-sin(m_cameraAngle.y + offset.y),
		-cos(m_cameraAngle.y + offset.y) * cos(m_cameraAngle.x + offset.x)
	);
	
func regenHealth(delta) -> void:
	if (m_currentHealth >= m_maxHealth): return;
	var currentTime : float = float(Time.get_ticks_msec()) * 0.001;
	if (currentTime - m_regenCooldown < m_lastHitTime): return;
	m_currentHealth = move_toward(m_currentHealth, m_maxHealth, m_healthRegenSpeed * delta)
	#print(name, " regened ", m_healthRegenSpeed * delta, " health. health: ", m_currentHealth);

func takeDamage(damage : float) -> void:
	if (isDead()): return;
	if (damage < 0): return;
	
	m_currentHealth -= damage;
	m_lastHitTime = float(Time.get_ticks_msec()) * 0.001;
	#print(name, " took ", damage, " damage. health: ", m_currentHealth);
	
func getTerrainType() -> Chunk.TerrainType:
	return (int(Chunk.TerrainType.Player0) + m_playerIndex) as Chunk.TerrainType;
	
func Tool_pickaxe(delta) -> bool:
	if (!m_usePickaxe): return true;
	m_toolCooldown = m_pickaxeCooldownMax;
	
	m_pickaxeAnimator.play();
	m_currentAnimator = m_pickaxeAnimator;
	
	var forward : Vector3 = getForward();
	World.s_worldInstance.changeSphere(
		global_position + (forward * m_pickaxeDistance),
		m_pickaxeRadius,
		getTerrainType(), 
		-1,
		m_pickaxeDPS * m_pickaxeCooldownMax
	);
	
	return false;
	
var m_drillProgressAni : float = 0.0;
func Tool_drill(delta) -> void:	
	m_drillAnimator.visible = true;
	
	var m_drillProgressAniTarget : float = 0;
	var forward : Vector3 = getForward();
	
	if (m_useTool && m_drillMovementFactor > 0):
		m_drillProgressAniTarget = 0.5;
		var worldSpace : PhysicsDirectSpaceState3D = get_world_3d().direct_space_state;
		var query : PhysicsRayQueryParameters3D = PhysicsRayQueryParameters3D.create(
			global_position, 
			global_position + (forward * (m_drillDistance + m_drillRadius) * m_drillMovementFactorMax), 
			collision_mask
		);
		var hit : Dictionary = worldSpace.intersect_ray(query);
		if (!hit.is_empty()):
			global_position = global_position.move_toward(hit.position - (forward * getPlayerRadius()), m_drillMovementSpeed * delta);
			m_drillMovementFactor -= (m_drillMovementSpeed * delta)
			m_gravity = 0;
	
	var drilled : bool = m_useTool && World.s_worldInstance.changeSphere(
		global_position + (forward * m_drillDistance),
		m_drillRadius,
		getTerrainType(), 
		-1,
		m_drillDPS * delta
	);
	
	if (drilled):
		m_drillMovementFactor = m_drillMovementFactorMax;
	else:
		m_drillMovementFactor = move_toward(m_drillMovementFactor, 0, delta / (m_drillMovementCooldown * m_drillMovementFactorMax));

	m_drillProgressAni = move_toward(m_drillProgressAni, m_drillProgressAniTarget, delta * 2.0);
	m_drillAnimator.m_animationProgress = m_drillProgressAni;
	m_drillAnimator.updateModel();

func Tool_blobgun(delta) -> void:
	if (m_currentAnimator == null):
		m_currentAnimator = m_blobgunAnimator;
	
	if (!m_useTool): return;
	m_toolCooldown = m_blobgunCooldownMax;
	
	m_blobgunAnimator.play();
	m_currentAnimator = m_blobgunAnimator;
	
	var forward : Vector3 = getForward();
	Projectile.Spawn(
		$Camera.global_position + forward,
		forward * m_blobgunSpeed,
		getTerrainType(), m_blobgunSize,
		0,
		m_blobgunSplashCount, m_blobgunSplashSize
	);

func Tool_shotgun(delta) -> void:
	if (m_currentAnimator == null):
		m_currentAnimator = m_shotgunAnimator;
		
	if (!m_useTool): return;
	m_toolCooldown = m_shotgunCooldownMax;
	
	m_shotgunAnimator.play();
	m_currentAnimator = m_shotgunAnimator;
	
	for i in range(m_shotgunPelletCount):
		var forward : Vector3 = getRandomForward(m_shotgunAngleMax);
		Projectile.Spawn(
			$Camera.global_position + forward,
			forward * m_shotgunPelletSpeed,
			getTerrainType(), m_shotgunPelletSize,
			[-1, -1, -1, 1, 1].pick_random()
		);
		
