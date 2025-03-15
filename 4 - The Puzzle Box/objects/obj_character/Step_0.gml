if (!m_enabled) return;

var pos = moveTowards(m_drawX, m_drawY, x, y, m_movementSpeed * getDeltaTime());
m_drawX = pos.x;
m_drawY = pos.y;
