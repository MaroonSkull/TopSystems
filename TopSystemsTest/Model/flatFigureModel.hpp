#pragma once
#include <string>
#include <variant>
#include <vector>


class FlatFigureModel {
public:
	struct Point {
		float x{ 0.f };
		float y{ 0.f };
		float z{ 0.f };
	};

	// структура, которая хранит сервисные данные о конкретной фигуре
	template <class T>
	class Figure : public T {
	private:
		inline static uint32_t counter{ 0 };
	public:
		const uint32_t id{ counter++ };
		std::string name{ T.name + " №" + id};
	};

	struct Triangle {
		const static std::string name;
		Point first;
		Point second;
		Point third;
	};

	struct Quad {
		const static std::string name;
		Point first;
		Point second;
		Point third;
		Point fourth;
	};

	struct Circle {
		const static std::string name;
		Point center;
		float radius;
	};

	struct Ngon {
		const static std::string name;
		Point center;
		Point first;
		float n;
		float radius;
	};

	struct CurveBezier3 {
		const static std::string name;
		Point start;
		Point end;
		Point first;
	};

	struct CurveBezier4 {
		const static std::string name;
		Point start;
		Point end;
		Point first;
		Point second;
	};

	FlatFigureModel() = default;
	~FlatFigureModel() = default;

	template <class T>
	Figure<T> *createFigure() {
		Figures_.push_back(Figure<T>);
		return Figures_.;
	}
private:
	std::vector<
		std::variant<
			Triangle,
			Quad,
			Circle,
			Ngon,
			CurveBezier3,
			CurveBezier4
		>
	> Figures_;
};

// вектор указателей на вариант
// то же самое что лист по памяти

// наследовать всё от фигуры, полиморфизм вместо варианта
// вектор полиморфных указателей, тогда надо сильно меньше шаблонов, код проще