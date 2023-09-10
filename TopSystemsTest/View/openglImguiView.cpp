#include <openglImguiView.hpp>
#include <iostream> // todo remove, just for debug

void view() {
	Resource text = LOAD_RESOURCE(Resources_glsl_1D_frag_glsl);
	Resource text1 = LOAD_RESOURCE(Resources_glsl_1D_vert_glsl);
	Resource text2 = LOAD_RESOURCE(Resources_glsl_1D_white_vert_glsl);
	std::cout << std::string(text.data(), text.size()) << std::endl;
	std::cout << std::string(text1.data(), text1.size()) << std::endl;
	std::cout << std::string(text2.data(), text2.size()) << std::endl;
}