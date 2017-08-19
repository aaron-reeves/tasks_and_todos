#ifndef CMYTREEWIDGET_H
#define CMYTREEWIDGET_H

#include <QTreeWidget>

#include <QObject>
#include <QAction>

#include "nongui/ctask.h"

class CMyTreeWidget : public QTreeWidget  {
  Q_OBJECT

  public:
    CMyTreeWidget( QWidget* parent );

    void populateTasks( CTaskList* taskList );

  protected slots:
    void catchItemClicked( QTreeWidgetItem* item, int column );
    void catchItemDoubleClicked( QTreeWidgetItem* item, int column );

    void deleteTask();
    void insertTask();
    void editTask();

  protected:
    virtual void mouseMoveEvent( QMouseEvent* event );
    virtual void contextMenuEvent(  QContextMenuEvent* event );

    QAction* deleteTaskAction;
    QAction* insertTaskAction;
    QAction* editTaskAction;

    QList<QTreeWidgetItem*> _items;
    CTaskList* _tasks;
};

#endif // CMYTREEWIDGET_H
