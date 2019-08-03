#ifndef ALGOVISUALIZE_MAZEGENERATOR_H
#define ALGOVISUALIZE_MAZEGENERATOR_H


#include "../Field.h"

class MazeGenerator {
protected:
    Field &field;

    virtual void prepare();

public:
    explicit MazeGenerator(Field &field);

    virtual void generate_maze();
};


#endif //ALGOVISUALIZE_MAZEGENERATOR_H
