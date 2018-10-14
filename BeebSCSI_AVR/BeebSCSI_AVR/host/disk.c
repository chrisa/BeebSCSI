#include "../fatfs/diskio.h"
#include <fcntl.h>
#include <unistd.h>

#define SECTOR_SIZE 512

int fd;

DSTATUS disk_initialize (BYTE pdrv)
{
        fd = open("BeebSCSI_FAT.dmg", O_RDWR);
        if (fd == -1)
                return STA_NOINIT;
        return 0;
}

DSTATUS disk_status (BYTE pdrv)
{
        return 0;
}

DRESULT disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count)
{
        if ((lseek(fd, sector * SECTOR_SIZE, SEEK_SET)) == -1)
                return RES_ERROR;

        for (int i = 0; i < count; i++)
        {
                BYTE *ptr = buff + (SECTOR_SIZE * i);
                if ((read(fd, ptr, SECTOR_SIZE)) != SECTOR_SIZE)
                        return RES_ERROR;
        }

        return RES_OK;
}

DRESULT disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count)
{
        if ((lseek(fd, sector * SECTOR_SIZE, SEEK_SET)) == -1)
                return RES_ERROR;

        for (int i = 0; i < count; i++)
        {
                const BYTE *ptr = buff + (SECTOR_SIZE * i);
                if ((write(fd, ptr, SECTOR_SIZE)) != SECTOR_SIZE)
                        return RES_ERROR;
        }

        return RES_OK;
}

DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff)
{
        switch (cmd)
        {
        case GET_SECTOR_COUNT:
                *(DWORD*) buff = 1171938; // ...
                break;
        default:
                break;
        }
        return RES_OK;
}
