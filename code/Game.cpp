/* ==========================================================================
   >File: Game.cpp
   >Date: 20180713
   >Author: Vik Pandher
   >Details: Here is the bulk of the game's structure.
   ========================================================================== */

#include "Game.h"


void initialize()
{
    // Set up the default key assignments
    // ---------------------------------------------------------------------- //
    s_keyKeys.setCapacity(15);
    s_keyButtons.setCapacity(15);

    s_keyKeys += VK_LEFT;
    s_keyButtons += new Button(&arrowLeftKeyAction);
    s_keyButtons[KEY_INDEX_ARROW_LEFT]->setSleepTimer(5);
    
    s_keyKeys += VK_UP;
    s_keyButtons += new Button(&arrowUpKeyAction);
    s_keyButtons[KEY_INDEX_ARROW_UP]->setSleepTimer(5);
    
    s_keyKeys += VK_RIGHT;
    s_keyButtons += new Button(&arrowRightKeyAction);
    s_keyButtons[KEY_INDEX_ARROW_RIGHT]->setSleepTimer(5);
    
    s_keyKeys += VK_DOWN;
    s_keyButtons += new Button(&arrowDownKeyAction);
    s_keyButtons[KEY_INDEX_ARROW_DOWN]->setSleepTimer(5);
    
    s_keyKeys += VK_RETURN;
    s_keyButtons += new Button(&enterKeyAction);
    s_keyButtons[KEY_INDEX_ENTER]->setSleepTimer(5);
    
    s_keyKeys += DEFAULT_ROLL_LEFT;
    s_keyButtons += new Button(&rollLeftKeyAction);

    s_keyKeys += DEFAULT_ROLL_RIGHT;
    s_keyButtons += new Button(&rollRightKeyAction);

    s_keyKeys += DEFAULT_YAW_LEFT;
    s_keyButtons += new Button(&yawLeftKeyAction);

    s_keyKeys += DEFAULT_YAW_RIGHT;
    s_keyButtons += new Button(&yawRightKeyAction);

    s_keyKeys += DEFAULT_PITCH_UP;
    s_keyButtons += new Button(&pitchUpKeyAction);

    s_keyKeys += DEFAULT_PITCH_DOWN;
    s_keyButtons += new Button(&pitchDownKeyAction);

    s_keyKeys += DEFAULT_ACCELERATE;
    s_keyButtons += new Button(&accelerateKeyAction);

    // NOTE: I noticed that on my keyboard, I'm unambe to simultaniously press 
    // some key combinations, like "Q" and "J". This might very from keybord
    // manufacturer to manufacturer. 
    s_keyKeys += DEFAULT_SHOOT;
    s_keyButtons += new Button(&shootKeyAction);
    s_keyButtons[KEY_INDEX_SHOOT]->setDreamTimer(5);

    s_keyKeys += DEFAULT_ZOOM;
    s_keyButtons += new Button(&zoomKeyAction);

    s_keyKeys += VK_RETURN;
    s_keyButtons += new Button(&pauseKeyAction);
    s_keyButtons[KEY_INDEX_PAUSE]->setDreamTimer(5); // So you can't spam it
    
    // Set up menus
    // ---------------------------------------------------------------------- //
    int width = g_screenBuffer->getWidth();
    int height = g_screenBuffer->getHeight();

    // main menu
    {
        s_mainCursor = 0;

        String title0 = "ENDLESS";
        int xPos00 = (width - ScreenBuffer::getTextBoxPixelWidth(title0)) / 2;
        int yPos00 = 7 * height / 8;
        s_mainTitle0 = new TextBox(xPos00, yPos00, title0);

        String title1 = "ASTEROIDS";
        int xPos01 = (width - ScreenBuffer::getTextBoxPixelWidth(title1, 2)) / 2;
        int yPos01 = yPos00 - ASCII_HEIGHT - 10;
        s_mainTitle1 = new TextBox(xPos01, yPos01, title1);
        s_mainTitle1->m_xScale = 2;
        s_mainTitle1->m_yScale = 2;

        String start = "START";
        int xPos02 = (width - ScreenBuffer::getTextBoxPixelWidth(start)) / 2;
        int yPos02 = height / 4;
        s_mainButtons += new TextButton(xPos02, yPos02, start, &changeGameStateToPlay);
        s_mainButtons[MAIN_INDEX_START]->m_selected = true;

        String controls = "CONTROLS";
        int xPos03 = (width - ScreenBuffer::getTextBoxPixelWidth(controls)) / 2;
        int yPos03 = yPos02 - ASCII_HEIGHT - 6;
        s_mainButtons += new TextButton(xPos03, yPos03, controls, &changeGameStateToControls);
    }

    // main menu entity
    {
        Point centerOfAttention(0, 0, 0);
        Point cameraLocation(-5, 0, 0);
        Vector upDirection(0, 1, 0);
        Vector perpendicularVector = (centerOfAttention - cameraLocation).crossProduct(upDirection);
        upDirection = perpendicularVector.crossProduct(centerOfAttention - cameraLocation);
        upDirection.normalize();
        float viewingAngle = _PI / 2;
        float nearPlane = 1;
        float farPlane = 100;
        s_mainCamera = new Camera(centerOfAttention, cameraLocation, upDirection, viewingAngle, nearPlane, farPlane);

        s_mainEntity = createShip(s_shipColor0, s_shipColor1, s_shipColor2);
        Vector rotationVector(rand() - (RAND_MAX / 2), rand() - (RAND_MAX / 2), rand() - (RAND_MAX / 2));
        rotationVector.normalize();
        s_mainEntity->angularVelocity.addRotation(rotationVector, SHIP_ROTATION);
    }

    // play menu stuff
    {
        s_zoomed = false;
        
        ///Point centerOfAttention(0, 0, 0);
        ///Point cameraLocation = 
        ///Vector upDirection(0, 1, 0);
        ///Vector perpendicularVector = (centerOfAttention - cameraLocation).crossProduct(upDirection);
        ///upDirection = perpendicularVector.crossProduct(centerOfAttention - cameraLocation);
        ///upDirection.normalize();
        float viewingAngle = _PI / 2;
        float nearPlane = 1;
        float farPlane = 2 * PLAY_BORDER_RADIUS + PLAY_CAMERA_DISTANCE + 5;
        s_playCamera = new Camera(Point(0, 0, 0), Point(0, PLAY_CAMERA_DISTANCE, 0), Vector(1, 0, 0), viewingAngle, nearPlane, farPlane);

        s_playShip = createShip(s_shipColor0, s_shipColor1, s_shipColor2);

        s_playTrailR = createTrail(5, s_trailColor0, s_trailColor1);
        s_playTrailL = createTrail(5, s_trailColor0, s_trailColor1);
        s_playLaser = createLaser(20, 10, s_laserColor0, s_laserColor1);

        s_playBorder = createBorder(PLAY_BORDER_RADIUS, 2, s_borderColor);
    }

    // pause menu
    {
        s_pauseCursor = 0;

        String resume = "RESUME";
        int xPos0 = (width - ScreenBuffer::getTextBoxPixelWidth(resume)) / 2;
        int yPos0 = 3 * height / 4;
        s_pauseButtons += new TextButton(xPos0, yPos0, resume, &changeGameStateToPlay);
        s_pauseButtons[PAUSE_INDEX_RESUME]->m_selected = true;

        String quit = "QUIT";
        int xPos1 = (width - ScreenBuffer::getTextBoxPixelWidth(quit)) / 2;
        int yPos1 = 2 * height / 4;
        s_pauseButtons += new TextButton(xPos1, yPos1, quit, &changeGameStateToEnd);
    }

    // end menu
    {
        s_endCursor = 0;

        String back = "BACK";
        int xPos0 = (width - ScreenBuffer::getTextBoxPixelWidth(back)) / 2;
        int yPos0 = height / 4;
        s_endButtons += new TextButton(xPos0, yPos0, back, &changeGameStateToMain);
        s_endButtons[END_INDEX_BACK]->m_selected = true;
    }

    // controls menu
    {
        s_controlsCursor = 0;

        String back = "BACK";
        int xPos0 = (width - ScreenBuffer::getTextBoxPixelWidth(back)) / 2;
        int yPos0 = 7 * height / 8;
        s_controlsButtons += new TextButton(xPos0, yPos0, back, &changeGameStateToMain);
        s_controlsButtons[CONTROLS_INDEX_BACK]->m_selected = true;

        int columnShift = 1 * width / 2;
        int xPosKeys = columnShift + 6;

        String rollLeft = "ROLL LEFT";
        int xPos1 = columnShift - ScreenBuffer::getTextBoxPixelWidth(rollLeft) - 6;
        int yPos1 = yPos0 - 2 * (ASCII_HEIGHT + 2);
        s_controlsBoxes += new TextBox(xPos1, yPos1, rollLeft);

        String rollLeftKey = getKeyString(s_keyKeys[KEY_INDEX_ROLL_LEFT]);
        s_controlsButtons += new TextButton(xPosKeys, yPos1, rollLeftKey, &changeGameStateToKeyChange);

        String rollRight = "ROLL RIGHT";
        int xPos2 = columnShift - ScreenBuffer::getTextBoxPixelWidth(rollRight) - 6;
        int yPos2 = yPos1 - ASCII_HEIGHT - 6;
        s_controlsBoxes += new TextBox(xPos2, yPos2, rollRight);

        String rollRightKey = getKeyString(s_keyKeys[KEY_INDEX_ROLL_RIGHT]);
        s_controlsButtons += new TextButton(xPosKeys, yPos2, rollRightKey, &changeGameStateToKeyChange);

        String yawLeft = "YAW LEFT";
        int xPos3 = columnShift - ScreenBuffer::getTextBoxPixelWidth(yawLeft) - 6;
        int yPos3 = yPos2 - ASCII_HEIGHT - 6;
        s_controlsBoxes += new TextBox(xPos3, yPos3, yawLeft);

        String yawLeftKey = getKeyString(s_keyKeys[KEY_INDEX_YAW_LEFT]);
        s_controlsButtons += new TextButton(xPosKeys, yPos3, yawLeftKey, &changeGameStateToKeyChange);

        String yawRight = "YAW RIGHT";
        int xPos4 = columnShift - ScreenBuffer::getTextBoxPixelWidth(yawRight) - 6;
        int yPos4 = yPos3 - ASCII_HEIGHT - 6;
        s_controlsBoxes += new TextBox(xPos4, yPos4, yawRight);

        String yawRightKey = getKeyString(s_keyKeys[KEY_INDEX_YAW_RIGHT]);
        s_controlsButtons += new TextButton(xPosKeys, yPos4, yawRightKey, &changeGameStateToKeyChange);

        String pitchUp = "PITCH UP";
        int xPos5 = columnShift - ScreenBuffer::getTextBoxPixelWidth(pitchUp) - 6;
        int yPos5 = yPos4 - ASCII_HEIGHT - 6;
        s_controlsBoxes += new TextBox(xPos5, yPos5, pitchUp);

        String pitchUpKey = getKeyString(s_keyKeys[KEY_INDEX_PITCH_UP]);
        s_controlsButtons += new TextButton(xPosKeys, yPos5, pitchUpKey, &changeGameStateToKeyChange);

        String pitchDown = "PITCH DOWN";
        int xPos6 = columnShift - ScreenBuffer::getTextBoxPixelWidth(pitchDown) - 6;
        int yPos6 = yPos5 - ASCII_HEIGHT - 6;
        s_controlsBoxes += new TextBox(xPos6, yPos6, pitchDown);

        String pitchDownKey = getKeyString(s_keyKeys[KEY_INDEX_PITCH_DOWN]);
        s_controlsButtons += new TextButton(xPosKeys, yPos6, pitchDownKey, &changeGameStateToKeyChange);

        String accelerate = "ACCELERATE";
        int xPos7 = columnShift - ScreenBuffer::getTextBoxPixelWidth(accelerate) - 6;
        int yPos7 = yPos6 - ASCII_HEIGHT - 6;
        s_controlsBoxes += new TextBox(xPos7, yPos7, accelerate);

        String accelerateKey = getKeyString(s_keyKeys[KEY_INDEX_ACCELERATE]);
        s_controlsButtons += new TextButton(xPosKeys, yPos7, accelerateKey, &changeGameStateToKeyChange);

        String shoot = "SHOOT";
        int xPos8 = columnShift - ScreenBuffer::getTextBoxPixelWidth(shoot) - 6;
        int yPos8 = yPos7 - ASCII_HEIGHT - 6;
        s_controlsBoxes += new TextBox(xPos8, yPos8, shoot);

        String shootKey = getKeyString(s_keyKeys[KEY_INDEX_SHOOT]);
        s_controlsButtons += new TextButton(xPosKeys, yPos8, shootKey, &changeGameStateToKeyChange);

        String zoom = "ZOOM";
        int xPos9 = columnShift - ScreenBuffer::getTextBoxPixelWidth(zoom) - 6;
        int yPos9 = yPos8 - ASCII_HEIGHT - 6;
        s_controlsBoxes += new TextBox(xPos9, yPos9, zoom);

        String zoomKey = getKeyString(s_keyKeys[KEY_INDEX_ZOOM]);
        s_controlsButtons += new TextButton(xPosKeys, yPos9, zoomKey, &changeGameStateToKeyChange);

        String default_ = "DEFAULT";
        int xPos10 = (width - ScreenBuffer::getTextBoxPixelWidth(default_)) / 2;
        int yPos10 = yPos9 - 2 * (ASCII_HEIGHT + 2);
        s_controlsButtons += new TextButton(xPos10, yPos10, default_, &setKeysToDefaults);
    }

    // key change menu
    {
        s_returnFromKeyChangeButton = new Button(&changeGameStateToControls);
    }

    changeGameStateToMain();
}


void deinitialize()
{
    // delete everything
}


void keyDown(const uint8_t k)
{
    for (int i = 0; i < s_keyKeys.size(); i++)
    {
        if (k == s_keyKeys[i])
        {
            s_keyButtons[i]->press();
        }
    }

    // This state translation occurs on update, but the key change is instant.
    if (s_gameState == GS_KEY_CHANGE)
    {
        s_keyKeys[s_controlsCursor + 4] = k;
        s_controlsButtons[s_controlsCursor]->m_text = getKeyString(k);
        s_returnFromKeyChangeButton->press();
    }
}


void keyUp(const uint8_t k)
{
    // Need to know when zoom has been releasesd
    if (k == s_keyKeys[KEY_INDEX_ZOOM])
    {
        s_zoomed = false;
    }
    
    for (int i = 0; i < s_keyKeys.size(); i++)
    {
        if (k == s_keyKeys[i])
        {
            s_keyButtons[i]->release();
        }
    }
}


void update()
{
    for (int i = 0; i < s_keyButtons.size(); i++)
    {
        s_keyButtons[i]->update();
    }

    switch (s_gameState)
    {
    case GS_MAIN:
    {
        for (int i = 0; i < s_mainButtons.size(); i++)
        {
            s_mainButtons[i]->update();
            s_mainButtons[i]->release();
        }
        s_mainEntity->update();
        break;
    }
    case GS_PLAY:
    {
        triggerEntitySpawner();

        if (!s_zoomed)
        {
            alignPlayCamera(*s_playShip, *s_playCamera, PLAY_CAMERA_DISTANCE, 0.1);
        }
        else
        {
            alignZoomCamera(*s_playShip, *s_playCamera, PLAY_CAMERA_DISTANCE, 1);
        }
        
        Array<Entity*> collidableEntities;
        collidableEntities += s_playShip;
        collidableEntities += s_playAsteroids;
        collidableEntities += s_playSaucers;
        collidableEntities += s_playBullets;

        calculateEntityCollisions(collidableEntities);
        calculateBorderCollisions(collidableEntities);

        hardCapSpeed(s_playShip, HARD_SPEED_LIMIT);
        softCapSpeed(s_playShip, SOFT_SPEED_LIMIT, LIMIT_DECELERATION);
        for (int i = 0; i < s_playAsteroids.size(); i++)
        {
            hardCapSpeed(s_playAsteroids[i], HARD_SPEED_LIMIT);
        }
        for (int i = 0; i < s_playSaucers.size(); i++)
        {
            hardCapSpeed(s_playSaucers[i], HARD_SPEED_LIMIT);
        }
        for (int i = 0; i < s_playBullets.size(); i++)
        {
            hardCapSpeed(s_playBullets[i], SHIP_BULLET_SPEED_LIMIT);
        }

        // HACKY BUG???
        // ---------------------------------------------------------------------
        // I tried only updating s_playBorder once, durring the initialization,
        // but for whatever reason if I don't update it here I get a read
        // access violation durring the draw step. But what are you gonna do?
        s_playBorder->update();

        s_playShip->update();
        for (int i = 0; i < s_playAsteroids.size(); i++)
        {
            s_playAsteroids[i]->update();
        }
        for (int i = 0; i < s_playSaucers.size(); i++)
        {
            s_playSaucers[i]->update();
        }
        for (int i = 0; i < s_playBullets.size(); i++)
        {
            s_playBullets[i]->update();
        }

        updateRTrailFrame(s_playTrailR->frame, s_playShip);
        updateLTrailFrame(s_playTrailL->frame, s_playShip);
        updateLaserFrame(s_playLaser->frame, 10, s_playShip);
        s_playTrailR->update();
        s_playTrailL->update();
        s_playLaser->update();

        for (int i = 0; i < s_playFlowers.size(); i++)
        {
            updateFlowerFrame(s_playFlowers[i]->frame);
            s_playFlowers[i]->update();
        }

        for (int i = 0; i < s_limboCounters.size();)
        {            
            // An entity is getting deleted
            if (s_limboCounters[i] == 0)
            {
                // Find out which array the entity resides in
                switch (s_limboEntities[i]->typeID)
                {
                case ENTITY_ID_FLOWER:
                {
                    deleteFromLimbo(i, s_playFlowers);
                    break;
                }
                case ENTITY_ID_SHIP_BULLET:
                case ENTITY_ID_SAUCER_BULLET:
                {
                    deleteFromLimbo(i, s_playBullets);
                    break;
                }
                case ENTITY_ID_ASTEROID:
                {
                    deleteFromLimbo(i, s_playAsteroids);
                    break;
                }
                case ENTITY_ID_SAUCER:
                {
                    deleteFromLimbo(i, s_playSaucers);
                    break;
                }
                case ENTITY_ID_SHIP:
                {
                    s_limboEntities.remove(i);
                    s_limboCounters.remove(i);
                    s_playShip->health *= -1;
                    s_playShip->health--;
                    if (s_playShip->health == 0)
                    {
                        // Game Over
                        changeGameStateToEnd();
                    }
                    else
                    {
                        s_playShip->drawProperties = DRAW_TRIANGLES | DRAW_DISTANCE_SHADING_OFF;
                    }
                    break;
                }
                default: // ??? Shouldn't be here
                    break;
                }
            }
            else
            {
                s_limboCounters[i] = s_limboCounters[i] - 1;
                i++;
            }
        }

        break;
    }
    case GS_PAUSE:
    {
        for (int i = 0; i < s_pauseButtons.size(); i++)
        {
            s_pauseButtons[i]->update();
            s_pauseButtons[i]->release();
        }
        break;
    }
    case GS_END:
    {
        for (int i = 0; i < s_endButtons.size(); i++)
        {
            s_endButtons[i]->update();
            s_endButtons[i]->release();
        }
        break;
    }
    ///case GS_SCORES:
    ///{
    ///    break;
    ///}
    case GS_CONTROLS:
    {
        for (int i = 0; i < s_controlsButtons.size(); i++)
        {
            s_controlsButtons[i]->update();
            s_controlsButtons[i]->release();
        }
        break;
    }
    case GS_KEY_CHANGE:
    {
        s_returnFromKeyChangeButton->update();
        s_returnFromKeyChangeButton->release();
        break;
    }
    }
}


void draw()
{
    g_screenBuffer->clear(COLOR_BLACK);
    switch (s_gameState)
    {
    case GS_MAIN:
    {
        g_screenBuffer->rasterize(*s_mainCamera, s_mainEntity);
        
        g_screenBuffer->drawTextBox(*s_mainTitle0);
        g_screenBuffer->drawTextBox(*s_mainTitle1);
        for (int i = 0; i < s_mainButtons.size(); i++)
        {
            g_screenBuffer->drawTextBox(*(s_mainButtons[i]));
        }
        String note("Created By:");
        g_screenBuffer->drawString(5, 5 + ASCII_HEIGHT + 1, note, COLOR_WHITE / 4);
        g_screenBuffer->drawString(5, 5, AUTHOR_STRING, COLOR_WHITE / 4);
        break;
    }
    case GS_PLAY:
    {
        if(!s_zoomed) g_screenBuffer->rasterize(*s_playCamera, s_playShip);
        g_screenBuffer->rasterize(*s_playCamera, s_playBorder);
        g_screenBuffer->rasterizeGroup(*s_playCamera, s_playAsteroids);
        g_screenBuffer->rasterizeGroup(*s_playCamera, s_playSaucers);
        g_screenBuffer->rasterizeGroup(*s_playCamera, s_playBullets);
        g_screenBuffer->rasterize(*s_playCamera, s_playTrailR);
        g_screenBuffer->rasterize(*s_playCamera, s_playTrailL);
        g_screenBuffer->rasterize(*s_playCamera, s_playLaser);
        g_screenBuffer->rasterizeGroup(*s_playCamera, s_playFlowers);
        int width = g_screenBuffer->getWidth();
        int height = g_screenBuffer->getHeight();
        String scoreString = String::stringFromInt(s_score);
        if (s_playShip->health > 0)
        {
            g_screenBuffer->drawHearts(s_playShip->health, 6, height - ASCII_HEIGHT - 5, COLOR_RED);
        }
        else
        {
            g_screenBuffer->drawHearts(ABS(s_playShip->health) - 1, 6, height - ASCII_HEIGHT - 5, COLOR_RED / 2);
        }
        g_screenBuffer->drawString(width - ScreenBuffer::getStringPixelWidth(scoreString) - 5, height - ASCII_HEIGHT - 5, scoreString, COLOR_WHITE);
        break;
    }
    case GS_PAUSE:
    {
        if (!s_zoomed) g_screenBuffer->rasterize(*s_playCamera, s_playShip);
        g_screenBuffer->rasterize(*s_playCamera, s_playBorder);
        g_screenBuffer->rasterizeGroup(*s_playCamera, s_playAsteroids);
        g_screenBuffer->rasterizeGroup(*s_playCamera, s_playSaucers);
        g_screenBuffer->rasterizeGroup(*s_playCamera, s_playBullets);
        g_screenBuffer->rasterize(*s_playCamera, s_playTrailR);
        g_screenBuffer->rasterize(*s_playCamera, s_playTrailL);
        g_screenBuffer->rasterize(*s_playCamera, s_playLaser);
        g_screenBuffer->rasterizeGroup(*s_playCamera, s_playFlowers);
        int width = g_screenBuffer->getWidth();
        int height = g_screenBuffer->getHeight();
        String scoreString = String::stringFromInt(s_score);
        if (s_playShip->health > 0)
        {
            g_screenBuffer->drawHearts(s_playShip->health, 6, height - ASCII_HEIGHT - 5, COLOR_RED);
        }
        else
        {
            g_screenBuffer->drawHearts(ABS(s_playShip->health) - 1, 6, height - ASCII_HEIGHT - 5, COLOR_RED / 2);
        }
        g_screenBuffer->drawString(width - ScreenBuffer::getStringPixelWidth(scoreString) - 5, height - ASCII_HEIGHT - 5, scoreString, COLOR_WHITE);
        g_screenBuffer->fade(2);
        for (int i = 0; i < s_pauseButtons.size(); i++)
        {
            g_screenBuffer->drawTextBox(*(s_pauseButtons[i]));
        }
        break;
    }
    case GS_END:
    {
        int width = g_screenBuffer->getWidth();
        int height = g_screenBuffer->getHeight();
        String finalScore = String("FINAL SCORE:");
        g_screenBuffer->drawString((width - ScreenBuffer::getStringPixelWidth(finalScore)) / 2, 3 * (height - ASCII_HEIGHT) / 4, finalScore, COLOR_WHITE);
        String scoreString = String::stringFromInt(s_score);
        g_screenBuffer->drawString((width - ScreenBuffer::getStringPixelWidth(scoreString)) / 2, (height - ASCII_HEIGHT) / 2, scoreString, COLOR_WHITE);
        for (int i = 0; i < s_endButtons.size(); i++)
        {
            g_screenBuffer->drawTextBox(*(s_endButtons[i]));
        }
        break;
    }
    ///case GS_SCORES :
    ///{
    ///    break;
    ///}
    case GS_CONTROLS:
    {
        for (int i = 0; i < s_controlsButtons.size(); i++)
        {
            g_screenBuffer->drawTextBox(*(s_controlsButtons[i]));
        }
        for (int i = 0; i < s_controlsBoxes.size(); i++)
        {
            g_screenBuffer->drawTextBox(*(s_controlsBoxes[i]));
        }
        break;
    }
    case GS_KEY_CHANGE:
    {
        for (int i = 0; i < s_controlsButtons.size(); i++)
        {
            g_screenBuffer->drawTextBox(*(s_controlsButtons[i]));
        }
        for (int i = 0; i < s_controlsBoxes.size(); i++)
        {
            g_screenBuffer->drawTextBox(*(s_controlsBoxes[i]));
        }
        g_screenBuffer->fade(16);

        String message("Enter new key for ");

        switch (s_controlsCursor)
        {
        case CONTROLS_INDEX_ROLL_LEFT :
        {
            message += "ROLL LEFT";
            break;
        }
        case CONTROLS_INDEX_ROLL_RIGHT :
        {
            message += "ROLL RIGHT";
            break;
        }
        case CONTROLS_INDEX_YAW_LEFT :
        {
            message += "YAW LEFT";
            break;
        }
        case CONTROLS_INDEX_YAW_RIGHT :
        {
            message += "YAW RIGHT";
            break;
        }
        case CONTROLS_INDEX_PITCH_UP :
        {
            message += "PITCH UP";
            break;
        }
        case CONTROLS_INDEX_PITCH_DOWN :
        {
            message += "PITCH DOWN";
            break;
        }
        case CONTROLS_INDEX_ACCELERATE :
        {
            message += "ACCELERATE";
            break;
        }
        case CONTROLS_INDEX_SHOOT :
        {
            message += "SHOOT";
            break;
        }
        }
        int xPos = (g_screenBuffer->getWidth() - ScreenBuffer::getStringPixelWidth(message)) / 2;
        int yPos = 3 * (g_screenBuffer->getHeight() - ScreenBuffer::getStringPixelHeight(message)) / 4;
        g_screenBuffer->drawString(xPos, yPos, message, COLOR_WHITE);
        break;
    }
    }

    g_screenBuffer->drawString(g_screenBuffer->getWidth() - ScreenBuffer::getStringPixelWidth(VERSION_STRING) - 5, 5, VERSION_STRING, COLOR_WHITE / 4);
}


// changeGameStateTo____

void changeGameStateToMain()
{
    s_gameState = GS_MAIN;
    s_score = 0;
    s_scoreTillNectHeart = 1000;
    s_gameCounter = 0;
    s_playShip->health = SHIP_HEALTH;
    s_playShip->drawProperties = DRAW_TRIANGLES | DRAW_DISTANCE_SHADING_OFF;
    s_playShip->locationPoint = Point(0, 0, 0);
    s_playShip->velocity = Vector(0, 0, 0);
    alignPlayCamera(*s_playShip, *s_playCamera, PLAY_CAMERA_DISTANCE, 1);
}


void changeGameStateToPlay()
{
    s_gameState = GS_PLAY;
}


void changeGameStateToPause()
{
    s_gameState = GS_PAUSE;
}


void changeGameStateToEnd()
{
    s_gameState = GS_END;

    // clear entities
    // -------------------------------------------------------------------------
    for (int i = 0; i < s_limboEntities.size();)
    {
        s_limboEntities.remove(i);
    }
    for (int i = 0; i < s_limboCounters.size();)
    {
        s_limboCounters.remove(i);
    }
    for (int i = 0; i < s_playAsteroids.size();)
    {
        delete s_playAsteroids[i];
        s_playAsteroids.remove(i);
    }
    for (int i = 0; i < s_playSaucers.size();)
    {
        delete s_playSaucers[i];
        s_playSaucers.remove(i);
    }
    for (int i = 0; i < s_playBullets.size();)
    {
        delete s_playBullets[i];
        s_playBullets.remove(i);
    }
    for (int i = 0; i < s_playFlowers.size();)
    {
        delete s_playFlowers[i];
        s_playFlowers.remove(i);
    }
}


void changeGameStateToControls()
{
    s_gameState = GS_CONTROLS;
}


void changeGameStateToKeyChange()
{
    s_gameState = GS_KEY_CHANGE;
}


// ____KeyAction

void arrowLeftKeyAction()
{
    switch (s_gameState)
    {
    case GS_MAIN:
    {
        break;
    }
    case GS_PLAY:
    {
        break;
    }
    case GS_PAUSE:
    {
        break;
    }
    case GS_END:
    {
        break;
    }
    ///case GS_SCORES :
    ///{
    ///    break;
    ///}
    case GS_CONTROLS:
    {
        break;
    }
    case GS_KEY_CHANGE:
    {
        break;
    }
    }
}


void arrowUpKeyAction()
{
    switch (s_gameState)
    {
    case GS_MAIN:
    {
        if (s_mainCursor > 0)
        {
            s_mainButtons[s_mainCursor]->m_selected = false;
            s_mainCursor--;
            s_mainButtons[s_mainCursor]->m_selected = true;
        }
        
        // change main entity rotation
        s_mainEntity->angularVelocity.setToIdentity();
        Vector rotationVector(rand() - (RAND_MAX / 2), rand() - (RAND_MAX / 2), rand() - (RAND_MAX / 2));
        rotationVector.normalize();
        s_mainEntity->angularVelocity.addRotation(rotationVector, _PI / 64);
        break;
    }
    case GS_PLAY:
    {
        break;
    }
    case GS_PAUSE:
    {
        if (s_pauseCursor > 0)
        {
            s_pauseButtons[s_pauseCursor]->m_selected = false;
            s_pauseCursor--;
            s_pauseButtons[s_pauseCursor]->m_selected = true;
        }
        break;
    }
    case GS_END:
    {
        if (s_endCursor > 0)
        {
            s_endButtons[s_endCursor]->m_selected = false;
            s_endCursor--;
            s_endButtons[s_endCursor]->m_selected = true;
        }
        break;
    }
    ///case GS_SCORES :
    ///{
    ///    break;
    ///}
    case GS_CONTROLS:
    {
        if (s_controlsCursor > 0)
        {
            s_controlsButtons[s_controlsCursor]->m_selected = false;
            s_controlsCursor--;
            s_controlsButtons[s_controlsCursor]->m_selected = true;
        }
        break;
    }
    case GS_KEY_CHANGE:
    {
        break;
    }
    }
}


void arrowRightKeyAction()
{
    switch (s_gameState)
    {
    case GS_MAIN:
    {
        break;
    }
    case GS_PLAY:
    {
        break;
    }
    case GS_PAUSE:
    {
        break;
    }
    case GS_END:
    {
        break;
    }
    ///case GS_SCORES :
    ///{
    ///    break;
    ///}
    case GS_CONTROLS:
    {
        break;
    }
    case GS_KEY_CHANGE:
    {
        break;
    }
    }
}


void arrowDownKeyAction()
{
    switch (s_gameState)
    {
    case GS_MAIN:
    {
        if (s_mainCursor < s_mainButtons.size() - 1)
        {
            s_mainButtons[s_mainCursor]->m_selected = false;
            s_mainCursor++;
            s_mainButtons[s_mainCursor]->m_selected = true;
        }

        // change main entity rotation
        s_mainEntity->angularVelocity.setToIdentity();
        Vector rotationVector(rand() - (RAND_MAX / 2), rand() - (RAND_MAX / 2), rand() - (RAND_MAX / 2));
        rotationVector.normalize();
        s_mainEntity->angularVelocity.addRotation(rotationVector, _PI / 64);
        break;
    }
    case GS_PLAY:
    {
        break;
    }
    case GS_PAUSE:
    {
        if (s_pauseCursor < s_pauseButtons.size() - 1)
        {
            s_pauseButtons[s_pauseCursor]->m_selected = false;
            s_pauseCursor++;
            s_pauseButtons[s_pauseCursor]->m_selected = true;
        }
        break;
    }
    case GS_END:
    {
        if (s_endCursor < s_endButtons.size() - 1)
        {
            s_endButtons[s_endCursor]->m_selected = false;
            s_endCursor++;
            s_endButtons[s_endCursor]->m_selected = true;
        }
        break;
    }
    ///case GS_SCORES :
    ///{
    ///    break;
    ///}
    case GS_CONTROLS:
    {
        if (s_controlsCursor < s_controlsButtons.size() - 1)
        {
            s_controlsButtons[s_controlsCursor]->m_selected = false;
            s_controlsCursor++;
            s_controlsButtons[s_controlsCursor]->m_selected = true;
        }
        break;
    }
    case GS_KEY_CHANGE:
    {
        break;
    }
    }
}


void enterKeyAction()
{
    switch (s_gameState)
    {
    case GS_MAIN:
    {
        s_mainButtons[s_mainCursor]->press();
        break;
    }
    case GS_PLAY:
    {
        break;
    }
    case GS_PAUSE:
    {
        s_pauseButtons[s_pauseCursor]->press();
        break;
    }
    case GS_END:
    {
        s_endButtons[s_endCursor]->press();
        break;
    }
    ///case GS_SCORES :
    ///{
    ///    break;
    ///}
    case GS_CONTROLS:
    {
        s_controlsButtons[s_controlsCursor]->press();
        break;
    }
    case GS_KEY_CHANGE:
    {
        break;
    }
    }
}


void rollLeftKeyAction()
{
    switch (s_gameState)
    {
    case GS_MAIN:
    {
        break;
    }
    case GS_PLAY:
    {
        if (!s_zoomed)
        {
            s_playShip->orientation.addRotation(Vector(1, 0, 0), SHIP_ROTATION);
        }
        else
        {
            s_playShip->orientation.addRotation(Vector(1, 0, 0), ZOOM_ROTATION);
        }
        break;
    }
    case GS_PAUSE:
    {
        break;
    }
    case GS_END:
    {
        break;
    }
    ///case GS_SCORES :
    ///{
    ///    break;
    ///}
    case GS_CONTROLS:
    {
        break;
    }
    case GS_KEY_CHANGE:
    {
        break;
    }
    }
}


void rollRightKeyAction()
{
    switch (s_gameState)
    {
    case GS_MAIN:
    {
        break;
    }
    case GS_PLAY:
    {
        if (!s_zoomed)
        {
            s_playShip->orientation.addRotation(Vector(1, 0, 0), -SHIP_ROTATION);
        }
        else
        {
            s_playShip->orientation.addRotation(Vector(1, 0, 0), -ZOOM_ROTATION);
        }
        break;
    }
    case GS_PAUSE:
    {
        break;
    }
    case GS_END:
    {
        break;
    }
    ///case GS_SCORES :
    ///{
    ///    break;
    ///}
    case GS_CONTROLS:
    {
        break;
    }
    case GS_KEY_CHANGE:
    {
        break;
    }
    }
}


void yawLeftKeyAction()
{
    switch (s_gameState)
    {
    case GS_MAIN:
    {
        break;
    }
    case GS_PLAY:
    {
        if (!s_zoomed)
        {
            s_playShip->orientation.addRotation(Vector(0, 1, 0), -SHIP_ROTATION);
        }
        else
        {
            s_playShip->orientation.addRotation(Vector(0, 1, 0), -ZOOM_ROTATION);
        }
        break;
    }
    case GS_PAUSE:
    {
        break;
    }
    case GS_END:
    {
        break;
    }
    ///case GS_SCORES :
    ///{
    ///    break;
    ///}
    case GS_CONTROLS:
    {
        break;
    }
    case GS_KEY_CHANGE:
    {
        break;
    }
    }
}


void yawRightKeyAction()
{
    switch (s_gameState)
    {
    case GS_MAIN:
    {
        break;
    }
    case GS_PLAY:
    {
        if (!s_zoomed)
        {
            s_playShip->orientation.addRotation(Vector(0, 1, 0), SHIP_ROTATION);
        }
        else
        {
            s_playShip->orientation.addRotation(Vector(0, 1, 0), ZOOM_ROTATION);
        }
        break;
    }
    case GS_PAUSE:
    {
        break;
    }
    case GS_END:
    {
        break;
    }
    ///case GS_SCORES :
    ///{
    ///    break;
    ///}
    case GS_CONTROLS:
    {
        break;
    }
    case GS_KEY_CHANGE:
    {
        break;
    }
    }
}


void pitchUpKeyAction()
{
    switch (s_gameState)
    {
    case GS_MAIN:
    {
        break;
    }
    case GS_PLAY:
    {
        if (!s_zoomed)
        {
            s_playShip->orientation.addRotation(Vector(0, 0, 1), -SHIP_ROTATION);
        }
        else
        {
            s_playShip->orientation.addRotation(Vector(0, 0, 1), -ZOOM_ROTATION);
        }
        break;
    }
    case GS_PAUSE:
    {
        break;
    }
    case GS_END:
    {
        break;
    }
    ///case GS_SCORES :
    ///{
    ///    break;
    ///}
    case GS_CONTROLS:
    {
        break;
    }
    case GS_KEY_CHANGE:
    {
        break;
    }
    }
}


void pitchDownKeyAction()
{
    switch (s_gameState)
    {
    case GS_MAIN:
    {
        break;
    }
    case GS_PLAY:
    {
        if (!s_zoomed)
        {
            s_playShip->orientation.addRotation(Vector(0, 0, 1), SHIP_ROTATION);
        }
        else
        {
            s_playShip->orientation.addRotation(Vector(0, 0, 1), ZOOM_ROTATION);
        }
        break;
    }
    case GS_PAUSE:
    {
        break;
    }
    case GS_END:
    {
        break;
    }
    ///case GS_SCORES :
    ///{
    ///    break;
    ///}
    case GS_CONTROLS:
    {
        break;
    }
    case GS_KEY_CHANGE:
    {
        break;
    }
    }
}


void accelerateKeyAction()
{
    switch (s_gameState)
    {
    case GS_MAIN:
    {
        break;
    }
    case GS_PLAY:
    {
        if (s_playShip->velocity.magnitude() < SOFT_SPEED_LIMIT)
        {
            Matrix rotationMatrix = s_playShip->orientation.getMatrix();
            s_playShip->velocity += Vector(SHIP_ACCELERATION, 0, 0) * rotationMatrix;
        }
        ///Vector direction = Vector(1, 0, 0) * rotationMatrix;
        ///s_playShip->locationPoint += direction;
        ///s_playCamera->centerOfAttention += direction;
        ///s_playCamera->cameraLocation += direction;
        break;
    }
    case GS_PAUSE:
    {
        break;
    }
    case GS_END:
    {
        break;
    }
    ///case GS_SCORES :
    ///{
    ///    break;
    ///}
    case GS_CONTROLS:
    {
        break;
    }
    case GS_KEY_CHANGE:
    {
        break;
    }
    }
}


void shootKeyAction()
{
    switch (s_gameState)
    {
    case GS_MAIN:
    {
        break;
    }
    case GS_PLAY:
    {
        if (s_playShip->health > 0)
        {
            s_playBullets += spawnShipBullet(s_playShip, s_shipBulletColor, SHIP_BULLET_SPEED_LIMIT);
            s_score--;
        }
        break;
    }
    case GS_PAUSE:
    {
        break;
    }
    case GS_END:
    {
        break;
    }
    ///case GS_SCORES :
    ///{
    ///    break;
    ///}
    case GS_CONTROLS:
    {
        break;
    }
    case GS_KEY_CHANGE:
    {
        break;
    }
    }
}


void zoomKeyAction()
{
    switch (s_gameState)
    {
    case GS_MAIN:
    {
        break;
    }
    case GS_PLAY:
    {
        s_zoomed = true;
        break;
    }
    case GS_PAUSE:
    {
        break;
    }
    case GS_END:
    {
        break;
    }
    ///case GS_SCORES :
    ///{
    ///    break;
    ///}
    case GS_CONTROLS:
    {
        break;
    }
    case GS_KEY_CHANGE:
    {
        break;
    }
    }
}


void pauseKeyAction()
{
    switch (s_gameState)
    {
    case GS_MAIN:
    {
        break;
    }
    case GS_PLAY:
    {
        changeGameStateToPause();
        break;
    }
    case GS_PAUSE:
    {
        break;
    }
    case GS_END:
    {
        break;
    }
    ///case GS_SCORES :
    ///{
    ///    break;
    ///}
    case GS_CONTROLS:
    {
        break;
    }
    case GS_KEY_CHANGE:
    {
        break;
    }
    }
}


// ...

void setKeysToDefaults()
{
    s_keyKeys[KEY_INDEX_ROLL_LEFT] = DEFAULT_ROLL_LEFT;
    s_controlsButtons[CONTROLS_INDEX_ROLL_LEFT]->m_text = getKeyString(DEFAULT_ROLL_LEFT);

    s_keyKeys[KEY_INDEX_ROLL_RIGHT] = DEFAULT_ROLL_RIGHT;
    s_controlsButtons[CONTROLS_INDEX_ROLL_RIGHT]->m_text = getKeyString(DEFAULT_ROLL_RIGHT);

    s_keyKeys[KEY_INDEX_YAW_LEFT] = DEFAULT_YAW_LEFT;
    s_controlsButtons[CONTROLS_INDEX_YAW_LEFT]->m_text = getKeyString(DEFAULT_YAW_LEFT);

    s_keyKeys[KEY_INDEX_YAW_RIGHT] = DEFAULT_YAW_RIGHT;
    s_controlsButtons[CONTROLS_INDEX_YAW_RIGHT]->m_text = getKeyString(DEFAULT_YAW_RIGHT);

    s_keyKeys[KEY_INDEX_PITCH_UP] = DEFAULT_PITCH_UP;
    s_controlsButtons[CONTROLS_INDEX_PITCH_UP]->m_text = getKeyString(DEFAULT_PITCH_UP);

    s_keyKeys[KEY_INDEX_PITCH_DOWN] = DEFAULT_PITCH_DOWN;
    s_controlsButtons[CONTROLS_INDEX_PITCH_DOWN]->m_text = getKeyString(DEFAULT_PITCH_DOWN);

    s_keyKeys[KEY_INDEX_ACCELERATE] = DEFAULT_ACCELERATE;
    s_controlsButtons[CONTROLS_INDEX_ACCELERATE]->m_text = getKeyString(DEFAULT_ACCELERATE);

    s_keyKeys[KEY_INDEX_SHOOT] = DEFAULT_SHOOT;
    s_controlsButtons[CONTROLS_INDEX_SHOOT]->m_text = getKeyString(DEFAULT_SHOOT);

    s_keyKeys[KEY_INDEX_ZOOM] = DEFAULT_ZOOM;
    s_controlsButtons[CONTROLS_INDEX_ZOOM]->m_text = getKeyString(DEFAULT_ZOOM);
}


void addToLimbo(Entity * entity, int counter)
{
    for (int i = 0; i < s_limboEntities.size(); i++)
    {
        if (entity == s_limboEntities[i])
        {
            return;
        }
    }

    s_limboEntities += entity;
    s_limboCounters += counter;
}


void deleteFromLimbo(int limboIndex, Array<Entity*> & searchEntities)
{
    Entity * deadEntity = s_limboEntities[limboIndex];
    s_limboEntities.remove(limboIndex);
    s_limboCounters.remove(limboIndex);
    
    for (int i = 0; i < searchEntities.size();)
    {
        if (deadEntity == searchEntities[i])
        {
            searchEntities.remove(i);
            delete deadEntity;
            break;
        }
        else
        {
            i++;
        }
    }
}


void calculateEntityCollisions(Array<Entity*> & entities)
{
    struct Collision
    {
        Entity * entityA;
        Entity * entityB;
        Point collisionLocation;
    };

    // Holds all the found collisions
    Array<Collision> collisions;

    for (int i = 0; i < entities.size(); i++)
    {
        for (int j = i + 1; j < entities.size(); j++)
        {
            if (i == j) continue;


            // Check if the entities' bounding radii overlap, if they don't collision isn't possilbe.
            float xDifference = entities[i]->locationPoint.x - entities[j]->locationPoint.x;
            float yDifference = entities[i]->locationPoint.y - entities[j]->locationPoint.y;
            float zDifference = entities[i]->locationPoint.z - entities[j]->locationPoint.z;
            float locationDifference = sqrt(xDifference * xDifference + yDifference * yDifference + zDifference * zDifference);
            float sumBoundingRadii = entities[i]->boundingRadius + entities[j]->boundingRadius;

            // There won't be any collisionpoints unless these requirements are met
            if (locationDifference < sumBoundingRadii && entities[i]->collidable && entities[j]->collidable)
            {
                // All the entity intersects between these two
                Array<Point> collisionPoints;

                Shape & shapeA = entities[i]->worldSpaceShape;
                Shape & shapeB = entities[j]->worldSpaceShape;

                // For the sake of speed, we'll only be comparing the smaller
                // entity's lines to the larger entity's faces.
                if (entities[i]->boundingRadius < entities[j]->boundingRadius)
                {
                    for (int k = 0; k < shapeA.lines.size(); k++)
                    {
                        for (int l = 0; l < shapeB.triangles.size(); l++)
                        {
                            Point intersectLocation;
                            if (fasterLineTriangleIntersect(shapeA.lines[k], shapeB.triangles[l], intersectLocation))
                            {
                                collisionPoints += intersectLocation;
                            }
                        }
                    }
                }
                else
                {
                    for (int k = 0; k < shapeB.lines.size(); k++)
                    {
                        for (int l = 0; l < shapeA.triangles.size(); l++)
                        {
                            Point intersectLocation;
                            if (fasterLineTriangleIntersect(shapeB.lines[k], shapeA.triangles[l], intersectLocation))
                            {
                                collisionPoints += intersectLocation;
                            }
                        }
                    }
                }

                // A collision has been found
                if (collisionPoints.size() > 0)
                {
                    // Get an average point from the collected collision poiints
                    float xAverage = 0;
                    float yAverage = 0;
                    float zAverage = 0;
                    for (int k = 0; k < collisionPoints.size(); k++)
                    {
                        xAverage += collisionPoints[k].x;
                        yAverage += collisionPoints[k].y;
                        zAverage += collisionPoints[k].z;
                    }
                    xAverage /= collisionPoints.size();
                    yAverage /= collisionPoints.size();
                    zAverage /= collisionPoints.size();

                    Collision newCollision;
                    newCollision.entityA = *(entities.getPointerTo(i));
                    newCollision.entityB = *(entities.getPointerTo(j));
                    newCollision.collisionLocation = Point(xAverage, yAverage, zAverage);

                    collisions += newCollision;
                }
            }
        }
    }

    // Resolve all the collisions
    for (int i = 0; i < collisions.size(); i++)
    {
        Entity * & entityA = collisions[i].entityA;
        Entity * & entityB = collisions[i].entityB;

        Point & centerOfMassA = entityA->locationPoint;
        Point & centerOfMassB = entityB->locationPoint;

        Vector & velocityA = entityA->velocity;
        Vector & velocityB = entityB->velocity;

        float & massA = entityA->mass;
        float & massB = entityB->mass;

        Point & collisionLocation = collisions[i].collisionLocation;

        Vector normalA = centerOfMassB - centerOfMassA;
        if (equalULP(normalA.magnitude(), 0))
        {
            // Objects are colliding, but their center of mass is in the same
            // location, can be caused by a faulty spawn, to fix, just have
            // them move away in random directions
            normalA.x = rand() - (RAND_MAX / 2);
            normalA.y = rand() - (RAND_MAX / 2);
            normalA.z = rand() - (RAND_MAX / 2);
            normalA.normalize();

            // Oh no, both objects aren't moveing or are moving, but in the same
            // direction!
            if (velocityA == velocityB)
            {
                velocityA += normalA * 0.01;
                velocityB -= normalA * 0.01;
            }
        }
        else
        {
            normalA.normalize();
        }
        Vector normalB = normalA * -1;

        Vector projectionA = velocityA.getProjection(normalA);
        float dotTestA = projectionA.dotProduct(normalA);
        if (dotTestA < 0 || equalULP(dotTestA, 0))
        {
            projectionA.x = 0;
            projectionA.y = 0;
            projectionA.z = 0;
        }
        Vector leftoverA = velocityA - projectionA;

        Vector projectionB = velocityB.getProjection(normalB);
        float dotTestB = projectionB.dotProduct(normalB);
        if (dotTestB < 0 || equalULP(dotTestB, 0))
        {
            projectionB.x = 0;
            projectionB.y = 0;
            projectionB.z = 0;
        }
        Vector leftoverB = velocityB - projectionB;

        Vector momentumA = projectionA * massA;
        Vector velocityReceivedB = momentumA / massB;

        Vector momentumB = projectionB * massB;
        Vector velocityReceivedA = momentumB / massA;

        velocityA = velocityReceivedA + leftoverA;
        velocityB = velocityReceivedB + leftoverB;

        // ---------------------------------------------------------------------
        // Ships loose health when hit by asteroids and saucers. First, make the
        // ship invincible and then put it in limbo. When it leaves limbo it's
        // health will decrement by 1 and it'll nolonger be invincible. The ship
        // can collide while invincible, but won't suffer any affects other then
        // physics. Ship invincibility is indicated by a negative health value.
        if (entityA->typeID == ENTITY_ID_SHIP && entityA->health >= 0 && (entityB->typeID == ENTITY_ID_ASTEROID || entityB->typeID == ENTITY_ID_SAUCER))
        {
            // explosion
            Entity * flower = createFlower(normalA, 16);
            flower->locationPoint = collisionLocation;
            s_playFlowers += flower;
            addToLimbo(flower, 20);
            
            // Ship gets sent to limbo.
            entityA->drawProperties = DRAW_LINES | DRAW_DISTANCE_SHADING_OFF;
            entityA->health *= -1;
            addToLimbo(entityA, 60);
        }
        if ((entityA->typeID == ENTITY_ID_ASTEROID || entityA->typeID == ENTITY_ID_SAUCER) && entityB->typeID == ENTITY_ID_SHIP && entityB->health >= 0)
        {
            // explosion
            Entity * flower = createFlower(normalA, 16);
            flower->locationPoint = collisionLocation;
            s_playFlowers += flower;
            addToLimbo(flower, 20);
            
            // Ship gets sent to limbo.
            entityB->drawProperties = DRAW_LINES | DRAW_DISTANCE_SHADING_OFF;
            entityB->health *= -1;
            addToLimbo(entityB, 60);
        }

        // -----------------------------------------------------------------
        // Ships loose health when hit by saucer bullets, and the saucer
        // bullets are destroyed
        if (entityA->typeID == ENTITY_ID_SHIP && entityA->health >= 0 && entityB->typeID == ENTITY_ID_SAUCER_BULLET)
        {
            // explosion
            Entity * flower = createFlower(normalA, 16);
            flower->locationPoint = collisionLocation;
            s_playFlowers += flower;
            addToLimbo(flower, 20);

            // bullet destroyed
            entityB->collidable = false;
            entityB->drawProperties = DRAW_LINES;
            addToLimbo(entityB, 10);

            // Ship gets sent to limbo.
            entityA->drawProperties = DRAW_LINES | DRAW_DISTANCE_SHADING_OFF;
            entityA->health *= -1;
            addToLimbo(entityA, 60);
        }
        if (entityA->typeID == ENTITY_ID_SAUCER_BULLET && entityB->typeID == ENTITY_ID_SHIP && entityB->health >= 0)
        {
            // explosion
            Entity * flower = createFlower(normalA, 16);
            flower->locationPoint = collisionLocation;
            s_playFlowers += flower;
            addToLimbo(flower, 20);

            // bullet destroyed
            entityA->collidable = false;
            entityA->drawProperties = DRAW_LINES;
            addToLimbo(entityA, 10);

            // Ship gets sent to limbo.
            entityB->drawProperties = DRAW_LINES | DRAW_DISTANCE_SHADING_OFF;
            entityB->health *= -1;
            addToLimbo(entityB, 60);
        }

        // ---------------------------------------------------------------------
        // Ship bullets explode on contact with an asteroid, the asteroid splits
        // in half if it's big enough, otherwise, it is completely destroyed.
        if (entityA->typeID == ENTITY_ID_SHIP_BULLET && entityB->typeID == ENTITY_ID_ASTEROID)
        {
            // explosion
            Entity * flower = createFlower(normalA, 16);
            flower->locationPoint = collisionLocation;
            s_playFlowers += flower;
            addToLimbo(flower, 20);

            // bullet destroyed
            entityA->collidable = false;
            entityA->drawProperties = DRAW_LINES;
            addToLimbo(entityA, 10);

            // asteroid destoryed
            entityB->collidable = false;
            entityB->drawProperties = DRAW_LINES;
            addToLimbo(entityB, 20);

            if (entityB->health == 2) s_score += 10;
            else if (entityB->health == 1) s_score += 20;
            else if (entityB->health == 0) s_score += 30;

            // asteroid splits
            if (entityB->health > 0)
            {
                Entity * asteroid0 = createAsteroid(entityB->health - 1, s_asteroidColor);
                Entity * asteroid1 = createAsteroid(entityB->health - 1, s_asteroidColor);

                asteroid0->locationPoint = entityB->locationPoint;
                asteroid1->locationPoint = entityB->locationPoint;

                float OldMagnitude = entityB->velocity.magnitude() * entityB->mass;

                asteroid0->velocity = randomOrthogonalVector(entityB->velocity) * OldMagnitude / asteroid0->mass;
                asteroid1->velocity = asteroid0->velocity * -1;

                s_playAsteroids += asteroid0;
                s_playAsteroids += asteroid1;
            }
        }
        if (entityA->typeID == ENTITY_ID_ASTEROID && entityB->typeID == ENTITY_ID_SHIP_BULLET)
        {
            // explosion
            Entity * flower = createFlower(normalA, 16);
            flower->locationPoint = collisionLocation;
            s_playFlowers += flower;
            addToLimbo(flower, 20);

            // bullet destroyed
            entityB->collidable = false;
            entityB->drawProperties = DRAW_LINES;
            addToLimbo(entityB, 10);

            // asteroid destroyed
            entityA->collidable = false;
            entityA->drawProperties = DRAW_LINES;
            addToLimbo(entityA, 20);

            if (entityA->health == 2) s_score += 10;
            else if (entityA->health == 1) s_score += 20;
            else if (entityA->health == 0) s_score += 30;

            // asteroid splits
            if (entityA->health > 0)
            {
                Entity * asteroid0 = createAsteroid(entityA->health - 1, s_asteroidColor);
                Entity * asteroid1 = createAsteroid(entityA->health - 1, s_asteroidColor);

                asteroid0->locationPoint = entityA->locationPoint;
                asteroid1->locationPoint = entityA->locationPoint;

                float OldMagnitude = entityA->velocity.magnitude() * entityA->mass;

                asteroid0->velocity = randomOrthogonalVector(entityA->velocity) * OldMagnitude / asteroid0->mass;
                asteroid1->velocity = asteroid0->velocity * -1;

                s_playAsteroids += asteroid0;
                s_playAsteroids += asteroid1;
            }
        }

        // ---------------------------------------------------------------------
        // Ship bullets explode on contact with saucers and saucer bullets; the
        // saucer and saucer bullets are also destroyed.
        if (entityA->typeID == ENTITY_ID_SHIP_BULLET && (entityB->typeID == ENTITY_ID_SAUCER || entityB->typeID == ENTITY_ID_SAUCER_BULLET))
        {
            // explosion
            Entity * flower = createFlower(normalA, 16);
            flower->locationPoint = collisionLocation;
            s_playFlowers += flower;
            addToLimbo(flower, 20);

            // bullet destroyed
            entityA->collidable = false;
            entityA->drawProperties = DRAW_LINES;
            addToLimbo(entityA, 10);

            // saucer or saucer bullet destoryed
            entityB->collidable = false;
            entityB->drawProperties = DRAW_LINES;
            addToLimbo(entityB, 20);

            if (entityB->typeID == ENTITY_ID_SAUCER) s_score += 50;
        }
        if ((entityA->typeID == ENTITY_ID_SAUCER || entityA->typeID == ENTITY_ID_SAUCER_BULLET) && entityB->typeID == ENTITY_ID_SHIP_BULLET)
        {
            // explosion
            Entity * flower = createFlower(normalA, 16);
            flower->locationPoint = collisionLocation;
            s_playFlowers += flower;
            addToLimbo(flower, 20);

            // bullet destroyed
            entityB->collidable = false;
            entityB->drawProperties = DRAW_LINES;
            addToLimbo(entityB, 10);

            // saucer or saucer bullet destroyed
            entityA->collidable = false;
            entityA->drawProperties = DRAW_LINES;
            addToLimbo(entityA, 20);

            if (entityA->typeID == ENTITY_ID_SAUCER) s_score += 50;
        }

        // ---------------------------------------------------------------------
        // Saucer bullets bounce off asteroids and saucers!!!
        /// // ---------------------------------------------------------------------
        /// // Saucer bullets explode on contact with an asteroid or saucer, but do
        /// // not destroy it
        /// if (entityA->typeID == ENTITY_ID_SAUCER_BULLET && (entityB->typeID == ENTITY_ID_ASTEROID || entityB->typeID == ENTITY_ID_SAUCER))
        /// {
        ///     // explosion
        ///     Entity * flower = createFlower(normalA, 16);
        ///     flower->locationPoint = collisionLocation;
        ///     s_playFlowers += flower;
        ///     s_limboEntities += flower;
        ///     s_limboCounters += 20;
        /// 
        ///     // bullet destroyed
        ///     entityA->collidable = false;
        ///     entityA->drawProperties = DRAW_LINES;
        ///     s_limboEntities += entityA;
        ///     s_limboCounters += 10;
        /// }
        /// if ((entityA->typeID == ENTITY_ID_ASTEROID || entityA->typeID == ENTITY_ID_SAUCER) && entityB->typeID == ENTITY_ID_SAUCER_BULLET)
        /// {
        ///     // explosion
        ///     Entity * flower = createFlower(normalA, 16);
        ///     flower->locationPoint = collisionLocation;
        ///     s_playFlowers += flower;
        ///     s_limboEntities += flower;
        ///     s_limboCounters += 20;
        /// 
        ///     // bullet destroyed
        ///     entityB->collidable = false;
        ///     entityB->drawProperties = DRAW_LINES;
        ///     s_limboEntities += entityB;
        ///     s_limboCounters += 10;
        /// }
    }
}


void calculateBorderCollisions(Array<Entity*> & entities)
{
    for (int i = 0; i < entities.size(); i++)
    {
        if (entities[i]->collidable)
        {
            // Check if the entity's bounding radii overlap, if they don't
            // collision isn't possilbe.
            float xDiff = s_playBorder->locationPoint.x - entities[i]->locationPoint.x;
            float yDiff = s_playBorder->locationPoint.y - entities[i]->locationPoint.y;
            float zDiff = s_playBorder->locationPoint.z - entities[i]->locationPoint.z;
            float locationDifference = sqrt(xDiff * xDiff + yDiff * yDiff + zDiff * zDiff);
            // In the special case of a border entity, its mass represents its inner
            // bounding radius and the its boudingRadius represents its outter 
            // bounding radius
            float averageBoundingRadius = (s_playBorder->mass + s_playBorder->boundingRadius) / 2;
            float minCollidingRadius = averageBoundingRadius - entities[i]->boundingRadius;
            
            if (locationDifference > minCollidingRadius)
            {
                // Ok, looks like collision could be possibe, but before checking
                // all the points, make sure the entity is moving toward the border.
                // If it's not, it might have already collided.
                const float & V_x = entities[i]->velocity.x;
                const float & V_y = entities[i]->velocity.y;
                const float & V_z = entities[i]->velocity.z;
                float nMag = sqrt(xDiff * xDiff + yDiff * yDiff + zDiff * zDiff);
                // Normal Vector from entity location to the center of the border
                // shape
                float N_x = xDiff / nMag;
                float N_y = yDiff / nMag;
                float N_z = zDiff / nMag;
                float NdotV = N_x * V_x + N_y * V_y + N_z * V_z;
                if (NdotV < 0 || equalULP(NdotV, 0))
                {
                    // If an asteroid is destroid before entering the
                    // border, it's peices end up circling just outside
                    // the border. To avoid this, give it additional
                    // velocity towards the center of the border. When
                    // entities are outside of it and far away.
                    if (NdotV < 0.5)
                    {
                        entities[i]->velocity.rotateTowards(Vector(N_x, N_y, N_z), 0.5);
                    }
                    
                    // Look's like the entity is moving into the border.
                    // If any of the entity's points are beyond the collision radius
                    // it's colliding!
                    const Array<Point> & points = entities[i]->frame.points;
                    for (int j = 0; j < points.size(); j++)
                    {
                        float pXDiff = xDiff - points[j].x;
                        float pYDiff = yDiff - points[j].y;
                        float pZDiff = zDiff - points[j].z;
                        float pointLocationDifference = sqrt(pXDiff * pXDiff + pYDiff * pYDiff + pZDiff * pZDiff);
                        if (pointLocationDifference > minCollidingRadius)
                        {
                            // Bullets explode on contact with the border
                            if (entities[i]->typeID == ENTITY_ID_SHIP_BULLET || entities[i]->typeID == ENTITY_ID_SAUCER_BULLET)
                            {
                                Entity * flower = createFlower(Vector(N_x, N_y, N_z), 16);
                                flower->locationPoint = entities[i]->locationPoint;
                                s_playFlowers += flower;
                                s_limboEntities += flower;
                                s_limboCounters += 20;
                                entities[i]->collidable = false;
                                entities[i]->drawProperties = DRAW_LINES;
                                s_limboEntities += entities[i];
                                s_limboCounters += 10;
                            }
                            // Point has passed the collidingradius, apply the
                            // collision as if the center of the entity was the
                            // collision point for simplicity and call it a day.
                            entities[i]->velocity.x = V_x - N_x * 2 * NdotV;
                            entities[i]->velocity.y = V_y - N_y * 2 * NdotV;
                            entities[i]->velocity.z = V_z - N_z * 2 * NdotV;
                            break;
                        }
                    }
                }
            }
        }
    }
}


void setSpawnLocation(Point & location)
{
    Vector locationVector(rand() - (RAND_MAX / 2), rand() - (RAND_MAX / 2), rand() - (RAND_MAX / 2));
    locationVector.normalize();
    locationVector *= PLAY_BORDER_RADIUS * 1.5;
    location += locationVector;
}


void spawnAsteroid(unsigned size)
{
    Entity * asteroid = createAsteroid(size, s_asteroidColor);
    setSpawnLocation(asteroid->locationPoint);
    asteroid->velocity.x = -asteroid->locationPoint.x;
    asteroid->velocity.y = -asteroid->locationPoint.y;
    asteroid->velocity.z = -asteroid->locationPoint.z;
    asteroid->velocity.normalize();
    asteroid->velocity /= 2 * size;
    s_playAsteroids += asteroid;
}


void spawnSaucer()
{
    Entity * saucer = createSaucer(s_saucerColor0, s_saucerColor1);
    setSpawnLocation(saucer->locationPoint);
    saucer->velocity.x = -saucer->locationPoint.x;
    saucer->velocity.y = -saucer->locationPoint.y;
    saucer->velocity.z = -saucer->locationPoint.z;
    saucer->velocity.normalize();
    saucer->velocity /= 2;
    saucer->health = SAUCER_FIRE_RATE;
    s_playSaucers += saucer;
}


void triggerEntitySpawner()
{
    // Saucers shoot bullets
    // -------------------------------------------------------------------------
    for (int i = 0; i < s_playSaucers.size(); i++)
    {
        if (s_playSaucers[i]->health > 0)
        {
            s_playSaucers[i]->health--;
        }
        else
        {
            Entity * bullet = spawnSaucerBullet(s_playSaucers[i], s_playShip->locationPoint, s_saucerBulletColor, SAUCER_BULLET_SPEED_LIMIT);
            s_playBullets += bullet;
            s_playSaucers[i]->health = SAUCER_FIRE_RATE;
        }
    }
    
    if (s_gameCounter > 0)
    {
        // Spawn an asteroids every 10 seconds
        if (s_gameCounter % 200 == 0) // 10 seconds
        {
            // Spawn one asteroid every spawn, then after 2 minute spawn two, and
            // after 4 spawn 3, and so on...
            for (int i = 0; i < (s_gameCounter / 2400) + 1; i++)
            {
                spawnAsteroid(2);
            }
        }

        // Spawn a saucers every 30 seconds
        if (s_gameCounter % 600 == 0) // 30 seconds
        {
            for (int i = 0; i < (s_gameCounter / 2400) + 1; i++)
            {
                spawnSaucer();
            }
        }
    }
    else
    {
        // Initial asteroid
        spawnAsteroid(2);
    }

    if (s_score > s_scoreTillNectHeart)
    {
        if (s_playShip->health >= 0)
        {
            s_playShip->health++;
        }
        else
        {
            s_playShip->health--;
        }
        s_scoreTillNectHeart += 1000;
    }

    s_gameCounter++;
}