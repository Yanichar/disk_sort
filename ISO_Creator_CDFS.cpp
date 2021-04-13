#include "ISO_Creator_CDFS.h"
#include <stdio.h>

bool ISOCreatorCDFS::WriteSystemArea(FILE * ISOFileOut, const unsigned char(&BlockBuffer)[CDFS_LOGICAL_BLOCK_SIZE]) {
    if (!ISOFileOut || !BlockBuffer) return false;
    for (unsigned int SysAreaCount = 0; SysAreaCount < CDFS_SYSTEM_AREA_BLOCK_COUNT; ++SysAreaCount)
        fwrite(BlockBuffer, 1, CDFS_LOGICAL_BLOCK_SIZE, ISOFileOut);
    return true;
}

bool ISOCreatorCDFS::WriteVolumeDescriptor(FILE * ISOFileOut, struct CDFS_VOLUME_DESCRIPTOR* pVolumeDescriptor) {
    if (!ISOFileOut) return false;
    fwrite(&pVolumeDescriptor->Type, 1, 1, ISOFileOut);
    fwrite(&pVolumeDescriptor->Identifier, 1, 5, ISOFileOut);
    fwrite(&pVolumeDescriptor->Version, 1, 1, ISOFileOut);
    fwrite(&pVolumeDescriptor->Unused1, 1, 1, ISOFileOut);
    fwrite(&pVolumeDescriptor->SystemIdentifier, 1, 32, ISOFileOut);
    fwrite(&pVolumeDescriptor->VolumeIdentifier, 1, 32, ISOFileOut);
    fwrite(&pVolumeDescriptor->Unused2, 1, 8, ISOFileOut);
    fwrite(&pVolumeDescriptor->VolumeSpaceSizeLE, 4, 1, ISOFileOut);
    fwrite(&pVolumeDescriptor->VolumeSpaceSizeBE, 4, 1, ISOFileOut);
    fwrite(&pVolumeDescriptor->Unused3, 1, 32, ISOFileOut);
    fwrite(&pVolumeDescriptor->VolumeSetSizeLE, 2, 1, ISOFileOut);
    fwrite(&pVolumeDescriptor->VolumeSetSizeBE, 2, 1, ISOFileOut);
    fwrite(&pVolumeDescriptor->VolumeSequenceNumberLE, 2, 1, ISOFileOut);
    fwrite(&pVolumeDescriptor->VolumeSequenceNumberBE, 2, 1, ISOFileOut);
    fwrite(&pVolumeDescriptor->LogicalBlockSizeLE, 2, 1, ISOFileOut);
    fwrite(&pVolumeDescriptor->LogicalBlockSizeBE, 2, 1, ISOFileOut);
    fwrite(&pVolumeDescriptor->PathTableSizeLE, 4, 1, ISOFileOut);
    fwrite(&pVolumeDescriptor->PathTableSizeBE, 4, 1, ISOFileOut);
    fwrite(&pVolumeDescriptor->LPathTableLocationLE, 4, 1, ISOFileOut);
    fwrite(&pVolumeDescriptor->OptLPathTableLocationLE, 4, 1, ISOFileOut);
    fwrite(&pVolumeDescriptor->MPathTableLocationBE, 4, 1, ISOFileOut);
    fwrite(&pVolumeDescriptor->OptMPathTableLocationBE, 4, 1, ISOFileOut);
    fwrite(&pVolumeDescriptor->RootDirEntry, 1, 34, ISOFileOut);
    fwrite(&pVolumeDescriptor->VolumeSetIdentifier, 1, 128, ISOFileOut);
    fwrite(&pVolumeDescriptor->PublisherIdentifier, 1, 128, ISOFileOut);
    fwrite(&pVolumeDescriptor->DataPreparerIdentifier, 1, 128, ISOFileOut);
    fwrite(&pVolumeDescriptor->ApplicationIdentifier, 1, 128, ISOFileOut);
    fwrite(&pVolumeDescriptor->CopyrightIdentifier, 1, 38, ISOFileOut);
    fwrite(&pVolumeDescriptor->AbstractIdentifier, 1, 36, ISOFileOut);
    fwrite(&pVolumeDescriptor->BiblioIdentifier, 1, 37, ISOFileOut);
    fwrite(&pVolumeDescriptor->VolumeCreation.DateTime, 1, 16, ISOFileOut);
    fwrite(&pVolumeDescriptor->VolumeCreation.TimeZoneOffset, 1, 1, ISOFileOut);
    fwrite(&pVolumeDescriptor->VolumeModification.DateTime, 1, 16, ISOFileOut);
    fwrite(&pVolumeDescriptor->VolumeModification.TimeZoneOffset, 1, 1, ISOFileOut);
    fwrite(&pVolumeDescriptor->VolumeExpiration.DateTime, 1, 16, ISOFileOut);
    fwrite(&pVolumeDescriptor->VolumeExpiration.TimeZoneOffset, 1, 1, ISOFileOut);
    fwrite(&pVolumeDescriptor->VolumeEffective.DateTime, 1, 16, ISOFileOut);
    fwrite(&pVolumeDescriptor->VolumeEffective.TimeZoneOffset, 1, 1, ISOFileOut);
    fwrite(&pVolumeDescriptor->FileStructVersion, 1, 1, ISOFileOut);
    fwrite(&pVolumeDescriptor->Unused4, 1, 1, ISOFileOut);
    fwrite(&pVolumeDescriptor->ApplicationUsed, 1, 512, ISOFileOut);
    fwrite(&pVolumeDescriptor->Reserved, 1, 653, ISOFileOut);
    return true;
}

bool ISOCreatorCDFS::WriteVolumeSetTerminator(FILE* ISOFileOut, uint8_t Type, const unsigned char (&Identifier)[6], uint8_t Version) {
    if (!ISOFileOut) return false;
    struct CDFS_VOLUME_DESCRIPTOR_SET_TERMINATOR VolDescSetTerm = { 0 };
    VolDescSetTerm.Type = Type;
    memcpy(&VolDescSetTerm.Identifier, Identifier, 5);
    VolDescSetTerm.Version = Version;
    return WriteVolumeSetTerminator(ISOFileOut, &VolDescSetTerm);
}

bool ISOCreatorCDFS::WriteVolumeSetTerminator(FILE* ISOFileOut, struct CDFS_VOLUME_DESCRIPTOR_SET_TERMINATOR* pVolumeSetTerminator) {
    if (!ISOFileOut) return false;
    fwrite(&pVolumeSetTerminator->Type, 1, 1, ISOFileOut);
    fwrite(&pVolumeSetTerminator->Identifier, 1, 5, ISOFileOut);
    fwrite(&pVolumeSetTerminator->Version, 1, 1, ISOFileOut);
    fwrite(&pVolumeSetTerminator->Unused, 1, 2041, ISOFileOut);
    return true;
}

bool ISOCreatorCDFS::WriteVolumeDirectory(FILE* ISOFileOut, uint8_t RecordLength, uint8_t ExtAttrRecordLength, uint32_t LBA, uint32_t DataLength, const unsigned char(&RecordingDate)[7], uint8_t FileFlags, uint8_t FileUnitSize, uint8_t InterleaveGapSize, uint16_t VolumeSequenceNumber, uint8_t LengthofIdentifier) {
    if (!ISOFileOut) return false;
    CDFS_VOLUME_DIRECTORY VolumeDirectory = { 0 };
    VolumeDirectory.RecordLength = RecordLength;
    VolumeDirectory.ExtAttrRecordLength = ExtAttrRecordLength;
    VolumeDirectory.LBALE = LBA;
    VolumeDirectory.LBABE = __builtin_bswap32(LBA);
    VolumeDirectory.DataLengthLE = DataLength;
    VolumeDirectory.DataLengthBE = __builtin_bswap32(DataLength);
    memcpy(&VolumeDirectory.RecordingDate, RecordingDate, 7);
    VolumeDirectory.FileFlags = FileFlags;
    VolumeDirectory.FileUnitSize = FileUnitSize;
    VolumeDirectory.InterleaveGapSize = InterleaveGapSize;
    VolumeDirectory.VolumeSequenceNumberLE = VolumeSequenceNumber;
    VolumeDirectory.VolumeSequenceNumberBE = __builtin_bswap16(VolumeSequenceNumber);
    VolumeDirectory.LengthofIdentifier = LengthofIdentifier;
    return WriteVolumeDirectory(ISOFileOut, &VolumeDirectory);
}

bool ISOCreatorCDFS::WriteVolumeDirectory(FILE* ISOFileOut, struct CDFS_VOLUME_DIRECTORY * pVolumeDirectory) {
    if (!ISOFileOut || !pVolumeDirectory) return false;
    fwrite(&pVolumeDirectory->RecordLength, 1, 1, ISOFileOut);
    fwrite(&pVolumeDirectory->ExtAttrRecordLength, 1, 1, ISOFileOut);
    fwrite(&pVolumeDirectory->LBALE, 4, 1, ISOFileOut);
    fwrite(&pVolumeDirectory->LBABE, 4, 1, ISOFileOut);
    fwrite(&pVolumeDirectory->DataLengthLE, 4, 1, ISOFileOut);
    fwrite(&pVolumeDirectory->DataLengthBE, 4, 1, ISOFileOut);
    fwrite(&pVolumeDirectory->RecordingDate, 1, 7, ISOFileOut);
    fwrite(&pVolumeDirectory->FileFlags, 1, 1, ISOFileOut);
    fwrite(&pVolumeDirectory->FileUnitSize, 1, 1, ISOFileOut);
    fwrite(&pVolumeDirectory->InterleaveGapSize, 1, 1, ISOFileOut);
    fwrite(&pVolumeDirectory->VolumeSequenceNumberLE, 2, 1, ISOFileOut);
    fwrite(&pVolumeDirectory->VolumeSequenceNumberBE, 2, 1, ISOFileOut);
    fwrite(&pVolumeDirectory->LengthofIdentifier, 1, 1, ISOFileOut);
    return true;
}

bool ISOCreatorCDFS::WriteVolumePathTable(FILE * ISOFileOut, uint8_t DirectoryIdentifier, uint8_t ExtAttrRecordLength, uint32_t LBALocation, uint16_t DirectoryParentNumber) {
    if (!ISOFileOut) return false;
    CDFS_VOLUME_PATH_TABLE VolumePathTable = { 0 };
    VolumePathTable.DirectoryIdentifier = DirectoryIdentifier;
    VolumePathTable.ExtAttrRecordLength = ExtAttrRecordLength;
    VolumePathTable.LBALocation = LBALocation;
    VolumePathTable.DirectoryParentNumber = DirectoryParentNumber;
    return WriteVolumePathTable(ISOFileOut, &VolumePathTable);
}

bool ISOCreatorCDFS::WriteVolumePathTable(FILE * ISOFileOut, struct CDFS_VOLUME_PATH_TABLE* pVolumePathTable) {
    if (!ISOFileOut || !pVolumePathTable) return false;
    fwrite(&pVolumePathTable->DirectoryIdentifier, 1, 1, ISOFileOut);
    fwrite(&pVolumePathTable->ExtAttrRecordLength, 1, 1, ISOFileOut);
    fwrite(&pVolumePathTable->LBALocation, 4, 1, ISOFileOut);
    fwrite(&pVolumePathTable->DirectoryParentNumber, 2, 1, ISOFileOut);
    return true;
}

bool ISOCreatorCDFS::CreateSingleFileISO(const char * szISOFileName, const char * szVolumeLabel) {
    // szISOFileName and szVolumeLabel assumes a valid pointer to an array of chars including a null terminator.
    // The sz prefix stands for "string zero" termintated.

    FILE * ISOFileOut = nullptr;
    unsigned int ByteCount = 0;

    // Some hardcore error trapping here.
    if (!szISOFileName || !szVolumeLabel) return false;
    if (strlen(szISOFileName) == 0 || strlen(szVolumeLabel) == 0 || strlen(szVolumeLabel) > 16) return false;

    ISOFileOut = fopen(szISOFileName, "wb");
    if ( ISOFileOut != nullptr ) {

        // Output overview to create 1 file within a CDFS ISO
        // Block 0 - Write system area.
        // Block 16 - Write primary volume descriptor.
        // Block 17 - Write supplemental volume descriptor.
        // Block 18 - Write volume set terminator.
        // Block 19 - Write primary volume directory descriptor.
        // Block 20 - Write volume path table in little endian.
        // Block 21 - Write volume path table in big endian.
        // Block 22 - Write all directory and file information descriptors.
        // Block 23 - Write volume path table in little endian.
        // Block 24 - Write volume path table in big endian.
        // Block 25 - Write all source file contents buffered in blocks of 2048, filling unused space with zeros.

        // This buffer is allocated on the stack for fast access.
        // 2048 bytes is the size of 1 logical block.
        unsigned char BlockBuffer[CDFS_LOGICAL_BLOCK_SIZE] = { 0 }; // In C++ this will fill arrays or structs with zeros in memory.

        // Generate the CDFS system area and output to the stream. The total CDFS system area size is 32768 bytes.
        // The CDFS system area is 16 logical blocks, filled with zeros, with each block being 2048 bytes in size.
        this->WriteSystemArea(ISOFileOut, BlockBuffer);

        // We are now writing at logical block 16
        CDFS_VOLUME_DESCRIPTOR VolDesc = { 0 };
        VolDesc.Type = CDFS_VOLUME_TYPE_PRIMARY;
        memcpy(VolDesc.Identifier, CDFS_DEFAULT_IDENTIFIER, 5);
        VolDesc.Version = CDFS_VOLUME_VERSION;
        memset(VolDesc.SystemIdentifier, 0x20, 32);
        memset(VolDesc.VolumeIdentifier, 0x20, 32);
        memcpy(VolDesc.VolumeIdentifier, szVolumeLabel, strlen(szVolumeLabel));
        VolDesc.VolumeSpaceSizeLE = 26;
        VolDesc.VolumeSpaceSizeBE = __builtin_bswap32(26);
        VolDesc.VolumeSetSizeLE = 1;
        VolDesc.VolumeSetSizeBE = __builtin_bswap16(1);
        VolDesc.VolumeSequenceNumberLE = 1;
        VolDesc.VolumeSequenceNumberBE = __builtin_bswap16(1);
        VolDesc.LogicalBlockSizeLE = CDFS_LOGICAL_BLOCK_SIZE;
        VolDesc.LogicalBlockSizeBE = __builtin_bswap16(CDFS_LOGICAL_BLOCK_SIZE);
        VolDesc.PathTableSizeLE = 10;
        VolDesc.PathTableSizeBE = __builtin_bswap32(10);
        VolDesc.LPathTableLocationLE = 20;
        VolDesc.OptLPathTableLocationLE = 0;
        VolDesc.MPathTableLocationBE = __builtin_bswap32(21);
        VolDesc.OptMPathTableLocationBE = 0;
        VolDesc.RootDirEntry[0] = 34;
        VolDesc.RootDirEntry[2] = 19;
        VolDesc.RootDirEntry[9] = 19;
        VolDesc.RootDirEntry[11] = 8;
        VolDesc.RootDirEntry[16] = 8;
        VolDesc.RootDirEntry[18] = 120;
        VolDesc.RootDirEntry[19] = 7;
        VolDesc.RootDirEntry[20] = 27;
        VolDesc.RootDirEntry[21] = 20;
        VolDesc.RootDirEntry[22] = 23;
        VolDesc.RootDirEntry[23] = 59;
        VolDesc.RootDirEntry[24] = 8;
        VolDesc.RootDirEntry[25] = 2;
        VolDesc.RootDirEntry[28] = 1;
        VolDesc.RootDirEntry[31] = 1;
        VolDesc.RootDirEntry[32] = 1;
        memset(&VolDesc.VolumeSetIdentifier, 0x20, 128);
        memset(&VolDesc.PublisherIdentifier, 0x20, 128);
        memset(&VolDesc.DataPreparerIdentifier, 0x20, 128);
        memset(&VolDesc.ApplicationIdentifier, 0x20, 128);
        memset(&VolDesc.CopyrightIdentifier, 0x20, 38);
        memset(&VolDesc.AbstractIdentifier, 0x20, 36);
        memset(&VolDesc.BiblioIdentifier, 0x20, 37);
        memcpy(VolDesc.VolumeCreation.DateTime, "2020072720264871", 16);
        VolDesc.VolumeCreation.TimeZoneOffset = 8;
        memcpy(VolDesc.VolumeModification.DateTime, "2020072720264871", 16);
        VolDesc.VolumeModification.TimeZoneOffset = 8;
        memcpy(VolDesc.VolumeExpiration.DateTime, "1900010100000000", 16);
        VolDesc.VolumeExpiration.TimeZoneOffset = 8;
        memcpy(VolDesc.VolumeEffective.DateTime, "1900010100000000", 16);
        VolDesc.VolumeEffective.TimeZoneOffset = 8;
        VolDesc.FileStructVersion = 0x01;

        this->WriteVolumeDescriptor(ISOFileOut, &VolDesc);

        // Logical block 17.
        // Secondary volume description in big endian wide chars (2 bytes per character, zero first).
        VolDesc.Type = CDFS_VOLUME_TYPE_SUPPLEMENTARY;
        CharToWideCharBE(VolDesc.VolumeIdentifier, szVolumeLabel, 32);
        WideCharStringSpaceFill(VolDesc.SystemIdentifier, 32);
        VolDesc.Unused3[0] = 0x25;
        VolDesc.Unused3[1] = 0x2F;
        VolDesc.Unused3[2] = 0x45;
        VolDesc.LPathTableLocationLE = 23;
        VolDesc.MPathTableLocationBE = __builtin_bswap32(24);
        VolDesc.RootDirEntry[2] = 0x16;
        VolDesc.RootDirEntry[8] = 0x0;
        VolDesc.RootDirEntry[9] = 0x16;
        WideCharStringSpaceFill(VolDesc.VolumeSetIdentifier, 128);
        WideCharStringSpaceFill(VolDesc.PublisherIdentifier, 128);
        WideCharStringSpaceFill(VolDesc.DataPreparerIdentifier, 128);
        WideCharStringSpaceFill(VolDesc.ApplicationIdentifier, 128);
        unsigned char copyright[39] = "\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0\0";
        memcpy(&VolDesc.CopyrightIdentifier, copyright, 38);
        unsigned char abstract[37] = " \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0";
        memcpy(&VolDesc.AbstractIdentifier, abstract, 36);
        WideCharStringSpaceFill(VolDesc.BiblioIdentifier, 37);

        this->WriteVolumeDescriptor(ISOFileOut, &VolDesc);

        // Logical Block 18.
        this->WriteVolumeSetTerminator(ISOFileOut, CDFS_VOLUME_TYPE_TERMINATOR, CDFS_DEFAULT_IDENTIFIER, CDFS_VOLUME_VERSION);

        // Logical Block 19.
        // DateTime format specifications.
        // DateTime[0] = Number of years since 1900.
        // DateTime[1] = Month of the year from 1 to 12.
        // DateTime[2] = Day of the month from 1 to 31.
        // DateTime[4] = Hour of the day from 0 to 23.
        // DateTime[5] = Minute of the hour from 0 to 59.
        // DateTime[6] = Second of the minute from 0 to 59.
        // DateTime[7] = Offset from GMT in 15 minute intervals from -48 (West) to +52 (East).
        unsigned char RecordingDate[7] = { 120,7,27,20,23,59,8 };
        this->WriteVolumeDirectory(ISOFileOut, 34, 0, 19, CDFS_LOGICAL_BLOCK_SIZE, RecordingDate, 2, 0, 0, 1, 1);
        fwrite("\0", 1, 1, ISOFileOut);

        this->WriteVolumeDirectory(ISOFileOut, 34, 0, 19, CDFS_LOGICAL_BLOCK_SIZE, RecordingDate, 2, 0, 0, 1, 1);
        fwrite("\1", 1, 1, ISOFileOut);

        this->WriteVolumeDirectory(ISOFileOut, 44, 0, 25, 52, RecordingDate, 0, 0, 0, 1, 10);
        fwrite("SAMPLEF0;1", 1, 10, ISOFileOut);

        // Check and add padding up to fill the block.
        uint32_t LBALocation = 20 * CDFS_LOGICAL_BLOCK_SIZE;
        uint32_t BlockFillSize = LBALocation - ftell(ISOFileOut);
        if (BlockFillSize != 0) fwrite(BlockBuffer, 1, BlockFillSize, ISOFileOut);

        // C++ file output buffer is now "aligned" and output will now be in blocks.
        // Logical block 20.
        this->WriteVolumePathTable(ISOFileOut, 1, 0, 19, 1);
        fwrite(BlockBuffer, 1, 2040, ISOFileOut);

        // Logical block 21.
        this->WriteVolumePathTable(ISOFileOut, 1, 0, __builtin_bswap32(19), __builtin_bswap16(1));
        fwrite(BlockBuffer, 1, 2040, ISOFileOut);

        // Logical block 22.
        this->WriteVolumeDirectory(ISOFileOut, 34, 0, 22, CDFS_LOGICAL_BLOCK_SIZE, RecordingDate, 2, 0, 0, 1, 1);
        fwrite("\0", 1, 1, ISOFileOut);

        this->WriteVolumeDirectory(ISOFileOut, 34, 0, 22, CDFS_LOGICAL_BLOCK_SIZE, RecordingDate, 2, 0, 0, 1, 1);
        fwrite("\1", 1, 1, ISOFileOut);

        char szFileName[] = "SampleFile.txt;1"; // The terminating null will be removed on output.
        unsigned int FileNameSize = strlen(szFileName);
        this->WriteVolumeDirectory(ISOFileOut, 66, 0, 25, 52, RecordingDate, 0, 0, 0, 1, FileNameSize * 2);
        for (ByteCount = 0; ByteCount < FileNameSize; ++ByteCount) {
            fwrite("\0", 1, 1, ISOFileOut);
            fwrite(&szFileName[ByteCount], 1, 1, ISOFileOut);
        }

        // Not the prettiest or safest way of doing this.
        // Fills the rest of the logical block with zeros.
        LBALocation = 23 * CDFS_LOGICAL_BLOCK_SIZE;
        BlockFillSize = LBALocation - ftell(ISOFileOut);
        fwrite(BlockBuffer, 1, BlockFillSize, ISOFileOut);

        // Logical block 23.
        this->WriteVolumePathTable(ISOFileOut, 1, 0, 22, 1);
        fwrite(BlockBuffer, 1, 2040, ISOFileOut);

        // Logical block 24.
        this->WriteVolumePathTable(ISOFileOut, 1, 0, __builtin_bswap32(22), __builtin_bswap16(1));
        fwrite(BlockBuffer, 1, 2040, ISOFileOut);

        // Logical block 25, and the data from SampleFile.txt !
        char szData[] = "This is some sample text for our CDFS demo ISO file.";
        fwrite(szData, 1, strlen(szData), ISOFileOut);

        LBALocation = 26 * CDFS_LOGICAL_BLOCK_SIZE;
        BlockFillSize = LBALocation - ftell(ISOFileOut);
        fwrite(BlockBuffer, 1, BlockFillSize, ISOFileOut);

        fclose(ISOFileOut);

    } else {

        return false;

    }

    return true;
}