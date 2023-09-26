#pragma once

class IController {
public:
	enum class state {
		none,
		triangleByCenter_centerSelected,
		triangleByCorners_firstCornerSelected,
		triangleByCorners_secondCornerSelected,
		squareByCenter_centerSelected,
		squareByCorners_firstCornerSelected,
		squareByCorners_secondCornerSelected,
		ngonByCenter_centerSelected,
		circleByCenter_centerSelected,
		// кривые тут же
	};
	virtual void onLeftMouseButton() = 0;
	virtual void onRightMouseButton() = 0;

	virtual void addTriangleByCenter() = 0;
	virtual void addTriangleByCorners() = 0;
	virtual void addSquareByCenter() = 0;
	virtual void addSquareByCorners() = 0;
	virtual void addNgonByCenter() = 0;
	virtual void addCircleByCenter() = 0;
	virtual void removeFigure() = 0;        
};