#ifndef GENERAL_LIST_H
#define GENERAL_LIST_H

#include <vector>
#include <TFT_eSPI.h>

struct FileInfo
{
    std::string name;
    bool isDirectory;
};



class GeneralList
{
public:
    GeneralList(TFT_eSPI* screen) : screen(screen) 
    {
        
    }

    void AddItem(bool isDirectory, std::string&& name)
    {
        items.emplace_back(std::move(name), isDirectory);
    }

    void ClearDisplay()
    {
        screen->fillRect(offset_left, 
            offset_top,
            screen->width() - offset_left - offset_right,
            screen->height() - offset_top - offset_bottom,
            bg_color);

    }

    int width()
    {
        return screen->width() - offset_left - offset_right;
    }

    int height()
    {
        return screen->height() - offset_top - offset_bottom;
    }

    void Draw()
    {
        screen->setTextPadding(width() - text_margin_x * 2);

        max_items = height() / item_height;
        const int size = items.size();
        for (int i = 0; i < max_items; i++)
        {
            int item = i + scroll_offset;
            int y = i * item_height + offset_top;

            if (item >= size) 
            {
                screen->fillRect(offset_left, 
                    y,
                    width(),
                    height() - y + offset_top,
                    bg_color);
                break;
            }

            const FileInfo& fileData = items[item];

            // todo remove need for copy
            const std::string& label = clipName(fileData.name);

            
            if (item == selected)
                screen->setTextColor(selected_text_color, bg_color);
            else
                screen->setTextColor(text_color, bg_color); 

            screen->drawString(label.c_str(), offset_left + text_margin_x, y, 1);
        }
    }

    std::string clipName(const std::string& name)
    {
        static std::string clipped;
        clipped = name;
        int maxWidth = width() - text_margin_x * 2;
        while (screen->textWidth(clipped.c_str()) > maxWidth)
        {
            clipped.pop_back();
        }
        if (clipped.size() < name.size())
        {
            clipped.replace(clipped.size()-3, 3, "...");
        }
        return clipped;
    }

    void Clear()
    {
        items.clear();
        selected = 0;
        scroll_offset = 0;
        ClearDisplay();
    }

    void SelectionUp()
    {
        ChangeSelected(selected - 1);
    }

    void SelectionDown()
    {
        ChangeSelected(selected + 1);
    }

    void SelectionPageUp()
    {
        ChangeSelected(selected - max_items);
    }

    void SelectionPageDown()
    {
        ChangeSelected(selected + max_items);
    }

    bool isEmpty() const
    {
        return items.empty();
    }

    FileInfo* GetSelectedItem()
    {
        if (items.empty()) return nullptr;
        return &items[selected];
    }

protected:
    void ChangeSelected(int new_selected)
    {
        // TODO better calculate new scroll offset after we change selection
        int new_scroll_offset = scroll_offset;
        if (new_selected < 0)
        {
            new_selected = (items.size() - 1);
            // TODO is this wrong for small lists?
            new_scroll_offset = new_selected - max_items + 1;
        }

        if (new_selected > (items.size() - 1))
        {
            new_selected = 0;
            new_scroll_offset = 0;
        }

        if (new_selected != selected)
        {
            if (new_selected < new_scroll_offset) new_scroll_offset = new_selected; 
            if (new_selected >= new_scroll_offset + max_items) new_scroll_offset = new_selected - max_items + 1;

            if (new_scroll_offset < 0) new_scroll_offset = 0;
            if (new_scroll_offset > items.size() - 1) new_scroll_offset = items.size() - 1;


            selected = new_selected;
            if (new_scroll_offset != scroll_offset)
            {
                scroll_offset = new_scroll_offset;
            }

            Draw();
        }
    }

    TFT_eSPI* screen;

    static constexpr int item_height = 12;

    static constexpr int text_margin_x = 4;

    static constexpr int offset_top = 32;
    static constexpr int offset_bottom = 32;
    static constexpr int offset_left = 10;
    static constexpr int offset_right = 10;

    static constexpr int bg_color = 0x0015;
    static constexpr int text_color = 0xFFFF;
    static constexpr int selected_text_color = 0x57CA;

    std::vector<FileInfo> items;
    int selected = 0;
    int scroll_offset = 0;
    int max_items = 0;
};


#endif