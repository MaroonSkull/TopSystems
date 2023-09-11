#include <TopSystemsTest.hpp>

int main() {
	try {
		IView *View = new OpenglImguiView();
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
