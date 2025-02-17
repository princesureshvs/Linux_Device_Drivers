#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define DEVICE "/dev/ioctl_device"  // Path to the device file in /dev

// IOCTL Command Definitions
#define IOCTL_GET_DATA _IOR('i', 1, int)  // Command to get data from the kernel (input: none, output: int)
#define IOCTL_SET_DATA _IOW('i', 2, int)  // Command to set data to the kernel (input: int, output: none)

int main()
{
    int fd, result;    // File descriptor for the device and result of the IOCTL call
    int data;          // Variable to store the data being sent or received

    // Open the device file with read and write access
    fd = open(DEVICE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");  
        return -1;  
    }

    // Set data using IOCTL
    data = 42;  // Set the data to be sent to the kernel
    result = ioctl(fd, IOCTL_SET_DATA, &data);  // Send the data to the kernel using IOCTL
    if (result < 0) {
        perror("Failed to set data");  
        close(fd);  
        return -1;  
    }
    printf("Data sent to kernel: %d\n", data);  // Print the data sent to the kernel

    // Get data using IOCTL
    result = ioctl(fd, IOCTL_GET_DATA, &data);  // Get data from the kernel using IOCTL
    if (result < 0) {
        perror("Failed to get data");  
        close(fd);  
        return -1;  
    }
    printf("Data received from kernel: %d\n", data);  

    close(fd);  
    return 0;   
}
