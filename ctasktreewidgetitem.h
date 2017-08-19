#ifndef CTASKTREEWIDGETITEM_H
#define CTASKTREEWIDGETITEM_H

#include <QObject>
#include <QTreeWidgetItem>

#include "nongui/ctask.h"

extern const QColor ColorPriorityA;
extern const QColor ColorPriorityB;
extern const QColor ColorPriorityC;
extern const QColor ColorPriorityD;
extern const QColor ColorOverdue;
extern const QColor ColorDueToday;
extern const QColor ColorDueSoon;

class CTaskTreeWidgetItem : public QObject, public QTreeWidgetItem {
  Q_OBJECT

  public:
    static const int colTask = 0;
    static const int colDueDate = 1;
    static const int colAlertDate = 2;
    static const int colViewEditNotes = 3;

    CTaskTreeWidgetItem( QTreeWidget* parent );
    CTaskTreeWidgetItem( QTreeWidget* parent, QStringList& strs );
    CTaskTreeWidgetItem( QTreeWidget* parent, CTask* task );
    CTaskTreeWidgetItem( QTreeWidgetItem* parent );
    CTaskTreeWidgetItem( QTreeWidgetItem* parent, CTask* task );
    ~CTaskTreeWidgetItem();

    void format();

    CTask* task() const { return _task; }

  public slots:
    void viewEditNotes();
    void dueDateTimeChanged( QDateTime dt );
    void alertDateTimeChanged( QDateTime dt );

  protected:
    void setTask( CTask* task );

    CTask* _task;
};

#endif // CTASKTREEWIDGETITEM_H

