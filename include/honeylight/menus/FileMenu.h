/**
 * FileMenu.h
 *
 * @author Shame Boi
 */
#pragma once

#include <cstdint>
#include <cstddef>

#include <honeylight/display_types.h>
#include <honeylight/menus/Menu.h>

class FileManager;

class FileMenu: public Menu {
private:
    static constexpr size_t const FullPageSize = 6;

    size_t index = 0;
    size_t size = 0;
    size_t currentPage = 0;
    size_t currentPageIndex = 0;
    size_t currentPageSize = 0;
    size_t totalPages = 0;

    void calculatePageFromIndex();

public:
    bool init(FileManager * fileManager);

    void next() override;

    void previous() override;

    char getMenuDisplayCharacter() const override {
        return 'P';
    }

    size_t getSize() const override {
        return size;
    }

    size_t getIndex() const override {
        return index;
    }

    void setIndex(size_t const newIndex) override {
        index = newIndex;
        calculatePageFromIndex();
    }

    inline color_t const &getMenuColor() const override {
        return Color::Blue;
    }

    inline bool isPaged() const override {
        return true;
    }

    inline size_t getCurrentPage() const override {
        return currentPage;
    }

    inline size_t getCurrentPageIndex() const override {
        return currentPageIndex;
    }

    inline size_t getCurrentPageSize() const override {
        return currentPageSize;
    }

    inline size_t getTotalPages() const override {
        return totalPages;
    };

    inline Menu::Type getType() const override {
        return Menu::Type::FilePatterns;
    };
};



