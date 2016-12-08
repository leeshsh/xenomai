/***************************************************************************
 *   Copyright (C) 2007 by trem (Philippe Reynes)                          *
 *   tremyfr@yahoo.fr                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/**
 * This is an example that shows how RTDM devices can be used
 * with a user space program.
 *
 * The device mydriver stores data that you write into.
 * When you read from this device, previously stored data is returned,
 * and the internal buffer is erased.
 *
 *
 * To test this application, you just need to:
 *
 * $ export LD_LIBRARY_PATH=<path of xenomai>/lib
 * $ insmod mydriver.ko
 * $ ./mydrivertest
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <rtdm/rtdm.h>

#define DEVICE_NAME		"mydriver"

int main(int argc, char *argv)
{
	char buf[1024];
	ssize_t size;
	int device;
	int ret;

	/* open the device */
	device = rt_dev_open(DEVICE_NAME, 0);
	if (device < 0) {
		printf("ERROR : can't open device %s (%s)\n",
		       DEVICE_NAME, strerror(-device));
		fflush(stdout);
		exit(1);
	}
	
	//得到缓存区最大值
	int max_size;
        size = rt_dev_ioctl (device, 0x01,&max_size );
        printf("get max data size of device %s\t: %d bytes\n", DEVICE_NAME, max_size);
	
	int  valid_size;
        size = rt_dev_ioctl (device, 0x02,&valid_size );
        printf("get valid data size of device %s\t: %d bytes\n", DEVICE_NAME, valid_size);


	/* first write */
	size = rt_dev_write (device, (const void *)buf, strlen(buf) + 1);
	printf("Write from device %s\t: %d bytes\n", DEVICE_NAME, size);

	size = rt_dev_ioctl (device, 0x02,&valid_size );
        printf("get valid data size of device %s\t: %d bytes\n", DEVICE_NAME, valid_size);	

	/* first read */
	size = rt_dev_read (device, (void *)buf, 1024);
	printf("Read in device %s\t: %s\n", DEVICE_NAME, buf);

	/* second read */
	size = rt_dev_read (device, (void *)buf, 1024);
	printf("Read in device again%s\t: %d bytes\n", DEVICE_NAME, size);

	/* close the device */
	ret = rt_dev_close(device);
	if (ret < 0) {
		printf("ERROR : can't close device %s (%s)\n",
		       DEVICE_NAME, strerror(-ret));
		fflush(stdout);
		exit(1);
	}

	return 0;
}
