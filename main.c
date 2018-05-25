#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#define I2C_PATH        "/dev/i2c-0"

unsigned int atohex(const char*);
int print_help();

int main(int argc, char *argv[])
{
    struct i2c_rdwr_ioctl_data i2c_iodata;
    unsigned char opt;
    int fd, len, rval = 0;
    unsigned int addr1, addr2, value = 0, r_flag = 0, w_flag = 0, v_flag = 0, b_flag = 0;
    char *pos;

    fd = open(I2C_PATH, O_RDWR);

    while((opt = getopt(argc, argv, "r:w:v:b")) != 255){
        switch(opt){
            case 'r':
                r_flag++;
                len = strlen(optarg);
                if(len < 3 || len > 6) print_help();
                if(0 != strncmp(optarg, "0x", 2)) print_help();
                pos = optarg + 2;
                if(len >= 3 && len <= 4){
                    addr1 = 0;
                    addr2 = atohex(pos);
                }
                else{
                    pos += (len - 4);
                    addr2 = atohex(pos);
                    *pos = 0;
                    pos -= (len - 4);
                    addr1 = atohex(pos);
                }
                fprintf(stdout, "f:[%s], addr1 = 0x%02x, addr2 = 0x%02x\n", optarg, addr1, addr2);
                break;
            case 'w':
                w_flag++;
                len = strlen(optarg);
                if(len < 3 || len > 6) print_help();
                if(0 != strncmp(optarg, "0x", 2)) print_help();
                pos = optarg + 2;
                if(len >= 3 && len <= 4){
                    addr1 = 0;
                    addr2 = atohex(pos);
                }
                else{
                    pos += (len - 4);
                    addr2 = atohex(pos);
                    *pos = 0;
                    pos -= (len - 4);
                    addr1 = atohex(pos);
                }
                fprintf(stdout, "f:[%s], addr1 = 0x%02x, addr2 = 0x%02x\n", optarg, addr1, addr2);
                break;
            case 'v':
                v_flag++;
                len = strlen(optarg);
                if(len < 3 || len > 4) print_help();
                if(0 != strncmp(optarg, "0x", 2)) print_help();
                pos = optarg + 2;
                value = atohex(pos);
                fprintf(stdout, "v:[%s], value = 0x%02x\n", optarg, value);
                break;
            case 'b':
                b_flag++;
                break;
            default:
                fprintf(stderr, "wrong parameters\n");
                fprintf(stdout, "opt = %d, optarg:[%s]\n", opt, optarg);
                print_help();
        }
    }

    if(b_flag){
        /*
        i2c_iodata.nmsgs = 1;
        i2c_iodata.msgs = (struct i2c_msg*)(malloc(sizeof(struct i2c_msg)*i2c_iodata.nmsgs));

        i2c_iodata.msgs[0].addr = 0x3b;
        i2c_iodata.msgs[0].flags = 0;
        i2c_iodata.msgs[0].len = 3;
        i2c_iodata.msgs[0].buf = (unsigned char*)malloc(3);
        i2c_iodata.msgs[0].buf[0] = 0x40;
        i2c_iodata.msgs[0].buf[1] = 0x00;
        i2c_iodata.msgs[0].buf[2] = 0x0e;
    
        rval = ioctl(fd, I2C_RDWR, &i2c_iodata);
        if(rval < 0){
            fprintf(stderr, "error during ioctl from I2C_RDWR, error code:%d\n", rval);
            exit(1);
        }
        fprintf(stdout, "config clock source success\n");
        */

        i2c_iodata.nmsgs = 1;
        i2c_iodata.msgs = (struct i2c_msg*)(malloc(sizeof(struct i2c_msg)*i2c_iodata.nmsgs));

        i2c_iodata.msgs[0].addr = 0x3b;
        i2c_iodata.msgs[0].flags = 0;
        i2c_iodata.msgs[0].len = 8;
        i2c_iodata.msgs[0].buf = (unsigned char*)malloc(8);
        i2c_iodata.msgs[0].buf[0] = 0x40;
        i2c_iodata.msgs[0].buf[1] = 0x02;
        i2c_iodata.msgs[0].buf[2] = 0x02;
        i2c_iodata.msgs[0].buf[3] = 0x71;
        i2c_iodata.msgs[0].buf[4] = 0x02;
        i2c_iodata.msgs[0].buf[5] = 0x3c;
        i2c_iodata.msgs[0].buf[6] = 0x21;
        i2c_iodata.msgs[0].buf[7] = 0x01;
    
        rval = ioctl(fd, I2C_RDWR, &i2c_iodata);
        if(rval < 0){
            fprintf(stderr, "error during ioctl from I2C_RDWR, error code:%d\n", rval);
            exit(1);
        }
    
        fprintf(stdout, "config pll success\n");
    }
    else if(r_flag){
        if(addr2 == 0x02)
        {
            i2c_iodata.nmsgs = 2;
            i2c_iodata.msgs = (struct i2c_msg*)(malloc(sizeof(struct i2c_msg)*i2c_iodata.nmsgs));
    
            i2c_iodata.msgs[0].addr = 0x3b;
            i2c_iodata.msgs[0].flags = 0;
            i2c_iodata.msgs[0].len = 2;
            i2c_iodata.msgs[0].buf = (unsigned char*)malloc(2);
            i2c_iodata.msgs[0].buf[0] = addr1;
            i2c_iodata.msgs[0].buf[1] = addr2;
    
            i2c_iodata.msgs[1].addr = 0x3b;
            i2c_iodata.msgs[1].flags = 1;
            i2c_iodata.msgs[1].len = 6;
            i2c_iodata.msgs[1].buf = (unsigned char*)malloc(6);
            i2c_iodata.msgs[1].buf[0] = 0x0;
            i2c_iodata.msgs[1].buf[1] = 0x0;
            i2c_iodata.msgs[1].buf[2] = 0x0;
            i2c_iodata.msgs[1].buf[3] = 0x0;
            i2c_iodata.msgs[1].buf[4] = 0x0;
            i2c_iodata.msgs[1].buf[5] = 0x0;
    
            rval = ioctl(fd, I2C_RDWR, &i2c_iodata);
            if(rval < 0){
                fprintf(stderr, "error during ioctl from I2C_RDWR, error code:%d\n", rval);
                exit(1);
            }
    
            fprintf(stdout, "value of address 0x%02x%02x is 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x \n", addr1, addr2, i2c_iodata.msgs[1].buf[0],i2c_iodata.msgs[1].buf[1], i2c_iodata.msgs[1].buf[2], i2c_iodata.msgs[1].buf[3], i2c_iodata.msgs[1].buf[4], i2c_iodata.msgs[1].buf[5]);
        }
        else{
            i2c_iodata.nmsgs = 2;
            i2c_iodata.msgs = (struct i2c_msg*)(malloc(sizeof(struct i2c_msg)*i2c_iodata.nmsgs));
    
            i2c_iodata.msgs[0].addr = 0x3b;
            i2c_iodata.msgs[0].flags = 0;
            i2c_iodata.msgs[0].len = 2;
            i2c_iodata.msgs[0].buf = (unsigned char*)malloc(2);
            i2c_iodata.msgs[0].buf[0] = addr1;
            i2c_iodata.msgs[0].buf[1] = addr2;
    
            i2c_iodata.msgs[1].addr = 0x3b;
            i2c_iodata.msgs[1].flags = 1;
            i2c_iodata.msgs[1].len = 1;
            i2c_iodata.msgs[1].buf = (unsigned char*)malloc(1);
            i2c_iodata.msgs[1].buf[0] = 0x0;
    
            rval = ioctl(fd, I2C_RDWR, &i2c_iodata);
            if(rval < 0){
                fprintf(stderr, "error during ioctl from I2C_RDWR, error code:%d\n", rval);
                exit(1);
            }
    
            fprintf(stdout, "value of address 0x%02x%02x is 0x%02x\n", addr1, addr2, i2c_iodata.msgs[1].buf[0]);
        }
    }
    else if(w_flag && v_flag){
        i2c_iodata.nmsgs = 1;
        i2c_iodata.msgs = (struct i2c_msg*)(malloc(sizeof(struct i2c_msg)*i2c_iodata.nmsgs));
    
        i2c_iodata.msgs[0].addr = 0x3b;
        i2c_iodata.msgs[0].flags = 0;
        i2c_iodata.msgs[0].len = 3;
        i2c_iodata.msgs[0].buf = (unsigned char*)malloc(3);
        i2c_iodata.msgs[0].buf[0] = addr1;
        i2c_iodata.msgs[0].buf[1] = addr2;
        i2c_iodata.msgs[0].buf[2] = value;
    
        rval = ioctl(fd, I2C_RDWR, &i2c_iodata);
        if(rval < 0){
            fprintf(stderr, "error during ioctl from I2C_RDWR, error code:%d\n", rval);
            exit(1);
        }
    
        fprintf(stdout, "write 0x%02x to 0x%02x%02x success\n", value, addr1, addr2);
    }
    else print_help();

    close(fd);
    return 0;
}

int print_help()
{
    fprintf(stderr, "I'm in %s\n", __func__);
    exit(0);
    return 0;
}

unsigned int atohex(const char *str)
{
    unsigned int value;

    sscanf(str, "%x", &value);
    
    return value;
}
