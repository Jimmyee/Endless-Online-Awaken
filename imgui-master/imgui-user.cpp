#include "imgui_user.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_PLACEMENT_NEW
#include "imgui_internal.h"

namespace ImGui
{
    bool ImageAnimButton(ImTextureID user_texture_id, ImTextureID user_texture_id2, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& uv2, const ImVec2& uv3, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        // Default to using texture ID as ID. User can still push string/integer prefixes.
        // We could hash the size/uv to create a unique ID but that would prevent the user from animating UV.
        PushID((void *)user_texture_id);
        const ImGuiID id = window->GetID("#image");
        PopID();

        const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
        const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding*2);
        const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + padding + size);
        ItemSize(bb);
        if (!ItemAdd(bb, &id))
            return false;

        bool hovered, held;
        bool pressed = ButtonBehavior(bb, id, &hovered, &held);

        // Render
        const ImU32 col = GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
        RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
        if (bg_col.w > 0.0f)
            window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));
        if(hovered)
        {
            window->DrawList->AddImage(user_texture_id2, image_bb.Min, image_bb.Max, uv2, uv3, GetColorU32(tint_col));
        }
        else
        {
            window->DrawList->AddImage(user_texture_id2, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));
        }

        return pressed;
    }
}
