#pragma once

#include "goblinEngine.core.hpp"

class GameObject {
public:
	GameObject(const char* name, GameObject* const parent = nullptr);
	GameObject(const GameObject& other) = delete;
	virtual ~GameObject();

	virtual void start();
	virtual void update();
	virtual void drawTop();
	virtual void drawBottom();

	static void _startAll();
	void _updateAll();
	void _drawTopAll();
	void _drawBottomAll();

	GameObject* const getParent() const;
	GameObject* const getRootParent();
	void setParent(GameObject* const parent);
	void removeParent();

	const std::vector<GameObject*>& getChildren() const;
	GameObject* const getChild(const char* childName, const bool recursive = false);
	GameObject* const getChildInScene(const char* childName);
	void setChild(GameObject* const child);
	void removeChild(GameObject* const child);
	void clearChildren(const bool free = true);

private:
	static std::vector<GameObject*> s_toStart;

private:
	const char* m_name;
	GameObject* m_parent;
	std::vector<GameObject*> m_children;
};