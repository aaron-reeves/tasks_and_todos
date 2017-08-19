#include "cmainwindow.h"
#include <QApplication>

#include <QtCore>

#include <ar_general_purpose/qcout.h>
#include <ar_general_purpose/cfilelist2.h>

#include "nongui/ctask.h"


void extractTasks( const QString& filename, CTaskList* tasks, const bool rewriteFile = false ) {
  QFile data( filename );

  if( !data.open( QFile::ReadOnly | QFile::Text ) ) {
    // FIXME: This is an error.
  }
  else {
    QFileInfo fi( filename );
    QDate createdDate = fi.lastModified().date();
    QRegExp todo( "\\bTODO\\s?:.+$" );
    QString lines;

    QTextStream in( &data );

    while( !in.atEnd() ) {
      QString line = in.readLine();
      QString modLine = line.trimmed();

      if(-1 != todo.indexIn( modLine ) ) {
        modLine = modLine.mid( todo.indexIn( modLine ) );
        modLine.remove( QRegExp( "TODO\\s?:" ) );
        modLine = modLine.simplified().trimmed();

        CTask* task = new CTask( modLine, CTask::TodoTxt, filename );
        task->addFile( filename );
        task->setCreationDate( createdDate );
        tasks->extendedAppend( task );
      }
      else {
        lines.append( line );
      }
    }

    data.close();

    if( rewriteFile ) {
      QFile outFile( filename );
      if( !data.open( QFile::WriteOnly | QFile::Text | QFile::Truncate ) ) {
        // FIXME: This is an error.
      }
      else {
        QTextStream out( &outFile );
        for( int i = 0; i < lines.count(); ++i ) {
          out << lines.at(i) << endl;
        }

        outFile.close();
      }
    }
  }
}


int main(int argc, char *argv[]) {
//  CTaskList tasks;
//  CFileList files( "C:/Users/areeves/Google Drive/Dropbox/ResearchProjectDocs", "*.md", true );
//
//  foreach( QString filename, files ) {
//    extractTasks( filename, &tasks );
//  }
//
//  tasks.cout( CTask::Taskpaper );
//
//  return 0;

  QApplication a(argc, argv);
  CMainWindow w;

  w.show();

  return a.exec();
}
