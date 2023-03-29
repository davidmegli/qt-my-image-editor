#pragma once

#include "DrawCommand.h"
#include <vector>
#include <memory>
#include <QPoint>
#include <QDebug>

using namespace std;

class CommandManager
{
public:
	CommandManager();
	void executeCommand(shared_ptr<DrawCommand> command, QPoint lastPoint);
	bool canUndo();
	void undoCommand();
	bool canRedo();
	void redoCommand();
	~CommandManager();

private:
	std::vector<shared_ptr<DrawCommand>> undoCommands;
	std::vector<shared_ptr<DrawCommand>> redoCommands;
};

