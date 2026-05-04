#ifndef UI_H
#define UI_H

#include <SD.h>
#include <TFT_eSPI.h>

#include "controller.h"
#include "core/bus.h"
#include "general_list.h"
#include "hwconfig.h"

#define BL_CHANNEL 0
#define BL_FREQ 5000
#define BL_RESOLUTION 8

#define BG_COLOR 0x0015
#define BAR_COLOR 0xAD55
#define TEXT_COLOR 0xFFFF
#define TEXT2_COLOR 0xA800
#define SELECTED_TEXT_COLOR 0x57CA
#define SELECTED_BG_COLOR 0x0560

extern HWConfig hw_config;
class UI
{
public:

    UI(TFT_eSPI* screen);
    ~UI();
    Cartridge* selectGame();
    void getNesFiles();
    void drawWindowBox(int x, int y, int w, int h);
    void drawBars();
    void pauseMenu(Bus* nes);
    void settingsMenu(Bus* nes);
    void initializeSettings();
    void loadEmulatorSettings(Bus* nes);
    bool paused = false;

private:
    void SleepMode();
    Cartridge* selectedFile(const FileInfo& file);

    void goDirectoryUp();
    void setBrightness(int value);
    void drawText(const char* text, const int x, const int y);

    void updateBatteryStatus();
    TFT_eSPI* screen = nullptr;

    // TODO use this
    static constexpr int ITEM_HEIGHT = 12;

    GeneralList fileList;
    std::string current_dir = "/";

    struct Settings
    {
        uint8_t volume = 100;
        uint8_t brightness = 100;
        uint8_t palette = 0;
    };
    Settings settings;
    void saveSettings(const Settings* s);
    void loadSettings(Settings* s);

    unsigned char last_battery_state = -1;
};

#endif