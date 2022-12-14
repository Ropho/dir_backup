#include "../include/header.hpp"


int main (int argc, char **argv) {


    if (argc != 3) {
        ERROR ("NOT ENOUGH ARGUMENTS CMD LIND");
    }

/////////////////////////////////////////////////
    DIR *src = opendir (argv[1]);
    if  (src == NULL) {
        ERROR ("OPEN SRC DIR ERROR");
    }
    DIR *dst = opendir (argv[2]);
    if  (dst == NULL) {
        ERROR ("OPEN DST DIR ERROR");
    }


    backup (argv[1], src,
            argv[2], dst);

/////////////////////////////////////////////////

    if (closedir (dst) < 0) {
        ERROR ("CLOSE DST DIR ERROR");
    }

    NOTE ("\nGOOD ENDING");
}