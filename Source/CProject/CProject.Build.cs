using UnrealBuildTool;

public class CProject : ModuleRules
{
	public CProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicIncludePaths.Add(ModuleDirectory);
		
		PublicDependencyModuleNames.Add("Core");
		PublicDependencyModuleNames.Add("CoreUObject");
		PublicDependencyModuleNames.Add("Engine");
		
		PrivateDependencyModuleNames.Add("InputCore");
		PrivateDependencyModuleNames.Add("Niagara");
		PrivateDependencyModuleNames.Add("AIModule");
		PrivateDependencyModuleNames.Add("GameplayTasks");
		PrivateDependencyModuleNames.Add("NavigationSystem");
		PrivateDependencyModuleNames.Add("PhysicsCore");
		
		PrivateDependencyModuleNames.Add("LevelSequence");
		PrivateDependencyModuleNames.Add("MovieScene");
		
		PrivateDependencyModuleNames.Add("MyDebugger");
	}
}
