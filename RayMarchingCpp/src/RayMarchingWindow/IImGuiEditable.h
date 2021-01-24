#pragma once

struct IImGuiEditable {
    virtual ~IImGuiEditable() {}

    virtual void RenderImGuiEditor() = 0;
    virtual std::string_view SectionName() const = 0;
};