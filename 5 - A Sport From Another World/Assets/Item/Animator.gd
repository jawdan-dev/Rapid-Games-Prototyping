extends Node3D
class_name Animator;

@export var m_secondaryAnimations : Array[Animator] = [];

@export var m_animationTime : float = 1.0;
@export var m_animationLoops : bool;
var m_animationProgress : float = 0;
var m_isPlaying : float = 0;
@export_category("Position")
@export var m_startPosition : Vector3;
@export var m_endPosition : Vector3;
@export_category("Rotation")
@export var m_startRotation : Vector3;
@export var m_endRotation : Vector3;


func getAnimationProgress() -> float:	
	var x : float = m_animationProgress;
	
	if (m_animationLoops): return x;
	return sin(x * PI);

func _process(delta: float) -> void:
	if (!m_isPlaying):
		if (m_animationProgress > 0.0):
			m_animationProgress = 1.0;
		return;

	m_animationProgress = move_toward(m_animationProgress, 1.0, delta / m_animationTime);
	if (m_animationProgress >= 1.0):
		if (m_animationLoops):
			m_animationProgress = 0.0;
		else:
			m_isPlaying = false;
	updateModel();
	
func play() -> void:
	if (m_isPlaying): return;
	m_isPlaying = true;
	m_animationProgress = 0.0;
	updateModel();
	
	for secondary : Animator in m_secondaryAnimations:
		secondary.play();
	
func stop() -> void:
	if (!m_isPlaying): return;
	m_isPlaying = false;
	m_animationProgress = 1.0;
	updateModel();
	
	for secondary : Animator in m_secondaryAnimations:
		secondary.stop();
	
func hasFinished() -> bool:
	return !m_isPlaying;
	return m_animationProgress >= 1.0;

func setProgress(progress : float) -> void:
	play();
	m_animationProgress = progress;
	updateModel();
	
func updateModel() -> void:
	position = m_startPosition.lerp(m_endPosition, getAnimationProgress());
	rotation = m_startRotation.lerp(m_endRotation, getAnimationProgress());
	
