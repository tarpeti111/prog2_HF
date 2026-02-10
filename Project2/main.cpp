#include "Handler.h"
#include "Handler.cpp"
#include "EventHandler.h"
//#include <crtdbg.h>
    
int main() {  

    Base base;
    Handler handler(&base);
    EventHandler eh(&base);

    static int actions;
    bool valid = false;
    static int turns = 1;

    bool win = false;
    bool endless = false;

    //végtelen mód kiválasztása
    string endless_choice;
    cout << "Endless mode? y/n" << endl;
    cin >> endless_choice;
    if (endless_choice == "y")
    {
        endless_choice = true;
    }
    system("CLS");


    handler.Initialize();

    while (!win || endless)
    {
        //actions reset
        actions = 5;

        while (actions > 0)
        {
            int choice;
            
            system("CLS");

            //jelenlegi játékban eltelt idõ kiírása
            handler.writeTimeAndAddYears(turns-1);

            cout << "----------------------------" << endl;
            cout << "NUMBER OF ACTIONS LEFT: " << actions << endl;
            cout << "----------------------------" << endl;

            //ha invázió akkor alien hollétének kiírása
            if (eh.getInvasionState())
            {
                cout << "Invasion is ongoing, alien currently at: " << base.getRoom(eh.getEnemyRoom()).getName() << endl;
                cout << "----------------------------" << endl;
            }

            //választás kérése a menübõl
            choice = handler.Menu();
            if (choice == 0)
                return 0;

            system("CLS");

            //a választott action végrehajtása
            valid = handler.Action(choice);
            if (valid)
                actions--;
            system("pause");
            system("CLS");

            //ha valid és van invázió
            if (valid && eh.getInvasionState())
            {
                eh.DoInvasion();
            }

            //ha az invázió megölt mindenkit akkor kilépés
            if (base.getPeopleSize() <= 0)
            {
                break;
            }
        }

        //ha van betegség
        if (eh.getSickness())
        {
            eh.SicknessCheck();
            eh.SicknessKill();
            eh.setSickness(false);
        }

        //ha meghalt mindenki a betegségben kilépés
        if (base.getPeopleSize() <= 0)
        {
            break;
        }

        turns++;

        //10 kör túlélésénél a player nyert
        if (turns >= 10)
        {
            win = true;
        }

        system("CLS");

        //kör végi misc. dolgok

        handler.EndTurnAddResources();  //alapanyag kalkuláció / esetleges halálok
        handler.AgeHandler();   //életkorok növelése / esetleges halálok

        system("CLS");

        FileManager::printImage("NEW_YEAR");    //új év kép

        system("pause");

        handler.arrival();  //új ember érkezése

        eh.DoRandomEvent(); //random event kiválasztása és végrehajtása
        system("pause");
    }

    //ha a player nem nyert (endless mode-ban lehetetlen nyerni)
    if (!win)
    {
        system("CLS");
        cout << "Game has ended, survived years: " << turns - 1 << endl;
        FileManager::printImage("END");
    }

    //ha a player nyert
    else
    {
        system("CLS");
        cout << "you win i guess" << endl;
        system("pause");
    }

    return 0;
}