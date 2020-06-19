#include "missions.h"
#include <QtEndian>

Missions::Missions(DatLibrary& lib, GameText& gametext) :
  DatTable<MissionTableEntry>(lib),
  m_gtext (&gametext)
{

}

QMap<int,Mission> Missions::getList()
{
  if (m_missions.isEmpty())
  {
    populateList();
  }

  return m_missions;
}

bool Missions::populateList()
{
  bool status = false;

  if (openFile(DatFileType_CONVERSE, "MISSION.TAB"))
  {
    status = true;
    int index = 0;
    MissionTableEntry* currentEntry = getEntry(index);

    while (currentEntry != nullptr)
    {
      // placeholder: not sure of the best way to determine whether a MISSION.TAB entry is
      // valid, but I've noticed that the fourth byte is always 0x01 for every valid entry
      if (currentEntry->unknown_a[1] == 0x01)
      {
        Mission m;
        if (currentEntry->actionRequired == 0)
        {
          m.action = MissionActionType_None;
        }
        else if (currentEntry->actionRequired == 2)
        {
          m.action = MissionActionType_DestroyShip;
        }
        else if (currentEntry->actionRequired == 3)
        {
          m.action = MissionActionType_DeliverItem;
        }
        m.startText    = getMissionText(qFromLittleEndian<quint16>(currentEntry->startTextIndex), m.startTextCommands);
        m.completeText = getMissionText(qFromLittleEndian<quint16>(currentEntry->completeTextIndex), m.completeTextCommands);
        m.objectiveId  = currentEntry->objectiveId;
        m.objectiveLocation = currentEntry->placeId;

        m_missions.insert(index, m);
      }
      index++;
      currentEntry = getEntry(index);
    }
  }

  return status;
}

QString Missions::getMissionText(uint16_t idxFileIndex, QVector<QPair<GTxtCmd,int> >& commands)
{
  QByteArray misTextIdxData;
  QByteArray misTextStrData;
  QString txt;

  if (m_lib->getFileByName(DatFileType_CONVERSE, "MISTEXT.IDX", misTextIdxData) &&
      m_lib->getFileByName(DatFileType_CONVERSE, "MISTEXT.TXT", misTextStrData))
  {
    const int idxOffset = (idxFileIndex + 1) * 4;
    int32_t txtOffset = 0;

    memcpy(&txtOffset, misTextIdxData.data() + idxOffset, 4);
    txtOffset = qFromLittleEndian<qint32>(txtOffset);

    if (txtOffset < misTextStrData.size())
    {
      const char* rawdata = misTextStrData.data();
      txt = m_gtext->readString(rawdata + txtOffset, commands);
    }
  }

  return txt;
}
