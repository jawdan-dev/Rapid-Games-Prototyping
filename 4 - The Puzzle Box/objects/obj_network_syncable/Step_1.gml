if (!m_enabled) return;
if (!m_networkUpdateData) return;

if (m_networkCooldown > 0) {
	m_networkCooldown -= getDeltaTime();
	return;
}


m_networkUpdateData = false;
m_networkCooldown = m_networkCooldownMax;

Network_sendPacket("update", Network_getData(id));