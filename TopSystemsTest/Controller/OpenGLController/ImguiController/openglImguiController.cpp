#include <openglImguiController.hpp>

OpenglImguiController::OpenglImguiController(FlatFigureModel* pModel) : pModel_(pModel) {
	if (pModel_ == nullptr)
		throw std::exception{ "pModel cannot be nullptr in controller constructor!" };
}

void OpenglImguiController::onLeftMouseButton(InputState state) {
	if (state == currentStateLeftMouseButton_)
		return;
	switch (currentStateLeftMouseButton_ = state) {
	case InputState::down:
		spdlog::info("onLeftMouseButton pressed");
		break;

	case InputState::released:
		spdlog::info("onLeftMouseButton released");
		break;

	default:
	case InputState::undefined:
		spdlog::error("onLeftMouseButton undefined");
		throw std::exception{ "undefined state have been recieved in OpenglImguiController::onLeftMouseButton" };
	}
}

void OpenglImguiController::onWheelMouseButton(InputState state) {
	spdlog::info("onWheelMouseButton {}",
		(state == InputState::down) ? "pressed" :
		(state == InputState::released) ? "released" : "undefined");
}

void OpenglImguiController::onRightMouseButton(InputState state) {
	spdlog::info("onRightMouseButton {}",
		(state == InputState::down) ? "pressed" :
		(state == InputState::released) ? "released" : "undefined");
}

void OpenglImguiController::onMouseHover(InputState state, float x, float y) {
	spdlog::info("onMouseHover {}",
		(state == InputState::hovered) ? "hovered" :
		(state == InputState::unhovered) ? "unhovered" : "undefined");
}

void OpenglImguiController::onScroll(float momentWheel) {
	spdlog::info("onScroll {}, {}",
		(momentWheel > 0.0f) ? "up" :
		(momentWheel < 0.0f) ? "down" : "undefined",
		momentWheel);
}

void OpenglImguiController::addTriangleByCenter()
{
}

void OpenglImguiController::addTriangleByCorners()
{
}

void OpenglImguiController::addSquareByCenter()
{
}

void OpenglImguiController::addSquareByCorners()
{
}

void OpenglImguiController::addNgonByCenter()
{
}

void OpenglImguiController::addCircleByCenter()
{
}

void OpenglImguiController::removeFigure()
{
}
