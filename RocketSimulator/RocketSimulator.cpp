//Author(s): Svarun Soda
//Class: Aerospace & Science Team of College of the Canyons

#include <stdio.h>
#include <iostream>
#include "..\VectorCalculator\vector.cpp"
#include <thread>
#include <chrono>
#include <conio.h>
#include <windows.h>
#include <fstream>
#define G_CONST           6.673 * pow(10, -11)

using namespace std;

//// FUNCTION LIST

void InitRocket();
void InitPlanet();
void LaunchRocket(int countdown);
void UpdateRocket();
void FirstTurnRocket();
void SecondTurnRocket();
void DrawLiveFeed();
void DrawFeedData();
void DrawWorldMap();
void RocketImpact();
void ParticleHit();
void BringCursorBack(int offsetX, int offsetY);
void Delay(long double seconds);

//// GLOBAL VARIABLES INITIALIZATION

Vector RocketVector;
Vector PlanetVector;

ofstream ParticleFile;

long double FlightTime = 0;
long double UpdateInterval = 0.05;
long double RocketAccumulatedAccelerationX = 0;
long double RocketAccumulatedAccelerationY = 0;
long double RocketAccumulatedAccelerationZ = 0;
long double AccelerationChange = UpdateInterval;
int RocketFlashed = 0;
int RocketFlashInterval = 2;
int ParticleTracker = 0;

bool FirstTurnDone = false;
bool SecondTurnDone = false;
bool SlowAcceleration = true;
bool CanDrawRocket = true;

std::string WorldMap[16] = {

    "           ---------  -------      --\\     -----------------        ",
    " /--------/        /  \\-----|     /// ----/                  \\------ ",
    " \\ /----           \\   -\\   /     /-                             /-\\",
    " /-     \\-       /-      \\-/    -/                           /----   ",
    "-         \\   --/             -/ -   \\                    -/        ",
    "           \\-\\  \\             |-/ \\\\- \\\\---              /          ",
    "             \\\\               /--\\-\\\\ /    \\    -     /-/           ",
    "              /---           /      \\/      \\  / \\  /-              ",
    "              \\   \\          \\--    /        \\/   \\-|               ",
    "               |   \\            \\   |                               ",
    "              /    /            |  /                                ",
    "              | --              \\-/                                 ",
    "              \\/                                                    ",
    "               /                       ----------  -------------    ",
    "  /-----------/\\        --------------/          \\/             |   ",
    "-/              -------/                                        \\---",

};

//// MAIN RUNNER FUNCTION

int main() {

    cout << "Rocket Simulator Program\n";
    cout << "Author(s): Svarun Soda";

    srand(time(NULL));

    cout << "\n\n\n";

    // INITIALIZING THE VECTORS

    InitPlanet(); cout << "\n\n";
    InitRocket(); cout << "\n\n";

    // LAUNCHING THE ROCKET

    LaunchRocket(10);

    // ROCKET MAIN FLIGHT LOOP

    DrawLiveFeed();

    do {

        UpdateRocket();
        
        DrawLiveFeed();

        Delay(UpdateInterval);

        FlightTime = FlightTime + UpdateInterval;

    } while (RocketVector.GetStartY() >= 0);

    // ROCKET HITS THE GROUND

    CanDrawRocket = true;

    DrawLiveFeed();

    BringCursorBack(0, 25);

    RocketImpact();

    // PROGRAM END

    cout << "\n\nPROGRAM FINISHED\n\n";

    system("pause");

    return 0;

}

//// FINISHING FUNCTIONS

void RocketImpact() {

    ParticleFile.close();

    cout << "\n\n------------------------------------ The rocket has landed! ------------------------------------\n\n";
    cout << "Final recorded data:\n";

    cout << RocketVector.ToStringPos();
    cout << RocketVector.ToStringAngle();

}

//// PARTICLE FUNCTIONS

void ParticleHit() {

    Vector ParticleVector;

    ParticleVector.SetName("Particle" + (ParticleTracker + 1));
    ParticleVector.SetOtherName(RocketVector.GetName());
    ParticleVector.SetStartX(rand() % 500 - 500, true);
    ParticleVector.SetStartY(rand() % 500 - 500);
    ParticleVector.SetStartZ(rand() % 500 - 500, true);
    ParticleVector.SetEndX(rand() % 500 - 500, true);
    ParticleVector.SetEndY(rand() % 500 - 500);
    ParticleVector.SetEndZ(rand() % 500 - 500, true);
    ParticleVector.UpdateLengthFromPoints();
    ParticleVector.UpdateRelAngle(RocketVector);

    ParticleFile << ParticleVector.ToStringPos() << flush;
    ParticleFile << RocketVector.ToStringPos() << flush;
    ParticleFile << ParticleVector.ToStringAngle() << flush;
    ParticleFile << "\n" << flush;

    ParticleTracker++;

}

//// ROCKET TRAJECTORY CALCULATION FUNCTIONS

void UpdateRocket() {

    long double gravityAcceleration = G_CONST * (PlanetVector.GetMass() / pow(PlanetVector.GetRadius() + RocketVector.GetStartY(), 2));

    int randomNumber = rand() % 10 + 1;

    if (randomNumber == 5) {

        ParticleHit();

    }

    if ((FirstTurnDone == false) && (FlightTime > 10)) {

        FirstTurnRocket();

    }

    if ((SecondTurnDone == false) && (FlightTime > 30)) {

        SecondTurnRocket();

    }

    if (RocketVector.GetFuel() > 0) {

        RocketVector.SetFuel(RocketVector.GetFuel() - UpdateInterval);

    } else {

        if (RocketVector.GetAccel() > 0) {

            RocketVector.SetAccel(0);

        }

        if (RocketVector.GetFuel() > 0) {

            RocketVector.SetFuel(0);

        }

    }

    long double rocketTargetAccelX = (RocketVector.GetAccel() * cos((M_PI / 180) * RocketVector.GetAngleY())) * sin((M_PI / 180) * RocketVector.GetAngleX());
    long double rocketTargetAccelY = (RocketVector.GetAccel() * sin((M_PI / 180) * RocketVector.GetAngleY())) - gravityAcceleration;
    long double rocketTargetAccelZ;

    if ((RocketVector.GetAngleY() == 90) || (RocketVector.GetAngleY() == -90)) {

        rocketTargetAccelZ = 0;

    } else {

        rocketTargetAccelZ = (RocketVector.GetAccel() * cos((M_PI / 180) * RocketVector.GetAngleY())) * cos((M_PI / 180) * RocketVector.GetAngleX());

    }

    if (SlowAcceleration == true) {

        if (RocketAccumulatedAccelerationX < rocketTargetAccelX) {

            RocketAccumulatedAccelerationX = RocketAccumulatedAccelerationX + (AccelerationChange);

        } else if (RocketAccumulatedAccelerationX > rocketTargetAccelX) {

            RocketAccumulatedAccelerationX = RocketAccumulatedAccelerationX - (AccelerationChange);

        }

        if (RocketAccumulatedAccelerationY < rocketTargetAccelY) {

            RocketAccumulatedAccelerationY = RocketAccumulatedAccelerationY + (AccelerationChange);

        } else if (RocketAccumulatedAccelerationY > rocketTargetAccelY) {

            RocketAccumulatedAccelerationY = RocketAccumulatedAccelerationY - (AccelerationChange);

        }

        if (RocketAccumulatedAccelerationZ < rocketTargetAccelZ) {

            RocketAccumulatedAccelerationZ = RocketAccumulatedAccelerationZ + (AccelerationChange);

        } else if (RocketAccumulatedAccelerationZ > rocketTargetAccelZ) {

            RocketAccumulatedAccelerationZ = RocketAccumulatedAccelerationZ - (AccelerationChange);

        }

    } else {

        RocketAccumulatedAccelerationX = rocketTargetAccelX;
        RocketAccumulatedAccelerationY = rocketTargetAccelY;
        RocketAccumulatedAccelerationZ = rocketTargetAccelZ;

    }

    RocketVector.UpdateLengthFromComponents(RocketVector.GetLengthX() + RocketAccumulatedAccelerationX, RocketVector.GetLengthY() + RocketAccumulatedAccelerationY, RocketVector.GetLengthZ() + RocketAccumulatedAccelerationZ);
    RocketVector.MoveVectorByLength();
    RocketVector.UpdateEndPoint();
    RocketVector.UpdateRelAngle(PlanetVector);

}

void FirstTurnRocket() {

    if (RocketVector.GetAngleY() < 20) {

        RocketVector.SetAngleY(RocketVector.GetAngleY() + 0.5, true);

    } else if (RocketVector.GetAngleY() > 20) {

        RocketVector.SetAngleY(RocketVector.GetAngleY() - 0.5, true);

    } else {

        FirstTurnDone = true;

    }

}

void SecondTurnRocket() {

    bool xInPlace = false;
    bool yInPlace = false;

    if (RocketVector.GetAngleX() < -30) {

        RocketVector.SetAngleX(RocketVector.GetAngleX() + 0.5, true);

    } else if (RocketVector.GetAngleX() > -30) {

        RocketVector.SetAngleX(RocketVector.GetAngleX() - 0.5, true);

    } else {

        xInPlace = true;

    }

    if (RocketVector.GetAngleY() < 0) {

        RocketVector.SetAngleY(RocketVector.GetAngleY() + 0.5, true);

    } else if (RocketVector.GetAngleY() > 0) {

        RocketVector.SetAngleY(RocketVector.GetAngleY() - 0.5, true);

    } else {

        yInPlace = true;

    }

    if ((xInPlace == true) && (yInPlace == true)) {

        SecondTurnDone = true;

    }

}

//// FEED-DRAWING FUNCTIONS

void DrawLiveFeed() {

    DrawFeedData();

    if (RocketVector.GetPosDegreeStatus() == true) {

        DrawWorldMap();

    }

}

void DrawFeedData() {

    cout << RocketVector.ToStringRocketLiveFeed(RocketAccumulatedAccelerationX, RocketAccumulatedAccelerationY, RocketAccumulatedAccelerationZ, FlightTime, ParticleTracker);
    
    BringCursorBack(0, -25);

}

void DrawWorldMap() {

    int mapSizeX = 69;
    int mapSizeY = 18;
    int distanceX = 45;

    // DRAWING THE MAP

    for (int i = 0; i < (mapSizeX + distanceX); i++) {

        cout << " ";

    }

    BringCursorBack(0, 0);

    for (int i = 0; i < mapSizeY; i++) {

        BringCursorBack(distanceX, 1);

        cout << "|";

        if ((i == 0) || (i == (mapSizeY - 1))) {

            for (int j = 0; j < (mapSizeX); j++) {

                BringCursorBack(distanceX + (j + 1), 0);

                cout << "-";

            }

        }
        else {

            cout << WorldMap[i - 1];

        }

        BringCursorBack(distanceX + mapSizeX, 0);

        cout << "|";

    }

    BringCursorBack(0, -mapSizeY);

    // DRAWING THE ROCKET

    if (CanDrawRocket == true) {

        int rocketPosX = ((RocketVector.GetStartX() / 180) * (mapSizeX / 2)) + (mapSizeX / 2);
        int rocketPosY = (-(RocketVector.GetStartZ() / 90) * (mapSizeY / 2)) + (mapSizeY / 2);

        BringCursorBack(distanceX + rocketPosX, rocketPosY);

        cout << "X";

        BringCursorBack(0, -rocketPosY);

        if (RocketFlashed == RocketFlashInterval) {

            CanDrawRocket = false;

            RocketFlashed = 0;

        }
        else {

            RocketFlashed++;

        }

    } else {

        if (RocketFlashed == RocketFlashInterval) {

            CanDrawRocket = true;

            RocketFlashed = 0;

        } else {

            RocketFlashed++;

        }

    }

}

//// ROCKET LAUNCHING FUNCTION

void LaunchRocket(int countdown) {

    cout << "------------------------------------ The rocket is ready to launch! ------------------------------------\n\n";

    system("pause");

    for (int i = countdown; i > 0; i--) {

        cout << "\n";
        cout << i;

        Delay(1);

    }

    cout << "\n0";

    ParticleFile.open("ParticleReport.txt");

    cout << "\n\n------------------------------------ The rocket has been launched! ------------------------------------\n\n";

}

//// INITIALIZATION FUNCTIONS

void InitRocket() {

    long double input;

    RocketVector.SetName("Rocket");
    RocketVector.SetOtherName(PlanetVector.GetName());
    RocketVector.SetPosDegreeStatus(true);
    cout << "Please input the Rocket Data:";
    cout << "\nMass: ";
    cin >> input;
    RocketVector.SetMass(input);
    cout << "Engine Acceleration: ";
    cin >> input;
    RocketVector.SetAccel(input);
    cout << "Fuel: ";
    cin >> input;
    RocketVector.SetFuel(input);
    cout << "Starting Longitude (X): ";
    cin >> input;
    RocketVector.SetStartX(input, false);
    /*cout << "Starting Altitude (Y): ";
    cin >> input;
    RocketVector.SetStartY(input);*/
    cout << "Starting Lattitude (Z): ";
    cin >> input;
    RocketVector.SetStartZ(input, false);
    RocketVector.SetAngleY(90, true);
    RocketVector.SetRadius(PlanetVector.GetRadius());

    cout << RocketVector.ToStringPos();
    cout << RocketVector.ToStringAngle();
    cout << RocketVector.ToStringMisc();

}

void InitPlanet() {

    long double input;

    PlanetVector.SetName("Planet");
    PlanetVector.SetPosDegreeStatus(true);
    cout << "Please input the Planet Data:";
    cout << "\nMass: ";
    cin >> input;
    PlanetVector.SetMass(input);
    cout << "Radius: ";
    cin >> input;
    PlanetVector.SetRadius(input);

    cout << PlanetVector.ToStringPos();
    cout << PlanetVector.ToStringMisc();

}

//// SUPPORTING FUNCTIONS

void BringCursorBack(int offsetX, int offsetY) {

    COORD newCursorPos;
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
    COORD currentCursorPos = consoleInfo.dwCursorPosition;

    newCursorPos.X = offsetX;
    newCursorPos.Y = currentCursorPos.Y + offsetY;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), newCursorPos);

}

void Delay(long double seconds) {

    std::this_thread::sleep_for(std::chrono::milliseconds(int(seconds * 1000)));

}