/**
 * BlankWhiteRenderer.cpp.c
 *
 * @author Shame Boi
 */

#include <honeylight/renderers/BlankWhiteRenderer.h>

bool BlankWhiteRenderer::renderTo(display_buffer_t * const buffer) {
    buffer->setAll(Color::White);
    return true;
}
