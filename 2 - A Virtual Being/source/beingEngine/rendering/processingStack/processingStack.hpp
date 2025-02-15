#pragma once
#include <beingEngine/core/core.hpp>
#include <beingEngine/rendering/processingStack/processingLayer.hpp>

class ProcessingStack {
public:
	ProcessingStack(const int width, const int height);
	ProcessingStack(const ProcessingStack& other) = delete;
	~ProcessingStack();

	inline const std::vector<ProcessingLayer*>& getLayers() const { return m_layers; };
	inline std::vector<ProcessingLayer*>& getLayers() { return m_layers; };

	void setSize(const int width, const int height);
	void pushLayer(ProcessingLayer* const layer);
	template<typename T, typename... Args>
	void emplaceLayer(Args... args) { return pushLayer(new T(args...)); }

	void process();
	void render(Renderer& renderer, const Matrix4& viewProjection);
	void clear();

private:
	int m_width, m_height;
	std::vector<ProcessingLayer*> m_layers;
};