#include <iostream>
#include "ISO_Creator_CDFS.h"

int main(int argc, char** argv)
{
    // Very easy implementation using a class.
    ISOCreatorCDFS CDFSCreator;
    CDFSCreator.CreateSingleFileISO("Sample.iso", "OneSampleCD");

    return 0;
}