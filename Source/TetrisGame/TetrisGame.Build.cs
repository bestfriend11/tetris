// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TetrisGame : ModuleRules
{
	public TetrisGame(ReadOnlyTargetRules Target) : base(Target)
	{
		// Use explicit or shared PCHs (precompiled headers) for faster compilation
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		// Public include paths (headers exposed to other modules)
		// Example: If you have public headers in "Public" subfolder
		PublicIncludePaths.AddRange(
			new string[] {
				// Path to module's public headers
				// System.IO.Path.Combine(ModuleDirectory, "Public"),
				
				// Path to shared headers from other modules
				// System.IO.Path.Combine(EngineDirectory, "Source/Runtime/Renderer/Public"),
			}
		);
				
		// Private include paths (internal headers not exposed to other modules)
		PrivateIncludePaths.AddRange(
			new string[] {
				// Path to module's private headers
				// System.IO.Path.Combine(ModuleDirectory, "Private"),
				
				// Path to third-party library headers
				// System.IO.Path.Combine(ModuleDirectory, "ThirdParty/MyLib/Include"),
			}
		);
			
		// Public dependency module names (modules we expose to other modules)
		// These modules must be accessible by anyone using our module
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",	// Core UE4 functionality
				
				// Common public dependencies:
				// "CoreUObject",	// Core UE4 object functionality
				// "Engine",		// Core engine functionality
				// "Networking",	// For network functionality
				// "UMG",			// For UI functionality
			}
		);
			
		// Private dependency module names (modules we use internally)
		// These modules are only needed for our module's implementation
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",	// Core UE4 object functionality
				"Engine",		// Core engine functionality
				"Slate",		// UI framework
				"SlateCore",	// Core UI functionality
				"EnhancedInput",// Enhanced input system
				
				// Common private dependencies:
				// "RenderCore",	// Rendering core functionality
				// "RHI",			// Render Hardware Interface
				// "Json",			// JSON parsing
				// "HTTP",			// HTTP requests
			}
		);
		
		// Dynamically loaded module names (optional modules loaded at runtime)
		// These modules may not always be available
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// Example optional modules:
				// "OnlineSubsystem",	// For online functionality
				// "Analytics",		// For analytics
				// "MoviePlayer",	// For movie playback
			}
		);
	}
}
