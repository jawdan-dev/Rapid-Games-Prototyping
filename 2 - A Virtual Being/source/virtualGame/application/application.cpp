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

struct ApplicationData {
	ProcessingStack processingStack;
	TextRenderer textRenderer;
	StoryInterpreter storyInterpreter;
	Renderer renderer;
};

#ifdef __EMSCRIPTEN__
struct AppData {
	Application* application;
	ApplicationData* data;
};
void updateFunction(void* const data) {
	AppData& appData = *(AppData*)data;
	appData.application->update(appData.data);
}
void startMainLoop(AppData* const data) {
	emscripten_set_main_loop_arg(updateFunction, (void*)data, 0, true);
}
#endif

void Application::start() {
	ApplicationData applicationData = {
		.processingStack = ProcessingStack(m_window.getWidth(), m_window.getHeight()),
	};
	// applicationData.processingStack.emplaceLayer<CRTDotsPostProcessing>();
	applicationData.processingStack.emplaceLayer<CRTCurveProcessing>(5.0f, screenRatio, 10);

#ifdef __EMSCRIPTEN__
	AppData appData = {
		.application = this,
		.data = &applicationData,
	};
	startMainLoop(&appData);
#else
	while (m_window.isOpen()) {
		update(&applicationData);
	}
#endif
}

void Application::update(void* const data) {
	ApplicationData& applicationData = *(ApplicationData*)data;

	applicationData.processingStack.setSize(m_window.getWidth(), m_window.getHeight());
	m_window.process();

	Time::s_time = &m_window.getTime();
	Input::s_input = &m_window.getInput();

	applicationData.processingStack.process();

	applicationData.renderer.clear();
	applicationData.processingStack.clear();

	applicationData.storyInterpreter.setScale((float)m_window.getWidth() / 1280.0f);
	applicationData.storyInterpreter.process();
	applicationData.storyInterpreter.draw(applicationData.renderer, applicationData.textRenderer);

	const Matrix4 viewProjection =
		Matrix4::orthographic(0, m_window.getWidth(), 0, m_window.getHeight(), -100.0f, 100.0f) *
		Matrix4::lookAt(Vector3(0, 0, 2), Vector3(0, 0, 0));

	applicationData.processingStack.render(applicationData.renderer, viewProjection);
	m_window.render();
}