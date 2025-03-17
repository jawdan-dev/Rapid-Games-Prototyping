event_inherited();

var target = GUI_isHovered(id) ? 1.0 : (m_disabled ? -1.0 : 0.0);

var change = target - m_offsetTransitionAmount;
if (change == 0) return;

if (change > 0) {
	m_offsetTransitionAmount += getDeltaTime() / m_offsetTransitionTime;
	if (m_offsetTransitionAmount > target)
		m_offsetTransitionAmount = target;
} else {
	m_offsetTransitionAmount -= getDeltaTime() / m_offsetTransitionTime;
	if (m_offsetTransitionAmount < target)
		m_offsetTransitionAmount = target;
}

var pos = m_offsetTransitionAmount >= 0 ? 
	m_originalPosition.interpolate(m_originalPosition.add(m_offsetTarget), ease(m_offsetTransitionAmount)) :
	m_originalPosition.interpolate(m_originalPosition.add(m_disabledTarget), ease(-m_offsetTransitionAmount));
m_cameraOffset.x = pos.x;
m_cameraOffset.y = pos.y;