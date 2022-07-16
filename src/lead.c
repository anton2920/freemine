/*
   FreeMine - a free Windows minesweeper clone written on C with SDL2
   Copyright © anton2920, 2019-2022

   This file is part of FreeMine.

   FreeMine is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   FreeMine is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with FreeMine. If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdbool.h>

#include "mines.h"


#ifdef __unix__
#define RECORDS_PATH ("records.bin")
#endif


void Write_Records(struct person *p)
{
    FILE *fp;

    if ((fp = fopen(RECORDS_PATH, "wb")) != NULL) {
        fwrite(p, sizeof(struct person), 3, fp);
        fclose(fp);
    } else {
        fprintf(stderr, "Error! Couldn't write records file!\n");
    }
}


void Read_Records(struct person *p)
{
    int i;
    FILE *fp;
    struct person *pp;

    if ((fp = fopen(RECORDS_PATH, "rb")) != NULL) {
        fread(p, sizeof(struct person), 3, fp);
        fclose(fp);
    } else {
        if ((fp = fopen(RECORDS_PATH, "wb")) != NULL) {
            for (pp = p, i = 0; i < 3; ++i, ++pp) {
                strcpy(pp->name, "Anonymous");
                pp->time = 999;
            }
            fwrite(p, sizeof(struct person), 3, fp);

            fclose(fp);
        }
    }
}


bool Check_Records(struct person *p, enum field_size s, int time)
{
    Read_Records(p);

    if ((time < p[0].time && s == small) || (time < p[1].time && s == medium)
        || (time < p[2].time && s == large)) {
        New_Record(p, s, time);
        return true;
    }

    return false;
}


void Print_Records(struct person *p)
{
    int i;

    Read_Records(p);
    printf(" ------------------------------------------------------------\n");
    for (i = 0; i < 3; ++i, ++p) {
        printf("%s:%s %d seconds, %s\n", (!i) ? "Beginner" : (i == 1) ? "Intermediate" : "Expert",
               (!i) ? "\t\t" : (i == 1) ? "\t" : "\t\t\t", p->time, p->name);
    }
    printf(" ------------------------------------------------------------\n");
}


void New_Record(struct person *p, enum field_size s, int time)
{
    char r_n[NAME_SIZE];

    printf("You are the fastest time for %s level. Please enter your name: ", (s == small) ? "beginner" :
                                                                              (s == medium) ? "intermediate"
                                                                                            : "expert");

    scanf("%s", r_n);

    while (getchar() != '\n')
        ;

    strcpy(p[(s == small) ? 0 : (s == medium) ? 1 : 2].name, r_n);
    p[(s == small) ? 0 : (s == medium) ? 1 : 2].time = time;

    Write_Records(p);
}
