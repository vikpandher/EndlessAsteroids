/* ==========================================================================
   >File: MenuUtilities.h
   >Date: 20180713
   >Author: Vik Pandher
   >Details: Stuff used in the menus.
   ========================================================================== */

#pragma once
#include "String.h"
#include "ColorUtilities.h"


class TextBox
{
public:
    TextBox() {}
    TextBox(unsigned x, unsigned y, String text) :
        m_x(x),
        m_y(y),
        m_text(text),
        m_xScale(1),
        m_yScale(1),
        m_color0(COLOR_WHITE),
        m_color1(COLOR_WHITE),
        m_color2(COLOR_BLACK),
        m_hasBorder(false),
        m_hasBackground(false),
        m_selected(false) {}
    TextBox(unsigned x, unsigned y, String text, Color color) :
        m_x(x),
        m_y(y),
        m_text(text),
        m_xScale(1),
        m_yScale(1),
        m_color0(color),
        m_color1(color),
        m_color2(COLOR_BLACK), 
        m_hasBorder(false),
        m_hasBackground(false),
        m_selected(false) {}
    TextBox(unsigned x, unsigned y, String text, Color color0, Color color1) :
        m_x(x),
        m_y(y),
        m_text(text),
        m_xScale(1),
        m_yScale(1),
        m_color0(color0),
        m_color1(color1),
        m_color2(COLOR_BLACK),
        m_hasBorder(true),
        m_hasBackground(false),
        m_selected(false) {}
    TextBox(unsigned x, unsigned y, String text, Color color0, Color color1, Color color2) :
        m_x(x),
        m_y(y),
        m_text(text),
        m_xScale(1),
        m_yScale(1),
        m_color0(color0),
        m_color1(color1),
        m_color2(color2),
        m_hasBorder(true),
        m_hasBackground(true),
        m_selected(false) {}

    // the bottom left coordinates of the text box
    unsigned m_x, m_y;
    String m_text;

    // size scaling factors
    unsigned m_xScale, m_yScale;

    // colors are in order: text, border, fill
    Color m_color0, m_color1, m_color2;

    // if false, no boarder
    bool m_hasBorder;

    // if false no fill color and the box isn't filled (unless selected)
    bool m_hasBackground;

    // is this text box selected (will be drawn different if it is)
    bool m_selected;
};


class Button
{
public:
    Button() : m_pressed(false), m_sleepTimerStart(0), m_sleepTimer(0), m_dreamTimerStart(0), m_dreamTimer(0) {}
    Button(void(*action)()) : m_pressed(false), m_sleepTimerStart(0), m_sleepTimer(0), m_dreamTimerStart(0), m_dreamTimer(0), m_action(action) {}

    // press
    // ====================================================================== //
    // Lets this button know it has been pressed. If it has been pressed, the
    // update calls will trigger this button's given action.
    inline void press()
    {
        if (!m_pressed)
        {
            m_pressed = true;
        }
    }

    // release
    // ====================================================================== //
    // Lets this button know it has been released.
    inline void release()
    {
        if (m_pressed)
        {
            m_pressed = false;
            m_sleepTimer = 0;
        }
    }

    // update
    // ====================================================================== //
    // Triggers this button's given action, if it has been pressed and it's
    // not asleep. If it is, decrement the sleep timer.
    inline void update()
    {
        if (m_pressed)
        {
            if (m_sleepTimer == 0 && m_dreamTimer == 0)
            {
                m_action();
                m_sleepTimer = m_sleepTimerStart;
                m_dreamTimer = m_dreamTimerStart;
            }
        }

        if (m_sleepTimer > 0)
        {
            m_sleepTimer--;
        }

        if (m_dreamTimer > 0)
        {
            m_dreamTimer--;
        }
    }

    // setSleepTimer
    // ====================================================================== //
    // Sets how long the sleep timer lasts. One update call decrements it
    // once.
    inline void setSleepTimer(int sleepTimerStart)
    {
        m_sleepTimerStart = sleepTimerStart;
    }


    // setDreamTimer
    // ====================================================================== //
    // Sets how long the dream timer lasts. One update call decrements it
    // once.
    inline void setDreamTimer(int dreamTimerStart)
    {
        m_dreamTimerStart = dreamTimerStart;
    }


    // setAction
    // ====================================================================== //
    // Sets the action function to the given action function.
    // 
    // @params
    // * void(*action)(), pointer to some function the button will call
    inline void setAction(void(*action)())
    {
        m_action = action;
    }

private:
    bool m_pressed;

    // How many "frames" (update calls) the sleep timer lasts. Sleep gets reset
    // when the button is released
    int m_sleepTimerStart;
    int m_sleepTimer;

    // Similar to the sleep timer, but doesn't get reset on release
    int m_dreamTimerStart;
    int m_dreamTimer;

    void(*m_action)();
};


class TextButton : public TextBox, public Button
{
public:
    TextButton() :
        Button() {}
    TextButton(void(*action)()) :
        Button(action) {}
    TextButton(unsigned x, unsigned y, String text, void(*action)()) :
        TextBox(x, y, text),
        Button(action) {}
    TextButton(unsigned x, unsigned y, String text, Color color, void(*action)()) :
        TextBox(x, y, text, color),
        Button(action) {}
    TextButton(unsigned x, unsigned y, String text, Color color0, Color color1, void(*action)()) :
        TextBox(x, y, text, color0, color1),
        Button(action) {}
    TextButton(unsigned x, unsigned y, String text, Color color0, Color color1, Color color2, void(*action)()) :
        TextBox(x, y, text, color0, color1, color2),
        Button(action) {}
};


// getKeyString
// ========================================================================== //
// Get the string that represents the given virtual key code.
// 
// @param
// * const uint8_t k, vitual key code
// 
// @return
// String representing the key
String getKeyString(const uint8_t k);