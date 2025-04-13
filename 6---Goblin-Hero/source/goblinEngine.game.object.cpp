#include "goblinEngine.game.object.hpp"

std::vector<GameObject*> GameObject::s_toStart;

GameObject::GameObject(const char* name, GameObject* const parent) :
	m_name(name), m_parent(nullptr), m_children() {
	setParent(parent);
	s_toStart.emplace_back(this);
}
GameObject::~GameObject() {
	setParent(nullptr);
	clearChildren(true);
	for (size_t i = 0; i < s_toStart.size(); i++) {
		if (s_toStart[i] != this)
			continue;
		s_toStart.erase(s_toStart.begin() + i);
		break;
	}
}

void GameObject::start() {}
void GameObject::update() {}
void GameObject::drawTop() {}
void GameObject::drawBottom() {}

void GameObject::_startAll() {
	for (size_t i = 0; i < s_toStart.size(); i++)
		s_toStart[i]->start();
	s_toStart.clear();
}
void GameObject::_updateAll() {
	update();
	for (size_t i = 0; i < m_children.size(); i++)
		m_children[i]->_updateAll();
}
void GameObject::_drawTopAll() {
	drawTop();
	for (size_t i = 0; i < m_children.size(); i++)
		m_children[i]->_drawTopAll();
}
void GameObject::_drawBottomAll() {
	drawBottom();
	for (size_t i = 0; i < m_children.size(); i++)
		m_children[i]->_drawBottomAll();
}

GameObject* const GameObject::getParent() const {
	return m_parent;
}
GameObject* const GameObject::getRootParent() {
	GameObject* root = this;
	while (root->getParent() != nullptr)
		root = root->getParent();
	return root;
}
void GameObject::setParent(GameObject* const parent) {
	if (m_parent == parent)
		return;

	if (m_parent != nullptr) m_parent->removeChild(this);
	if (parent != nullptr) parent->setChild(this);

	m_parent = parent;
}
void GameObject::removeParent() {
	if (m_parent == nullptr) return;
	m_parent->removeChild(this);
	m_parent = nullptr;
}

const std::vector<GameObject*>& GameObject::getChildren() const {
	return m_children;
}
GameObject* const GameObject::getChild(const char* childName, const bool recursive) {
	if (recursive) {
		GameObject* child;
		for (size_t i = 0; i < m_children.size(); i++) {
			if (strcmp(childName, m_children[i]->m_name) != 0) {
				child = m_children[i]->getChild(childName, true);
				if (child == nullptr)
					continue;
				return child;
			}
			return m_children[i];
		}
	} else {
		for (size_t i = 0; i < m_children.size(); i++) {
			if (strcmp(childName, m_children[i]->m_name) != 0)
				continue;
			return m_children[i];
		}
	}
	return nullptr;
}
GameObject* const GameObject::getChildInScene(const char* childName) {
	GameObject* const root = getRootParent();
	return root->getChild(childName, true);
}
void GameObject::setChild(GameObject* const child) {
	for (size_t i = 0; i < m_children.size(); i++)
		if (child == m_children[i])
			return;
	m_children.emplace_back(child);
}
void GameObject::removeChild(GameObject* const child) {
	for (size_t i = 0; i < m_children.size(); i++) {
		if (child != m_children[i])
			continue;
		m_children.erase(m_children.begin() + i);
		break;
	}
}
void GameObject::clearChildren(const bool free) {
	for (size_t i = 0; i < m_children.size(); i++)
		delete m_children[i];
	m_children.clear();
}