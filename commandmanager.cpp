#include "commandmanager.h"

CommandManager::CommandManager()
{
}

void CommandManager::executeCommand(shared_ptr<DrawCommand> command, QPoint lastPoint)
{
    qDebug() << "CommandManager::executeCommand";
    command->execute(lastPoint);
    if (canUndo() && undoCommands.back()->isCollapsible(command))
    {
        qDebug() << "CommandManager::executeCommand - isCollapsible";
        undoCommands.back()->collapse(command);
    }
    else
    {
        qDebug() << "CommandManager::executeCommand - !isCollapsible";
        undoCommands.push_back(command->clone());
    }
    //FIXME: Fa l'undo solo dell'ultimo tratto, forse perchè nel mousePressEvent, quando crea un nuovo
    //shared_ptr, viene eliminato il riferimento al vecchio shared_ptr. Questo vuol dire che la copia non è stata effettuata correttamente nel Vector
    //non incrementando il contatore. Per risolvere il problema, ho aggiunto il metodo clone() in DrawCommand.h e DrawFreeHandCommand.h
    //ma il problema non è ancora risolto


    //Update: undo annulla tutto l'ultimo tratto, redo ripristina piccoli tratti alla volta
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
        qDebug() << "CommandManager::undoCommand - canUndo";
        shared_ptr<DrawCommand> command = undoCommands.back();
        command->undo();
        undoCommands.pop_back();
        redoCommands.push_back(command->clone());
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
        qDebug() << "CommandManager::redoCommand - canRedo";
        shared_ptr<DrawCommand> command = redoCommands.back();
        command->redo();
        redoCommands.pop_back();
        undoCommands.push_back(command->clone());
    }
}

CommandManager::~CommandManager()
{
}
