#include <QCoreApplication>

#include <QtCore>

#include <ar_general_purpose/qcout.h>
#include <ar_general_purpose/cfilelist2.h>

#include "ctask.h"


void extractTasks( const QString& filename, CTaskList* tasks ) {
  QFileInfo fi( filename );
  QDate createdDate = fi.lastModified().date();
  QRegExp todo( "\\bTODO\\s?:.+$" );

  QFile data( filename );

  if( data.open( QFile::ReadOnly | QFile::Text ) ) {
    QTextStream in( &data );
    int counter = 0;
    while( !in.atEnd() ) {
      QString line = in.readLine().trimmed();

      if(-1 != todo.indexIn( line ) ) {
        line = line.mid( todo.indexIn( line ) );
        line.remove( QRegExp( "TODO\\s?:" ) );
        line = line.simplified().trimmed();

        CTask* task = new CTask( line, CTask::TodoTxt );
        task->addFile( filename );
        task->setCreationDate( createdDate );
        tasks->append( task );
      }

      ++counter;
    }
  }

}


int main(int argc, char *argv[]) {
//  QCoreApplication a(argc, argv);

//  return a.exec();

//  CTask task( test, CTask::TodoTxt );

//  task.debug();

//  return 0;

//  CTaskList list( "C:/Users/areeves/Documents/Programming/tasks_and_todos/sampleDataFiles/todo.txt", CTask::TodoTxt );

//  list.cout( CTask::TodoTxt );
//  cout << endl << endl;
//  list.cout( CTask::Taskpaper );

//  cout << endl << endl;
//  CTaskList list( "C:/Users/areeves/Documents/Programming/tasks_and_todos/sampleDataFiles/tasks.taskpaper", CTask::Taskpaper );
//  list.cout( CTask::TodoTxt );

  CTaskList tasks;
  CFileList files( "C:/Users/areeves/Google Drive/Dropbox/ResearchProjectDocs", "*.md", true );

  foreach( QString filename, files ) {
    extractTasks( filename, &tasks );
  }

  tasks.cout( CTask::Taskpaper );

  return 0;
}
