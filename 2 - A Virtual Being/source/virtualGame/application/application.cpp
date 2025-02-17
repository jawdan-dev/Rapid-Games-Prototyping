#include "application.hpp"

#include <beingEngine/rendering/processingStack/processingStack.hpp>
#include <beingEngine/rendering/renderer/renderer.hpp>
#include <virtualGame/postProcessing/crtCurve.hpp>
#include <virtualGame/postProcessing/crtDots.hpp>
#include <virtualGame/postProcessing/test.hpp>
#include <virtualGame/story/interpreter/interpreter.hpp>
#include <virtualGame/story/textRenderer/textRenderer.hpp>

Application::Application() :
	m_window(640 * 2, 360 * 2) {}

void Application::start() {
	ProcessingStack processingStack(m_window.getWidth(), m_window.getHeight());
	// processingStack.emplaceLayer<CRTDotsPostProcessing>();
	processingStack.emplaceLayer<CRTCurveProcessing>(5.0f, screenRatio, 10);

	TextRenderer textRenderer;
	StoryInterpreter storyInterpreter;
	Renderer renderer;

	while (m_window.isOpen()) {
		processingStack.setSize(m_window.getWidth(), m_window.getHeight());
		m_window.process();

		Time::s_time = &m_window.getTime();
		Input::s_input = &m_window.getInput();

		processingStack.process();
		Vector2 inputFlip = Input::s_input->getMousePosition();
		inputFlip.y() = m_window.getHeight() - inputFlip.y();
		Input::s_input->setWeakMousePosition(inputFlip);

		storyInterpreter.setScale((float)m_window.getWidth() / 1280.0f);
		storyInterpreter.process();
		storyInterpreter.draw(renderer, textRenderer);

		const Matrix4 viewProjection =
			Matrix4::orthographic(0, m_window.getWidth(), 0, m_window.getHeight(), -100.0f, 100.0f) *
			Matrix4::lookAt(Vector3(0, 0, 2), Vector3(0, 0, 0));

		processingStack.render(renderer, viewProjection);
		m_window.render();

		renderer.clear();
		processingStack.clear();
	}
}
