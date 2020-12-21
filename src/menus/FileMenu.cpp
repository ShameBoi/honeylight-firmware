/**
 * FileMenu.cpp
 *
 * @author Shame Boi
 */

#include <honeylight/managers/FileManager.h>
#include <honeylight/menus/FileMenu.h>

bool FileMenu::init(FileManager * const fileManager) {
    index = fileManager->getActivePatternIndex();
    size = fileManager->getPatternCount();
    if (size == 0) {
        index = 0;
        totalPages = 1;
        currentPage = 0;
        currentPageSize = 0;
        currentPageIndex = 0;
        return true;
    }

    totalPages = ((size - 1) / FullPageSize) + 1;
    calculatePageFromIndex();
    return false;
}

void FileMenu::next() {
    ++index;
    calculatePageFromIndex();
}

void FileMenu::previous() {
    --index;
    calculatePageFromIndex();
}

void FileMenu::calculatePageFromIndex() {
    currentPage = index / FullPageSize;
    currentPageIndex = index % FullPageSize;
    if ((currentPage + 1) == totalPages) {
        currentPageSize = size % FullPageSize;
        if (currentPageSize == 0) {
            currentPageSize = FullPageSize;
        }
    } else {
        currentPageSize = FullPageSize;
    }
}

