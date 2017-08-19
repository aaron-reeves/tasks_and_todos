#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QDateTimeEdit>

#include "ctasktreewidgetitem.h"
#include "nongui/ctask.h"

namespace Ui {
  class CMainWindow;
}


class CMainWindow : public QMainWindow {
  Q_OBJECT

  public:
    explicit CMainWindow( QWidget *parent = 0 );
    ~CMainWindow();

  protected slots:
    void btnTest();
    void updateWidgets( QTreeWidgetItem* item, int column );

    void actionNew();
    void actionOpenFile();
    void actionOpenFileset();
    void actionSave();
    void actionSaveAs();
    void actionClose();
    void actionExit();

  protected:
    void populateItems();

  private:
    Ui::CMainWindow *ui;

    CTaskList* _tasks;
    bool _populatingForm;
};

#endif // CMAINWINDOW_H
