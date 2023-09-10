#pragma once

class IView {
public:
	IView();
	virtual ~IView();

	virtual bool init();

};