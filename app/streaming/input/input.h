#pragma once

#include "settings/streamingpreferences.h"
#include "backend/computermanager.h"

#include <SDL.h>

#define SDL_CODE_HIDE_CURSOR 1
#define SDL_CODE_SHOW_CURSOR 2
#define SDL_CODE_UNCAPTURE_MOUSE 3

struct GamepadState {
    SDL_GameController* controller;
    SDL_JoystickID jsId;
    short index;

#if !SDL_VERSION_ATLEAST(2, 0, 9)
    SDL_Haptic* haptic;
    int hapticMethod;
    int hapticEffectId;
#endif

    SDL_TimerID mouseEmulationTimer;
    uint32_t lastStartDownTime;

    short buttons;
    short lsX, lsY;
    short rsX, rsY;
    unsigned char lt, rt;
};

#ifdef Q_OS_DARWIN
#include <CoreGraphics/CGError.h>
extern "C" {
    typedef int CGSConnection;
    typedef enum {
        CGSGlobalHotKeyEnable = 0,
        CGSGlobalHotKeyDisable = 1,
    } CGSGlobalHotKeyOperatingMode;

    extern CGSConnection _CGSDefaultConnection(void);

    extern CGError CGSGetGlobalHotKeyOperatingMode(CGSConnection connection,
                                                   CGSGlobalHotKeyOperatingMode* mode);

    extern CGError CGSSetGlobalHotKeyOperatingMode(CGSConnection connection,
                                                   CGSGlobalHotKeyOperatingMode mode);
}
#endif

#define MAX_GAMEPADS 4
#define MAX_FINGERS 2

#define GAMEPAD_HAPTIC_METHOD_NONE 0
#define GAMEPAD_HAPTIC_METHOD_LEFTRIGHT 1
#define GAMEPAD_HAPTIC_METHOD_SIMPLERUMBLE 2

#define GAMEPAD_HAPTIC_SIMPLE_HIFREQ_MOTOR_WEIGHT 0.33
#define GAMEPAD_HAPTIC_SIMPLE_LOWFREQ_MOTOR_WEIGHT 0.8

class SdlInputHandler
{
public:
    explicit SdlInputHandler(StreamingPreferences& prefs, NvComputer* computer,
                             int streamWidth, int streamHeight);

    ~SdlInputHandler();

    void setWindow(SDL_Window* window);

    void handleKeyEvent(SDL_KeyboardEvent* event);

    void handleMouseButtonEvent(SDL_MouseButtonEvent* event);

    void handleMouseMotionEvent(SDL_MouseMotionEvent* event);

    void handleMouseWheelEvent(SDL_MouseWheelEvent* event);

    void handleControllerAxisEvent(SDL_ControllerAxisEvent* event);

    void handleControllerButtonEvent(SDL_ControllerButtonEvent* event);

    void handleControllerDeviceEvent(SDL_ControllerDeviceEvent* event);

    void handleJoystickArrivalEvent(SDL_JoyDeviceEvent* event);

    void sendText(QString& string);

    void rumble(unsigned short controllerNumber, unsigned short lowFreqMotor, unsigned short highFreqMotor);

    void handleTouchFingerEvent(SDL_TouchFingerEvent* event);

    int getAttachedGamepadMask();

    void raiseAllKeys();

    void notifyMouseLeave();

    void notifyFocusLost();

    void notifyFocusGained();

    bool isCaptureActive();

    bool isSystemKeyCaptureActive();

    void setCaptureActive(bool active);

    bool isMouseInVideoRegion(int mouseX, int mouseY, int windowWidth = -1, int windowHeight = -1);

    void updateMousePositionReport(int mouseX, int mouseY);

    void flushMousePositionUpdate();

    void updateKeyboardGrabState();

    static
    QString getUnmappedGamepads();

private:
    enum KeyCombo {
        KeyComboQuit,
        KeyComboUngrabInput,
        KeyComboToggleFullScreen,
        KeyComboToggleStatsOverlay,
        KeyComboToggleMouseMode,
        KeyComboToggleCursorHide,
        KeyComboToggleMinimize,
        KeyComboPasteText,
        KeyComboMax
    };

    GamepadState*
    findStateForGamepad(SDL_JoystickID id);

    void sendGamepadState(GamepadState* state);

    void handleAbsoluteFingerEvent(SDL_TouchFingerEvent* event);

    void handleRelativeFingerEvent(SDL_TouchFingerEvent* event);

    void performSpecialKeyCombo(KeyCombo combo);

    static
    Uint32 longPressTimerCallback(Uint32 interval, void* param);

    static
    Uint32 mouseMoveTimerCallback(Uint32 interval, void* param);

    static
    Uint32 mouseEmulationTimerCallback(Uint32 interval, void* param);

    static
    Uint32 releaseLeftButtonTimerCallback(Uint32 interval, void* param);

    static
    Uint32 releaseRightButtonTimerCallback(Uint32 interval, void* param);

    static
    Uint32 dragTimerCallback(Uint32 interval, void* param);

    static
    int clipboardThreadProc(void *ptr);

    SDL_Window* m_Window;
    bool m_MultiController;
    bool m_GamepadMouse;
    bool m_SwapMouseButtons;
    bool m_ReverseScrollDirection;
    bool m_SwapFaceButtons;
    SDL_TimerID m_MouseMoveTimer;
    SDL_atomic_t m_MouseDeltaX;
    SDL_atomic_t m_MouseDeltaY;

    SDL_SpinLock m_MousePositionLock;
    struct {
        int x, y;
        int windowWidth, windowHeight;
    } m_MousePositionReport;
    SDL_atomic_t m_MousePositionUpdated;
    bool m_MouseWasInVideoRegion;
    bool m_PendingMouseButtonsAllUpOnVideoRegionLeave;

    int m_GamepadMask;
    GamepadState m_GamepadState[MAX_GAMEPADS];
    QSet<short> m_KeysDown;
    bool m_FakeCaptureActive;
    QString m_OldIgnoreDevices;
    QString m_OldIgnoreDevicesExcept;
    StreamingPreferences::CaptureSysKeysMode m_CaptureSystemKeysMode;
    int m_MouseCursorCapturedVisibilityState;

#ifdef Q_OS_DARWIN
    CGSGlobalHotKeyOperatingMode m_OldHotKeyMode;
#endif

    struct {
        KeyCombo keyCombo;
        SDL_Keycode keyCode;
        SDL_Scancode scanCode;
        bool enabled;
    } m_SpecialKeyCombos[KeyComboMax];

    SDL_TouchFingerEvent m_LastTouchDownEvent;
    SDL_TouchFingerEvent m_LastTouchUpEvent;
    SDL_TimerID m_LongPressTimer;
    int m_StreamWidth;
    int m_StreamHeight;
    bool m_AbsoluteMouseMode;
    bool m_AbsoluteTouchMode;

    SDL_TouchFingerEvent m_TouchDownEvent[MAX_FINGERS];
    SDL_TimerID m_LeftButtonReleaseTimer;
    SDL_TimerID m_RightButtonReleaseTimer;
    SDL_TimerID m_DragTimer;
    char m_DragButton;
    int m_NumFingersDown;

    SDL_Thread* m_ClipboardThread;
    SDL_atomic_t m_ShutdownClipboardThread;
    QString m_ClipboardData;
    SDL_cond* m_ClipboardHasData;
    SDL_mutex* m_ClipboardLock;

    static const int k_ButtonMap[];
};
