event_inherited();
depth--;

m_canInteractFunc = function() { return true; };
m_hoverable = false;
m_disabled = false;

if (!variable_global_exists("m_hoveredElement") ||
	!variable_global_exists("m_activeElement")) {
	globalvar m_hoveredElement;
	m_hoveredElement = noone;
	globalvar m_activeElement;
	m_activeElement = noone;
}

m_customMap = ds_map_create();

m_wordLookIndex = 0;