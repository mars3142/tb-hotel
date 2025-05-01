#pragma once

#include "GC/led_matrix.h"

namespace gc
{
    class Game
    {
    public:
        explicit Game(const LedMatrix& led_matrix)
            : led_matrix(led_matrix)
        {
        }

    private:
        const LedMatrix& led_matrix;
    };
}
