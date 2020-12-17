/**
 * BlankWhiteRenderer.cpp.c
 *
 * @author Shame Boi
 */

#include <honeylight/renderers/BlankWhiteRenderer.h>

bool BlankWhiteRenderer::renderTo(DisplayBuffer * const buffer) {
    buffer->setAll(Color::White);
    return true;
}
