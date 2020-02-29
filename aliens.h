#ifndef ALIENS_H
#define ALIENS_H

#include <QByteArray>
#include <QVector>
#include <QString>
#include <QImage>
#include <QMap>
#include "enums.h"
#include "palette.h"
#include "datlibrary.h"

#define ANM_RECORD_SIZE_BYTES 16
#define ANM_FIRST_RECORD_OFFSET 0x1A

struct Alien
{
  int id;
  QString name;
  AlienRace race;
};

typedef struct __attribute__((packed)) AlienTableEntry
{
  uint16_t nameOffset;
  uint8_t race;
  uint8_t unknown[5];
} AlienTableEntry;

class Aliens
{
public:
  Aliens(DatLibrary& lib, Palette& pal);
  void clear();
  QMap<int,Alien> getAlienList();
  bool getAlien(int id, Alien& alien);
  bool getAnimationFrames(int id, QMap<int,QImage>& frames);
  QString getName(int id);

private:
  DatLibrary* m_lib;
  Palette* m_pal;
  static const QVector<QString> s_animationMap;
  QMap<int,Alien> m_alienList;

  bool populateAlienList();
  QMap< int, QVector<int> > getListOfFrames(const QByteArray& anmData) const;
  bool buildFrame(QVector<int> delIdList, QString delFilenamePrefix, const QVector<QRgb> pal, QImage& frame) const;
};

#endif // ALIENS_H