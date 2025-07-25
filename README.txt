Delete .git, then git init again to initialize a new timeline of git.

Create an assets folder inside SDL_TEMPLATE and put assets there.
separate assets by music, fx, img, models, etc.

Copy sdl2 from cloud to C:\SDL2 (create own SDL2 folder in C:)
Copy spdlog from cloud to C:\spdlog (create own spdlog folder in C:)
Copy glm from cloud to your C:\glm (create own glm folder in C:)
Include stb_image -> https://raw.githubusercontent.com/nothings/stb/refs/heads/master/stb_image.h

add all in project solution -> properties -> c/c++ -> additional include libraries
(Copy-paste all folders inside the project folder for publishing the app, just configure the additional include and linking)

Copy imgui from pc/laptop to project directory (Project -> Add an existing item)

Install EditorConfig Guidelines by Ivan.Z

rename output name exe by Project Properties -> Configuration Properties -> General -> Target Name (Example.exe)

/=======================================================================================================================/
imGUI

download imGUI
https://github.com/ocornut/imgui

Create imgui folder in your project
copy *.h and *.cpp files on the imgui/docking/*
Paste it in imgui folder in your project
go to imgui/docking/backends/
copy the desired *.h and *.cpp base on your project setup
Paste it also in igui folder in your project
in your visual studio, project->project properties->c/c++->general->additional include directories
add imgui
In Solution Explorer, add a new filter named "imgui" for both header and source files
Add "existing item" to the corresponding filter

initializer: https://github.com/jelemiekun/sdl-opengl-prac/blob/master/SDL_TEMPLATE/ImGUIWindow.cpp
call imguiinit on your initializer and
call imguiwindow::render on your render function
call imguiwindow::clean on your clean function

forward SDL events to imgui inside while(SDL_PollEvent): 
ImGui_ImplSDL2_ProcessEvent(&event);

/========/
put ImGui::ShowemoWindow() inside your render function
find the widget you would like to use
find and open imgui_demo.cpp inside your project's filter
search the widget's title you would like to use
copy and paste


/========================================/
Inno setup for deployment
