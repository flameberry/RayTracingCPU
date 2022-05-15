#pragma once

namespace Flameberry
{
    class ImGuiLayer
    {
    public:
        static void OnAttach();
        static void OnDetach();
        static void OnBegin();
        static void OnEnd();

        static void SetDarkThemeColors();
    private:
    };
}