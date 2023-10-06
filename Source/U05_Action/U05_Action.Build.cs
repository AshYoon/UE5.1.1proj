// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class U05_Action : ModuleRules
{
	public U05_Action(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine", "InputCore", "GameplayTasks","UMG","PhysicsCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Slate","SlateCore"
        });

        PublicIncludePaths.Add(ModuleDirectory);

    }
}
