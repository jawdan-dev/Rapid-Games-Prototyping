#include "processingLayer.hpp"

ProcessingLayer::ProcessingLayer() :
	m_width(0), m_height(0),
	m_framebuffer(0), m_depthTexture(0),
	m_baseTexture(0), m_textures(),
	m_renderer() {
	glGenFramebuffers(1, &m_framebuffer);
	glGenRenderbuffers(1, &m_depthTexture);

	glGenTextures(1, &m_baseTexture);
	m_textures.emplace_back(m_baseTexture);
}
ProcessingLayer::~ProcessingLayer() {
	if (m_framebuffer) {
		glDeleteFramebuffers(1, &m_framebuffer);
		m_framebuffer = 0;
	}
	if (m_baseTexture) {
		glDeleteTextures(1, &m_baseTexture);
		m_baseTexture = 0;
	}
}

void ProcessingLayer::addTexture(const GLuint texture) {
	m_textures.emplace_back(texture);
	updateTextures();
}
void ProcessingLayer::setSize(const int width, const int height) {
	if (width == m_width && height == m_height)
		return;

	m_width = width;
	m_height = height;

	updateTextures();
	onResize();
}
void ProcessingLayer::onResize() {}

void ProcessingLayer::passRenderer(Renderer& previousRenderer, const Matrix4& viewProjection) {
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glViewport(0, 0, m_width, m_height);
	previousRenderer.render(viewProjection);
}
void ProcessingLayer::clear() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_renderer.clear();
}

void ProcessingLayer::updateTextures() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

	std::vector<GLenum> m_attachments;
	for (int i = 0; i < m_textures.size(); i++) {
		m_attachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);

		glBindTexture(GL_TEXTURE_2D, m_textures[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glGenerateMipmap(GL_TEXTURE_2D);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textures[0], 0);
	}

	glBindRenderbuffer(GL_RENDERBUFFER, m_depthTexture);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthTexture);

	glDrawBuffers(m_attachments.size(), m_attachments.data());

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
