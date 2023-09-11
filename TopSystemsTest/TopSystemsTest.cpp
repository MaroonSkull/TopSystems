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
	}
	catch (const std::runtime_error& e) {
		std::cerr << "std::runtime_error: " << e.what() << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "std::exception: " << e.what() << std::endl;
	}
	catch (const char* e) {
		std::cerr << e << std::endl;
	}
	catch (...) {
		std::cerr << "Unknown exception!" << std::endl;
	}
	return 0;
}
