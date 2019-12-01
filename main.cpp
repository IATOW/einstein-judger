#include <QCoreApplication>
#include <QProcess>

#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::string;
using std::endl;
using std::to_string;
using std::stoi;

void run();
string input(string prompt);

int main(int argc, char *argv[])
{
    run();

    return 0;
}

string input(string prompt) {
    cout << prompt;
    string ans;
    getline(std::cin, ans);
    return ans;
}

void set_layout(int board[5][5], string layout, int player) {
    if (player == 1) {
        board[0][0] = layout[0] - '0';
        board[0][1] = layout[1] - '0';
        board[0][2] = layout[2] - '0';
        board[1][0] = layout[3] - '0';
        board[1][1] = layout[4] - '0';
        board[2][0] = layout[5] - '0';
    } else if (player == 2) {
        board[4][4] = '0' - layout[0];
        board[4][3] = '0' - layout[1];
        board[4][2] = '0' - layout[2];
        board[3][4] = '0' - layout[3];
        board[3][3] = '0' - layout[4];
        board[2][4] = '0' - layout[5];
    }
}

void print_board(int board[5][5]) {
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            printf("%3d ", board[i][j]);
        }
        putchar('\n');
    }
    putchar('\n');
}

int winner(int board[5][5]) {
    if (board[0][0] < 0) {
        return 2;
    } else if (board[4][4] > 0) {
        return 1;
    }

    int red = 0;
    int blue = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (board[i][j] > 0) {
                red++;
            } else if (board[i][j] < 0) {
                blue++;
            }
        }
    }

    if (blue == 0) {
        return 1;
    } else if (red == 0) {
        return 2;
    }

    return 0;
}

bool check_valid(int board[5][5], int from, int to, int player) {
    return true;
}

void make_move(int board[5][5], int from, int to) {
    int x1 = from / 10;
    int y1 = from % 10;
    int x2 = to / 10;
    int y2 = to % 10;

    int temp = board[x1][y1];
    board[x2][y2] = temp;
    board[x1][y1] = 0;
}

void run () {
    string path1 = input("enter engine 1 path: ");
    string path2 = input("enter engine 2 path: ");
    int amount = std::stoi(input("enter amount: "));

    int win1 = 0;
    int win2 = 0;
    for (int i = 0; i < amount; i++) {
        cout << "-------- GAME#" << i + 1 << " INFO --------" << endl;

        QProcess * engine1 = new QProcess();
        QProcess * engine2 = new QProcess();
        int board[5][5] = {{ 0 }};

        engine1->start(QString(path1.c_str()));
        engine1->waitForStarted();
        engine2->start(QString(path2.c_str()));
        engine2->waitForStarted();

        engine1->write("name?\r\n");
        engine1->waitForBytesWritten();
        engine1->waitForReadyRead();
        string name1 = QString(engine1->readAll()).toStdString();

        engine2->write("name?\r\n");
        engine2->waitForBytesWritten();
        engine2->waitForReadyRead();
        string name2 = QString(engine2->readAll()).toStdString();


//        cout << "name 1: " << name1 << "name 2: " << name2;

        engine1->write("redfirst\r\n");
        engine1->waitForBytesWritten();
        engine1->write("red\r\n");
        engine1->waitForBytesWritten();
        engine1->waitForReadyRead();
        int layout1 = stoi(QString(engine1->readAll()).toStdString());

        engine2->write("redfirst\r\n");
        engine2->waitForBytesWritten();
        engine2->write("blue\r\n");
        engine2->waitForBytesWritten();
        engine2->waitForReadyRead();
        int layout2 = stoi(QString(engine2->readAll()).toStdString());

        cout << "layout 1: " << layout1 << endl << "layout 2: " << layout2 << endl;

        set_layout(board, to_string(layout1), 1);
        set_layout(board, to_string(layout2), 2);

        cout << "initial board:\n";
        print_board(board);

        engine1->write("begin\r\n");
        engine1->waitForBytesWritten();
        engine1->write((to_string(layout2) + "\r\n").c_str());
        engine1->waitForBytesWritten();

        engine2->write("begin\r\n");
        engine2->waitForBytesWritten();
        engine2->write((to_string(layout1) + "\r\n").c_str());
        engine2->waitForBytesWritten();


        int turn = 1;
        char lastmove[100] = { 0 };
        int step = 1;
        while (winner(board) == 0) {
            int dice = rand() % 6 + 1;

            cout << "-------- GAME#" << i + 1 << ", " << "STEP#" << step << " --------" << endl;
            step++;

            cout << "DICE " << dice << endl;

            cout << "\nOUTPUT FROM ENGINE:\n";
            if (turn == 1) {
                engine1->write(lastmove);
                engine1->waitForBytesWritten();
                engine1->write((to_string(dice) + "\r\n").c_str());
                engine1->waitForBytesWritten();
                engine1->waitForReadyRead(-1);
                string move = QString(engine1->readAll()).toStdString();
                cout << move;
                int from, to, d;
                sscanf(move.c_str(), "move %d %d %d", &from, &to, &d);
                sprintf(lastmove, "move %d %d %d\r\n", from, to, d);
                make_move(board, from, to);
            } else if (turn == 2) {
                engine2->write(lastmove);
                engine2->waitForBytesWritten();
                engine2->write((to_string(dice) + "\r\n").c_str());
                engine2->waitForBytesWritten();
                engine2->waitForReadyRead(-1);
                string move = QString(engine2->readAll()).toStdString();
                cout << move;
                int from, to, d;
                sscanf(move.c_str(), "move %d %d %d", &from, &to, &d);
                sprintf(lastmove, "move %d %d %d\r\n", from, to, d);
                make_move(board, from, to);
            }

            turn = 3 - turn;

            cout << "\nBOARD:\n";
            print_board(board);
        }

        if (winner(board) == 1) {
            win1++;
        } else {
            win2++;
        }

        engine1->kill();
        engine2->kill();

        cout << "WIN1: " << win1 << ", WIN2: " << win2 << endl;
    }

}
