#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    int64_t x;
    int64_t y;
    int64_t vx;
    int64_t vy;
} Robot;

#define VECTOR_NAME     RobotVector
#define VECTOR_TYPE     Robot
#define VECTOR_FUN_NAME robot
#include "../utils/vector.h"

#define WIDTH  101
#define HEIGHT 103

typedef struct {
    char map[HEIGHT][WIDTH + 1];
    bool duplicated;
} State;

void stepRobot(Robot *robot, size_t i, void *stateVoid) {
    State *state = (State *)stateVoid;

    robot->x = (robot->x + robot->vx + WIDTH) % WIDTH;
    robot->y = (robot->y + robot->vy + HEIGHT) % HEIGHT;

    if (state->map[robot->y][robot->x] == '#') {
        state->duplicated = true;
        return;
    }

    state->map[robot->y][robot->x] = '#';
}

int main(int argc, char **argv) {
    FILE *input = fopen(argv[1], "r");

    RobotVector robots = vector_robot_new();

    int64_t rx;
    int64_t ry;
    int64_t vx;
    int64_t vy;
    while (fscanf(input, "p=%ld,%ld v=%ld,%ld\n", &rx, &ry, &vx, &vy) != EOF) {
        vector_robot_push(&robots,
                          (Robot){
                                  .x = rx,
                                  .y = ry,
                                  .vx = vx,
                                  .vy = vy,
                          });
    }

    State state = {
            .duplicated = false,
    };

    size_t steps = 0;
    do {
        state.duplicated = false;
        memset(state.map, ' ', sizeof(char) * WIDTH * HEIGHT);
        for (size_t y = 0; y < HEIGHT; y++) {
            state.map[y][WIDTH] = '\0';
        }
        vector_robot_each(&robots, stepRobot, &state);
        steps++;
    } while (state.duplicated);

    for (size_t y = 0; y < 103; y++) {
        printf("%s\n", state.map[y]);
    }
    printf("%ld\n", steps);

    vector_robot_delete(&robots);
    fclose(input);
    return 0;
}
