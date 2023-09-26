#include <TopSystemsTest.hpp>

int main() {
	try {
		//model here
		FlatFigureModel* pModel = new FlatFigureModel();
		IController *pController = new OpenglImguiController(pModel);
		IView* pView = new OpenglImguiView(pModel, pController);

		while (!pView->shouldClose()) {
			// отлавливаем все события
			glfw::pollEvents();


			pView->draw();
		}

		// Under VisualStudio, this must be called before main finishes to workaround a known VS issue
		spdlog::drop_all();
	}
	catch (const std::runtime_error& e) {
		spdlog::error("std::runtime_error: {}", e.what());
	}
	catch (const std::exception& e) {
		spdlog::error("std::exception: {}", e.what());
	}
	catch (const char* e) {
		spdlog::error(e, "\r\n");
	}
	catch (...) {
		spdlog::error("Unknown exception!");
	}
	return 0;
}
