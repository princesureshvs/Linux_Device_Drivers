# Linux Device Drivers  

This repository contains my work on **Linux Device Drivers**, covering fundamental driver development concepts, kernel-space programming, and interaction with hardware. The focus is on **low-level embedded driver implementation** using **C and the Linux kernel**.  

---

## 📁 Repository Structure  

### 1️⃣ **Programs/**  
Contains various Linux device driver implementations, categorized as follows:  

- **Basic Programs**  
  - `basic_calculator` – Simple calculator using kernel space.  
  - `Basic_dd` – Basic device driver implementation.  
  - `Basic_uart_driver` – UART (Serial Communication) driver.  

- **Driver Implementations**  
  - `gpio_driver` – General Purpose Input/Output (GPIO) driver.  
  - `Led_using_sysfs` – LED control using the sysfs interface.  
  - `keyboard_interrupt` – Handling keyboard interrupts in the kernel.  
  - `Driver_using_ioctl` – Implementing **IOCTL** (Input/Output Control) in drivers.  
  - `gpio_dts` – Device Tree-based GPIO driver.  
  - `send_int_to_kernel` – Sending integers from user space to kernel space.  
  - `circular_buffer` – Implementing circular buffer in the kernel.  
  - `pir_sensor` – PIR sensor interfacing in Linux.  
  - `wiringpi` – Experiments with WiringPi for GPIO control.  
  - `Debugfs_simple_program` – Debugfs-based driver example.  
  - `Procfs_fileops` – Using procfs for driver interaction.  
  - `Vmalloc_Kmalloc` – Memory allocation using `kmalloc` and `vmalloc`.  

### 2️⃣ **Reference_Notes/**  
This folder contains study materials, research notes, and reference documents for understanding **Linux kernel internals, driver development, and debugging techniques**.  

---

## 🚀 Work in Progress  
This repository is continuously updated as I explore more topics in **Linux Device Drivers**. Contributions, suggestions, and discussions are welcome!  
