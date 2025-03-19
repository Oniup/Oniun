project "GLAD"
	kind "StaticLib"
	language "C"
	targetdir(TargetDirectory)
	objdir(ObjectTargetDirectory)

	includedirs {
		"glad/include",
	}

	files {
		"glad/src/glad.c",
		"glad/include/glad/glad.h",
		"glad/include/KHR/khrplatform.h",
	}

	filter "system:linux"
		pic "On"
		systemversion "latest"
		staticruntime "On"

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		symbols "On"
		optimize "On"

	filter "configurations:Dist"
		runtime "Release"
		symbols "Off"
		optimize "On"
