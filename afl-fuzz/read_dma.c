// #include <linux/init.h>
// #include <linux/module.h>
// #include <linux/compiler.h>
#include <scsi/sg.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#define DEVICE_NODE "/dev/sda1"
#define INQ_REPLY_LEN 96
#define INQ_CMD_CODE 0x12
#define INQ_CMD_LEN 6
#define DISPLAY
#define LBA_SIZE 512
#define CMD_LEN 16
#define BLOCK_MAX 65535
#define LBA_MAX (1<<30)
#define BUFLEN 3



// static int __init simple_init(void)
// {
// 	printk(KERN_ALERT"Hellow World\n");

// }

// static void __exit simple_exit(void)
// {
// 	printk(KERN_ALERT"Goodbye\n");
// }

int main(int argc, char *argv[])
{
    
	char buf[BUFLEN];
	FILE *file;

    file = fopen(argv[1], "r");
	if (!file) {
		fprintf(stderr, "[-] ERROR: Failed to open a file [%s]\n", argv[1]);
		return -1;
	}

	

	char c;
    unsigned char buffer[LBA_SIZE*1];
    unsigned char sense_buffer[32];
    sg_io_hdr_t io_hdr;
	int device_, k, ok;
	int i;
	int j;
	int lba = 1;
	int no_of_blocks = 1;
	for (j = 0; j<BUFLEN; j++) {
	    c = getc(file);
	    if (c == EOF)
	    	break;
	    buf[j] = c;
  	}
	unsigned char input1 = buf[0];
	unsigned char input2 = buf[1];
	unsigned char input3 = buf[2];
	unsigned char cmd_blk[CMD_LEN] = {input1, input2, input3, 0, 0, (1>>8), 
		1, (lba>>8), lba, (lba>>24), (lba>>16), 0, 0, 0x40, 0x25, 0};
	if ((device_ = open(DEVICE_NODE, O_RDWR)) < 0) {
		printf("%d\n", device_);
		return 1;
	}
	for (i = 0; i < LBA_SIZE; i++) {
		buffer[i] = 0;
	}
	printf("\n");

	memset(&io_hdr, 0, sizeof(sg_io_hdr_t));
    io_hdr.interface_id = 'S';
    io_hdr.cmd_len = sizeof(cmd_blk);
    /* io_hdr.iovec_count = 0; */  /* memset takes care of this */
    io_hdr.mx_sb_len = sizeof(sense_buffer);
    io_hdr.dxfer_direction = SG_DXFER_FROM_DEV;
    io_hdr.dxfer_len = LBA_SIZE;
    io_hdr.dxferp = buffer;
    io_hdr.cmdp = cmd_blk;
    io_hdr.sbp = sense_buffer;
    io_hdr.timeout = 20000; 

    if (ioctl(device_, SG_IO, &io_hdr) < 0) {
        perror("sg_simple0: Inquiry SG_IO ioctl error");
        return 1;
    }

    /* now for the error processing */
    #ifdef DISPLAY
	printf("********data buffer after ioctl***********\n");
	for(i=0; i<(LBA_SIZE * 1) ; i++){
		printf("%hx ",buffer[i]);
	}
	#endif
	printf("\n");

	printf("\n*********duration = %d\n", io_hdr.duration);

	return 1;

}



