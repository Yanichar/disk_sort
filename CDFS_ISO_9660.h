#ifndef CDFS_ISO_9660H
#define CDFS_ISO_9660H

#include <cstdint>

constexpr int CDFS_SYSTEM_AREA_SIZE = 32768;
constexpr uint32_t CDFS_SYSTEM_AREA_BLOCK_COUNT = 16;
constexpr uint32_t CDFS_LOGICAL_BLOCK_SIZE = 2048;

constexpr uint8_t CDFS_VOLUME_TYPE_BOOT = 0;
constexpr uint8_t CDFS_VOLUME_TYPE_PRIMARY = 1;
constexpr uint8_t CDFS_VOLUME_TYPE_SUPPLEMENTARY = 2;
constexpr uint8_t CDFS_VOLUME_TYPE_PARTITION_DESC = 3;
constexpr uint8_t CDFS_VOLUME_TYPE_TERMINATOR = 255;
// Identifier only takes 5 chars, but this code has been designed to be null friendly for programmers.
constexpr unsigned char CDFS_DEFAULT_IDENTIFIER[6] = "CD001"; // Beware, there is a hidden null at the end.
constexpr uint8_t CDFS_VOLUME_VERSION = 1;

// This code has been modified to differentiate between ASCII(clear text) data types and integer data types.
// uint8_t and unsigned char both have a size of 1 byte and are interchangeable.
struct CDFS_VOLUME_DATETIME_FORMAT {

    // DateTime[16] formatting.
    // All numbered values must have a prefix of 0 if less than 10, 100 or 1000.
    // YYYYMMDDHHMMSSXX
    // YYYY = Year 0001 to 9999.
    // MM = Month 01 to 12.
    // DD = Day 01 to 31.
    // HH = Hour 00 to 23.
    // MM = Minute 00 to 59.
    // SS = Second 00 to 59.
    // XX = Hundredths of a second from 00 to 99.
    unsigned char DateTime[16];
    uint8_t TimeZoneOffset;
};

struct CDFS_VOLUME_DESCRIPTOR {
    uint8_t Type;
    unsigned char Identifier[5];
    uint8_t Version;
    uint8_t Unused1[1];
    unsigned char SystemIdentifier[32];
    unsigned char VolumeIdentifier[32];
    uint8_t Unused2[8];
    uint32_t VolumeSpaceSizeLE;
    uint32_t VolumeSpaceSizeBE;
    uint8_t Unused3[32];
    uint16_t VolumeSetSizeLE;
    uint16_t VolumeSetSizeBE;
    uint16_t VolumeSequenceNumberLE;
    uint16_t VolumeSequenceNumberBE;
    uint16_t LogicalBlockSizeLE;
    uint16_t LogicalBlockSizeBE;
    uint32_t PathTableSizeLE;
    uint32_t PathTableSizeBE;
    uint32_t LPathTableLocationLE;
    uint32_t OptLPathTableLocationLE;
    uint32_t MPathTableLocationBE;
    uint32_t OptMPathTableLocationBE;
    unsigned char RootDirEntry[34];
    unsigned char VolumeSetIdentifier[128];
    unsigned char PublisherIdentifier[128];
    unsigned char DataPreparerIdentifier[128];
    unsigned char ApplicationIdentifier[128];
    unsigned char CopyrightIdentifier[38];
    unsigned char AbstractIdentifier[36];
    unsigned char BiblioIdentifier[37];
    struct CDFS_VOLUME_DATETIME_FORMAT VolumeCreation;
    struct CDFS_VOLUME_DATETIME_FORMAT VolumeModification;
    struct CDFS_VOLUME_DATETIME_FORMAT VolumeExpiration;
    struct CDFS_VOLUME_DATETIME_FORMAT VolumeEffective;
    uint8_t FileStructVersion;
    uint8_t Unused4[1];
    uint8_t ApplicationUsed[512];
    uint8_t Reserved[653];
};

struct CDFS_VOLUME_DESCRIPTOR_SET_TERMINATOR {
    uint8_t Type;
    unsigned char Identifier[5];
    uint8_t Version;
    uint8_t Unused[2041];
};

struct CDFS_VOLUME_DIRECTORY {
    uint8_t RecordLength;
    uint8_t ExtAttrRecordLength;
    uint32_t LBALE;
    uint32_t LBABE;
    uint32_t DataLengthLE;
    uint32_t DataLengthBE;
    unsigned char RecordingDate[7];
    uint8_t FileFlags;
    uint8_t FileUnitSize;
    uint8_t InterleaveGapSize;
    uint16_t VolumeSequenceNumberLE;
    uint16_t VolumeSequenceNumberBE;
    uint8_t LengthofIdentifier;
};

struct CDFS_VOLUME_PATH_TABLE {
    uint8_t DirectoryIdentifier;
    uint8_t ExtAttrRecordLength;
    uint32_t LBALocation;
    uint16_t DirectoryParentNumber;
};

inline void WideCharStringSpaceFill(unsigned char * Src, unsigned int BuffSize) {
    if (!Src) return;
    for (unsigned int CurPos = 0; CurPos < BuffSize; CurPos++) {
        Src[CurPos] = 0;
        CurPos++;
        if (CurPos < BuffSize) Src[CurPos] = 0x20;
    }
}

inline void CharToWideCharBE(unsigned char * Dest, const char * Src, unsigned int DestBuffSize) {
    if (!Dest || !Src || !DestBuffSize) return;
    unsigned int CurPos = 0;
    unsigned int CurSrcPos = 0;
    while (CurPos < DestBuffSize) {
        Dest[CurPos] = 0;
        CurPos++;
        if (CurSrcPos < strlen(Src)) {
            Dest[CurPos] = Src[CurSrcPos];
            CurSrcPos++;
        } else {
            Dest[CurPos] = 0x20;
        }
        CurPos++;
    }
}

#endif