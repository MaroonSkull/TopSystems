#pragma once
#include <spdlog/spdlog.h>
#include <imgui.h>
#include <IController.hpp>
#include <flatFigureModel.hpp>

// тут будет обработка всех действий пользователя, вызов модели
class OpenglImguiController final : public IController {
private:
	void tryCatchCallback();
public:
	OpenglImguiController(FlatFigureModel *);

	void onLeftMouseButton(); // обработка расположения фигур
	void onRightMouseButton(); // перемещение по пространству

	void addTriangleByCenter();
	void addTriangleByCorners();
	void addSquareByCenter();
	void addSquareByCorners();
	void addNgonByCenter();
	void addCircleByCenter();
	void removeFigure();
};