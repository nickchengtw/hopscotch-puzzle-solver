#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <math.h>
#include <stack>
#include <map>
#include <assert.h>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <numeric>
#include <set>
#include <queue>

using namespace std;

int counter = 0;
vector<vector<int>> step;

class Hopscotch
{
private:
    bool board[15];
    int remain;
public:
    Hopscotch()
    {
        for (int i = 0; i < 15; i++)
        {
            board[i] = true;
        }
        remain = 15;
    }

    bool get(int R, int C)
    {
        assert(R >= 0 && R <= 4 && R >= C);
        int pos = (int)((R+1)*R)/2 + C;
        return board[pos];
    }

    void set(int R, int C, bool value)
    {
        assert(R >= 0 && R <= 4 && R >= C);
        int pos = (int)((R+1)*R)/2 + C;
        board[pos] = value;
    }

    void output(bool* arr, int& n)
    {
        copy(board, board+15, arr);
        n = remain;
    }

    void load(bool* arr, int n)
    {
        copy(arr, arr+15, board);
        remain = n;
    }

    void remove(int R, int C)
    {
        assert(Hopscotch::get(R, C));
        set(R, C, false);
        remain--;
    }

    void add(int R, int C)
    {
        assert(!Hopscotch::get(R, C));
        set(R, C, true);
        remain++;
    }

    bool canHop(int startR, int startC, int endR, int endC)
    {
        assert(startR >= 0 && startR <= 4 && startR >= startC);
        assert(endR >= 0 && endR <= 4 && endR >= endC);
        if (Hopscotch::get(startR, startC) && !Hopscotch::get(endR, endC))
        {
            if (startR == endR)
            {
                if (startC+2 == endC) if (Hopscotch::get(startR, startC+1)) return true;
                if (startC-2 == endC) if (Hopscotch::get(startR, startC-1)) return true;
            }
            else if (startR+2 == endR)
            {
                if (startC == endC) if (Hopscotch::get(startR+1, startC)) return true;
                if (startC+2 == endC) if (Hopscotch::get(startR+1, startC+1)) return true;
            }
            else if (startR-2 == endR)
            {
                if (startC == endC) if (Hopscotch::get(startR-1, startC)) return true;
                if (startC-2 == endC) if (Hopscotch::get(startR-1, startC-1)) return true;
            }
        }
        return false;
    }

    void hop(int startR, int startC, int endR, int endC)
    {
        assert(startR >= 0 && startR <= 4 && startR >= startC);
        assert(endR >= 0 && endR <= 4 && endR >= endC);
        assert(Hopscotch::canHop(startR, startC, endR, endC));
        Hopscotch::remove(startR, startC);
        Hopscotch::add(endR, endC);
        if (startR == endR)
        {
            if (startC > endC) Hopscotch::remove(startR, startC-1);
            if (startC < endC) Hopscotch::remove(startR, startC+1);
        }
        else if (startR > endR)
        {
            if (startC == endC) Hopscotch::remove(startR-1, startC);
            if (startC > endC) Hopscotch::remove(startR-1, startC-1);
        }
        else if (startR < endR)
        {
            if (startC == endC) Hopscotch::remove(startR+1, startC);
            if (startC < endC) Hopscotch::remove(startR+1, startC+1);
        }
    }

    void show()
    {
        for (int i = 0; i < 5; i++)
        {
            for (int k = 0; k < 4-i; k++)
            {
                printf(" ");
            }
            for (int j = 0; j <= i; j++)
            {
                bool pile = Hopscotch::get(i, j);
                if (pile)
                {
                    printf("! ");
                }
                else
                {
                    printf(". ");
                }
            }
            printf("\n");
        }
    }

    bool win()
    {
        if (remain == 1) return true;
        return false;
    }
};

bool solve(Hopscotch& hopscotch)
{
    if (hopscotch.win()) return true;
    for (int startR = 0; startR < 5; startR++)
    {
        for (int startC = 0; startC <= startR; startC++)
        {
            for (int endR = 0; endR < 5; endR++)
            {
                for (int endC = 0; endC <= endR; endC++)
                {
                    if (hopscotch.canHop(startR, startC, endR, endC))
                    {
                        hopscotch.show();
                        bool board[15];
                        int remain;
                        hopscotch.output(board, remain);

                        printf("try %d,%d to %d,%d\n\n", startR, startC, endR, endC);
                        hopscotch.hop(startR, startC, endR, endC);
                        step.push_back(vector<int>{startR, startC, endR, endC});
                        counter++;
                        if (solve(hopscotch)) return true;
                        hopscotch.load(board, remain);
                        step.pop_back();
                    }
                }
            }
        }
    }
    hopscotch.show();
    printf("Nothing to try. Revert 1 step\n");
    return false;
}

int main(int argc, char const *argv[])
{
    Hopscotch hopscotch = Hopscotch();
    bool finished = false;
    for (int R = 0; R < 5 && !finished; R++)
    {
        for (int C = 0; C <= R; C++)
        {
            hopscotch.remove(R, C);
            counter++;
            finished = solve(hopscotch);
            if (finished) break;
            hopscotch.add(R, C);
        }
    }
    
    printf("\n\n\n\n\n\n\n\n\n\n====================\n");
    printf("Solved! try %d times\n", counter);
    printf("====================\n");
    printf("Showing answer\n\n");
    hopscotch = Hopscotch();
    printf("\nRemove %d,%d\n", step[0][2], step[0][3]);
    hopscotch.remove(step[0][2], step[0][3]);
    hopscotch.show();
    for (auto &&i : step)
    {
        printf("\nFrom %d,%d to %d,%d\n", i[0], i[1], i[2], i[3]);
        hopscotch.hop(i[0], i[1], i[2], i[3]);
        hopscotch.show();
    }
    printf("\nEnd\n");
    return 0;
}