#ifndef ABOUTBOX_H
#define ABOUTBOX_H

#include <QDialog>

namespace Ui {
class AboutBox;
}

/**
 * Provides a simple "about" dialog box with links to the project repo.
 */
class AboutBox : public QDialog
{
  Q_OBJECT

public:
  explicit AboutBox(QWidget *parent = nullptr);
  ~AboutBox();

private slots:
  void on_m_buttonClose_clicked();

private:
  Ui::AboutBox *ui;
};

#endif // ABOUTBOX_H
