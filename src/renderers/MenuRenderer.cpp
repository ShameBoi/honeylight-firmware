/**
 * MenuRenderer.cpp
 *
 * @author Shame Boi
 */

#include <honeylight/renderers/MenuRenderer.h>

bool MenuRenderer::renderTo(DisplayBuffer *const buffer) {
    buffer->setAll(BackgroundColor);

    for (size_t rowIndex = 0, colIndex = 0, iter = 0;
         iter < filePatternEntries && rowIndex < PatternMenuHeight;
         ++iter) {
        buffer->set(rowIndex + PatternMenuRow,
                    colIndex,
                    iter == highlightedEntry ? SelectedItemColor : FilePatternMenuItemColor);
        ++colIndex;
        if (colIndex >= getMenuRowWidth(rowIndex)) {
            ++rowIndex;
            colIndex = 0;
        }
    }

    for (size_t rowIndex = 0, colIndex = 0, iter = 0;
         iter < builtInEntries && rowIndex < BuiltInMenuHeight;
         ++iter) {
        buffer->set(rowIndex + BuiltInMenuRow,
                    colIndex,
                    (iter + filePatternEntries) == highlightedEntry ? SelectedItemColor : BuiltInMenuItemColor);
        ++colIndex;
        if (colIndex >= getMenuRowWidth(rowIndex)) {
            ++rowIndex;
            colIndex = 0;
        }
    }
    return Font::renderCharacter(displayedChar, SelectedItemColor, MenuWidth, buffer);
}
