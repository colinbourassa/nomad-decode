#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QVector>
#include <QTableWidgetItem>
#include <QImage>
#include <QGraphicsScene>
#include <QTreeWidgetItem>
#include <QMap>
#include "enums.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  m_inventory(m_lib, m_palette),
  m_places(m_lib, m_palette, m_pclasses),
  m_palette(m_lib),
  m_pclasses(m_lib),
  m_aliens(m_lib, m_palette)
{
  ui->setupUi(this);
  ui->m_objectImageView->scale(3, 3);
  ui->m_planetView->scale(2, 2);
  ui->m_alienView->scale(3, 3);

  m_lib.openData("/home/cmb/opt/games/nomad");
  populatePlaceWidgets();
  populateObjectWidgets();
  populateAlienWidgets();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_actionOpen_game_data_dir_triggered()
{
  m_gamedir = QFileDialog::getExistingDirectory(this, "Select directory containing Nomad .DAT files", "/home", QFileDialog::ShowDirsOnly);
  m_lib.openData(m_gamedir);
}

void MainWindow::on_actionExit_triggered()
{
  this->close();
}

void MainWindow::on_actionClose_data_files_triggered()
{
  m_lib.closeData();
  m_aliens.clear();
  m_places.clear();
  m_pclasses.clear();
  m_inventory.clear();
}

void MainWindow::populatePlaceWidgets()
{
  QMap<int,Place> places = m_places.getPlaceList();

  foreach (Place p, places.values())
  {
    const int rowcount = ui->m_placeTable->rowCount();
    ui->m_placeTable->insertRow(rowcount);
    ui->m_placeTable->setItem(rowcount, 0, new QTableWidgetItem(QString("%1").arg(p.id)));
    ui->m_placeTable->setItem(rowcount, 1, new QTableWidgetItem(p.name));
  }
  ui->m_placeTable->resizeColumnsToContents();
}

void MainWindow::populateObjectWidgets()
{
  QMap<int,InventoryObj> objs = m_inventory.getObjectList();
  ui->m_objTable->clear();
  foreach (InventoryObj obj, objs.values())
  {
    const int rowcount = ui->m_objTable->rowCount();

    ui->m_objTable->insertRow(rowcount);
    ui->m_objTable->setItem(rowcount, 0, new QTableWidgetItem(QString("%1").arg(obj.id)));
    ui->m_objTable->setItem(rowcount, 1, new QTableWidgetItem(obj.name));
  }
  ui->m_objTable->resizeColumnsToContents();
}

void MainWindow::populateAlienWidgets()
{
  QMap<int,Alien> aliens = m_aliens.getAlienList();
  ui->m_alienTable->clear();
  foreach (Alien a, aliens.values())
  {
    const int rowcount = ui->m_alienTable->rowCount();
    ui->m_alienTable->insertRow(rowcount);
    ui->m_alienTable->setItem(rowcount, 0, new QTableWidgetItem(QString("%1").arg(a.id)));
    ui->m_alienTable->setItem(rowcount, 1, new QTableWidgetItem(a.name));

    const QString racename = s_raceNames.contains(a.race) ? s_raceNames[a.race] : "(invalid/unknown)";
    ui->m_alienTable->setItem(rowcount, 2, new QTableWidgetItem(racename));
  }
  ui->m_alienTable->resizeColumnsToContents();
}


void MainWindow::on_m_objTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
  Q_UNUSED(currentColumn)
  Q_UNUSED(previousRow)
  Q_UNUSED(previousColumn)

  int id = ui->m_objTable->item(currentRow, 0)->text().toInt();
  QPixmap pm = m_inventory.getInventoryImage(id);
  m_objScene.addPixmap(pm);
  ui->m_objectImageView->setScene(&m_objScene);
  ui->m_objectTypeLabel->setText("Type: " + getInventoryObjTypeText(m_inventory.getObjectType(id)));
}

void MainWindow::on_m_placeTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
  Q_UNUSED(currentColumn)
  Q_UNUSED(previousRow)
  Q_UNUSED(previousColumn)

  bool status = true;
  const int id = ui->m_placeTable->item(currentRow, 0)->text().toInt();
  QPixmap pm = m_places.getPlaceSurfaceImage(id, status);
  if (status)
  {
    m_planetSurfaceScene.addPixmap(pm);
    ui->m_planetView->setScene(&m_planetSurfaceScene);
  }

  Place p;
  if (m_places.getPlace(id, p))
  {
    PlaceClass pclassData;
    if (m_pclasses.pclassData(p.pclass, pclassData))
    {
      ui->m_planetClassData->setText(pclassData.name);
      const QString tempString = QString("%1 (%2)").arg(pclassData.temperature).arg(pclassData.temperatureRange);
      ui->m_planetTemperatureData->setText(tempString);
    }
  }
}

void MainWindow::on_m_alienTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
  Q_UNUSED(currentColumn)
  Q_UNUSED(previousRow)
  Q_UNUSED(previousColumn)

  const int id = ui->m_alienTable->item(currentRow, 0)->text().toInt();

  Alien a;
  if (m_aliens.getAlien(id, a))
  {
    QMap<int,QImage> animFrames;
    if (m_aliens.getAnimationFrames(id, animFrames) && (animFrames.count() > 0))
    {
      m_alienScene.clear();
      m_alienScene.addPixmap(QPixmap::fromImage(animFrames.values().at(0)));
      ui->m_alienView->setScene(&m_alienScene);
    }
  }
}
