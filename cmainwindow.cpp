#include "cmainwindow.h"
#include "ui_cmainwindow.h"

#include <QtCore>
#include <QFileDialog>
#include <QDateTimeEdit>

#include "ctasktreewidgetitem.h"

#include "nongui/ctask.h"

// Adapted from https://stackoverflow.com/questions/28829192/how-to-get-the-number-of-items-of-a-qtreewidget
int treeCount(QTreeWidget *tree, QTreeWidgetItem *parent = 0) {
  int count = 0;
  if (parent == 0) {
    int topCount = tree->topLevelItemCount();
    for (int i = 0; i < topCount; i++) {
      QTreeWidgetItem *item = tree->topLevelItem(i);
      //if (item->isExpanded()) {
        count += treeCount(tree, item);
      //}
    }
    count += topCount;
  }
  else {
    int childCount = parent->childCount();
    for (int i = 0; i < childCount; i++) {
      QTreeWidgetItem *item = parent->child(i);
      //if (item->isExpanded()) {
        count += treeCount(tree, item);
      //}
    }
    count += childCount;
  }
  return count;
}


CMainWindow::CMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::CMainWindow) {
  _tasks = NULL;
  _populatingForm = false;

  ui->setupUi(this);
}


CMainWindow::~CMainWindow() {
  delete _tasks;

  delete ui;
}


void CMainWindow::updateWidgets( QTreeWidgetItem* item, int column ) {
  if( !_populatingForm ) {
    _populatingForm = true;

    Q_UNUSED( column );

    qDebug() << "CMainWindow::updateWidgets()";

    CTaskTreeWidgetItem* ttItem = static_cast<CTaskTreeWidgetItem*>( item );

    ttItem->task()->setCompleted( ttItem->checkState( CTaskTreeWidgetItem::colTask ) );

    ttItem->format();

    _populatingForm = false;
  }
}


void CMainWindow::btnTest() {
  if( ( NULL == _tasks ) || _tasks->isEmpty() ) {
    _tasks = new CTaskList( "C:/Users/areeves/Documents/Programming/tasks_and_todos/sampleDataFiles/todo.txt", CTask::TodoTxt );

    populateItems();
  }
}


void CMainWindow::actionNew() {

}


void CMainWindow::actionOpenFile() {
//  QFileDialog dialog( this );
//  dialog.setFileMode( QFileDialog::ExistingFile );
//  dialog.setNameFilter( "todo.txt files (*.txt);;Taskpaper files (*.taskpaper)" );
//  dialog.setViewMode( QFileDialog::Detail );

  QString filename = QFileDialog::getOpenFileName( this, "Open file", QString(), "Todo or Taskpaper files (*.txt *.taskpaper);;Any files (*.*)" );

  if( filename.isEmpty() )
    return;

  CTask::TaskFormat fmt = CTask::UnspecifiedFormat;

  if( filename.endsWith( ".txt" ) )
    fmt = CTask::TodoTxt;
  else if( filename.endsWith( ".taskpaper" ) )
    fmt = CTask::Taskpaper;

  if( CTask::UnspecifiedFormat != fmt ) {
    _tasks = new CTaskList( filename, fmt );
    this->populateItems();
  }
}


void CMainWindow::actionOpenFileset() {
  QStringList filenames = QFileDialog::getOpenFileNames( this, "Open file", QString(), "Todo or Taskpaper files (*.txt *.taskpaper);;All files (*.*)" );

  qDebug() << filenames;
}


void CMainWindow::populateItems() {
  _populatingForm = true;

  qDebug() << "I'm populating.";

  ui->treeWidget->populateTasks( _tasks );

  _populatingForm = false;
}


void CMainWindow::actionSave() {

}


void CMainWindow::actionSaveAs() {

}


void CMainWindow::actionClose() {

}


void CMainWindow::actionExit() {
  this->close();
}
