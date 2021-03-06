// RarItem.cpp

#include "StdAfx.h"

#include "RarItem.h"

namespace NArchive{
namespace NRar{

bool CItem::IgnoreItem() const
{
  switch(HostOS)
  {
    case NHeader::NFile::kHostMSDOS:
    case NHeader::NFile::kHostOS2:
    case NHeader::NFile::kHostWin32:
      return ((Attrib & NHeader::NFile::kLabelFileAttribute) != 0);
  }
  return false;
}

bool CItem::IsDir() const
{
  if (GetDictSize() == NHeader::NFile::kDictDirectoryValue)
    return true;
  switch(HostOS)
  {
    case NHeader::NFile::kHostMSDOS:
    case NHeader::NFile::kHostOS2:
    case NHeader::NFile::kHostWin32:
      if ((Attrib & FILE_ATTRIBUTE_DIRECTORY) != 0)
        return true;
  }
  return false;
}

UInt32 CItem::GetWinAttributes() const
{
  UInt32 winAttributes;
  switch(HostOS)
  {
    case NHeader::NFile::kHostMSDOS:
    case NHeader::NFile::kHostOS2:
    case NHeader::NFile::kHostWin32:
      winAttributes = Attrib;
      break;
    default:
	  //Attrib is a valid mode_t value in this case. ( emperically )
	  //Convert to the form expected by NFile::NDirectory::MySetFileAttributes
	  //Based on the version in ZipItem.cpp
	  winAttributes = (((UInt32)Attrib) << 16) | FILE_ATTRIBUTE_UNIX_EXTENSION;

  }
  if (IsDir())
    winAttributes |= NHeader::NFile::kWinFileDirectoryAttributeMask;
  return winAttributes;
}

}}
