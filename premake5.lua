workspace "opengl-renderer"
   configurations { "Debug", "Release" }

project "Renderer"
   kind "ConsoleApp"
   language "C++"
   targetdir "build"
   objdir ("%{cfg.targetdir}/obj/")
   
   targetname "renderer-%{cfg.buildcfg}"

   files { "**.h", "**.c", "**.cpp" }
   links { "glfw", "GL", "X11", "pthread", "Xrandr", "Xi", "dl" }
   includedirs { "include" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      
   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"