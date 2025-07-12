delete .git then git init again to initialize new timeline of git

create assets folder inside SDL_TEMPLATE and put assets there.
separate assets by music, fx, and img.

copy sdl2 from cloud to C:\SDL2 (create own SDL2 folder in C:)
copy spdlog from cloud to C:\spdlog (create own spdlog folder in C:)
copy glm from cloud to your C:\glmm (create own glm folder in C:)

add all in project solution -> properties -> c/c++ -> additional include libraries
(copy paste all folder inside the project folder for publshing the app, just configure the additional include and linking)

copy imgui from pc/laptop to project directory (Project -> Add an existing item)

install EditorConfig Guidelines by Ivan.Z

rename output name exe by Project Properties -> Configuration Properties -> General -> Target Name (Example.exe)

/=======================================================================================================================/
imGUI

download imGUI
https://github.com/ocornut/imgui

create imgui folder in your project
copy *.h and *.cpp files on the imgui/docking/*
paste it in imgui folder in your project
go to imgui/docking/backends/
copy the desired *.h and *.cpp base on your project setup
paste it also in igui folder in your project
in your visual studio, project->project properties->c/c++->general->additional include directories
add imgui
on solution explorer, add new filter named "imgui" for both header and source files
add existing item on corresponding filter

initializer: https://github.com/jelemiekun/sdl-opengl-prac/blob/master/SDL_TEMPLATE/ImGuiWindow.cpp
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
