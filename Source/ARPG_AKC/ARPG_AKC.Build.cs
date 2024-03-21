// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ARPG_AKC : ModuleRules
{
	public ARPG_AKC(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "AITestSuite", "AITestSuite" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "ProceduralMeshComponent" });
        
        PrivateDependencyModuleNames.AddRange(new string[] {"GameplayAbilities", "GameplayTags", "GameplayTasks"});
    }
}
