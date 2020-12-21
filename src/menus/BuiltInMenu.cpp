/**
 * BuiltInMenu.cpp
 *
 * @author Shame Boi
 */

#include <honeylight/menus/BuiltInMenu.h>

void BuiltInMenu::next() {
    ++index;
    setSelectedBuiltInRenderer();
}

void BuiltInMenu::previous() {
    --index;
    setSelectedBuiltInRenderer();
}

void BuiltInMenu::setSelectedBuiltInRenderer() {
    switch (index) {
        default:
        case 0:
            selectedBuiltInRenderer = RendererType::Rainbow;
            break;
        case 1:
            selectedBuiltInRenderer = RendererType::BlankWhite;
            break;
    }
}
