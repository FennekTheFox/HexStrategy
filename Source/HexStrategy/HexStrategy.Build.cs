// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HexStrategy : ModuleRules
{
	public HexStrategy(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {"PaperZD", "Slate", "EditorScriptingUtilities", "ProceduralMeshComponent", "AIModule", "GameplayAbilities", "GameplayTasks", "GameplayTags", "Core", "CoreUObject", "Engine", "InputCore", "Paper2D", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
