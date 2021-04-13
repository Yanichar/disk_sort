#pragma once

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <stdlib.h>

#include "CDFS_ISO_9660.h"

class ISOCreatorCDFS {
public:
    bool CreateSingleFileISO(const char * szISOFileName, const char * szVolumeLabel);

    bool WriteSystemArea(FILE* ISOFileOut, const unsigned char(&BlockBuffer)[CDFS_LOGICAL_BLOCK_SIZE]);

    static bool WriteVolumeDescriptor(FILE * ISOFileOut, struct CDFS_VOLUME_DESCRIPTOR * pVolumeDescriptor);

    static bool WriteVolumeSetTerminator(FILE * ISOFileOut, uint8_t Type, const unsigned char(&Identifier)[6], uint8_t Version);
    static bool WriteVolumeSetTerminator(FILE * ISOFileOut, struct CDFS_VOLUME_DESCRIPTOR_SET_TERMINATOR * pVolumeSetTerminator);

    static bool WriteVolumeDirectory(FILE* ISOFileOut, uint8_t RecordLength, uint8_t ExtAttrRecordLength, uint32_t LBA, uint32_t DataLength, const unsigned char(&RecordingDate)[7], uint8_t FileFlags, uint8_t FileUnitSize, uint8_t InterleaveGapSize, uint16_t VolumeSequenceNumber, uint8_t LengthofIdentifier);
    static bool WriteVolumeDirectory(FILE* ISOFileOut, struct CDFS_VOLUME_DIRECTORY * pVolumeDirectory);

    static bool WriteVolumePathTable(FILE* ISOFileOut, uint8_t DirectoryIdentifier, uint8_t ExtAttrRecordLength, uint32_t LBALocation, uint16_t DirectoryParentNumber);
    static bool WriteVolumePathTable(FILE* ISOFileOut, struct CDFS_VOLUME_PATH_TABLE* pVolumePathTable);
};