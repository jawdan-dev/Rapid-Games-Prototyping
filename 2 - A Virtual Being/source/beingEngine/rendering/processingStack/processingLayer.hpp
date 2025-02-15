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
	inline GLuint getTexture(const int index = 0) { return m_textures[index]; }
	inline GLuint getFramebuffer() { return m_framebuffer; }
	inline Renderer& getRenderer() { return m_renderer; }

	void addTexture(const GLuint texture);
	void setSize(const int width, const int height);
	virtual void onResize();

	virtual void setup() {};
	virtual void process() {};
	virtual void draw() = 0;

	void passRenderer(Renderer& previousRenderer, const Matrix4& viewProjection);
	void clear();

private:
	void updateTextures();

private:
	int m_width, m_height;
	GLuint m_framebuffer, m_depthTexture;
	GLuint m_baseTexture;
	std::vector<GLuint> m_textures;
	Renderer m_renderer;
};