using UnrealBuildTool;

public class CProject : ModuleRules
{
	public CProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicIncludePaths.Add(ModuleDirectory);
		
		PublicDependencyModuleNames.Add("Core");
		
		PrivateDependencyModuleNames.Add("CoreUObject");
		PrivateDependencyModuleNames.Add("Engine");
		PrivateDependencyModuleNames.Add("InputCore");
		PrivateDependencyModuleNames.Add("Niagara");
		
	}
}
