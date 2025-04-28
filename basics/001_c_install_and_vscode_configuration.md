# C 的安装和 vscode 配置 （Windows 系统）

## 安装 C 环境

### 1. 下载并安装 MSYS2

- 官方下载地址：[https://www.msys2.org/](https://www.msys2.org/)
- 下载完成后安装。  
  若安装过程中卡在 50%，可取消安装，清空安装文件夹，**断开网络后重新安装**。

### 2. 安装 GCC 编译器

打开 MSYS2 终端，依次输入以下命令：

- 更新包数据库并升级所有已安装的软件。

```bash
pacman -Syu
```

- 再次检查是否有更新，无需实际操作。

```bash
pacman -Su
```

- 安装开发工具: 
    - `--need` 只安装未安装的软件包
    - `base-devel`：常用基础开发工具（如 make, gcc 辅助工具）
    - `mingw-w64-x86_64-toolchain`：完整的 64 位 C/C++ 开发工具链

```bash
pacman -S --need base-devel mingw-w64-x86_64-toolchain
```

一路同意。其中，`pacman` 是 MSYS2 的包管理器，使用 MSYS2 的目的为下载安装 gcc。

### 3. 配置环境变量

- 打开安装好的 MSYS2 文件夹，进入 `mingw64/bin` 目录。
- 将该文件夹路径添加到系统环境变量 `Path` 中。

完成后，打开 `cmd`，输入以下命令检查：

```bash
gcc -v
```

若看到类似以下输出，则说明安装成功：

```plaintext
Using built-in specs.
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=D:/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/15.1.0/lto-wrapper.exe
Target: x86_64-w64-mingw32
Configured with: ../gcc-15.1.0/configure ...
Thread model: posix
Supported LTO compression algorithms: zlib zstd
gcc version 15.1.0 (Rev1, Built by MSYS2 project)
```

---

## 安装配置 VSCode

1. 打开 VSCode。
2. 在扩展（Extensions）中搜索并安装以下插件：
   - `C/C++` （由 Microsoft 发布）
   - `Code Runner` （快速运行 C/C++ 程序）
3. Ctrl + Shift + P 搜索 C/C++, 进入 Edit Configurations (UI):
  - 修改 `Compiler path` 为 gcc 的路径
  - 修改 `IntelliSense mode` 为 "windows-gcc-x64"
  - `Terminal > Configuration Tasks...`, 选择 "C/C++: gcc.exe"
  - 操作后生成 `.vscode` 文件夹

