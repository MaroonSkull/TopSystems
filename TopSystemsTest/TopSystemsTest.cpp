#include <TopSystemsTest.hpp>

int main() {
	try {
		IView *View = new OpenglImguiView();

		// тут надо накинуть обработчики в controller
		// можно передать вью в контроллер при создании, чтобы контроллер сам к этой вью подключился
		// wnd.keyEvent.setCallback([](glfw::Window&, glfw::KeyCode, int, glfw::KeyState, glfw::ModifierKeyBit) {
		//	std::cout << "hi";
		// });

		while (!View->shouldClose()) {
			View->draw();
			// передаём управление в обработку ввода
			// Controller->control();
			glfw::pollEvents();
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
