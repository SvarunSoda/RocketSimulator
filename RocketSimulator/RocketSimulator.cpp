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

void InitRocket1();
void InitRocket2();
void InitPlanet();
void LaunchRocket(int countdown);
void UpdateRocket1();
void UpdateRocket2();
void FirstTurnRocket();
void SecondTurnRocket();
void DrawLiveFeed();
void DrawFeedData();
void DrawWorldMap1();
void DrawWorldMap2();
void RocketImpact();
void ParticleHit();
void BringCursorBack(int offsetX, int offsetY);
void Delay(long double seconds);

//// GLOBAL VARIABLES INITIALIZATION

Vector RocketVector1;
Vector RocketVector2;
Vector PlanetVector;

ofstream ParticleFile;

long double FlightTime = 0;
long double UpdateInterval = 0.05;
long double RocketAccumulatedAccelerationX = 0;
long double RocketAccumulatedAccelerationY = 0;
long double RocketAccumulatedAccelerationZ = 0;
long double MeteorAccumulatedAccelerationX = 0;
long double MeteorAccumulatedAccelerationY = 0;
long double MeteorAccumulatedAccelerationZ = 0;
long double AccelerationErrorTolerance = UpdateInterval;
long double TargetErrorTolerance = 0.1;
long double AccelerationChange = UpdateInterval;
long double AngleChange = TargetErrorTolerance;
long double MaxHeight = 0;
int RocketFlashed = 0;
int RocketFlashInterval = 2;
int ParticleTracker = 0;
int ProgramType;
int FinishType;

bool FirstTurnDone = false;
bool SecondTurnDone = false;
bool SlowAcceleration = true;
bool CanDrawRocket = true;

std::string WorldMap[16] = {

    "           ---------  -------      --\\     -----------------        ",
    " /--------/        /  \\-----|     /// ----/                  \\------ ",
    " \\ /----           \\   -\\   /     /-                             /-\\",
    " /-     \\-       /-      \\-/ -  -/                           /--/-   ",
    "-         \\   --/             -/ -   \\                    -/        ",
    "           \\-\\  \\             |-/ \\\\- \\\\---              / /        ",
    "             \\\\               /--\\-\\\\ /    \\    -     /-/           ",
    "              /---           /      \\/      \\  / \\  /- \\            ",
    "              \\   \\          \\--    /        \\/   \\-|               ",
    "               |   \\            \\   |              \\ \\-\\/-\\\\        ",
    "              /    /            |  //               -- /    |   /   ",
    "              | --              \\-/                    \\----/  /    ",
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

    // PROGRAM BRANCH

    string programEntry;

    while (1 > 0) {

        cout << "Please enter the simulator type you would like to run (Enter \"SimpleFlight\" or \"Interceptor\"): ";
        cin >> programEntry;

        if (((programEntry.compare("SimpleFlight") == 0) || (programEntry.compare("simpleflight") == 0))) {

            ProgramType = 0;

            break;

        } else if (((programEntry.compare("Interceptor") == 0) || (programEntry.compare("interceptor") == 0))) {

            ProgramType = 1;

            break;

        }

    }

    cout << "\n\n\n";

    // INITIALIZING THE VECTORS

    InitPlanet(); cout << "\n\n";

    if (ProgramType == 1) {

        InitRocket2();

    }

    InitRocket1(); cout << "\n\n";

    // LAUNCHING THE ROCKET

    if (ProgramType == 0) {

        LaunchRocket(10);

    } else if (ProgramType == 1) {

        LaunchRocket(3);

    }

    // ROCKET MAIN FLIGHT LOOP

    DrawLiveFeed();

    while (1 > 0) {

        UpdateRocket1();
        
        if (ProgramType == 1) {

            UpdateRocket2();

        }

        DrawLiveFeed();

        Delay(UpdateInterval);

        FlightTime = FlightTime + UpdateInterval;

        if (RocketVector1.GetStartY() < 0) {

            FinishType = 0;

            break;

        } else {

            if (ProgramType == 1) {

                if (RocketVector2.GetStartY() < 0) {

                    FinishType = 1;

                    break;

                }

                if ((((RocketVector2.GetStartX() - RocketVector1.GetStartX()) < 1) && ((RocketVector2.GetStartX() - RocketVector1.GetStartX()) > -1)) && (((RocketVector2.GetStartY() - RocketVector1.GetStartY()) < 10000) && ((RocketVector2.GetStartY() - RocketVector1.GetStartY()) > -10000)) && (((RocketVector2.GetStartZ() - RocketVector1.GetStartZ()) < 1) && ((RocketVector2.GetStartZ() - RocketVector1.GetStartZ()) > -1))) {

                    FinishType = 2;

                    break;

                }

            }

        }

    }

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

    if (FinishType == 0) {

        cout << "\n\n------------------------------------ The rocket has landed! ------------------------------------\n\n";

    } else if (FinishType == 1) {

        cout << "\n\n------------------------------------ The meteor has hit the earth! ------------------------------------\n\n";

    } else if (FinishType == 2) {

        cout << "\n\n------------------------------------ The meteor has been intercepted! ------------------------------------\n\n";

    }

    cout << "Final recorded data:\n";

    cout << RocketVector1.ToStringPos();
    cout << RocketVector1.ToStringAngle();

}

//// PARTICLE FUNCTIONS

void ParticleHit() {

    Vector ParticleVector;

    ParticleVector.SetName("Particle #" + (ParticleTracker + 1));
    ParticleVector.SetOtherName(RocketVector1.GetName());
    ParticleVector.SetStartX(rand() % 500 - 500, true);
    ParticleVector.SetStartY(rand() % 500 - 500);
    ParticleVector.SetStartZ(rand() % 500 - 500, true);
    ParticleVector.SetEndX(rand() % 500 - 500, true);
    ParticleVector.SetEndY(rand() % 500 - 500);
    ParticleVector.SetEndZ(rand() % 500 - 500, true);
    ParticleVector.UpdateLengthFromPoints();
    ParticleVector.UpdateRelAngle(RocketVector1);

    ParticleFile << ParticleVector.ToStringPos() << flush;
    ParticleFile << RocketVector1.ToStringPos() << flush;
    ParticleFile << ParticleVector.ToStringAngle() << flush;
    ParticleFile << "\n" << flush;

    ParticleTracker++;

}

//// ROCKET TRAJECTORY CALCULATION FUNCTIONS

void UpdateRocket1() {

    long double gravityAcceleration = G_CONST * (PlanetVector.GetMass() / pow(PlanetVector.GetRadius() + RocketVector1.GetStartY(), 2));

    if (ProgramType == 0) {

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

    }

    if (RocketVector1.GetFuel() > 0) {

        RocketVector1.SetFuel(RocketVector1.GetFuel() - UpdateInterval);

    } else {

        if (RocketVector1.GetAccel() > 0) {

            RocketVector1.SetAccel(0);

        }

        if (RocketVector1.GetFuel() > 0) {

            RocketVector1.SetFuel(0);

        }

    }

    long double rocketTargetAccelHorizontal = (RocketVector1.GetAccel() * cos((M_PI / 180) * RocketVector1.GetAngleY()));
    
    if ((RocketVector1.GetAngleY() == 90) || (RocketVector1.GetAngleY() == -90)) {

        rocketTargetAccelHorizontal = 0;

    }

    long double rocketTargetAccelVertical = (RocketVector1.GetAccel() * sin((M_PI / 180) * RocketVector1.GetAngleY()));

    long double rocketTargetAccelX = rocketTargetAccelHorizontal * sin((M_PI / 180) * RocketVector1.GetAngleX());
    long double rocketTargetAccelY = rocketTargetAccelVertical - gravityAcceleration;
    long double rocketTargetAccelZ = rocketTargetAccelHorizontal * cos((M_PI / 180) * RocketVector1.GetAngleX());

    if ((RocketVector1.GetAngleY() == 90) || (RocketVector1.GetAngleY() == -90)) {

        rocketTargetAccelZ = 0;

    }

    if (SlowAcceleration == true) {

        if (RocketAccumulatedAccelerationX < (rocketTargetAccelX - AccelerationErrorTolerance)) {

            RocketAccumulatedAccelerationX = RocketAccumulatedAccelerationX + (AccelerationChange);

        } else if (RocketAccumulatedAccelerationX > (rocketTargetAccelX + AccelerationErrorTolerance)) {

            RocketAccumulatedAccelerationX = RocketAccumulatedAccelerationX - (AccelerationChange);

        }

        if (RocketAccumulatedAccelerationY < (rocketTargetAccelY - AccelerationErrorTolerance)) {

            RocketAccumulatedAccelerationY = RocketAccumulatedAccelerationY + (AccelerationChange);

        } else if (RocketAccumulatedAccelerationY > (rocketTargetAccelY + AccelerationErrorTolerance)) {

            RocketAccumulatedAccelerationY = RocketAccumulatedAccelerationY - (AccelerationChange);

        }

        if (RocketAccumulatedAccelerationZ < (rocketTargetAccelZ - AccelerationErrorTolerance)) {

            RocketAccumulatedAccelerationZ = RocketAccumulatedAccelerationZ + (AccelerationChange);

        } else if (RocketAccumulatedAccelerationZ > (rocketTargetAccelZ + AccelerationErrorTolerance)) {

            RocketAccumulatedAccelerationZ = RocketAccumulatedAccelerationZ - (AccelerationChange);

        }

    } else {

        RocketAccumulatedAccelerationX = rocketTargetAccelX;
        RocketAccumulatedAccelerationY = rocketTargetAccelY;
        RocketAccumulatedAccelerationZ = rocketTargetAccelZ;
        
    }

    RocketVector1.UpdateLengthFromComponents(RocketVector1.GetLengthX() + RocketAccumulatedAccelerationX, RocketVector1.GetLengthY() + RocketAccumulatedAccelerationY, RocketVector1.GetLengthZ() + RocketAccumulatedAccelerationZ);
    RocketVector1.MoveVectorByLength();
    RocketVector1.UpdateEndPoint();
    RocketVector1.UpdateRelAngle(PlanetVector);

    if (ProgramType == 1) {

        RocketVector1.UpdateTargetAnglePredictive(RocketVector2);

        long double currentAngleX = RocketVector1.GetAngleX();
        long double targetAngleX = RocketVector1.GetTargetAngleX();

        if (currentAngleX < (targetAngleX - TargetErrorTolerance)) {

            RocketVector1.SetAngleX(currentAngleX + AngleChange, false, true);

        } else if (currentAngleX > (targetAngleX + TargetErrorTolerance)) {

            RocketVector1.SetAngleX(currentAngleX - AngleChange, false, true);

        }

        long double currentAngleY = RocketVector1.GetAngleY();
        long double targetAngleY = RocketVector1.GetTargetAngleY();

        if (currentAngleY < (targetAngleY - TargetErrorTolerance)) {

            RocketVector1.SetAngleY(currentAngleY + AngleChange, true);

        } else if (currentAngleY > (targetAngleY + TargetErrorTolerance)) {

            RocketVector1.SetAngleY(currentAngleY - AngleChange, true);

        }

    }

}

void UpdateRocket2() {

    long double gravityAcceleration = G_CONST * (PlanetVector.GetMass() / pow(PlanetVector.GetRadius() + RocketVector2.GetStartY(), 2));

    if (RocketVector2.GetFuel() > 0) {

        RocketVector2.SetFuel(RocketVector2.GetFuel() - UpdateInterval);

    } else {

        if (RocketVector2.GetAccel() > 0) {

            RocketVector2.SetAccel(0);

        }

        if (RocketVector2.GetFuel() > 0) {

            RocketVector2.SetFuel(0);

        }

    }

    long double rocketTargetAccelHorizontal = (RocketVector2.GetAccel() * cos((M_PI / 180) * RocketVector2.GetAngleY()));

    if ((RocketVector2.GetAngleY() == 90) || (RocketVector2.GetAngleY() == -90)) {

        rocketTargetAccelHorizontal = 0;

    }

    long double rocketTargetAccelVertical = (RocketVector2.GetAccel() * sin((M_PI / 180) * RocketVector2.GetAngleY()));

    long double rocketTargetAccelX = rocketTargetAccelHorizontal * sin((M_PI / 180) * RocketVector2.GetAngleX());
    long double rocketTargetAccelY = rocketTargetAccelVertical - gravityAcceleration;
    long double rocketTargetAccelZ = rocketTargetAccelHorizontal * cos((M_PI / 180) * RocketVector2.GetAngleX());

    if ((RocketVector2.GetAngleY() == 90) || (RocketVector2.GetAngleY() == -90)) {

        rocketTargetAccelZ = 0;

    }

    if (SlowAcceleration == true) {

        if (MeteorAccumulatedAccelerationX < (rocketTargetAccelX - AccelerationErrorTolerance)) {

            MeteorAccumulatedAccelerationX = MeteorAccumulatedAccelerationX + (AccelerationChange);

        } else if (MeteorAccumulatedAccelerationX > (rocketTargetAccelX + AccelerationErrorTolerance)) {

            MeteorAccumulatedAccelerationX = MeteorAccumulatedAccelerationX - (AccelerationChange);

        }

        if (MeteorAccumulatedAccelerationY < (rocketTargetAccelY - AccelerationErrorTolerance)) {

            MeteorAccumulatedAccelerationY = MeteorAccumulatedAccelerationY + (AccelerationChange);

        } else if (MeteorAccumulatedAccelerationY > (rocketTargetAccelY + AccelerationErrorTolerance)) {

            MeteorAccumulatedAccelerationY = MeteorAccumulatedAccelerationY - (AccelerationChange);

        }

        if (MeteorAccumulatedAccelerationZ < (rocketTargetAccelZ - AccelerationErrorTolerance)) {

            MeteorAccumulatedAccelerationZ = MeteorAccumulatedAccelerationZ + (AccelerationChange);

        } else if (MeteorAccumulatedAccelerationZ > (rocketTargetAccelZ + AccelerationErrorTolerance)) {

            MeteorAccumulatedAccelerationZ = MeteorAccumulatedAccelerationZ - (AccelerationChange);

        }

    } else {

        MeteorAccumulatedAccelerationX = rocketTargetAccelX;
        MeteorAccumulatedAccelerationY = rocketTargetAccelY;
        MeteorAccumulatedAccelerationZ = rocketTargetAccelZ;

    }

    RocketVector2.UpdateLengthFromComponents(RocketVector2.GetLengthX() + MeteorAccumulatedAccelerationX, RocketVector2.GetLengthY() + MeteorAccumulatedAccelerationY, RocketVector2.GetLengthZ() + MeteorAccumulatedAccelerationZ);
    RocketVector2.MoveVectorByLength();
    RocketVector2.UpdateEndPoint();
    RocketVector2.UpdateRelAngle(PlanetVector);

}

void FirstTurnRocket() {

    if (RocketVector1.GetAngleY() < (20 - TargetErrorTolerance)) {

        RocketVector1.SetAngleY(RocketVector1.GetAngleY() + AngleChange, true);

    } else if (RocketVector1.GetAngleY() > (20 + TargetErrorTolerance)) {

        RocketVector1.SetAngleY(RocketVector1.GetAngleY() - AngleChange, true);

    } else {

        FirstTurnDone = true;

    }

}

void SecondTurnRocket() {

    bool xInPlace = false;
    bool yInPlace = false;

    if (RocketVector1.GetAngleX() < (-30 - TargetErrorTolerance)) {

        RocketVector1.SetAngleX(RocketVector1.GetAngleX() + AngleChange, false, true);

    } else if (RocketVector1.GetAngleX() > (-30 + TargetErrorTolerance)) {

        RocketVector1.SetAngleX(RocketVector1.GetAngleX() - AngleChange, false, true);

    } else {

        xInPlace = true;

    }

    if (RocketVector1.GetAngleY() < (0 - TargetErrorTolerance)) {

        RocketVector1.SetAngleY(RocketVector1.GetAngleY() + AngleChange, true);

    } else if (RocketVector1.GetAngleY() > (0 + TargetErrorTolerance)) {

        RocketVector1.SetAngleY(RocketVector1.GetAngleY() - AngleChange, true);

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

    if (RocketVector1.GetPosDegreeStatus() == true) {

        if (ProgramType == 0) {

            DrawWorldMap1();

        } else if (ProgramType == 1) {

            DrawWorldMap2();

        }

    }

}

void DrawFeedData() {

    cout << RocketVector1.ToStringRocketLiveFeed(RocketAccumulatedAccelerationX, RocketAccumulatedAccelerationY, RocketAccumulatedAccelerationZ, FlightTime, ParticleTracker);
    
    BringCursorBack(0, -25);

}

void DrawWorldMap1() {

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

        int rocketPosX = ((RocketVector1.GetStartX() / 180) * (mapSizeX / 2)) + (mapSizeX / 2);
        int rocketPosY = (-(RocketVector1.GetStartZ() / 90) * (mapSizeY / 2)) + (mapSizeY / 2);

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

void DrawWorldMap2() {

    int mapSizeX = 48;
    int mapSizeY = 11;
    int distanceX = 60;
    int spacing = 2;

    for (int l = 0; l < 2; l++) {

        // DRAWING THE MAP
        
        BringCursorBack(distanceX, 0);

        for (int i = 0; i < (mapSizeX); i++) {

            cout << " ";

        }

        BringCursorBack(0, 0);

        for (int k = 0; k < mapSizeY; k++) {

            BringCursorBack(distanceX, 1);

            cout << "|";

            if ((k == 0) || (k == (mapSizeY - 1))) {

                for (int j = 0; j < (mapSizeX); j++) {

                    BringCursorBack(distanceX + (j + 1), 0);

                    cout << "-";

                }

            } else {

                for (int j = 0; j < (mapSizeX); j++) {

                    BringCursorBack(distanceX + (j + 1), 0);

                    cout << " ";

                }

            }

            BringCursorBack(distanceX + mapSizeX, 0);

            cout << "|";

        }

        // DRAWING THE ROCKET

        //if (CanDrawRocket == true) {

            BringCursorBack(0, -(mapSizeY));

            for (int i = 0; i < 2; i++) {

                int rocketPosX = ((RocketVector1.GetStartX() / 180) * (mapSizeX / 2)) + (mapSizeX / 2);
                int rocketPosY = (-(RocketVector1.GetStartZ() / 90) * (mapSizeY / 2)) + (mapSizeY / 2);

                if (i == 1) {

                    rocketPosX = ((RocketVector2.GetStartX() / 180) * (mapSizeX / 2)) + (mapSizeX / 2);
                    rocketPosY = (-(RocketVector2.GetStartZ() / 90) * (mapSizeY / 2)) + (mapSizeY / 2);

                }

                if (l == 1) {

                    rocketPosX = ((RocketVector1.GetStartX() / 180) * (mapSizeX / 2)) + (mapSizeX / 2);
                    rocketPosY =((MaxHeight - RocketVector1.GetStartY()) / MaxHeight) * mapSizeY;

                    if (i == 1) {

                        rocketPosX = ((RocketVector2.GetStartX() / 180) * (mapSizeX / 2)) + (mapSizeX / 2);
                        rocketPosY = ((MaxHeight - RocketVector2.GetStartY()) / MaxHeight) * mapSizeY;

                    }

                }

                if (rocketPosY > mapSizeY) {

                    rocketPosY = mapSizeY;

                } else if (rocketPosY < 0) {

                    rocketPosY = 0;

                }

                BringCursorBack(distanceX + rocketPosX, rocketPosY);

                if (i == 0) {

                    cout << "R";

                } else if (i == 1) {

                    cout << "M";

                }

                BringCursorBack(0, -(rocketPosY));

            }

            BringCursorBack(0, mapSizeY);

        //}

        if (l == 0) {

            BringCursorBack(0, spacing);

        } else if (l == 1) {

            BringCursorBack(0, -((mapSizeY * 2) + spacing));

        }

    }

    /*if (CanDrawRocket == true) {

        if (RocketFlashed == RocketFlashInterval) {

            CanDrawRocket = false;

            RocketFlashed = 0;

        } else {

            RocketFlashed++;

        }

    } else {

        if (RocketFlashed == RocketFlashInterval) {

            CanDrawRocket = true;

            RocketFlashed = 0;

        } else {

            RocketFlashed++;

        }

    }*/

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

void InitRocket1() {

    long double input;

    RocketVector1.SetName("Rocket");

    if (ProgramType == 0) {

        RocketVector1.SetOtherName(PlanetVector.GetName());

    } else if (ProgramType == 1) {

        RocketVector1.SetOtherName(RocketVector2.GetName());

    }

    RocketVector1.SetPosDegreeStatus(true);
    cout << "Please input the Rocket Data:";
    cout << "\nMass: ";
    cin >> input;
    RocketVector1.SetMass(input);
    cout << "Engine Acceleration: ";
    cin >> input;
    RocketVector1.SetAccel(input);
    cout << "Fuel: ";
    cin >> input;
    RocketVector1.SetFuel(input);
    cout << "Starting Longitude (X): ";
    cin >> input;
    RocketVector1.SetStartX(input, false);
    /*cout << "Starting Altitude (Y): ";
    cin >> input;
    RocketVector1.SetStartY(input);*/
    cout << "Starting Lattitude (Z): ";
    cin >> input;
    RocketVector1.SetStartZ(input, false);
    RocketVector1.SetAngleY(90, true);
    RocketVector1.SetRadius(PlanetVector.GetRadius());

    cout << RocketVector1.ToStringPos();
    cout << RocketVector1.ToStringAngle();
    cout << RocketVector1.ToStringMisc();

}

void InitRocket2() {

    RocketVector2.SetName("Meteor");
    RocketVector2.SetOtherName(PlanetVector.GetName());
    RocketVector2.SetPosDegreeStatus(true);
    RocketVector2.SetMass(rand() % 10000 + 8000);
    //RocketVector2.SetStartX(rand() % 180 - 180, true);
    RocketVector2.SetStartX(0, true);
    MaxHeight = (rand() % 200000 + 150000);
    RocketVector2.SetStartY(MaxHeight);
    //RocketVector2.SetStartZ(rand() % 90 - 90, true);
    RocketVector2.SetStartZ(0, true);
    RocketVector2.SetAngleY(-90, true);
    RocketVector2.SetRadius(PlanetVector.GetRadius());

    cout << RocketVector2.ToStringPos();
    cout << RocketVector2.ToStringAngle();
    cout << RocketVector2.ToStringMisc();

    cout << "\n\n";

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