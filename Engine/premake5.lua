project "Oniun"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "Off"
    targetdir(TargetDirectory)
    objdir(ObjectTargetDirectory)
    systemversion "latest"

    files {
        "Source/**.h",
        "Source/**.cpp",
    }

    includedirs {
        "Source",
        "%{wks.location}/Thirdparty/glad/include",
        "%{wks.location}/Thirdparty/glfw/include",
        "%{wks.location}/Thirdparty/imgui",
        "%{wks.location}/Thirdparty/stb",
        "%{wks.location}/Thirdparty/glm",
    }

    links {
        "GLAD",
        "GLFW",
        "ImGui",
    }

	defines {
		"GLFW_INCLUDE_NONE",
		"FMT_HEADER_ONLY",

		VersionMajor,
		VersionMinor,
		VersionPatch,
	}

    -- Platform
    filter "system:windows"
        defines { "ONU_PLATFORM_WINDOWS" }
		filter "action:vs*"
			buildoptions { "/utf-8" }
			defines { "_CRT_SECURE_NO_WARNINGS" }

	filter "system:linux"
	    defines { "ONU_PLATFORM_LINUX" }

	filter "system:macos"
	    defines { "ONU_PLATFORM_MACOS" }

	-- Build mode
	filter "configurations:Debug"
	    defines { "ONU_DEBUG" }
	    runtime "Debug"
	    symbols "On"

	filter "configurations:Release"
	    defines { "ONU_RELEASE" }
	    runtime "Release"
	    symbols "On"
	    optimize "On"

	filter "configurations:Dist"
	    defines { "ONU_Dist" }
	    runtime "Release"
	    symbols "Off"
	    optimize "On"
