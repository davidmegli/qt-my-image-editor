#include "CommandManager.h"

CommandManager::CommandManager()
{
}

void CommandManager::executeCommand(shared_ptr<DrawCommand> command, QPoint lastPoint)
{
	qDebug() << "CommandManager::executeCommand";
	command->execute(lastPoint);
	undoCommands.push_back(command);
	redoCommands.clear();
}

bool CommandManager::canUndo()
{
	return !undoCommands.empty();
}

void CommandManager::undoCommand()
{
	qDebug() << "CommandManager::undoCommand";
	if (canUndo())
	{
		shared_ptr<DrawCommand> command = undoCommands.back();
		command->undo();
		undoCommands.pop_back();
		redoCommands.push_back(command);
	}
}

bool CommandManager::canRedo()
{
	return !redoCommands.empty();
}

void CommandManager::redoCommand()
{
	qDebug() << "CommandManager::redoCommand";
	if (canRedo())
	{
		shared_ptr<DrawCommand> command = redoCommands.back();
		command->execute();
		redoCommands.pop_back();
		undoCommands.push_back(command);
	}
}

CommandManager::~CommandManager()
{
}
