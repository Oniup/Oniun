project "ImGui"
	kind "StaticLib"
	language "C++"
	targetdir(TargetDirectory)
	objdir(ObjectTargetDirectory)

	includedirs {
		"imgui/imgui",
		"glfw/include",
		"glad/include",
	}

	files {
		"imgui/imgui/imconfig.h",
		"imgui/imgui/imgui_demo.cpp",
		"imgui/imgui/imgui_draw.cpp",
		"imgui/imgui/imgui_internal.h",
		"imgui/imgui/imgui_tables.cpp",
		"imgui/imgui/imgui_widgets.cpp",
		"imgui/imgui/imgui.cpp",
		"imgui/imgui/imgui.h",
		"imgui/imgui/imstb_rectpack.h",
		"imgui/imgui/imstb_textedit.h",
		"imgui/imgui/imstb_truetype.h",
		"imgui/imgui/backends/imgui_impl_glfw.h",
		"imgui/imgui/backends/imgui_impl_glfw.cpp",
		"imgui/imgui/backends/imgui_impl_opengl3.h",
		"imgui/imgui/backends/imgui_impl_opengl3.cpp",
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
		optimize "Off"
