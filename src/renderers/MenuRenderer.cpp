/**
 * MenuRenderer.cpp
 *
 * @author Shame Boi
 */

#include <honeylight/renderers/MenuRenderer.h>

bool MenuRenderer::renderTo(display_buffer_t *const buffer) {
    buffer->setAll(BackgroundColor);

    for (size_t iter = 0; iter < totalEntries && iter < display_buffer_t::length; ++iter) {
        if (iter == highlightedEntry) {
            buffer->set(iter, SelectedItemColor);
        } else {
            buffer->set(iter, MenuItemColor);
        }
    }
    return true;
}
