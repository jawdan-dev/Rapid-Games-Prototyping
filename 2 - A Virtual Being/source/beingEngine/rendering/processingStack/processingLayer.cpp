#include "processingLayer.hpp"

ProcessingLayer::ProcessingLayer() :
	m_width(0), m_height(0),
	m_framebuffer(0), m_texture(0), m_depthTexture(0),
	m_renderer() {
	glGenFramebuffers(1, &m_framebuffer);
	glGenTextures(1, &m_texture);
	glGenRenderbuffers(1, &m_depthTexture);
}
ProcessingLayer::~ProcessingLayer() {
	if (m_framebuffer) {
		glDeleteFramebuffers(1, &m_framebuffer);
		m_framebuffer = 0;
	}
	if (m_texture) {
		glDeleteTextures(1, &m_texture);
		m_texture = 0;
	}
}

void ProcessingLayer::setSize(const int width, const int height) {
	if (width == m_width && height == m_height) return;

	m_width = width;
	m_height = height;

	
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthTexture);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
	
	// glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glGenerateMipmap(GL_TEXTURE_2D);
	// glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);
	
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthTexture);
	
	const GLenum targetDrawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(sizeof(targetDrawBuffers) / sizeof(*targetDrawBuffers), targetDrawBuffers);
	glViewport(0, 0, m_width, m_height);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void ProcessingLayer::onResize() {}

void ProcessingLayer::passRenderer(Renderer& previousRenderer, const Matrix4& viewProjection) {
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	previousRenderer.render(viewProjection);
}
void ProcessingLayer::clear() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_renderer.clear();
}