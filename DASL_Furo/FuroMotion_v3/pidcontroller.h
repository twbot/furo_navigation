#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H


class PIDController
{
    float gain[3];
    float dp;
    float dv;
    float u;
    float sum_e;
    float limit_sum_e;
public:
    void InitGain(float p, float i, float d)
    {
        gain[0] = p;
        gain[1] = i;
        gain[2] = d;
        sum_e = 0;
        SetIntgralLimit(1000)

    }
    void SetIntgralLimit(float l)
    {
        limit_sum_e = l;
    }

    void SetDesiredValue(float dPos, float dVel)
    {
        dp = dpos;
        dv = dVel;
    }
    float Update(float rPos,float rVel)
    {
        float e = (dp -rPos);
        sum_e += e;

        if(sum_e > limit_sum_e)
        {
            sum_e = limit_sum_e;
        }
        u = gain[0] * e + gain[1] * (dv-dVel) + gain[2]*sum_e;


    }
    float GetContolInput()
    {
        return u;
    }

};

#endif // PIDCONTROLLER_H
