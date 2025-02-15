#include "processingStack.hpp"

ProcessingStack::ProcessingStack(const int width, const int height) :
	m_width(width), m_height(height),
	m_layers() {}
ProcessingStack::~ProcessingStack() {}

void ProcessingStack::setSize(const int width, const int height) {
	if (width == m_width && height == m_height)
		return;

	m_width = width;
	m_height = height;

	for (auto layer : m_layers)
		layer->setSize(m_width, m_height);
}
void ProcessingStack::pushLayer(ProcessingLayer* const layer) {
	layer->setSize(m_width, m_height);
	m_layers.emplace_back(layer);
}

void ProcessingStack::render(Renderer& renderer, const Matrix4& viewProjection) {
	Renderer* lastRenderer = &renderer;

	for (auto layer : m_layers) {
		layer->passRenderer(*lastRenderer, viewProjection);
		layer->draw();
		lastRenderer = &layer->getRenderer();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	lastRenderer->render(viewProjection);
}
void ProcessingStack::clear() {
	for (auto layer : m_layers)
		layer->clear();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}