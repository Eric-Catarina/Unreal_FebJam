// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FebGameJam : ModuleRules
{
	public FebGameJam(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
