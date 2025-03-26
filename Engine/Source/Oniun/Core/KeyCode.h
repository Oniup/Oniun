#pragma once

namespace Oniun
{
    enum class KeyCode
    {
        Unknown      = -1,

        Space        = 32,
        Apostrophe   = 39,  /* ' */
        Comma        = 44,  /* , */
        Minus        = 45,  /* - */
        Period       = 46,  /* . */
        Slash        = 47,  /* / */
        N0           = 48,
        N1           = 49,
        N2           = 50,
        N3           = 51,
        N4           = 52,
        N5           = 53,
        N6           = 54,
        N7           = 55,
        N8           = 56,
        N9           = 57,
        Semicolon    = 59,  /* ; */
        Equal        = 61,  /* = */
        A            = 65,
        B            = 66,
        C            = 67,
        D            = 68,
        E            = 69,
        F            = 70,
        G            = 71,
        H            = 72,
        I            = 73,
        J            = 74,
        K            = 75,
        L            = 76,
        M            = 77,
        N            = 78,
        O            = 79,
        P            = 80,
        Q            = 81,
        R            = 82,
        S            = 83,
        T            = 84,
        U            = 85,
        V            = 86,
        W            = 87,
        X            = 88,
        Y            = 89,
        Z            = 90,
        LeftBracket  = 91,  /* [ */
        Backslash    = 92,  /* \ */
        RightBracket = 93,  /* ] */
        GraveAccent  = 96,  /* ` */

        Escape       = 256,
        Enter        = 257,
        Tab          = 258,
        Backspace    = 259,
        Insert       = 260,
        Delete       = 261,
        Right        = 262,
        Left         = 263,
        Down         = 264,
        Up           = 265,
        PageUp       = 266,
        PageDown     = 267,
        Home         = 268,
        End          = 269,
        CapsLock     = 280,
        ScrollLock   = 281,
        NumLock      = 282,
        PrintScreen  = 283,
        Pause        = 284,
        F1           = 290,
        F2           = 291,
        F3           = 292,
        F4           = 293,
        F5           = 294,
        F6           = 295,
        F7           = 296,
        F8           = 297,
        F9           = 298,
        F10          = 299,
        F11          = 300,
        F12          = 301,
        F13          = 302,
        F14          = 303,
        F15          = 304,
        F16          = 305,
        F17          = 306,
        F18          = 307,
        F19          = 308,
        F20          = 309,
        F21          = 310,
        F22          = 311,
        F23          = 312,
        F24          = 313,
        F25          = 314,
        Kp0          = 320,
        Kp1          = 321,
        Kp2          = 322,
        Kp3          = 323,
        Kp4          = 324,
        Kp5          = 325,
        Kp6          = 326,
        Kp7          = 327,
        Kp8          = 328,
        Kp9          = 329,
        KpDecimal    = 330,
        KpDivide     = 331,
        KpMultiply   = 332,
        KpSubtract   = 333,
        KpAdd        = 334,
        KpEnter      = 335,
        KpEqual      = 336,
        LeftShift    = 340,
        LeftControl  = 341,
        LeftAlt      = 342,
        LeftSuper    = 343,
        RightShift   = 344,
        RightControl = 345,
        RightAlt     = 346,
        RightSuper   = 347,
        Menu         = 348,

        MaxCount     = Menu + 1,
    };

    enum class KeyMod
    {
        /// @brief If this bit is set one or more Shift keys were held down.
        Shift    = 0x0001,
        /// @brief if this bit is set one or more control keys were held down.
        Control  = 0x0002,
        /// @brief if this bit is set one or more alt keys were held down.
        Alt      = 0x0004,
        /// @brief if this bit is set one or more super keys were held down.
        Super    = 0x0008,
        /// @brief if this bit is set the caps lock key is enabled.
        CapsLock = 0x0010,
        /// @brief if this bit is set the num lock key is enabled.
        NumLock  = 0x0020,
    };

    enum class MouseButton
    {
        Left     = 0,
        Right    = 1,
        Middle   = 2,
        B1       = 3,
        B2       = 4,
        B3       = 5,
        B4       = 6,
        B5       = 7,
    };

#define MAX_CONNECTED_GAMEPADS_COUNT 16

    enum class GamepadButton
    {
        B0          = 0,
        B1          = 1,
        B2          = 2,
        B3          = 3,
        LeftBumper  = 4,
        RightBumper = 5,
        Back        = 6,
        Start       = 7,
        Guide       = 8,
        LeftThumb   = 9,
        RightThumb  = 10,
        DpadUp      = 11,
        DpadRight   = 12,
        DpadDown    = 13,
        DpadLeft    = 14,
        Last        = DpadLeft,
        Cross       = B0,
        Circle      = B1,
        Square      = B2,
        Triangle    = B3,
        A           = B0,
        B           = B1,
        X           = B2,
        Y           = B3,
        MaxCount    = DpadLeft + 1,
    };

    enum class GamepadAxis
    {
        LeftX        = 0,
        LeftY        = 1,
        RightX       = 2,
        RightY       = 3,
        LeftTrigger  = 4,
        RightTrigger = 5,
        MaxCount     = RightTrigger + 1,
    };
}
