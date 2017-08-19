#ifndef CTASKEDITFORM_H
#define CTASKEDITFORM_H

#include <QDialog>

#include "nongui/ctask.h"

namespace Ui {
  class CTaskEditForm;
}

class CTaskEditForm : public QDialog {
  Q_OBJECT

  public:
    explicit CTaskEditForm( QStringList projects, CTask* task, QWidget *parent = 0 );
    ~CTaskEditForm();

    void updateTask( CTask* task );

  protected slots:
    void showHideDueDateTime();
    void showHideAlertDateTime();

  protected:
    void setupTask( CTask* task );

  private:
    Ui::CTaskEditForm *ui;
};

#endif // CTASKEDITFORM_H
