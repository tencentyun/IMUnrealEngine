本文主要介绍如何快速地将腾讯云 IM SDK（Unreal Engine）集成到您的项目中，只要按照如下步骤进行配置，就可以完成 SDK 的集成工作。

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

## 集成 SDK
1. 下载 SDK 及配套的 [SDK 源码](https://github.com/tencentyun/IMUnrealEngine)（有疑问可加入QQ群号：764231117 咨询）。
2. 把项目中的 `IMSDK` 文件夹拷贝到您项目中的 **Source/[project_name]** 目录下，其中 **[project_name]** 表示你项目的名称。
3. 编辑你项目中的 **[project_name].Build.cs**文件。添加下面函数
```
// 加载各个平台IM底层库
private void loadTIMSDK(ReadOnlyTargetRules Target) {
    string _TIMSDKPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "TIMSDK"));
    bEnableUndefinedIdentifierWarnings = false;
    PublicIncludePaths.Add(Path.Combine(_TIMSDKPath, "include"));
    if (Target.Platform == UnrealTargetPlatform.Android) {
        PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });
        AdditionalPropertiesForReceipt.Add(new ReceiptProperty("AndroidPlugin", Path.Combine(ModuleDirectory, "TIMSDK", "Android", "APL_armv7.xml")));
        
        string Architecture = "armeabi-v7a";
        // string Architecture = "arm64-v8a";
        // string Architecture = "armeabi";
        PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory,"TIMSDK", "Android", Architecture, "libImSDK.so"));
    }
    else if (Target.Platform == UnrealTargetPlatform.IOS) {
        PublicAdditionalLibraries.AddRange(
            new string[] {
                "z","c++",
                "z.1.1.3",
                "sqlite3",
                "xml2"
            }
        );
    PublicFrameworks.AddRange(new string[]{
            "Security",
            "AdSupport",
            "CoreTelephony",
            "CoreGraphics",
            "UIKit"
        });
        PublicAdditionalFrameworks.Add(new UEBuildFramework("ImSDK_CPP",_TIMSDKPath+"/ios/ImSDK_CPP.framework.zip", ""));
    }
    else if(Target.Platform == UnrealTargetPlatform.Mac) {
        PublicAdditionalLibraries.AddRange(new string[] {
            "resolv",
            "z",
            "c++",
            "bz2",
            "sqlite3",
        });
    PublicFrameworks.AddRange(
            new string[] {
                "AppKit",
                "Security",
                "CFNetwork",
                "SystemConfiguration",
            });
        PublicFrameworks.Add(Path.Combine(_TIMSDKPath, "Mac", "Release","ImSDKForMac_CPP.framework"));
    }
    else if (Target.Platform == UnrealTargetPlatform.Win64) {
        PublicAdditionalLibraries.Add(Path.Combine(_TIMSDKPath, "win64", "Release","ImSDK.lib"));
        PublicDelayLoadDLLs.Add(Path.Combine(_TIMSDKPath, "win64", "Release", "ImSDK.dll"));
        RuntimeDependencies.Add("$(BinaryOutputDir)/ImSDK.dll", Path.Combine(_TIMSDKPath, "win64", "Release", "ImSDK.dll"));
    }
}
```
4. 在**[project_name].Build.cs**文件调用该函数
![](https://imgcache.qq.com/operation/dianshi/other/TRTCSDK.82d81b0b8fe050772b3c8e02c4578b920515a580.jpg)
5. 到目前为止你已经集成了IM SDK。可在你的cpp 文件中使用IM的能力了。`#include "V2TIMManager.h"`
```
// 获取sdk单例对象
V2TIMManager* timInstance = V2TIMManager::GetInstance();
// 获取sdk版本号
V2TIMString timString = timInstance->GetVersion();
// 初始化sdk
V2TIMSDKConfig timConfig;
timConfig.initPath = static_cast<V2TIMString>("D:\\");
timConfig.logPath = static_cast<V2TIMString>("D:\\");
bool isInit = timInstance->InitSDK(SDKAppID, timConfig);
```
## 打包

#### macOS端
1. File -> Package Project -> Mac

#### Windows\s端
1. File->Package Project->Windows->Windows(64-bit)
![](https://imgcache.qq.com/operation/dianshi/other/win.ba79ccce59ae58718e6c35c16cdef55531456a70.png)

#### iOS\s端
1. 打包项目。File -> Package Project -> iOS

#### Android\s端
1.开发调试：详见[Android快速入门](https://docs.unrealengine.com/4.27/zh-CN/SharingAndReleasing/Mobile/Android/GettingStarted/)

2.打包项目：详见[打包Android项目](https://docs.unrealengine.com/4.27/zh-CN/SharingAndReleasing/Mobile/Android/PackagingAndroidProject/)

## IM全平台 C++ API文档
[API概览](https://comm.qq.com/imsdk/ue4/md_introduction_CPP%E6%A6%82%E8%A7%88.html)
