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
	void getChildren(const char* childName, std::vector<GameObject*>* const output, const bool recursive = false);
	void addChild(GameObject* const child);
	void removeChild(GameObject* const child);
	void clearChildren(const bool free = true);
	void sortChildren(bool(*func)(GameObject* const, GameObject* const), const bool fullPass = true);

	GameObject* const getObjectInScene(const char* name);
	void getObjectsInScene(const char* name, std::vector<GameObject*>* const output);

	void queueFree();
	static void _freeAll();

private:
	static std::vector<GameObject*> s_toStart;
	static std::set<GameObject*> s_toFree;

private:
	bool m_enabled;
	const char* m_name;
	GameObject* m_parent;
	std::vector<GameObject*> m_children;
};