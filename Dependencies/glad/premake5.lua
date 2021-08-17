workspace "GLAD"
	
	configurations 
	{
		"Debug",
		"Release",
		"Dist"
	}

	platforms { "x86", "x86_64" }

	filter "platforms:x86"
		architecture "x86"

	filter "platforms:x86_64"
		architecture "x86_64"

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project  "Glad"
	kind "staticlib"
	language "C"

	files
	{
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
	}

	targetdir ("lib/" .. outputdir)
	objdir ("lib-int/" .. outputdir)

	includedirs
	{
		"include"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "on"
		buildoptions "/MT"

