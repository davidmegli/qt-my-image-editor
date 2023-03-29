#pragma once
#include <QPoint>
#include <memory>

class DrawCommand
{
public:
	DrawCommand();
	virtual void execute(QPoint destPoint) = 0;
	virtual void execute() = 0;
	virtual void undo() = 0;
	virtual void redo() = 0;
	virtual std::shared_ptr<DrawCommand> clone() const = 0;
	~DrawCommand();
private:

};

