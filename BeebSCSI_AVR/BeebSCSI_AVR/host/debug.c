#include "../beebscsi.h"

bool debugFlag_filesystem = true;
bool debugFlag_scsiCommands = true;
bool debugFlag_scsiBlocks = false;
bool debugFlag_scsiFcodes = true;
bool debugFlag_scsiState = true;
bool debugFlag_fatfs = true;

void debugString_P(const char *s) {
        fprintf(stderr, "%s", s);
}

void debugString(const char *s) {
        fprintf(stderr, "%s", s);
}

void debugStringInt8Hex_P(const char *s, uint8_t integerValue, bool newLine)
{
        fprintf(stderr, "%s", s);
        if (newLine) fprintf(stderr, "0x%02x\r\n", integerValue);
        else fprintf(stderr, "0x%02x", integerValue);
}

void debugStringInt16_P(const char *s, uint16_t integerValue, bool newLine)
{
        fprintf(stderr, "%s", s);
        if (newLine) fprintf(stderr, "%u\r\n", integerValue);
        else fprintf(stderr, "%u", integerValue);
}

void debugStringInt32_P(const char *s, uint32_t integerValue, bool newLine)
{
        fprintf(stderr, "%s", s);
        if (newLine) fprintf(stderr, "%lu\r\n", (unsigned long) integerValue);
        else fprintf(stderr, "%lu", (unsigned long) integerValue);
}

void debugSectorBufferHex(uint8_t *buffer, uint16_t numberOfBytes)
{
        uint16_t i = 0;
        uint16_t index = 16;
        uint16_t width = 16; // Width of output in bytes

        for (uint16_t byteNumber = 0; byteNumber < numberOfBytes; byteNumber += 16)
        {
                for (i = 0; i < index; i++)
                {
                        fprintf(stderr, "%02x ", buffer[i + byteNumber]);
                }
                for (uint16_t spacer = index; spacer < width; spacer++)
                fprintf(stderr, "        ");

                fprintf(stderr, ": ");

                for (i=0; i < index; i++)
                {
                        if (buffer[i + byteNumber] < 32 || buffer[i + byteNumber] >126) fprintf(stderr, ".");
                        else fprintf(stderr, "%c",buffer[i + byteNumber]);
                }

                fprintf(stderr, "\r\n");
        }

        fprintf(stderr, "\r\n");
}

void debugLunDescriptor(uint8_t *buffer)
{
        debugString_P(PSTR("File system: LUN Descriptor contents:\r\n"));

        // The first 4 bytes are the Mode Select Parameter List (ACB-4000 manual figure 5-18)
        debugString_P(PSTR("File system: Mode Select Parameter List:\r\n"));
        debugStringInt16_P(PSTR("File system:   Reserved (0) = "), buffer[0], true);
        debugStringInt16_P(PSTR("File system:   Reserved (0) = "), buffer[1], true);
        debugStringInt16_P(PSTR("File system:   Reserved (0) = "), buffer[2], true);
        debugStringInt16_P(PSTR("File system:   Length of Extent Descriptor List (8) = "), buffer[3], true);

        // The next 8 bytes are the Extent Descriptor list (there can only be one of these
        // and it's always 8 bytes) (ACB-4000 manual figure 5-19)
        debugString_P(PSTR("File system: Extent Descriptor List:\r\n"));
        debugStringInt16_P(PSTR("File system:   Density code = "), buffer[4], true);
        debugStringInt16_P(PSTR("File system:   Reserved (0) = "), buffer[5], true);
        debugStringInt16_P(PSTR("File system:   Reserved (0) = "), buffer[6], true);
        debugStringInt16_P(PSTR("File system:   Reserved (0) = "), buffer[7], true);
        debugStringInt16_P(PSTR("File system:   Reserved (0) = "), buffer[8], true);
        debugStringInt32_P(PSTR("File system:   Block size = "), ((uint32_t)buffer[9] << 16) +
        ((uint32_t)buffer[10] << 8) + (uint32_t)buffer[11], true);

        // The next 12 bytes are the Drive Parameter List (ACB-4000 manual figure 5-20)
        debugString_P(PSTR("File system: Drive Parameter List:\r\n"));
        debugStringInt16_P(PSTR("File system:   List format code = "), buffer[12], true);
        debugStringInt16_P(PSTR("File system:   Cylinder count = "), (buffer[13] << 8) + buffer[14], true);
        debugStringInt16_P(PSTR("File system:   Data head count = "), buffer[15], true);
        debugStringInt16_P(PSTR("File system:   Reduced write current cylinder = "), (buffer[16] << 8) + buffer[17], true);
        debugStringInt16_P(PSTR("File system:   Write pre-compensation cylinder = "), (buffer[18] << 8) + buffer[19], true);
        debugStringInt16_P(PSTR("File system:   Landing zone position = "), buffer[20], true);
        debugStringInt16_P(PSTR("File system:   Step pulse output rate code = "), buffer[21], true);

        // Note:
        //
        // The drive size (actual data storage) is calculated by the following formula:
        //
        // tracks = heads * cylinders
        // sectors = tracks * 33
        // (the '33' is because SuperForm uses a 2:1 interleave format with 33 sectors per
        // track (F-2 in the ACB-4000 manual))
        // bytes = sectors * block size (block size is always 256 bytes)
}
