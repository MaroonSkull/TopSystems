#pragma once
#include <string>
#include <memory>
#include <variant>
#include <list>

namespace model {

	struct Point {
		float x{ 0.f };
		float y{ 0.f };
		float z{ 0.f };
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

	class FlatFigures {
	public:
		// структура, которая хранит сервисные данные о конкретной фигуре
		template <class T>
		struct Figure : public T {
		private:
			inline static uint32_t counter_{ 0 };
		public:
			// const приводит к C2280 в MSVC, т.к. конструктор присваивания автоматически удаляется
			uint32_t id{ counter_++ };
			std::string name_{ T::name + " №" + id};
			T obj_; // да, это intrusive
		};

		using allFigures_t = std::variant<
			FlatFigures::Figure<Triangle>,
			FlatFigures::Figure<Quad>,
			FlatFigures::Figure<Circle>,
			FlatFigures::Figure<Ngon>,
			FlatFigures::Figure<CurveBezier3>,
			FlatFigures::Figure<CurveBezier4>
		>;

		FlatFigures() = default;
		~FlatFigures() = default;

		template <class T>
		void createFigure() {
			Figures_.push_back(Figure<T>);
		}
	private:
		std::list<allFigures_t> Figures_;

		class Memento {
			friend class FlatFigures;
		private:
			std::list<allFigures_t> Figures_;
		public:
			Memento(const std::list<allFigures_t>& Figures) : Figures_(Figures) {}
			~Memento() = default;
		};

		std::unique_ptr<Memento> createMemento() {
			return std::make_unique<Memento>(Memento(Figures_));
		}

		void restoreFromMemento(const Memento& Mem) {
			Figures_ = Mem.Figures_;
		}
	};

}

// наследовать всё от фигуры, полиморфизм вместо варианта
// вектор полиморфных указателей, тогда надо сильно меньше шаблонов, код проще