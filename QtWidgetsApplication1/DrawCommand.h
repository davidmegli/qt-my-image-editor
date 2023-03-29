#pragma once
#include <QPoint>

class DrawCommand
{
public:
	DrawCommand();
	virtual void execute(QPoint destPoint) = 0;
	virtual void execute() = 0;
	virtual void undo() = 0;
	virtual void redo() = 0;
	~DrawCommand();
private:

};

