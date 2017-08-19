#include "ctaskeditform.h"
#include "ui_ctaskeditform.h"

#include "ctasktreewidgetitem.h"

CTaskEditForm::CTaskEditForm( QStringList projects, CTask* task, QWidget *parent ) :  QDialog( parent ), ui( new Ui::CTaskEditForm ) {
  ui->setupUi(this);
  QPixmap pix( 16, 16 );

  std::sort( projects.begin(), projects.end() );
  QListWidgetItem* listItem;
  for( int i = 0; i < projects.count(); ++i ) {
    listItem = new QListWidgetItem( projects.at(i), ui->listProjects );
    ui->listProjects->insertItem( i, listItem );
  }

  ui->cboPriority->addItem( "Unspecified priority", CTask::UnspecifiedPriority );
  pix.fill( QColor( 200, 200, 200 ) );
  ui->cboPriority->setItemIcon( 0, QIcon( pix ) );

  ui->cboPriority->addItem( "Highest priority (A)", CTask::A );
  pix.fill( ColorPriorityA );
  ui->cboPriority->setItemIcon( 1, QIcon( pix ) );

  ui->cboPriority->addItem( "High priority (B)", CTask::B );
  pix.fill( ColorPriorityB );
  ui->cboPriority->setItemIcon( 2, QIcon( pix ) );

  ui->cboPriority->addItem( "Medium priority (C)", CTask::C );
  pix.fill( ColorPriorityC );
  ui->cboPriority->setItemIcon( 3, QIcon( pix ) );

  ui->cboPriority->addItem( "Priority (D)", CTask::D );
  pix.fill( ColorPriorityD );
  ui->cboPriority->setItemIcon( 4, QIcon( pix ) );

  ui->cboPriority->addItem( "Not a priority", CTask::E );
  pix.fill( QColor( "white" ) );
  ui->cboPriority->setItemIcon( 5, QIcon( pix ) );

  setupTask( task );
}


CTaskEditForm::~CTaskEditForm() {
  delete ui;
}


void CTaskEditForm::setupTask( CTask* task ) {
  ui->cbxTaskComplete->setChecked( task->completed() );
  ui->teTask->setPlainText( task->task() );
  ui->teNotes->setPlainText( task->notes().join( '\n' ) );

  if( task->due().isValid() ) {
    ui->dteDueDateTime->setDateTime( task->due() );
    ui->dteDueDateTime->setVisible( true );
    ui->btnSetDueDateTime->setText( "Clear due date" );
  }
  else {
    ui->dteDueDateTime->setVisible( false );
    ui->btnSetDueDateTime->setText( "Set due date" );
  }

  if( task->alert().isValid() ) {
    ui->dteAlertDateTime->setDateTime( task->alert() );
    ui->dteAlertDateTime->setVisible( true );
    ui->btnSetAlertDateTime->setText( "Clear alert" );
  }
  else {
    ui->dteAlertDateTime->setVisible( false );
    ui->btnSetAlertDateTime->setText( "Set alert" );
  }

  ui->cboPriority->setCurrentIndex( qMin( task->priority(), CTask::E ) );

  for( int i = 0; i < ui->listProjects->count(); ++i ) {
    ui->listProjects->item(i)->setSelected( task->projects().contains( ui->listProjects->item( i )->text() ) );
  }
}


void CTaskEditForm::updateTask( CTask* task ) {
  task->setCompleted( Qt::Checked == ui->cbxTaskComplete->checkState() );

  task->setTask( ui->teTask->toPlainText().trimmed() );

  if( ui->teNotes->toPlainText().trimmed().isEmpty() ) {
    if( !task->notes().isEmpty() )
      task->clearNotes();
  }
  else {
    task->setNotes( ui->teNotes->toPlainText().split( '\n' ) );
  }

  task->setPriority( CTask::TaskPriority( ui->cboPriority->currentData().toInt() ) );

  QDateTime newDueDateTime = ui->dteDueDateTime->dateTime();
  QDateTime newAlertDateTime = ui->dteAlertDateTime->dateTime();
  qDebug() << newDueDateTime << newAlertDateTime;

  if( ui->dteDueDateTime->isVisibleTo( this ) )
    task->setDue( newDueDateTime );
  else {
    qDebug() << "This is the wrong trigger!";
    task->setDue( QDateTime() );
  }

  if( ui->dteAlertDateTime->isVisibleTo( this ) )
    task->setAlert( newAlertDateTime );
  else
    task->setAlert( QDateTime() );

  for( int i = 0; i < ui->listProjects->count(); ++i ) {
    if( ui->listProjects->item(i)->isSelected() )
      task->addProject( ui->listProjects->item( i )->text() );
    else
      task->removeProject( ui->listProjects->item( i )->text() );
  }
}


void CTaskEditForm::showHideDueDateTime() {
  ui->dteDueDateTime->setVisible( !ui->dteDueDateTime->isVisible() );

  if( ui->dteDueDateTime->isVisible() ) {
    if( ui->dteDueDateTime->dateTime() == QDateTime::fromString( "2000-01-01 00:00:00", "yyyy-MM-dd hh:mm:ss" ) )
      ui->dteDueDateTime->setDateTime( QDateTime::currentDateTime() );

    ui->btnSetDueDateTime->setText( "Clear due date" );
  }
  else
    ui->btnSetDueDateTime->setText( "Set due date" );
}


void CTaskEditForm::showHideAlertDateTime() {
  ui->dteAlertDateTime->setVisible( !ui->dteAlertDateTime->isVisible() );

  if( ui->dteAlertDateTime->isVisible() ) {
    if( ui->dteAlertDateTime->dateTime() == QDateTime::fromString( "2000-01-01 00:00:00", "yyyy-MM-dd hh:mm:ss" ) )
      ui->dteAlertDateTime->setDateTime( QDateTime::currentDateTime() );

    ui->btnSetAlertDateTime->setText( "Clear alert date" );
  }
  else
    ui->btnSetAlertDateTime->setText( "Set alert date" );
}
