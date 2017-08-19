#include "cmytreewidget.h"

#include <QtCore>
#include <QWidget>
#include <QMouseEvent>
#include <QtGui>
#include <QMenu>
#include <QHeaderView>


#include "ctasktreewidgetitem.h"
#include "ctaskeditform.h"

#include "nongui/ctask.h"

CMyTreeWidget::CMyTreeWidget( QWidget* parent ) : QTreeWidget( parent ) {
  deleteTaskAction = new QAction( "Delete task", this );
  insertTaskAction = new QAction( "Insert task", this );
  editTaskAction = new QAction( "Edit task", this );

  connect( deleteTaskAction, SIGNAL( triggered() ), this, SLOT( deleteTask() ) );
  connect( insertTaskAction, SIGNAL( triggered() ), this, SLOT( insertTask() ) );
  connect( editTaskAction, SIGNAL( triggered() ), this, SLOT( editTask() ) );
}


void CMyTreeWidget::populateTasks( CTaskList* taskList ) {
  _tasks = taskList;

  this->clear();
  _items.clear();

  QStringList strs;

  QStringList projects = _tasks->projects().values();
  std::sort( projects.begin(), projects.end() );

  bool unsortedTaskFound = false;

  for( int i = 0; i < projects.count(); ++i ) {
    QString currentProject = projects.at(i);
    strs.clear();
    strs << currentProject;

    CTaskTreeWidgetItem* category = new CTaskTreeWidgetItem( (QTreeWidget*)0, strs );
    QFont f = category->font(0);
    f.setBold( true );
    category->setFont( 0, f );
    _items.append( category );

    for( int j = 0; j < _tasks->count(); ++j ) {
      if( _tasks->at(j)->projects().isEmpty() )
        unsortedTaskFound = true;

      if( _tasks->at(j)->projects().contains( currentProject ) ) {
        CTaskTreeWidgetItem* item = new CTaskTreeWidgetItem( category, _tasks->at(j) );
        _items.append( item );
        category->addChild( item );
      }
    }
  }

  if( unsortedTaskFound ) {
    strs.clear();
    strs << "Other tasks";

    CTaskTreeWidgetItem* category = new CTaskTreeWidgetItem( (QTreeWidget*)0, strs );
    QFont f = category->font(0);
    f.setBold( true );
    category->setFont( 0, f );
    _items.append( category );

    for( int j = 0; j < _tasks->count(); ++j ) {
      if( _tasks->at(j)->projects().isEmpty() ) {
        CTaskTreeWidgetItem* item = new CTaskTreeWidgetItem( category, _tasks->at(j) );
        _items.append( item );
        category->addChild( item );
      }
    }
  }

  this->insertTopLevelItems(0, _items);

  this->header()->resizeSection( CTaskTreeWidgetItem::colTask, 200 );
  this->expandAll();
}


void CMyTreeWidget::mouseMoveEvent( QMouseEvent* event ) {
  QTreeWidget::mouseMoveEvent( event );

  int minX = 0;
  int maxX = 0;

  for( int i = CTaskTreeWidgetItem::colTask; i < CTaskTreeWidgetItem::colViewEditNotes; ++i ) {
    minX = minX + this->columnWidth( i );
  }

  maxX = minX + this->columnWidth( CTaskTreeWidgetItem::colViewEditNotes );

  //qDebug() << minX << maxX << event->pos().x();

  CTaskTreeWidgetItem* ttItem = static_cast<CTaskTreeWidgetItem*>( this->itemAt( event->pos() ) );

  if( NULL != ttItem ) {
    CTask* task = ttItem->task();

    if(
      (event->pos().x() > minX)
      && (event->pos().x() < maxX)
      && ( NULL != task )
      && task->hasNotes()
    ) {
      this->setCursor( Qt::PointingHandCursor );
    }
    else
      this->setCursor( Qt::ArrowCursor );
  }
}


void CMyTreeWidget::catchItemClicked( QTreeWidgetItem* item, int column ) {
  if( CTaskTreeWidgetItem::colViewEditNotes == column ) {
    CTask* task = static_cast<CTaskTreeWidgetItem*>( item )->task();
    if( task->hasNotes() ) {
      qDebug() << "Show notes.";
    }
  }
}


void CMyTreeWidget::catchItemDoubleClicked( QTreeWidgetItem* item, int column ) {
  if( CTaskTreeWidgetItem::colViewEditNotes == column ) {
    CTask* task = static_cast<CTaskTreeWidgetItem*>( item )->task();
    if( task->hasNotes() ) {
      qDebug() << "Do nothing.";
    }
  }
  else {
    editTask();
  }
}


void CMyTreeWidget::contextMenuEvent(  QContextMenuEvent* event ) {
  CTaskTreeWidgetItem* ttItem = static_cast<CTaskTreeWidgetItem*>( this->itemAt( event->pos() ) );
  CTask* task = NULL;
  if( NULL != ttItem )
    task = ttItem->task();

  QMenu popup( this );
  popup.addAction( editTaskAction );
  editTaskAction->setEnabled( ( NULL != task ) );

  popup.addAction( deleteTaskAction );
  popup.addAction( insertTaskAction );
  popup.exec( event->globalPos() );
}


void CMyTreeWidget::deleteTask() {
  qDebug() << "Delete task";
}


void CMyTreeWidget::insertTask() {
  qDebug() << "Insert task";
}


void CMyTreeWidget::editTask() {
  CTaskTreeWidgetItem* ttItem = static_cast<CTaskTreeWidgetItem*>( this->currentItem() );
  CTask* task = NULL;
  if( NULL != ttItem )
    task = ttItem->task();

  if( NULL != task ) {
    CTaskEditForm frm( _tasks->projects().values(), task, this );
    if( QDialog::Accepted == frm.exec() ) {
      frm.updateTask( task );
      ttItem->format();
    }
  }
}

