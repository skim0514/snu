import fcntl
import struct
import ctypes


class AtaCmd(ctypes.Structure):
    """ATA Command Pass-Throughhttp://www.t10.org/ftp/t10/document.04/04-262r8.pdf"""

    _fields_ = [
        ('opcode', ctypes.c_ubyte),
        ('protocol', ctypes.c_ubyte),
        ('flags', ctypes.c_ubyte),
        ('features', ctypes.c_ubyte),
        ('sector_count', ctypes.c_ubyte),
        ('lba_low', ctypes.c_ubyte),
        ('lba_mid', ctypes.c_ubyte),
        ('lba_high', ctypes.c_ubyte),
        ('device', ctypes.c_ubyte),
        ('command', ctypes.c_ubyte),
        ('reserved', ctypes.c_ubyte),
        ('control', ctypes.c_ubyte)]


class SgioHdr(ctypes.Structure):
    """<scsi/sg.h> sg_io_hdr_t."""

    _fields_ = [
        ('interface_id', ctypes.c_int),
        ('dxfer_direction', ctypes.c_int),
        ('cmd_len', ctypes.c_ubyte),
        ('mx_sb_len', ctypes.c_ubyte),
        ('iovec_count', ctypes.c_ushort),
        ('dxfer_len', ctypes.c_uint),
        ('dxferp', ctypes.c_void_p),
        ('cmdp', ctypes.c_void_p),
        ('sbp', ctypes.c_void_p),
        ('timeout', ctypes.c_uint),
        ('flags', ctypes.c_uint),
        ('pack_id', ctypes.c_int),
        ('usr_ptr', ctypes.c_void_p),
        ('status', ctypes.c_ubyte),
        ('masked_status', ctypes.c_ubyte),
        ('msg_status', ctypes.c_ubyte),
        ('sb_len_wr', ctypes.c_ubyte),
        ('host_status', ctypes.c_ushort),
        ('driver_status', ctypes.c_ushort),
        ('resid', ctypes.c_int),
        ('duration', ctypes.c_uint),
        ('info', ctypes.c_uint)]


def SwapString(str):
    """Swap 16 bit words within a string.

  String data from an ATA IDENTIFY appears byteswapped, even on little-endian
  achitectures. I don't know why. Other disk utilities I've looked at also
  byte-swap strings, and contain comments that this needs to be done on all
  platforms not just big-endian ones. So... yeah.
  """
    s = []
    for x in range(0, len(str) - 1, 2):
        s.append(str[x + 1])
        s.append(str[x])
    return "".join(s.decode()).strip()


def GetDriveIdSgIo(dev):
    """Return information from interrogating the drive.

  This routine issues a SG_IO ioctl to a block device, which
  requires either root privileges or the CAP_SYS_RAWIO capability.

  Args:
    dev: name of the device, such as 'sda' or '/dev/sda'

  Returns:
    (serial_number, fw_version, model) as strings
  """

    if dev[0] != '/':
        dev = '/dev/' + dev
    ata_cmd = AtaCmd(opcode=0xa1,  # ATA PASS-THROUGH (12)
                     protocol=4 << 1,  # PIO Data-In
                     # flags field
                     # OFF_LINE = 0 (0 seconds offline)
                     # CK_COND = 1 (copy sense data in response)
                     # T_DIR = 1 (transfer from the ATA device)
                     # BYT_BLOK = 1 (length is in blocks, not bytes)
                     # T_LENGTH = 2 (transfer length in the SECTOR_COUNT field)
                     flags=0x2e,
                     features=0, sector_count=0,
                     lba_low=0, lba_mid=0, lba_high=0,
                     device=0,
                     command=0xec,  # IDENTIFY
                     reserved=0, control=0)
    ASCII_S = 83
    SG_DXFER_FROM_DEV = -3
    sense = ctypes.c_buffer(64)
    identify = ctypes.c_buffer(512)
    sgio = SgioHdr(interface_id=ASCII_S, dxfer_direction=SG_DXFER_FROM_DEV,
                   cmd_len=ctypes.sizeof(ata_cmd),
                   mx_sb_len=ctypes.sizeof(sense), iovec_count=0,
                   dxfer_len=ctypes.sizeof(identify),
                   dxferp=ctypes.cast(identify, ctypes.c_void_p),
                   cmdp=ctypes.addressof(ata_cmd),
                   sbp=ctypes.cast(sense, ctypes.c_void_p), timeout=3000,
                   flags=0, pack_id=0, usr_ptr=None, status=0, masked_status=0,
                   msg_status=0, sb_len_wr=0, host_status=0, driver_status=0,
                   resid=0, duration=0, info=0)
    SG_IO = 0x2285  # <scsi/sg.h>
    with open(dev, 'r') as fd:
        if fcntl.ioctl(fd, SG_IO, sgio) != 0:
            print("fcntl failed")
            return None
        if ord(sense[0]) != 0x72 or ord(sense[8]) != 0x9 or ord(sense[9]) != 0xc:
            return None
        # IDENTIFY format as defined on pg 91 of
        # http://t13.org/Documents/UploadedDocuments/docs2006/D1699r3f-ATA8-ACS.pdf
        serial_no = SwapString(identify[20:40])
        fw_rev = SwapString(identify[46:53])
        model = SwapString(identify[54:93])
        return (serial_no, fw_rev, model)


if __name__ == '__main__':
    print(GetDriveIdSgIo("/dev/sda1"))
