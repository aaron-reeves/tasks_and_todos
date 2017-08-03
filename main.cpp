#include <QCoreApplication>

#include <ar_general_purpose/qcout.h>

#include "ctask.h"


int main(int argc, char *argv[]) {
//  QCoreApplication a(argc, argv);

//  return a.exec();


//  QString test = "(A) 2017-07-31 A new todo item with high priority, a @contextTag, a project name, and a due date due:2017-08-02 +ProjectName";
//  CTask task( test, CTask::TodoTxt );

//  task.debug();

//  return 0;

//  CTaskList list( "C:/Users/areeves/Documents/Programming/tasks/todo.txt", CTask::TodoTxt );

//  list.cout( CTask::TodoTxt );
//  cout << endl << endl;
//  list.cout( CTask::Taskpaper );

//  cout << endl << endl;
  CTaskList list( "C:/Users/areeves/Documents/Programming/tasks/tasks.taskpaper", CTask::Taskpaper );
  list.cout( CTask::TodoTxt );


  return 0;
}
