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
    

    unsigned char buffer[LBA_SIZE*2];
    unsigned char sense_buffer[32];
    sg_io_hdr_t io_hdr;
	int device_, k, ok;
	int i;
	int lba;
	int no_of_blocks;

	printf("enter the LBA you want to read: ");
	scanf("%d",&lba);

	if(lba > LBA_MAX){
		printf("LBA larger than %d not allowed with this version\n",LBA_MAX);
		return 1;
	}

	printf("Enter the NUMBER OF BLOCK you want to read: ");
	scanf("%d",&no_of_blocks);

	if((no_of_blocks > BLOCK_MAX)||(no_of_blocks <= 0) ){
		printf("it is not possible to read more than %d BLOCKS\n",BLOCK_MAX);
		printf("we are reading 1 BLOCK for you\n");
		no_of_blocks = 1;
	}
	unsigned char cmd_blk[CMD_LEN] = {0x85, 0x0D, 0x26, 0, 0, (no_of_blocks>>8), 
		no_of_blocks, (lba>>8), lba, (lba>>24), (lba>>16), 0, 0, 0x40, 0x25, 0};
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
    io_hdr.dxfer_len = LBA_SIZE * no_of_blocks;
    io_hdr.dxferp = buffer;
    io_hdr.cmdp = cmd_blk;
    io_hdr.sbp = sense_buffer;
    io_hdr.timeout = 20000; 

    printf("********data buffer initializing***********\n");
	for(i=0;i<512*no_of_blocks;i++){
		buffer[i] = 0;
	}
	buffer[2] = 'S';
	buffer[3] = 'E';
	buffer[4] = 'A';
	buffer[5] = 'N';

	printf("*****data buffer after initializing********\n");
	for(i=0;i<512*no_of_blocks;i++){
		printf("%hx ",buffer[i]);
	}
	printf("\n");

    if (ioctl(device_, SG_IO, &io_hdr) < 0) {
        perror("sg_simple0: Inquiry SG_IO ioctl error");
        return 1;
    }

    /* now for the error processing */
	printf("write successful\n");
	
	printf("\n*********duration = %d\n", io_hdr.duration);

	return 1;

}



