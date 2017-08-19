#ifndef CTASK_H
#define CTASK_H

#include <QtCore>


class CTask {
  public:
    enum TaskFormat {
      UnspecifiedFormat,
      TodoTxt,
      Taskpaper
    };

    enum TaskPriority {
      UnspecifiedPriority,
      A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z
    };

    CTask();
    CTask(const QString& str, const CTask::TaskFormat fmt , const QString& sourceFilename, const int orderInFile = -1 );
    CTask( const CTask& other );
    ~CTask();

    QString task( void ) const { return _task; }
    TaskPriority priority( void ) const { return _priority; }
    QDate creationDate( void ) const { return _creationDate; }
    bool completed( void ) const { return _completed; }
    QDate completedDate( void ) const { return _completedDate; }
    QStringList projects( void ) const { return _projects; }
    QStringList contexts( void ) const { return _contexts; }
    QDateTime due( void ) const { return _due; }
    QDate threshold( void ) const { return _threshold; }
    QStringList files( void ) const { return _files; }
    QStringList notes( void ) const { return _notes; }
    QDateTime alert( void ) const { return _alert; }

    bool isModified( void ) const { return _isModified; }
    bool isValid( void ) const;

    QString sourceFilename() const { return _sourceFilename; }
    int orderInFile() const { return _orderInFile; }

    void setTask( const QString& val ) { _isModified = ( _task != val.trimmed() ); _task = val.trimmed(); }

    void setCompleted( bool val ) { _isModified = ( val != _completed ); _completed = val; }

    void setPriority( const TaskPriority priority ) { _isModified = ( _priority != priority ); _priority = priority; }
    void setPriority( const QString& str );

    void setCreationDate( QDate val ) {  _isModified = ( val != _creationDate ); _creationDate = val; }
    void setCreationDate(const QString& str );

    void setThreshold( QDate val ) { _isModified = ( val != _threshold ); _threshold = val; }
    void setThreshold( const QString& str );

    void setCompletedDate( QDate val ) { _isModified = ( val != _completedDate ); _completedDate = val; }
    void setCompletedDate( const QString& str );

    void setDue( QDateTime val ) { qDebug() << "Due date changed to " << val; _isModified = ( val != _due ); _due = val; }
    void setDue(const QString& str );

    void setAlert( QDateTime val ) { _isModified = ( val != _alert ); _alert = val; }
    void setAlert( const QString& str );

    QString project() const;
    void setProject( const QString& str );

    void addProject( QString str );
    void removeProject( QString str );

    //void setProjects( QStringList val ) { _projects = val; _isModified = true; }
    //void setContexts( QStringList val ) { _contexts = val; _isModified = true; }

    //void setFiles( QStringList val ) { _files = val; _isModified = true; }
    void addFile( const QString& str ) { _files.append( str ); _isModified = true; }

    bool hasNotes() const { return !_notes.isEmpty(); }
    void setNotes( QStringList val ) { _isModified = ( val != _notes ); _notes = val; }
    void clearNotes() { _isModified = ( !_notes.isEmpty() ); _notes.clear(); }

    void setSourceFilename( const QString& val ) { _isModified = ( val != _sourceFilename ); _sourceFilename = val; }
    void setOrderInFile( const int val ) { _isModified = ( val != _orderInFile ); _orderInFile = val; }

    void parseTaskpaper( QString str );

    QString asString( const TaskFormat fmt ) const;

    void debug() const;

    static QString priorityToString( const TaskPriority priority );

  protected:
    void initialize();

    void parseTodoTxt(QString str );

    QString asTodoTxtString() const;
    QString asTaskpaperString() const;

    QDate extractDate( QString str ) const;
    QDateTime extractDateTime( QString str ) const;
    QString extractString( QString str ) const;

    QString _task;
    TaskPriority _priority;
    QDate _creationDate;
    bool _completed;
    QDate _completedDate;
    QStringList _projects;
    QStringList _contexts;
    QDateTime _due;
    QDate _threshold;
    QStringList _files;
    QStringList _notes;
    QDateTime _alert;


    QString _sourceFilename;
    int _orderInFile;

    bool _isModified;

    /*
    2017-07-31 See *.taskpaper documents
    (A) 2017-07-31 A new todo item with high priority, a @contextTag, a project name, and a due date due:2017-08-02 +ProjectName
    x 2017-07-31 Another new todo item but with low priority, a @contextTag, a project name, and a due date due:2017-08-02 +ProjectName alert:2017-08-02_08:15:00  note:"What is a threshold date?" t:2017-07-31
    */

};


class CTaskList : public QList<CTask*> {
  public:
    CTaskList();
    CTaskList( const QString& filename, const CTask::TaskFormat fmt );
    CTaskList( const CTaskList& other );
    ~CTaskList();

    void extendedAppend( CTask* value );

    void debug() const;

    QString asString( const CTask::TaskFormat fmt ) const;

    void cout( const CTask::TaskFormat fmt );

    void sortByProject();

    QSet<QString> projects() const { return _projects; }
    QSet<QString> contexts() const { return _contexts; }

  protected:
    void initialize();
    void processToDoTxtFile( const QString& filename );
    void processTaskpaperFile( const QString& filename );

    QSet<QString> _projects;
    QSet<QString> _contexts;

    QString _filename;
    QDateTime _fileTimestamp;
    CTask::TaskFormat _format;
};

#endif // CTASK_H
