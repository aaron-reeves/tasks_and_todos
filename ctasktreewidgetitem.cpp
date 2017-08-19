#include "ctasktreewidgetitem.h"

const QColor ColorPriorityA = QColor( 192, 0, 0 );
const QColor ColorPriorityB = QColor( 255, 153, 0 );
const QColor ColorPriorityC = QColor( 118, 147, 60 );
const QColor ColorPriorityD = QColor( 54, 96, 146 );
const QColor ColorOverdue = QColor( 242, 220, 219 );
const QColor ColorDueToday = QColor( 255, 255, 204 );
const QColor ColorDueSoon = QColor( 235, 241, 222 );


CTaskTreeWidgetItem::CTaskTreeWidgetItem( QTreeWidget* parent ) : QTreeWidgetItem( parent, QTreeWidgetItem::UserType ) {
  _task = NULL;
}

CTaskTreeWidgetItem::CTaskTreeWidgetItem( QTreeWidget* parent, QStringList& strs ) : QTreeWidgetItem( parent, strs, QTreeWidgetItem::UserType ) {
  _task = NULL;
}

CTaskTreeWidgetItem::CTaskTreeWidgetItem( QTreeWidgetItem* parent ) : QTreeWidgetItem( parent, QTreeWidgetItem::UserType ) {
  _task = NULL;
}

CTaskTreeWidgetItem::CTaskTreeWidgetItem( QTreeWidget* parent, CTask* task ) : QTreeWidgetItem( parent, QTreeWidgetItem::UserType ) {
  setTask( task );
}

CTaskTreeWidgetItem::CTaskTreeWidgetItem( QTreeWidgetItem* parent, CTask* task ) : QTreeWidgetItem( parent, QTreeWidgetItem::UserType ) {
  setTask( task );
}


CTaskTreeWidgetItem::~CTaskTreeWidgetItem() {
  // Do nothing here.
}


void CTaskTreeWidgetItem::setTask( CTask* task ) {
  _task = task;
  this->format();
}


void CTaskTreeWidgetItem::format() {
  if( NULL == _task )
    return;

  if( _task->completed() )
    this->setCheckState( colTask, Qt::Checked );
  else
    this->setCheckState( colTask, Qt::Unchecked );

  this->setFlags( this->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEditable );

  // Task text
  this->setText( colTask, _task->task() );

  // Task format
  // Set font back to its default...
  QFont f = this->font( colTask );
  f.setBold( false );
  f.setStyle( QFont::StyleNormal );
  f.setStrikeOut( _task->completed() );

  QColor textColor;

  if( _task->completed() )
    textColor = QColor( "black" );
  else {
    switch( _task->priority() ) {
      case CTask::A: textColor = ColorPriorityA; break;
      case CTask::B: textColor = ColorPriorityB; break;
      case CTask::C: textColor = ColorPriorityC; break;
      case CTask::D: textColor = ColorPriorityD; break;
      default: textColor = QColor( "black" ); break;
    }
  }

  this->setFont( colTask, f );
  this->setTextColor( colTask, textColor );

  if( _task->completed() ) {
    this->setText( colDueDate, "" );
    this->setText( colAlertDate, "" );
  }
  else {
    // Due date
    if( _task->due().isValid() ) {
      this->setText( colDueDate, _task->due().toString( "d MMM yyyy") );

      if( QDate::currentDate() > _task->due().date() )
        this->setTextColor( colDueDate, ColorPriorityA );
      else if( QDate::currentDate() == _task->due().date() )
        this->setTextColor( colDueDate, ColorPriorityB );
      else if( QDate::currentDate().addDays( 7 ) > _task->due().date() )
        this->setTextColor( colDueDate, ColorPriorityB );
      else
        this->setTextColor( colDueDate, QColor( "black" ) );
    }

    // Alert date
    if( _task->alert().isValid() )
      this->setText( colAlertDate, _task->alert().toString( "d MMM yyyy h:mm AP"));
  }

  if( _task->hasNotes() ) {
    this->setText( colViewEditNotes, "view notes" );

    QFont f = this->font(0);
    f.setBold( false );
    f.setStyle( QFont::StyleNormal );
    f.setUnderline( true );
    f.setStrikeOut( false );
    this->setFont( colViewEditNotes, f );
    this->setTextColor( colViewEditNotes, QColor( "blue" ) );
  }
  else {
    // Restore default
    this->setText( colViewEditNotes, "" );

    QFont f = this->font(0);
    f.setBold( false );
    f.setStyle( QFont::StyleNormal );
    f.setUnderline( false );
    this->setFont( colViewEditNotes, f );
    this->setTextColor( colViewEditNotes, QColor( "black" ) );
  }
}


void CTaskTreeWidgetItem::viewEditNotes() {

}


void CTaskTreeWidgetItem::dueDateTimeChanged( QDateTime dt ) {
  if( NULL != _task )
    _task->setDue( dt );
}


void CTaskTreeWidgetItem::alertDateTimeChanged( QDateTime dt ) {
  if( NULL != _task )
    _task->setAlert( dt );
}



