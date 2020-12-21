/**
 * MenuRenderer.cpp
 *
 * @author Shame Boi
 */

#include <honeylight/renderers/MenuRenderer.h>

bool MenuRenderer::renderTo(DisplayBuffer *const buffer) {
    buffer->setAll(BackgroundColor);

    size_t row = PatternMenuRow;
    color_t activeColor = Color::Violet;
    for (Menu * curMenu : menus) {
        for (size_t colIndex = 0; colIndex < curMenu->getCurrentPageSize(); ++colIndex) {
            if (curMenu->getType() == activeMenu) {
                activeColor = curMenu->getSelectedItemColor();
                buffer->set(row,
                            colIndex,
                            colIndex == curMenu->getCurrentPageIndex() ? activeColor : curMenu->getMenuColor());
            } else {
                buffer->set(row,
                            colIndex,
                            curMenu->getMenuColor());
            }
        }

        if (curMenu->isPaged() && curMenu->getTotalPages() > 1) {
            for (size_t colIndex = 0; colIndex < curMenu->getTotalPages(); ++colIndex) {
                if (curMenu->getType() == activeMenu) {
                    buffer->set(row + 1,
                                colIndex,
                                colIndex == curMenu->getCurrentPage() ? activeColor : curMenu->getMenuColor());
                } else {
                    buffer->set(row + 1,
                                colIndex,
                                curMenu->getMenuColor());
                }
            }
        }
        row += 2;
    }

    return Font::renderCharacter(displayedChar, activeColor, MenuWidth, buffer);
}
