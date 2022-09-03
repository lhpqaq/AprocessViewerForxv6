# 开发日志

小组成员：刘昊鹏 陈旭 王江涛

## 任务0：前导

> 2022-06-22

#### 配置环境

- 首先在 GitHub fork [xv6-k210](https://github.com/abrasumente233/xv6-k210) 项目；

- 安装环境依赖

  `sudo apt update && sudo apt install gcc-riscv64-unknown-elf` 安装64位 RISC-V 的编译器。

  `sudo apt install qemu-system-misc` ，安装 RISC-V 的 QEMU模拟器

  `sudo apt install python3` ，安装python3，因为我们的测试脚本是用 Python 写的；

  `sudo apt install dosfstools` ，安装 mkfs.vfat 工具；

- 运行

  `make fs` 生成一个 FAT32 的文件系统镜像，并将它保存在 `fs.img` ;

  修改makefile第一行，运行平台为qemu

  ```makefile
  #platform	:= k210
  platform	:= qemu
  ```

  `make run` ，在 QEMU 上运行 xv6-k210：

  <img src="D:\大三下\小学期\AprocessViewerForxv6\figs\image-20220903224104567.png" alt="image-20220903224104567" style="zoom:80%;" />

  
  
  ## 任务1：实现进程相关的系统调用
  
  > 2022-06-22 上午
  
  阅读代码和xv6的相关资料
  
  1. xv6 手册: https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf ，以及其中文译本： https://th0ar.gitbooks.io/xv6-chinese/content/
  2. build a OS（关于 xv6 的笔记）：https://xiayingp.gitbook.io/build_a_os/
  
  搜索在xv6中添加新的系统调用的方法
  
  >  2022-06-22 下午
  
  - 在xv6-user目录下
  
    - 在user.h文件中，添加新系统调用封装后的函数声明，假设其函数名为`getppid`。
  
      - ```c
        int getppid(void);
        ```
  
    - 在usys.pl文件末尾，添加如下行：
  
      - ```perl
        entry("getppid");
        ```
  
  - 在kernel目录下
  
    - 在include/sysnum.h文件中，添加新系统调用号的宏定义：
  
      - ```c
        #define SYS_getppid     27
        ```
  
      - 其中，27为新的合法系统调用号
  
    - 在syscall.c文件中，添加功能函数的声明，并更新系统调用表：
  
      - ```c
        extern uint64 sys_getppid(void);
        
        static uint64 (*syscalls[])(void) = {
            ......
            [SYS_getppid]    sys_getppid,
        };
        
        static char *sysnames[] = {
            ......
            [SYS_getppid]     "getppid",
        };
        ```
  
    - 在sysproc.c文件中，实现`sys_getppid`函数的功能如下：
  
      - ```c
        uint64 sys_getppid(void)
        {
          return myproc()->parent->pid;
        }
        ```
  
  - 在xv6-user目录下
  
    - 创建getppid.c文件，编写系统调用`getppid`的测试函数`test_getppid`如下：
  
      ```c
      int test_getppid()
      {
          //TEST_START(__func__);
          int pid = getppid();
          //assert(pid >= 0);
          printf("getppid success.\nppid = %d\n", pid);
          //TEST_END(__func__);
          return 0;
      }
      ```
    
  - 在根目录下
  
    - 在Makefile文件中加一行如下：
  
      ```makefile
      UPROGS=\
          $U/_init\
          $U/_sh\
          $U/_cat\
          ...
          $U/_getppid\
      ```

​	运行结果

​	<img src="D:\大三下\小学期\AprocessViewerForxv6\figs\image-20220903225404920.png" alt="image-20220903225404920"  />

> 2022-06-23

