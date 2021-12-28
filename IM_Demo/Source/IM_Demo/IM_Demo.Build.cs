// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class IM_Demo : ModuleRules
{
	public IM_Demo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] {"UMG", "Slate", "SlateCore"});

		loadDebugDll(Target);

		loadTIMSDK(Target);
	}

	// 加载生成userSig 的framework。该方案仅适用于调试Demo，正式上线前请将 UserSig 计算代码和密钥迁移到您的后台服务器上，以避免加密密钥泄露导致的流量盗用。
	private void loadDebugDll(ReadOnlyTargetRules Target)
	{
		string _DebugPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "Debug"));
		if (Target.Platform == UnrealTargetPlatform.Android)
		{
				PublicIncludePaths.Add(Path.Combine(_DebugPath, "include"));   
		}
		else if (Target.Platform == UnrealTargetPlatform.IOS)
		{
				// 添加插件的包含路径
				PublicIncludePaths.Add(Path.Combine(_DebugPath, "include"));
				PublicAdditionalLibraries.Add(Path.Combine(_DebugPath, "ios", "libTXLiteAVTestUserSig.a"));
		}
		else if(Target.Platform == UnrealTargetPlatform.Mac)
		{
				// 添加插件的包含路径
				PublicIncludePaths.Add(Path.Combine(_DebugPath, "include"));
				PublicFrameworks.Add(Path.Combine(_DebugPath, "Mac", "Release","TXLiteAVTestUserSig.framework"));
				
		}
		else if (Target.Platform == UnrealTargetPlatform.Win64)
		{
				PublicIncludePaths.Add(Path.Combine(_DebugPath, "include"));
				PublicFrameworks.Add(Path.Combine(_DebugPath, "win64", "Release","TXLiteAVTestUserSig.dll"));
				PublicAdditionalLibraries.Add(Path.Combine(_DebugPath, "win64", "Release","TXLiteAVTestUserSig.lib"));
				RuntimeDependencies.Add("$(BinaryOutputDir)/TXLiteAVTestUserSig.dll", Path.Combine(_DebugPath, "win64", "Release", "TXLiteAVTestUserSig.dll"));
		}
	}

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
			// PublicAdditionalFrameworks.Add(new UEBuildFramework("ImSDK",_TIMSDKPath+"/ios/ImSDK.framework.zip", ""));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory,"TIMSDK/iOS/ImSDK_CPP.framework/ImSDK_CPP"));
			RuntimeDependencies.Add(Path. Combine(ModuleDirectory,"TIMSDK/iOS/ImSDK_CPP.framework/Info.plist"), UnrealBuildTool.StagedFileType.SystemNonUFS);
			RuntimeDependencies.Add(Path.Combine(ModuleDirectory,"TIMSDK/iOS/ImSDK_CPP.framework/ImSDK_CPP"),UnrealBuildTool.StagedFileType.SystemNonUFS);
		}
		else if(Target.Platform == UnrealTargetPlatform.Mac) {
			// PublicFrameworks.Add(Path.Combine(_TIMSDKPath, "Mac", "Release","ImSDKForMac.framework"));
			PublicFrameworks.Add(Path.Combine(_TIMSDKPath, "Mac", "Release","ImSDKForMac_CPP.framework"));
		}
		else if (Target.Platform == UnrealTargetPlatform.Win64) {
			PublicAdditionalLibraries.Add(Path.Combine(_TIMSDKPath, "win64", "Release","ImSDK.lib"));
			PublicDelayLoadDLLs.Add(Path.Combine(_TIMSDKPath, "win64", "Release", "ImSDK.dll"));
			RuntimeDependencies.Add("$(BinaryOutputDir)/ImSDK.dll", Path.Combine(_TIMSDKPath, "win64", "Release", "ImSDK.dll"));
		}
	}
}
