#include "goblinEngine.game.object.hpp"

std::set<GameObject*> GameObject::s_toStart;
std::set<GameObject*> GameObject::s_toFree;

GameObject::GameObject(const char* name, GameObject* const parent) :
	m_enabled(true), m_name(name),
	m_parent(nullptr), m_children() {
	setParent(parent);
	queueStart();
}
GameObject::~GameObject() {
	removeParent();
	clearChildren();
	auto it = s_toStart.find(this);
	if (it != s_toStart.end())
		s_toStart.erase(it);
}

void GameObject::start() {}
void GameObject::update() {}
void GameObject::drawTop() {}
void GameObject::drawBottom() {}


void GameObject::queueStart() {
	s_toStart.emplace(this);
}

void GameObject::_startAll() {
	for (auto it = s_toStart.begin(); it != s_toStart.end(); it++)
		(*it)->start();
	s_toStart.clear();
}
void GameObject::_updateAll() {
	if (!m_enabled)
		return;
	for (size_t i = 0; i < m_children.size(); i++)
		m_children[i]->_updateAll();
	update();
}
void GameObject::_drawTopAll() {
	if (!m_enabled)
		return;
	for (size_t i = 0; i < m_children.size(); i++)
		m_children[i]->_drawTopAll();
	drawTop();
}
void GameObject::_drawBottomAll() {
	if (!m_enabled)
		return;
	for (size_t i = 0; i < m_children.size(); i++)
		m_children[i]->_drawBottomAll();
	drawBottom();
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

	if (parent == nullptr) {
		removeParent();
		return;
	}

	if (m_parent != nullptr)
		m_parent->removeChild(this);

	parent->addChild(this);
}
void GameObject::removeParent() {
	if (m_parent == nullptr)
		return;
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
void GameObject::getChildren(const char* childName, std::vector<GameObject*>* const output, const bool recursive) {
	if (recursive) {
		for (size_t i = 0; i < m_children.size(); i++) {
			if (strcmp(childName, m_children[i]->m_name) == 0)
				output->push_back(m_children[i]);
			m_children[i]->getChildren(childName, output, true);
		}
	} else {
		for (size_t i = 0; i < m_children.size(); i++) {
			if (strcmp(childName, m_children[i]->m_name) == 0)
				output->push_back(m_children[i]);
		}
	}
}
void GameObject::addChild(GameObject* const child) {
	if (child->m_parent == this)
		return;

	child->removeParent();
	m_children.emplace_back(child);
	child->m_parent = this;
}
void GameObject::removeChild(GameObject* const child) {
	for (size_t i = 0; i < m_children.size(); i++) {
		if (m_children[i] != child)
			continue;
		m_children.erase(m_children.begin() + i);
		child->m_parent = nullptr;
		break;
	}
}
void GameObject::clearChildren() {
	for (size_t i = 0; i < m_children.size(); i++)
		delete m_children[i];
	m_children.clear();
}
void GameObject::sortChildren(bool (*func)(GameObject* const, GameObject* const), const bool fullPass) {
	size_t end = m_children.size();
	bool sorted = false;
	while (!sorted) {
		sorted = true;
		end--;

		for (size_t i = 0; i < end; i++) {
			if (!func(m_children[i + 1], m_children[i]))
				continue;

			GameObject* temp = m_children[i + 1];
			m_children[i + 1] = m_children[i];
			m_children[i] = temp;

			sorted = !fullPass;
		}
	}
}

GameObject* const GameObject::getObjectInScene(const char* name) {
	GameObject* const root = getRootParent();
	return root->getChild(name, true);
}
void GameObject::getObjectsInScene(const char* name, std::vector<GameObject*>* output) {
	GameObject* const root = getRootParent();
	root->getChildren(name, output, true);
}

void GameObject::queueFree() {
	s_toFree.emplace(this);
}
void GameObject::_freeAll() {
	for (auto it = s_toFree.begin(); it != s_toFree.end(); it++)
		(*it)->~GameObject();
	s_toFree.clear();
}