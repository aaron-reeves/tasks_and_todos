#include "ctask.h"

#include <ar_general_purpose/qcout.h>
#include <ar_general_purpose/strutils.h>

CTask::CTask() {
  initialize();
}


CTask::CTask( const QString& str, const CTask::TaskFormat fmt, const int orderInFile /* = -1 */ ) {
  initialize();

  setOrderInFile( orderInFile );

  switch( fmt ) {
    case TodoTxt:
      parseTodoTxt( str );
      break;
    case Taskpaper:
      parseTaskpaper( str );
      break;
    default:
      qDebug() << "FIXME: This is a problem.";
      break;
  }
}


CTask::CTask( const CTask& other ) {
  _task = other._task;
  _priority = other._priority;
  _creationDate = other._creationDate;
  _completed = other._completed;
  _projects = other._projects;
  _contexts = other._contexts;
  _due = other._due;
  _threshold = other._threshold;
  _files = other._files;
  _notes = other._notes;
  _alert = other._alert;

  _orderInFile = other._orderInFile;
  _isModified = other._isModified;
}


CTask::~CTask() {
  // Nothing do do here.
}


void CTask::initialize() {
  // Use most Qt default values

  _orderInFile = -1;
  _priority = UnspecifiedPriority;
  _completed = false;
  _isModified = false;
}


/*
2017-07-31 See *.taskpaper documents
(A) 2017-07-31 A new todo item with high priority, a @contextTag, a project name, and a due date due:2017-08-02 +ProjectName
x 2017-07-31 Another new todo item but with low priority, a @contextTag, a project name, and a due date due:2017-08-02 +ProjectName alert:2017-08-02_08:15:00  note:"What is a threshold date?" t:2017-07-31
*/
void CTask::parseTodoTxt( QString str ) {
  str = str.trimmed();

  // Pull out completed flag or priority (located at the beginning of the string)
  QRegExp completed( "^[Xx]\\b" );
  QRegExp priority( "^[(][A-Za-z][)]\\s+" );

  if( 0 == completed.indexIn( str ) ) {
    setCompleted( true );
    str.remove( completed );
  }

  if( 0 == priority.indexIn( str ) ) {
    setPriority( priority.capturedTexts().at(0).trimmed().toUpper() );
    str.remove( priority );
  }

  str = str.trimmed();

  // Pull out creation date (at the start of the string)
  QRegExp creationDate( "^[0-9]{4}-[0-9]{2}-[0-9]{2}\\b" );
  if( 0 == creationDate.indexIn( str ) ) {
    setCreationDate( creationDate.capturedTexts().at(0).trimmed() );
    str.remove( creationDate );
  }

  // Pull out the threshold date
  QRegExp tdate( "\\bt:[0-9]{4}-[0-9]{2}-[0-9]{2}\\b" );
  if( -1 != tdate.indexIn( str ) ) {
    setThreshold( tdate.capturedTexts().at(0).trimmed().split( ':' ).at(1).trimmed() );
    str.remove( tdate );
  }

  // Pull out the completion date
  QRegExp completedDate( "\\bcompleted:[0-9]{4}-[0-9]{2}-[0-9]{2}\\b" );
  if( -1 != completedDate.indexIn( str ) ) {
    setCompletedDate( completedDate.capturedTexts().at(0).trimmed().split( ':' ).at(1).trimmed() );
    setCompleted( true );
    str.remove( completedDate );
  }

  // Pull out the due date or datetime
  QRegExp due( "\\bdue:[0-9]{4}-[0-9]{2}-[0-9]{2}(_[0-9]{2}:[0-9]{2}:[0-9]{2})?\\b" );
  if( -1 != due.indexIn( str ) ) {
    setDue( due.capturedTexts().at(0).trimmed().split( "e:" ).at(1).trimmed() );
    str.remove( due );
  }

  // Pull out the alarm datetime
  QRegExp alert( "\\balert:[0-9]{4}-[0-9]{2}-[0-9]{2}(_[0-9]{2}:[0-9]{2}:[0-9]{2})?\\b" );
  if( -1 != alert.indexIn( str ) ) {
    setAlert( alert.capturedTexts().at(0).trimmed().split( "rt:" ).at(1).trimmed() );
    str.remove( alert );
  }

  // Find all project names and context tags.
  // Word boundaries don't seem to work with + or @, so this is a bit more involved...
  QRegExp project( "^[+]{1}[A-Za-z0-9]+$" );
  QRegExp context( "^[@]{1}[A-Za-z0-9]+$" );
  QStringList parts = str.split( QRegExp( "[^\\w+@]+" ) );

  foreach( QString part, parts ) {
    if( project.exactMatch( part ) ) {
      _projects.append( part.right( part.length() - 1 ) );

      // Remove the project from the string.
      str.remove( part );
    }

    if( context.exactMatch( part ) )
      _contexts.append( part.right( part.length() - 1 ) );
  }

  // Pull out all files
  QRegExp file( "\\bfile:\"[^\"]+\"" );
  if( -1 != file.indexIn( str ) ) {
    foreach( QString cap, file.capturedTexts() ) {
      str.remove( cap );
      _files.append( cap.remove( "file:" ).remove( "\"" ) );
    }
  }

  // Pull out all notes
  QRegExp note( "\\bnote:\"[^\"]+\"" );
  if( -1 != note.indexIn( str ) ) {
    foreach( QString cap, note.capturedTexts() ) {
      str.remove( cap );
      _notes.append( cap.remove( "note:" ).remove( "\"" ) );
    }
  }

  // Whatever's left is the task itself.
  setTask( str.simplified().trimmed() );

  _isModified = false;
}


void CTask::setPriority(const QString& str ) {
  if( str.isEmpty() )
    setPriority( UnspecifiedPriority );
  else {
    int c =  str.at(1).toLatin1();

    if( (c >= 65) && (c <= 90) )
      setPriority( TaskPriority( c - 64 ) );
    else
      setPriority( UnspecifiedPriority );
  }
}


void CTask::setCreationDate( const QString& str ) {
  setCreationDate( QDate::fromString( str, "yyyy-MM-dd" ) );
}


void CTask::setThreshold(const QString& str ) {
  setThreshold( QDate::fromString( str, "yyyy-MM-dd" ) );
}

void CTask::setCompletedDate(const QString& str ) {
  setCompletedDate( QDate::fromString( str, "yyyy-MM-dd" ) );
}


void CTask::setDue( const QString& str ) {
  if( QRegExp( "^[0-9]{4}-[0-9]{2}-[0-9]{2}$" ).exactMatch( str ) )
    setDue( QDateTime::fromString( str, "yyyy-MM-dd" ) );
  else if( QRegExp( "^[0-9]{4}-[0-9]{2}-[0-9]{2}_[0-9]{2}:[0-9]{2}:[0-9]{2}$" ).exactMatch( str ) )
    setDue( QDateTime::fromString( str, "yyyy-MM-dd_HH:mm:ss") );
  else
    setDue( QDateTime() );
}


void CTask::setAlert( const QString& str ) {
  if( QRegExp( "^[0-9]{4}-[0-9]{2}-[0-9]{2}$" ).exactMatch( str ) )
    setAlert( QDateTime::fromString( str, "yyyy-MM-dd" ) );
  else if( QRegExp( "^[0-9]{4}-[0-9]{2}-[0-9]{2}_[0-9]{2}:[0-9]{2}:[0-9]{2}$" ).exactMatch( str ) )
    setAlert( QDateTime::fromString( str, "yyyy-MM-dd_HH:mm:ss") );
  else
    setAlert( QDateTime() );
}


QDate CTask::extractDate( QString str ) const {
  int len = str.indexOf( ')' ) - str.indexOf( '(' ) - 1;
  str = str.mid( str.indexOf( '(' ) + 1, len );
  return QDate::fromString( str, "yyyy-MM-dd" );
}


QDateTime CTask::extractDateTime( QString str ) const {
  int len = str.indexOf( ')' ) - str.indexOf( '(' ) - 1;
  str = str.mid( str.indexOf( '(' ) + 1, len );
  return QDateTime::fromString( str, "yyyy-MM-dd_hh:mm:ss" );
}


QString CTask::extractString( QString str ) const {
  int len = str.indexOf( ')' ) - str.indexOf( '(' ) - 1;
  str = str.mid( str.indexOf( '(' ) + 1, len );
  return str;
}


void CTask::parseTaskpaper( QString str ) {
  str = str.right( str.length() - 1 ).simplified().trimmed();
  QRegExp re;

  // Pull out creation date
  re = QRegExp( "\\B@created[(][0-9]{4}-[0-9]{2}-[0-9]{2}[)]\\B" );
  if( -1 != re.indexIn( str ) ) {
    setCreationDate( extractDate( re.capturedTexts().at(0) ) );
    str.remove( re );
  }

  // Pull out threshold date
  re = QRegExp( "\\B@threshold[(][0-9]{4}-[0-9]{2}-[0-9]{2}[)]\\B" );
  if( -1 != re.indexIn( str ) ) {
    setThreshold( extractDate( re.capturedTexts().at(0) ) );
    str.remove( re );
  }

  // Pull out completion date
  re = QRegExp( "\\B@done[(][0-9]{4}-[0-9]{2}-[0-9]{2}[)]\\B" );
  if( -1 != re.indexIn( str ) ) {
    setCompletedDate( extractDate( re.capturedTexts().at(0) ) );
    setCompleted( true );
    str.remove( re );
  }

  // Pull out completed
  re = QRegExp( "\\B@done\\B" );
  if( -1 != re.indexIn( str ) ) {
    setCompleted( true );
    str.remove( re );
  }

  // Pull out alert datetime
  re = QRegExp( "\\B@alert[(][0-9]{4}-[0-9]{2}-[0-9]{2}(_[0-9]{2}:[0-9]{2}:[0-9]{2})?[)]\\B" );
  if( -1 != re.indexIn( str ) ) {
    setAlert( extractDateTime( re.capturedTexts().at(0) ) );
    str.remove( re );
  }

  // Pull out due datetime
  re = QRegExp( "\\B@due[(][0-9]{4}-[0-9]{2}-[0-9]{2}(_[0-9]{2}:[0-9]{2}:[0-9]{2})?[)]\\B" );
  if( -1 != re.indexIn( str ) ) {
    setDue( extractDateTime( re.capturedTexts().at(0) ) );
    str.remove( re );
  }

  // Pull out priority
  re = QRegExp( "\\B@priority[(][A-Za-z][)]\\B" );
  if( -1 != re.indexIn( str ) ) {
    setPriority( re.capturedTexts().at(0).at(10) );
    str.remove( re );
  }

  // Pull out all files
  re = QRegExp( "\\B@file[(][^()]+[)]\\B" );
  if( -1 != re.indexIn( str ) ) {
    foreach( QString cap, re.capturedTexts() ) {
      str.remove( cap );
      _files.append( extractString( cap ) );
    }
  }

  // Find all context tags.
  // At this point, anything left with an @ should be a context tag.
  QRegExp context( "^[@]{1}[A-Za-z0-9]+$" );
  QStringList parts = str.split( QRegExp( "[^\\w@]+" ) );

  foreach( QString part, parts ) {
    if( context.exactMatch( part ) )
      _contexts.append( part.right( part.length() - 1 ) );
  }

  setTask( str.simplified().trimmed() );
}


void CTask::setProject( const QString& str ) {
  _projects.clear();

  if( !str.isEmpty() )
    _projects.append( str );

  _isModified = true;
}


QString CTask::project() const {
  if( projects().isEmpty() )
    return QString();
  else
    return projects().at(0);
}


bool CTask::isValid() const {
  qDebug() << "FIXME: Write this function.";
  return true;
}


QString CTask::priorityToString( const TaskPriority priority ) {
  if( UnspecifiedPriority == priority )
    return "Unspecified";
  else {
    char c = int( priority ) + 64;
    return QString( c );
  }
}


/*
2017-07-31 See *.taskpaper documents
(A) 2017-07-31 A new todo item with high priority, a @contextTag, a project name, and a due date due:2017-08-02 +ProjectName
x 2017-07-31 Another new todo item but with low priority, a @contextTag, a project name, and a due date due:2017-08-02 +ProjectName alert:2017-08-02_08:15:00  note:"What is a threshold date?" t:2017-07-31
*/
QString CTask::asString( const CTask::TaskFormat fmt ) const {
  QString result;

  switch( fmt ) {
    case TodoTxt:
      result = asTodoTxtString();
      break;
    case Taskpaper:
      result = asTaskpaperString();
      break;
    default:
      qDebug() << "FIXME: This is a problem.";
      break;
  }

  return result;
}


QString CTask::asTodoTxtString() const {
  QString result;

  if( completed() ) {
    result.append( "x " );

    if( completedDate().isValid() ) {
      result.append( QString( "completed:%1 ").arg( completedDate().toString( "yyyy-MM-dd" ) ) );
    }
  }
  else if( UnspecifiedPriority != priority() ) {
    result.append( QString( "(%1) " ).arg( priorityToString( priority() ) ) );
  }

  if( creationDate().isValid() )
    result.append( creationDate().toString( "yyyy-MM-dd" ) ).append( " " );

  result.append( task() ).append( " " );

  foreach( QString note, notes() )
    result.append( QString( "note:\"%1\" " ).arg( note )  );

  foreach( QString file, files() )
    result.append( QString( "file:\"%1\" " ).arg( file )  );

  if( threshold().isValid() )
    result.append( QString( "t:%1 ").arg( threshold().toString( "yyyy-MM-dd" ) ) );

  if( due().isValid() )
    result.append( QString( "due:%1 ").arg( due().toString( "yyyy-MM-dd_HH:mm:ss" ) ) );

  if( alert().isValid() )
    result.append( QString( "alert:%1 ").arg( alert().toString( "yyyy-MM-dd_HH:mm:ss" ) ) );

  foreach( QString project, projects() ) {
    project = QString( "+%1" ).arg( camelCase( project ) );
    result.append( project  ).append( " " );
  }

  return result.trimmed();
}


/*
 * 2017-07-31 See *.taskpaper documents
(A) 2017-07-31 A new todo item with high priority, a @contextTag, a project name, and a due date due:2017-08-02_00:00:00 +ProjectName
x 2017-07-31 Another new todo item but with low priority, a @contextTag, a project name, and a due date note:"What is a threshold date?" t:2017-07-31 due:2017-08-02_00:00:00 alert:2017-08-02_08:15:00 +ProjectName
*/
QString CTask::asTaskpaperString() const {
  QString result = QString( "- %1 " ).arg( task() );

  if( priority() != UnspecifiedPriority )
    result.append( QString( "@priority(%1) " ).arg( priorityToString( priority() ) ) );

  if( creationDate().isValid() )
    result.append( QString( "@created(%1) " ).arg( creationDate().toString( "yyyy-MM-dd" ) ) );

  if( threshold().isValid() )
    result.append( QString( "@threshold(%1) ").arg( threshold().toString( "yyyy-MM-dd" ) ) );

  if( due().isValid() )
    result.append( QString( "@due(%1) ").arg( due().toString( "yyyy-MM-dd_HH:mm:ss" ) ) );

  if( alert().isValid() )
    result.append( QString( "@alert(%1) ").arg( alert().toString( "yyyy-MM-dd_HH:mm:ss" ) ) );

  foreach( QString file, files() ) {
    result.append( QString( "file(%1) " ).arg( file )  );
  }

  if( completed() ) {
    if( completedDate().isValid() )
      result.append( QString( "@done(%1)" ).arg( completedDate().toString( "yyyy-MM-dd" ) ) );
    else
      result.append( "@done" );
  }

  result = result.trimmed();

  if( !notes().isEmpty() ) {
    result.append( "\r\n" );
    foreach( QString note, notes() ) {
      result.append( note  );
      result.append( "\r\n" );
    }
  }

  return result.trimmed();
}


void CTask::debug() const {
  qDebug() << "  task:" << _task;
  qDebug() << "  priority:" << _priority;
  qDebug() << "  creationDate:" << _creationDate;
  qDebug() << "  completed:" << _completed;
  qDebug() << "  projects:" << _projects;
  qDebug() << "  contexts:" << _contexts;
  qDebug() << "  due:" << _due;
  qDebug() << "  threshold:" << _threshold;
  qDebug() << "  files:" << _files;
  qDebug() << "  notes:" << _notes;
  qDebug() << "  alert:" << _alert;
  qDebug() << "  isModified:" << _isModified;
  qDebug() << "  isValid:" << isValid();
}


CTaskList::CTaskList() : QList<CTask>() {
  initialize();
}


CTaskList::CTaskList( const QString& filename, const CTask::TaskFormat fmt ) : QList<CTask>() {
  initialize();

  _filename = filename;
  _format = fmt;

  QFileInfo fi( filename );

  if( !fi.exists() )
    return;

  _fileTimestamp = fi.lastModified();

  switch( fmt ) {
    case CTask::TodoTxt:
      this->processToDoTxtFile( filename );
      break;
    case CTask::Taskpaper:
      this->processTaskpaperFile( filename );
      break;
    default:
      qDebug() << "FIXME: This is a problem.";
  }
}


void CTaskList::processToDoTxtFile( const QString& filename ) {
  QFile data( filename );

  if( data.open( QFile::ReadOnly | QFile::Text ) ) {
    QTextStream in( &data );
    int counter = 0;
    while( !in.atEnd() ) {
      QString line = in.readLine().trimmed();

      if( !line.isEmpty() )
        this->append( CTask( line, CTask::TodoTxt, counter ) );

      ++counter;
    }
  }
}


void CTaskList::processTaskpaperFile( const QString& filename ) {
  QFile data( filename );

  if( data.open( QFile::ReadOnly | QFile::Text ) ) {
    QTextStream in( &data );
    int counter = 0;
    QString project;
    QStringList notes;
    bool inTask = false;
    CTask task;

    while( !in.atEnd() ) {
      QString line = in.readLine().trimmed();

      if( line.isEmpty() )
        continue;

      if( !line.startsWith( '-' ) ) {
        if( line.endsWith( ':' ) ) {
          if( inTask ) {
            // Finish the last task...
            task.setProject( project );
            task.setNotes( notes );
            this->append( task );

            // ... and start the next task.
            ++counter;
            task = CTask();
            notes.clear();
            task.setOrderInFile( counter );
            inTask = false;
          }
          project = line.left( line.length() - 1 ).trimmed();
        }
        else {
          notes.append( line );
        }
      }
      else {
        if( inTask ) {
          // Finish the last task...
          task.setProject( project );
          task.setNotes( notes );
          this->append( task );

          // ... and start the next one.
          ++counter;
          task = CTask();
          notes.clear();
          task.setOrderInFile( counter );
          task.parseTaskpaper( line );
          inTask = false;
        }
        else {
          inTask = true;
          task.parseTaskpaper( line );
        }
      }

      ++counter;
    }

    if( inTask ) {
      // Finish the last task.
      task.setProject( project );
      task.setNotes( notes );
      this->append( task );
    }
  }

}


CTaskList::CTaskList( const CTaskList& other ) : QList<CTask>( other ) {
  _filename = other._filename;
  _fileTimestamp = other._fileTimestamp;
  _format = other._format;
}


CTaskList::~CTaskList() {
  // Do nothing
}


void CTaskList::initialize() {
  _filename = QString();
  _fileTimestamp = QDateTime();
  _format = CTask::UnspecifiedFormat;
}

void CTaskList::debug() const {
  qDebug() << "CTaskList:" << this->count() << "items.";
  foreach( CTask task, *this ) {
    task.debug();
    qDebug();
  }
  qDebug();
}


void CTaskList::sortByProject() {
  std::sort( this->begin(), this->end(), [](const CTask a, const CTask b) -> bool { return a.project() < b.project(); } );
}

QString CTaskList::asString( const CTask::TaskFormat fmt ) const {
  QString result;

  foreach( CTask task, *this ) {
    result.append( task.asString( fmt ) );
    result.append( "\r\n" );
  }

  return result;
}


void CTaskList::cout( const CTask::TaskFormat fmt ) {
  if( this->isEmpty() )
    return;

  QString project;

  switch( fmt ) {
    case CTask::TodoTxt:
      foreach( CTask task, *this ) {
        ::cout << task.asString( fmt ) << endl;
      }
      break;
    case CTask::Taskpaper:
      this->sortByProject();

      project = this->at(0).project();

      if( !project.isEmpty() )
        ::cout << project << ":" << endl;

      foreach( CTask task, *this ) {
        if( task.project() != project ) {
          project = task.project();
          ::cout << endl << project << ":" << endl;
        }
        ::cout << task.asString( fmt ) << endl;
      }

      break;
    default:
      qDebug() << "FIXME: This is a problem";
      break;
  }
}



