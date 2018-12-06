/*
 * Game.c
 *
 *  Created on: Dec 1, 2018
 *      Author: Isaac
 */
#include "Game.h"

/*********************************************** Common Threads *********************************************************************/
/*
 * Idle thread
 */
void IdleThread()
{
    while(1);
}

/*
 * Thread to draw all the objects in the game
 */
void DrawObjects(){
    //initialize all previous balls to center of screen
    PrevBall_t prevBall[MAX_NUM_OF_BALLS];
    for(int i=0; i < MAX_NUM_OF_BALLS; ++i){
        prevBall[i].CenterX = MAX_SCREEN_X >> 1;
        prevBall[i].CenterY = MAX_SCREEN_Y >> 1;
    }
    PrevPlayer_t prevPlayer[MAX_NUM_OF_PLAYERS];
    prevPlayer[Host].Center = PADDLE_X_CENTER;
    prevPlayer[Client].Center = PADDLE_X_CENTER;

    LCD_DrawRectangle(160 - 32, 160 + 32, 0, 4, LCD_BLUE);
    LCD_DrawRectangle(160 - 32, 160 + 32, 236, 240, LCD_ORANGE);

    while(1){
        // wait LCD semaphore
        G8RTOS_WaitSemaphore(&lcdI2CReady);
        // Erase Ball
        for(int i=0; i < MAX_NUM_OF_BALLS; ++i){
            if(gameState.balls[i].alive){
                LCD_DrawRectangle(prevBall[i].CenterX - BALL_SIZE_D2, prevBall[i].CenterX + BALL_SIZE_D2, prevBall[i].CenterY - BALL_SIZE_D2, prevBall[i].CenterY + BALL_SIZE_D2, BACK_COLOR);
            }
        }
        // Draw Ball
        for(int i=0; i < MAX_NUM_OF_BALLS; ++i){
            if(gameState.balls[i].alive){
                LCD_DrawRectangle(gameState.balls[i].currentCenterX - BALL_SIZE_D2, gameState.balls[i].currentCenterX + BALL_SIZE_D2, gameState.balls[i].currentCenterY - BALL_SIZE_D2, gameState.balls[i].currentCenterY + BALL_SIZE_D2, gameState.balls[i].color);
                prevBall[i].CenterX = gameState.balls[i].currentCenterX;
                prevBall[i].CenterY = gameState.balls[i].currentCenterY;
            }
        }
        // Player was updated in JoystickThread, draw players
        UpdatePlayerOnScreen(&prevPlayer[Host], &gameState.players[Host]);
        prevPlayer[Host].Center = gameState.players[Host].currentCenter;
        UpdatePlayerOnScreen(&prevPlayer[Client], &gameState.players[Client]);
        prevPlayer[Client].Center = gameState.players[Client].currentCenter;
        //signal LCD semaphore
        G8RTOS_SignalSemaphore(&lcdI2CReady);
        sleep(20);
    }
}

/*
 * Thread to update LEDs based on score
 */
void MoveLEDs()
{
    //wait LED semaphore
    G8RTOS_WaitSemaphore(&ledI2CReady);
    //Host LEDS
    switch(gameState.LEDScores[Host]){
        case 0:
            LP3943_LedModeSet(BLUE, 0x0000);
            break;
        case 1:
            LP3943_LedModeSet(BLUE, 0x0100);
            break;
        case 2:
            LP3943_LedModeSet(BLUE, 0x0300);
            break;
        case 3:
            LP3943_LedModeSet(BLUE, 0x0700);
            break;
        case 4:
            LP3943_LedModeSet(BLUE, 0x0F00);
            break;
        case 5:
            LP3943_LedModeSet(BLUE, 0x1F00);
            break;
        case 6:
            LP3943_LedModeSet(BLUE, 0x3F00);
            break;
        case 7:
            LP3943_LedModeSet(BLUE, 0x7F00);
            break;
        case 8:
            LP3943_LedModeSet(BLUE, 0xFF00);
            break;
    }
    //Client LEDS
    switch(gameState.LEDScores[Client]){
        case 0:
            LP3943_LedModeSet(RED, 0x0000);
            break;
        case 1:
            LP3943_LedModeSet(RED, 0x0080);
            break;
        case 2:
            LP3943_LedModeSet(RED, 0x00C0);
            break;
        case 3:
            LP3943_LedModeSet(RED, 0x00E0);
            break;
        case 4:
            LP3943_LedModeSet(RED, 0x00F0);
            break;
        case 5:
            LP3943_LedModeSet(RED, 0x00F8);
            break;
        case 6:
            LP3943_LedModeSet(RED, 0x00FC);
            break;
        case 7:
            LP3943_LedModeSet(RED, 0x00FE);
            break;
        case 8:
            LP3943_LedModeSet(RED, 0x00FF);
            break;
    }
    G8RTOS_SignalSemaphore(&ledI2CReady);
}

/*********************************************** Common Threads *********************************************************************/

/*********************************************** Public Functions *********************************************************************/
/*
 * Returns either Host or Client depending on button press
 */
playerType GetPlayerRole(){

    char startScreen[] = "PONG";
    char startScreen0[] = "Press Top Button for Host";
    char startScreen1[] = "Press Bottom Button for Client";
    LCD_Text(50, 100, startScreen0, LCD_WHITE);
    LCD_Text(45, 120, startScreen1, LCD_WHITE);
    LCD_Text(100, 20, startScreen, LCD_WHITE);
    //init B0 top
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN4);
    //init B2 bottom
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN4);
    uint8_t B0;
    uint8_t B2;
    while(1){
        B0 = GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN4);
        B2 = GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN4);
        if(!B0){
            //This Player is Host
            return Host;
        }
        else if(!B2){
            //This Player is Client
            return Client;
        } else {
            //keep looping
        }
    }

}


/*
 * Draw players given center X center coordinate
 */
void DrawPlayer(void){
    LCD_DrawRectangle(PADDLE_X_CENTER - 32, PADDLE_X_CENTER + 32, ARENA_MIN_Y, TOP_PADDLE_EDGE, LCD_BLUE);
    LCD_DrawRectangle(PADDLE_X_CENTER - 32, PADDLE_X_CENTER + 32, BOTTOM_PADDLE_EDGE, ARENA_MAX_Y, LCD_ORANGE);
}

/*
 * Updates player's paddle based on current and new center
 */
void UpdatePlayerOnScreen(PrevPlayer_t * prevPlayerIn, GeneralPlayerInfo_t * outPlayer){
    int16_t deltaX;
    int16_t yStart;
    int16_t yEnd;
    if(outPlayer->position)
    {
        yStart = ARENA_MIN_Y;
        yEnd = ARENA_MIN_Y + PADDLE_WID;
    }
    else{
        yStart = ARENA_MAX_Y - PADDLE_WID;
        yEnd = ARENA_MAX_Y;
    }
    deltaX = outPlayer->currentCenter - prevPlayerIn->Center;
    if(deltaX > 0){
        //moving to right
        LCD_DrawRectangle(prevPlayerIn->Center- PADDLE_LEN_D2, outPlayer->currentCenter - PADDLE_LEN_D2,yStart,yEnd,BACK_COLOR);
        LCD_DrawRectangle(prevPlayerIn->Center + PADDLE_LEN_D2, outPlayer->currentCenter + PADDLE_LEN_D2,yStart,yEnd,outPlayer->color);
    }
    else if(deltaX < 0){
        //moving to left
        LCD_DrawRectangle(outPlayer->currentCenter + PADDLE_LEN_D2, prevPlayerIn->Center + PADDLE_LEN_D2,yStart,yEnd,BACK_COLOR);
        LCD_DrawRectangle(outPlayer->currentCenter - PADDLE_LEN_D2, prevPlayerIn->Center - PADDLE_LEN_D2,yStart,yEnd,outPlayer->color);
    }
}
/*
 * Function updates ball position on screen
 */
void UpdateBallOnScreen(PrevBall_t * previousBall, Ball_t * currentBall, uint16_t outColor);

/*
 * Initializes and prints initial game state
 */
void InitBoardState()
{
    G8RTOS_WaitSemaphore(&lcdI2CReady);
    LCD_Clear(BACK_COLOR);
    LCD_DrawRectangle(ARENA_MIN_X-3, ARENA_MIN_X, ARENA_MIN_Y, ARENA_MAX_Y, LCD_WHITE);
    LCD_DrawRectangle(ARENA_MAX_X, ARENA_MAX_X+3, ARENA_MIN_Y, ARENA_MAX_Y, LCD_WHITE);
    DrawPlayer(); //Draws both the players, cause fuck it
    //print host score
    char score[2];
    if(gameState.overallScores[Host] < 10){
        sprintf(score, "0%d", gameState.overallScores[Host]);
    } else {
        sprintf(score, "%d", gameState.overallScores[Host]);
    }
    LCD_Text(0, 10, score, gameState.players[Host].color);
    //print client score
    if(gameState.overallScores[Client] < 10){
        sprintf(score, "0%d", gameState.overallScores[Client]);
    } else {
        sprintf(score, "%d", gameState.overallScores[Client]);
    }
    LCD_Text(0, ARENA_MAX_Y-25, score, gameState.players[Client].color);
    gameState.players[Host].currentCenter = MAX_SCREEN_X >> 1;
    gameState.players[Client].currentCenter = MAX_SCREEN_X >> 1;
    G8RTOS_SignalSemaphore(&lcdI2CReady);
}

/*********************************************** Public Functions *********************************************************************/



/*********************************************** Client Threads *********************************************************************/
/*
 * Thread for client to join game
 */
void JoinGame(){

    G8RTOS_InitSemaphore(&lcdI2CReady,1);
    G8RTOS_InitSemaphore(&ledI2CReady,1);
    G8RTOS_InitSemaphore(&wifiReady,1);
    // Connect to network
    initCC3100(Client);

    // Set initial SpecificPlayerInfo_t
    clientPlayer.IP_address = getLocalIP();
    clientPlayer.displacement = 0;
    clientPlayer.playerNumber = Client;
    clientPlayer.ready = false;
    clientPlayer.joined = true;
    clientPlayer.acknowledge = false;

    gameState.player.acknowledge = false;
    ///*
    // loop send and receive until info is received
    while(!gameState.player.acknowledge){
        SendDataToHost(); // send player info to host
        ReceiveDataFromHost(); // wait for response from host
    }
    //*/

    // Light Green LED
    P2->SEL0 &= ~0x03;
    P2->SEL1 &= ~0x03;
    P2->DIR |= 0x03;
    P2->OUT &= 0x03;
    P1->SEL0 &= ~0x01;
    P1->SEL1 &= ~0x01;
    P1->DIR |= 0x01;
    P1->OUT &= 0x01;

    // Init board state, semaphores, and add threads
    //init board
    InitBoardState();

    G8RTOS_AddThread(IdleThread, 255, "Idle");
    G8RTOS_AddThread(SendDataToHostThread, 3, "TxHost");
    G8RTOS_AddThread(DrawObjects, 3, "DrawObj");
    G8RTOS_AddThread(ReadJoystickClient, 3, "RdJyCnt");
    G8RTOS_AddThread(ReceiveDataFromHostThread, 3, "RxHost");

    // Kill self

}
/*
 * Thread that receives game state packets from host
 */
void ReceiveDataFromHost()
{
        ReceiveData(&gameState, GAME_SIZE);
}
void ReceiveDataFromHostThread(){
    while(1){
        G8RTOS_WaitSemaphore(&wifiReady);
        if(ReceiveData(&gameState, GAME_SIZE) < 0)
        {
            G8RTOS_SignalSemaphore(&wifiReady);
            sleep(1);
        }
        else{
            G8RTOS_SignalSemaphore(&wifiReady);
            if(gameState.gameDone)
            {
                G8RTOS_AddThread(EndOfGameClient, 1, "EndClient");
            }
            sleep(2);
        }
    }
}


/*
 * Thread that sends UDP packets to host
 */
void SendDataToHost()
{
    SendData(&clientPlayer, HOST_IP_ADDR, 10);
}
void SendDataToHostThread(){
    while(1){
        G8RTOS_WaitSemaphore(&wifiReady);
        SendData(&clientPlayer,HOST_IP_ADDR, 10);
        G8RTOS_SignalSemaphore(&wifiReady);
        clientPlayer.displacement = 0;
        sleep(2);
    }
}

/*
 * Thread to read client's joystick
 */
void ReadJoystickClient(){
    int16_t x_joystick;
    int16_t y_joystick;
    while(1){
        GetJoystickCoordinates(&x_joystick, &y_joystick);
        clientPlayer.displacement = (x_joystick >> JOYSTICK_SHIFT);
        MoveLEDs();
        sleep(10);
    }
}

/*
 * End of game for the client
 */
void EndOfGameClient()
{
    G8RTOS_WaitSemaphore(&lcdI2CReady);
    G8RTOS_WaitSemaphore(&wifiReady);
    G8RTOS_WaitSemaphore(&ledI2CReady);
    G8RTOS_Genocide();
    G8RTOS_InitSemaphore(&lcdI2CReady, 1);
    G8RTOS_InitSemaphore(&wifiReady, 1);
    G8RTOS_InitSemaphore(&ledI2CReady, 1);
    if(gameState.winner)
    {
        LCD_Clear(LCD_BLUE);
    }
    else
    {
        LCD_Clear(LCD_ORANGE);
    }
    while(!gameState.player.ready){
        ReceiveDataFromHost(); // wait for response from host
    }
    //init board
    InitBoardState();
    //add threads
    G8RTOS_AddThread(IdleThread, 255, "Idle");
    G8RTOS_AddThread(SendDataToHostThread, 3, "TxHost");
    G8RTOS_AddThread(DrawObjects, 3, "DrawObj");
    G8RTOS_AddThread(ReadJoystickClient, 3, "RdJyCnt");
    G8RTOS_AddThread(ReceiveDataFromHostThread, 3, "RxHost");

    G8RTOS_KillSelf();
}

/*********************************************** Client Threads *********************************************************************/


/*********************************************** Host Threads *********************************************************************/
/*
 * Thread for the host to create a game
 */
void CreateGame()
{
    __enable_interrupts();
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN5);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN5, GPIO_HIGH_TO_LOW_TRANSITION);
    G8RTOS_AddAPeriodicEvent(StartNewGame, 6,PORT4_IRQn);
    G8RTOS_InitSemaphore(&lcdI2CReady,1);
    G8RTOS_InitSemaphore(&ledI2CReady,1);
    G8RTOS_InitSemaphore(&wifiReady,1);
    gameState.players[0].color = LCD_ORANGE;
    gameState.players[0].currentCenter = PADDLE_X_CENTER;
    gameState.players[0].position = Client;
    gameState.players[1].color = LCD_BLUE;
    gameState.players[1].currentCenter = PADDLE_X_CENTER;
    gameState.players[1].position = Host;
    for(int i= 0; i < MAX_NUM_OF_BALLS;i++)
    {
        gameState.balls[i].alive = false;
    }
    gameState.player.IP_address = HOST_IP_ADDR;
    gameState.player.acknowledge = false;
    gameState.player.displacement = 0;
    gameState.player.joined = false;
    gameState.player.playerNumber = Host;
    gameState.player.ready = false;
    gameState.gameDone = false;
    gameState.numberOfBalls = 0;
    gameState.LEDScores[0] = 0;
    gameState.LEDScores[1] = 0;
    gameState.overallScores[0] = 0;
    gameState.overallScores[1] = 0;
    gameState.winner = false;
    initCC3100(Host);
    while(!clientPlayer.joined)
    {
        ReceiveDataFromClient();
    }

    gameState.player.acknowledge = true;
    for(int i = 0;i < 5; i++)
    {
    SendDataToClient();
    }

    P2->SEL0 &= ~0x03;
    P2->SEL1 &= ~0x03;
    P2->DIR |= 0x03;
    P2->OUT &= 0x03;
    P1->SEL0 &= ~0x01;
    P1->SEL1 &= ~0x01;
    P1->DIR |= 0x01;
    P1->OUT &= 0x01;

    //init board
    InitBoardState();
    //add threads
    G8RTOS_AddThread(SendDataToClientThread, 3, "TxClient");
    G8RTOS_AddThread(GenerateBall, 3, "GenBall");
    G8RTOS_AddThread(IdleThread, 255, "Idle");
    G8RTOS_AddThread(DrawObjects, 3, "DrawObj");
    G8RTOS_AddThread(ReadJoystickHost, 3, "RdJyHst");
    G8RTOS_AddThread(ReceiveDataFromClientThread, 3, "RxClient");
}


/*
 * Thread that sends game state to client
 */
void SendDataToClient(){
    SendData(&gameState,clientPlayer.IP_address, GAME_SIZE);
}
void SendDataToClientThread(){
    while(1){
        if((gameState.LEDScores[Client] > 7) || (gameState.LEDScores[Host] > 7))
        {
            gameState.gameDone = true;
            gameState.player.ready = false;
            if(gameState.LEDScores[Client] > 7){
                gameState.winner = Client;
            }
            else{
                gameState.winner = Host;
            }
            G8RTOS_AddThread(EndOfGameHost, 1, "EndHost");
        }
        G8RTOS_WaitSemaphore(&wifiReady);
        SendData(&gameState,clientPlayer.IP_address, GAME_SIZE);
        G8RTOS_SignalSemaphore(&wifiReady);

        sleep(2);
    }
}

/*
 * Thread that receives UDP packets from client
 */
void ReceiveDataFromClient()
{
    ReceiveData(&clientPlayer, 10);
}
void ReceiveDataFromClientThread(){
    while(1){
        G8RTOS_WaitSemaphore(&wifiReady);
        if(ReceiveData(&clientPlayer, 10) < 0)
        {
            G8RTOS_SignalSemaphore(&wifiReady);
            sleep(1);
        }
        else{
            G8RTOS_SignalSemaphore(&wifiReady);
            gameState.players[Client].currentCenter -= clientPlayer.displacement;
            if(gameState.players[Client].currentCenter < (ARENA_MIN_X + PADDLE_LEN_D2)){
                // paddle too far to the left
                gameState.players[Client].currentCenter = ARENA_MIN_X + PADDLE_LEN_D2;
            }
            else if(gameState.players[Client].currentCenter > (ARENA_MAX_X - PADDLE_LEN_D2)){
                // paddle too far to the right
                gameState.players[Client].currentCenter = ARENA_MAX_X - PADDLE_LEN_D2;
            }
            sleep(2);
        }
    }
}


/*
 * Generate Ball thread
 */
void GenerateBall(){
    uint16_t sleepTime = 2000;
    char ballNum[2] = "0";
    char ballString[6];
    while(1){
    if(gameState.numberOfBalls < MAX_NUM_OF_BALLS)
        {
            sprintf(ballString,"%s","Ball");
            sprintf(ballNum,"%d",gameState.numberOfBalls);
            strcat(ballString,ballNum);
            G8RTOS_AddThread(MoveBall, 5, ballString);
            gameState.numberOfBalls++;
            if(sleepTime > 200){
                sleepTime -= 100;
            }

        }
    sleep(sleepTime);
    }
}

/*
 * Thread to read host's joystick
 */
void ReadJoystickHost(){
    int16_t x_joystick;
    int16_t y_joystick;
    while(1){
        GetJoystickCoordinates(&x_joystick, &y_joystick);
        gameState.players[Host].currentCenter -= (x_joystick >> JOYSTICK_SHIFT);
        if(gameState.players[Host].currentCenter < (ARENA_MIN_X + PADDLE_LEN_D2)){
            // paddle too far to the left
            gameState.players[Host].currentCenter = ARENA_MIN_X + PADDLE_LEN_D2;
        }
        else if(gameState.players[Host].currentCenter > (ARENA_MAX_X - PADDLE_LEN_D2)){
            // paddle too far to the right
            gameState.players[Host].currentCenter = ARENA_MAX_X - PADDLE_LEN_D2;
        }
        sleep(10);
    }
}

void PaddleHit(playerPosition position, uint8_t ball,int8_t* xv)
{
    int dx;
    int xl,xr,xm;
    xl = gameState.players[position].currentCenter - PADDLE_LEN_D2 >> 1;
    xr = gameState.players[position].currentCenter + PADDLE_LEN_D2 >> 1;
    xl -= gameState.balls[ball].currentCenterX;
    xr -= gameState.balls[ball].currentCenterX;
    xm = gameState.players[position].currentCenter - gameState.balls[ball].currentCenterX;
    xl = abs(xl);
    xr = abs(xr);
    xm = abs(xm);
    if((xl < xm) && (xl < xr))
    {
        *xv +=  2;
    }
    else if(xr < xm)
    {
        *xv -= 2;
    }
    else
    {
        *xv >>= 1;
    }
}

/*
 * Thread to move a single ball
 */
void MoveBall()
{
    uint8_t xCoord;
    int8_t xV;
    int8_t yV;
    uint8_t threadBall;
    for(int i= 0; i < MAX_NUM_OF_BALLS;i++)
    {
        if(gameState.balls[i].alive == false)
        {
            gameState.balls[i].alive = true;
            gameState.balls[i].color = rand();
            gameState.balls[i].currentCenterY = MAX_SCREEN_Y >> 1;
            xCoord = rand();
            if(xCoord >= ARENA_MAX_X - BALL_SIZE_D2)
            {
                xCoord = ARENA_MAX_X - BALL_SIZE_D2;
            }
            else if(xCoord <= ARENA_MIN_X + BALL_SIZE_D2)
            {
                xCoord = ARENA_MIN_X + BALL_SIZE_D2;
            }
            gameState.balls[i].currentCenterX = xCoord;
            xV = rand();
            xV >>= 5;
            yV = rand();
            yV >>= 5;
            if(yV == 0){
                yV = 1;
            }
            threadBall = i;
            CurrentlyRunningThread->isBall = true;
            CurrentlyRunningThread->ballNum = threadBall;
            sleep(80);
            break;
        }

    }
    int w = PADDLE_LEN_D2 + BALL_SIZE_D2;
    int dx;
    while(1){
        gameState.balls[threadBall].currentCenterX += xV;
        if(gameState.balls[threadBall].currentCenterX <= ARENA_MIN_X + BALL_SIZE_D2)
        {
            //hit left wall
            xV = -xV;
            gameState.balls[threadBall].currentCenterX += xV + BALL_SIZE_D2;
        }
        else if(gameState.balls[threadBall].currentCenterX >= ARENA_MAX_X - BALL_SIZE_D2)
        {
            //hit right wall
            xV = -xV;
            gameState.balls[threadBall].currentCenterX += xV - BALL_SIZE_D2;
        }
        gameState.balls[threadBall].currentCenterY += yV;
        if(gameState.balls[threadBall].currentCenterY >= BOTTOM_PADDLE_EDGE - BALL_SIZE_D2)
        {
            //hit bottom threshold
            //check left side of paddle and right side of paddle
            dx = gameState.balls[threadBall].currentCenterX - gameState.players[BOTTOM].currentCenter;
            if(abs(dx) <= w)
            {
                //collision with client paddle
                PaddleHit(BOTTOM,threadBall,&xV);
                yV = -yV;
            }
            else
            {
                //no collision, point for host
                gameState.LEDScores[Host] += 1;
                gameState.balls[threadBall].alive = false;
                gameState.balls[threadBall].currentCenterX -= xV;
                gameState.balls[threadBall].currentCenterY -= yV;
                LCD_DrawRectangle(gameState.balls[threadBall].currentCenterX - BALL_SIZE_D2,gameState.balls[threadBall].currentCenterX + BALL_SIZE_D2,gameState.balls[threadBall].currentCenterY - BALL_SIZE_D2, gameState.balls[threadBall].currentCenterY + BALL_SIZE_D2, BACK_COLOR);
                gameState.numberOfBalls--;
                MoveLEDs();
                G8RTOS_KillSelf();
            }
        }
        if(gameState.balls[threadBall].currentCenterY <= TOP_PADDLE_EDGE + BALL_SIZE_D2)
        {
            //hit top threshold
            //check paddle collision
            dx = gameState.balls[threadBall].currentCenterX - gameState.players[TOP].currentCenter;
            if(abs(dx) <= w)
            {
                //collision with HOST paddle
                PaddleHit(TOP,threadBall,&xV);
                yV = -yV;
            }
            else
            {
                //no collision, point for CLIENT
                gameState.LEDScores[Client] += 1;
                gameState.balls[threadBall].alive = false;
                gameState.balls[threadBall].currentCenterX -= xV;
                gameState.balls[threadBall].currentCenterY -= yV;
                LCD_DrawRectangle(gameState.balls[threadBall].currentCenterX - BALL_SIZE_D2,gameState.balls[threadBall].currentCenterX + BALL_SIZE_D2,gameState.balls[threadBall].currentCenterY - BALL_SIZE_D2, gameState.balls[threadBall].currentCenterY + BALL_SIZE_D2, BACK_COLOR);
                gameState.numberOfBalls--;
                MoveLEDs();
                G8RTOS_KillSelf();
            }
        }
        sleep(20);
    }
}

/*
 * End of game for the host
 */
void EndOfGameHost()
{
    G8RTOS_WaitSemaphore(&lcdI2CReady);
    G8RTOS_WaitSemaphore(&wifiReady);
    G8RTOS_WaitSemaphore(&ledI2CReady);
    G8RTOS_Genocide();
    G8RTOS_InitSemaphore(&lcdI2CReady, 1);
    G8RTOS_InitSemaphore(&wifiReady, 1);
    G8RTOS_InitSemaphore(&ledI2CReady, 1);
    if(gameState.winner)
    {
        LCD_Clear(LCD_BLUE);
    }
    else
    {
        LCD_Clear(LCD_ORANGE);
    }
    char startScreen0[] = "Press Right Button for New Game";
    LCD_Text(50, 100, startScreen0, LCD_WHITE);
    gameState.players[0].currentCenter = PADDLE_X_CENTER;
    gameState.players[1].currentCenter = PADDLE_X_CENTER;
    for(int i= 0; i < MAX_NUM_OF_BALLS;i++)
    {
        gameState.balls[i].alive = false;
    }
    gameState.player.ready = false;
    gameState.gameDone = false;
    gameState.numberOfBalls = 0;
    gameState.LEDScores[0] = 0;
    gameState.LEDScores[1] = 0;
    gameState.overallScores[gameState.winner] += 1;
    gameState.winner = false;
    P4IFG = 0;
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN5);

    while(!gameState.player.ready);
    for(int i = 0;i < 5; i++)
    {
    //SendDataToClient();
    }
    //init board
    InitBoardState();
    //add threads
    G8RTOS_AddThread(SendDataToClientThread, 3, "TxClient");
    G8RTOS_AddThread(GenerateBall, 3, "GenBall");
    G8RTOS_AddThread(IdleThread, 255, "Idle");
    G8RTOS_AddThread(DrawObjects, 3, "DrawObj");
    G8RTOS_AddThread(ReadJoystickHost, 3, "RdJyHst");
    G8RTOS_AddThread(ReceiveDataFromClientThread, 3, "RxClient");

    G8RTOS_KillSelf();
}

void StartNewGame(void)
{
    gameState.player.ready = true;
    P4IFG = 0;
    GPIO_disableInterrupt(GPIO_PORT_P4, GPIO_PIN5);

}
void destroyBall(uint8_t ball){
    gameState.numberOfBalls--;
    gameState.balls[ball].alive = false;
}
/*********************************************** Host Threads *********************************************************************/
