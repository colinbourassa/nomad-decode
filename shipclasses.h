#ifndef SHIPCLASSES_H
#define SHIPCLASSES_H

#include <QString>
#include <QMap>
#include "dattable.h"

struct ShipClass
{
  QString name;
};

typedef struct __attribute__((packed)) ShipClassTableEntry
{
  uint16_t nameOffset;
  uint8_t missileStartQty;
  uint8_t missileType;
  uint8_t missileLoadType;
  uint8_t shieldType;
  uint8_t scannerType;
  uint8_t engineType;
  uint16_t startingStrengthA; // one of these two 16-bit fields is shield strength,
  uint16_t startingStrengthB; // and the other is hull strength; needs more research
} ShipClassTableEntry;

class ShipClasses : public DatTable<ShipClassTableEntry>
{
public:
  ShipClasses(DatLibrary& lib);
  virtual ~ShipClasses();
  QMap<int,ShipClass> getList();
  QString getName(int id);

protected:
  bool populateList();

private:
  QMap<int,ShipClass> m_shipClasses;
};

#endif // SHIPCLASSES_H
