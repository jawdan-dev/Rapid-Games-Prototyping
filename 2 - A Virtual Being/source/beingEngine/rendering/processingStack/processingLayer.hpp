#pragma once
#include <beingEngine/core/core.hpp>
#include <beingEngine/rendering/renderer/renderer.hpp>

class ProcessingLayer {
public:
	ProcessingLayer();
	ProcessingLayer(const ProcessingLayer& other) = delete;
	virtual ~ProcessingLayer();

	inline const int getWidth() const { return m_width; }
	inline const int getHeight() const { return m_height; }
	inline GLuint getTexture() { return m_texture; }
	inline GLuint getFramebuffer() { return m_framebuffer; }
	inline Renderer& getRenderer() { return m_renderer; }

	void setSize(const int width, const int height);
	virtual void onResize();

	void passRenderer(Renderer& previousRenderer, const Matrix4& viewProjection);
	virtual void draw() = 0;
	void clear();

private:
	int m_width, m_height;
	GLuint m_framebuffer, m_texture, m_depthTexture;
	Renderer m_renderer;
};