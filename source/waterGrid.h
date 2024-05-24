#pragma once

class waterGrid
{
private: 

    int N;
    float a;
    float h;
    float c;
    float dt;

private: 

    

public:

    // *=*=*=*=*=*=*=*=*=*=
    //   Object Creation
    // *=*=*=*=*=*=*=*=*=*=

    waterGrid();
    waterGrid(int N, float a, float c);
    ~waterGrid();

    waterGrid(const waterGrid&) = delete;
    waterGrid(waterGrid&&) = delete;
    waterGrid& operator=(const waterGrid&) = delete;
    waterGrid& operator=(waterGrid&&) = delete;

    void InitGL();

private:

    

};

