本文主要介绍如何快速运行腾讯云 即时通信 IM Demo（Unreal Engine）。

>! 目前 支持Windows、MacOs、ios、Android。

## 环境要求
- 建议Unreal Engine 4.27.1 及以上版本。
- **Android 端开发：**
  - Android Studio版本4.0及以上版本。
  - Visual Studio 2017 15.6版或更高。
  - 只支持真机调试
- **iOS & macOS 端开发：**
  - Xcode 11.0及以上版本。
  - osx 系统版本要求 10.11 及以上版本
  - 请确保您的项目已设置有效的开发者签名。
- **Windows 开发：**
    - 操作系统：Windows 7 SP1 或更高的版本（基于 x86-64 的 64 位操作系统）。
    - 磁盘空间：除安装 IDE 和一些工具之外还应有至少 1.64 GB 的空间。
    - 安装 [Visual Studio 2019](https://visualstudio.microsoft.com/zh-hans/downloads/)。

## 前提条件
您已 [注册腾讯云](https://cloud.tencent.com) 账号，并完成实名认证。

## 操作步骤
[](id:step1)
### 步骤1：创建新的应用
1. 登录 [即时通信 IM 控制台](https://console.cloud.tencent.com/im)。
>?如果您已有应用，请记录其 SDKAppID 并 [获取密钥信息](#step2)。
>同一个腾讯云帐号，最多可创建300个即时通信 IM 应用。若已有300个应用，您可以先 [停用并删除](https://cloud.tencent.com/document/product/269/32578#.E5.81.9C.E7.94.A8.2F.E5.88.A0.E9.99.A4.E5.BA.94.E7.94.A8) 无需使用的应用后再创建新的应用。**应用删除后，该 SDKAppID 对应的所有数据和服务不可恢复，请谨慎操作。**
>
2. 单击**+添加新应用**。
3. 在**创建应用**对话框中输入您的应用名称，单击**确定**。
  创建完成后，可在控制台总览页查看新建应用的状态、业务版本、SDKAppID、创建时间以及到期时间。请记录 SDKAppID 信息。
  ![](https://main.qcloudimg.com/raw/2753962b67754a9ebb2a2a5b8042f2ef.png)

[](id:step2)

## 步骤2：获取密钥信息

1. 单击目标应用卡片，进入应用的基础配置页面，
2. 在**基本信息**区域，单击**显示密钥**，复制并保存密钥信息。
>!请妥善保管密钥信息，谨防泄露。

[](id:step3)
### 步骤3：配置 Demo 工程文件
1. 下载即时通信 IM Demo 工程，具体下载地址请参见 [Demo 下载](https://github.com/tencentyun/IMUnrealEngine)（有疑问可加入QQ群号：764231117 咨询）。
2. 找到并打开 `/IM_Demo/Source/debug/include/DebugDefs.h` 文件。
3. 设置 `DebugDefs.h` 文件中的相关参数：
<ul><li/>SDKAPPID：默认为 0 ，请设置为实际的 SDKAppID。
	<li/>SECRETKEY：默认为 "" ，请设置为实际的密钥信息。</ul>
	<img src="https://imgcache.qq.com/operation/dianshi/other/UE4.6a419c2e7f7085671529d3694cb99458527c2970.png"/>

>?
>- 本文提到的生成 UserSig 的方案是在客户端代码中配置 SECRETKEY，该方法中 SECRETKEY 很容易被反编译逆向破解，一旦您的密钥泄露，攻击者就可以盗用您的腾讯云流量，因此**该方法仅适合本地跑通 Demo 和功能调试**。
>- 正确的 UserSig 签发方式是将 UserSig 的计算代码集成到您的服务端，并提供面向 App 的接口，在需要 UserSig 时由您的 App 向业务服务器发起请求获取动态 UserSig。更多详情请参见 [服务端生成 UserSig](https://cloud.tencent.com/document/product/647/17275#Server)。

[](id:step4)
### 步骤4：编译打包运行
双击打开 `/IM_Demo/IM_Demo.uproject`，编译运行调试。

# macOS端
1. File -> Package Project -> Mac

# Windows端
1. File->Package Project->Windows->Windows(64-bit)
![](https://imgcache.qq.com/operation/dianshi/other/win.ba79ccce59ae58718e6c35c16cdef55531456a70.png)

# iOS\s端
1. 打包项目。File -> Package Project -> iOS

# Android\s端
1.开发调试：详见[Android快速入门](https://docs.unrealengine.com/4.27/zh-CN/SharingAndReleasing/Mobile/Android/GettingStarted/)

2.打包项目：详见[打包Android项目](https://docs.unrealengine.com/4.27/zh-CN/SharingAndReleasing/Mobile/Android/PackagingAndroidProject/)

## TRTC全平台 C++ API文档
[API概览](https://comm.qq.com/imsdk/ue4/md_introduction_CPP%E6%A6%82%E8%A7%88.html)

## 常见问题

### 安卓“Attempt to construct staged filesystem reference from absolute path"”报错
关闭UE4项目，打开cmd

>adb shell

>cd sdcard

>ls (you should see the UE4Game directory listed)

>rm -r UE4Game

重新编译项目
