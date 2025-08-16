#include "app/cli_handler.h"

int main(int argc, char** argv) {
    app::CLIHandler cli;
    return cli.run(argc, argv);
}
