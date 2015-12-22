#ifndef C_MATCHGLOBAL_H
#define C_MATCHGLOBAL_H

//int symbolIdDotPattern = 1;
//int symbolIdHyeroglyph = 2;

struct SStatItem
{
    int msecs;
    int dist;
    bool right;
    bool tip;

    SStatItem ()
    {
        msecs = 0;
        dist = 0;
        right = false;
        tip = false;
    }
};

#endif // C_MATCHGLOBAL_H
