/* ==========================================================================
   >File: MenuUtilities.cpp
   >Date: 20180713
   >Author: Vik Pandher
   >Details: Stuff used in the menus.
   ========================================================================== */

#include "MenuUtilities.h"



String getKeyString(const uint8_t k)
{
    switch (k)
    {
    case VK_LBUTTON:    return String("VK_LBUTTON");
    case VK_RBUTTON:    return String("VK_RBUTTON");
    case VK_CANCEL:     return String("VK_CANCEL");
    case VK_MBUTTON:    return String("VK_MBUTTON");
    case VK_XBUTTON1:   return String("VK_XBUTTON1");
    case VK_XBUTTON2:   return String("VK_XBUTTON2");
    case VK_BACK:       return String("BACKSPACE");
    case VK_TAB:        return String("TAB");
    case VK_CLEAR:      return String("VK_CLEAR");
    case VK_RETURN:     return String("ENTER");
    case VK_SHIFT:      return String("SHIFT");
    case VK_CONTROL:    return String("CTRL");
    case VK_MENU:       return String("ALT");
    case VK_PAUSE:      return String("VK_PAUSE");
    case VK_CAPITAL:    return String("CAPS LOCK");
    case VK_ESCAPE:     return String("ESC");
    case VK_SPACE:      return String("SPACE");
    case VK_PRIOR:      return String("PAGUE UP");
    case VK_NEXT:       return String("PAGE DOWN");
    case VK_END:        return String("END");
    case VK_HOME:       return String("HOME");
    case VK_LEFT:       return String("LEFT ARROW");
    case VK_UP:         return String("UP ARROW");
    case VK_RIGHT:      return String("RIGHT ARROW");
    case VK_DOWN:       return String("DOWN ARROW");
    case VK_SELECT:     return String("VK_SELECT");
    case VK_PRINT:      return String("VK_PRINT");
    case VK_EXECUTE:    return String("VK_EXECURE");
    case VK_SNAPSHOT:   return String("PRINT SCREEN");
    case VK_INSERT:     return String("INS");
    case VK_DELETE:     return String("DEL");
    case VK_HELP:       return String("VK_HELP");
    case 0x30:          return String("0");
    case 0x31:          return String("1");
    case 0x32:          return String("2");
    case 0x33:          return String("3");
    case 0x34:          return String("4");
    case 0x35:          return String("5");
    case 0x36:          return String("6");
    case 0x37:          return String("7");
    case 0x38:          return String("8");
    case 0x39:          return String("9");
    case 0x41:          return String("A");
    case 0x42:          return String("B");
    case 0x43:          return String("C");
    case 0x44:          return String("D");
    case 0x45:          return String("E");
    case 0x46:          return String("F");
    case 0x47:          return String("G");
    case 0x48:          return String("H");
    case 0x49:          return String("I");
    case 0x4A:          return String("J");
    case 0x4B:          return String("K");
    case 0x4C:          return String("L");
    case 0x4D:          return String("M");
    case 0x4E:          return String("N");
    case 0x4F:          return String("O");
    case 0x50:          return String("P");
    case 0x51:          return String("Q");
    case 0x52:          return String("R");
    case 0x53:          return String("S");
    case 0x54:          return String("T");
    case 0x55:          return String("U");
    case 0x56:          return String("V");
    case 0x57:          return String("W");
    case 0x58:          return String("X");
    case 0x59:          return String("Y");
    case 0x5A:          return String("Z");
    case VK_NUMPAD0:    return String("NUM 0");
    case VK_NUMPAD1:    return String("NUM 1");
    case VK_NUMPAD2:    return String("NUM 2");
    case VK_NUMPAD3:    return String("NUM 3");
    case VK_NUMPAD4:    return String("NUM 4");
    case VK_NUMPAD5:    return String("NUM 5");
    case VK_NUMPAD6:    return String("NUM 6");
    case VK_NUMPAD7:    return String("NUM 7");
    case VK_NUMPAD8:    return String("NUM 8");
    case VK_NUMPAD9:    return String("NUM 9");
    case VK_MULTIPLY:   return String("VK_MULTIPLY");
    case VK_ADD:        return String("VK_ADD");
    case VK_SEPARATOR:  return String("VK_SEPARATOR");
    case VK_SUBTRACT:   return String("VK_SUPTRACT");
    case VK_DECIMAL:    return String("VK_DECIMAL");
    case VK_DIVIDE:     return String("VK_DIVIDE");
    case VK_F1:         return String("F1");
    case VK_F2:         return String("F2");
    case VK_F3:         return String("F3");
    case VK_F4:         return String("F4");
    case VK_F5:         return String("F5");
    case VK_F6:         return String("F6");
    case VK_F7:         return String("F7");
    case VK_F8:         return String("F8");
    case VK_F9:         return String("F9");
    case VK_F10:        return String("F10");
    case VK_F11:        return String("F11");
    case VK_F12:        return String("F12");
    case VK_F13:        return String("F13");
    case VK_F14:        return String("F14");
    case VK_F15:        return String("F15");
    case VK_F16:        return String("F16");
    case VK_F17:        return String("F17");
    case VK_F18:        return String("F18");
    case VK_F19:        return String("F19");
    case VK_F20:        return String("F20");
    case VK_F21:        return String("F21");
    case VK_F22:        return String("F22");
    case VK_F23:        return String("F23");
    case VK_F24:        return String("F24");
    case VK_NUMLOCK:    return String("NUM LOCK");
    case VK_SCROLL:     return String("SCROLL LOCK");
    case VK_LSHIFT:     return String("LEFT SHIFT");
    case VK_RSHIFT:     return String("RIGHT SHIFT");
    case VK_LCONTROL:   return String("LEFT CONTROL");
    case VK_RCONTROL:   return String("RIGHT CONTROL");
    case VK_OEM_1:      return String("; :");
    case VK_OEM_PLUS:   return String("= +");
    case VK_OEM_COMMA:  return String(", <");
    case VK_OEM_MINUS:  return String("- _");
    case VK_OEM_PERIOD: return String(". >");
    case VK_OEM_2:      return String("/ ?");
    case VK_OEM_3:      return String("` ~");
    case VK_OEM_4:      return String("[ {");
    case VK_OEM_5:      return String("\\ |");
    case VK_OEM_6:      return String("] }");
    case VK_OEM_7:      return String("' \"");
    default: return String("???");
    }
}

