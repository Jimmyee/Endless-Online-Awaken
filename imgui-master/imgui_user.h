#ifndef IMGUI_USER_H_INCLUDED
#define IMGUI_USER_H_INCLUDED

#include "imgui.h"

typedef void* ImTextureID;

namespace ImGui
{
    bool ImageAnimButton(ImTextureID user_texture_id, ImTextureID user_texture_id2, const ImVec2& size,
                         const ImVec2& uv0 = ImVec2(0,0),  const ImVec2& uv1 = ImVec2(1,1),
                         const ImVec2& uv2 = ImVec2(0,0),  const ImVec2& uv3 = ImVec2(1,1), int frame_padding = -1,
                         const ImVec4& bg_col = ImVec4(0,0,0,0), const ImVec4& tint_col = ImVec4(1,1,1,1));
}

#endif // IMGUI_USER_H_INCLUDED
