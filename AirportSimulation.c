/* Authored by Edala Naveen (edala_naveen@srmap.edu.in) for reference of CSE-325 final project. */
/* Authored on 1 May 2021 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

#define MAXQUEUE 5

typedef enum action { ARRIVE, DEPART } Action;

typedef struct plane {
    int id;
    int tm;
} Plane;

typedef Plane QueueEntry;

typedef struct queue {
    int count;
    int front;
    int rear;
    QueueEntry entry [MAXQUEUE];
} Queue;
void Append(QueueEntry, Queue *);
void CreateQueue(Queue *);
void Serve(QueueEntry *, Queue *);
int QueueSize(Queue *);
bool QueueEmpty(Queue *);
bool QueueFull(Queue *);

void CreateQueue(Queue *q) {
    q -> count = 0;
    q -> front = 0;
    q -> rear = -1;
}

bool QueueFull (Queue *q) {
    return q -> count >= MAXQUEUE;
}

int QueueSize(Queue *q) {
    return q -> count;
}

bool QueueEmpty(Queue *q) {
    return q -> count <= 0;
}

void Append(QueueEntry x, Queue *q) {
    if (QueueFull(q)) {
        printf(" ");
    } else { 
        q -> count++;
        q -> rear = (q -> rear + 1) % MAXQUEUE;
        q -> entry[q -> rear] = x;
    }
}

void Serve(QueueEntry *x, Queue *q) {
    if(QueueEmpty(q)) {
        printf(" ");
    } else {
        q -> count--;
        *x = q -> entry[q -> front];
        q -> front = (q -> front + 1) % MAXQUEUE;
    }
}

void Randomize(void) {
    srand((unsigned int) (time(NULL) % 10000));
}

int PoissonRandom(double expectedvalue) {
    int n = 0;
    double limit;
    double x;
    limit = exp(-expectedvalue);
    x = rand() / (double)INT_MAX;
    while (x > limit) {
        n++;
        x *= rand() / (double)INT_MAX;
    }
    return n;
}

void Start (int *endtime, double *expectarrive, double *expectdepart) {
    bool ok;
    int tmp;
    printf("This program simulates an airport with only one runway.\n One plane can land or depart in each unit of time.\n Upto %d planescan be waiting to land or takeoff at anytime.\n", MAXQUEUE);
    printf("How many units of time will the simulation run? ");
    scanf("%d", endtime);
    Randomize();
    do {
        printf("Expected number of arrivals per unit time? ");
        scanf("%lf", expectarrive);
        printf("Expected number of departures per unit time? ");
        scanf("%lf", expectdepart);
        if (*expectarrive < 0.0 || *expectdepart < 0.0) {
            printf("These numbers must be non-negative.\n");
            ok = false;
        } else if (*expectarrive + *expectdepart > 1.0) {
            printf("The airport will become saturated. Enter 1 to read new numbers: ");
            scanf("%d", &tmp);
            if (tmp == 1) {
                ok = false;
            }
        } else {
            ok = true;
        }
    } while (ok == false);
}

void NewPlane(Plane *p, int *nplanes, int curtime, Action kind) {
    (*nplanes)++;
    p -> id = *nplanes;
    p -> tm = curtime;
    switch(kind) {
        case ARRIVE:
            printf("Plane%3d ready to land.\n", *nplanes);
            break;
        case DEPART:
            printf("Plane%3d ready to takeoff.\n", *nplanes);
            break;
    }
}

void Refuse (Plane p, int *nrefuse, Action kind) {
    switch(kind) {
        case ARRIVE:
            printf("Plane%3d directed to another airport.\n", p.id);
            break;
        case DEPART:
            printf("Plane%3d told to try later.\n", p.id);
            break;
    }
    (*nrefuse)++;
}

void Land(Plane p, int curtime, int *nland, int *landwait) {
    int wait;
    wait = curtime - p.tm;
    printf("%3d : Plane%3d landed; in queue %d units.\n", curtime, p.id, wait);
    (*nland)++;
    *landwait += wait;
}

void Fly(Plane p, int curtime, int *ntakeoff, int *takeoffwait) {
    int wait;
    wait = curtime - p.tm;
    printf("%3d : Plane%3d took off; in queue %d units.\n", curtime, p.id, wait);
    (*ntakeoff)++;
    *takeoffwait += wait;
}

void ldle(int curtime, int *idletime) {
    printf("%3d : Runway is idle.\n", curtime);
    (*idletime)++;
}

void Conclude(int nplanes, int nland, int ntakeoff, int nrefuse, int landwait, int takeoffwait, int idletime, int endtime, Queue *pt, Queue *pl) {
    printf("Simulation has concluded after %d units.\n", endtime);
    printf("Total number of planes processed : %3d \n", nplanes);
    printf("Number of planes landed : %3d \n", nland);
    printf("Number of planes taken off : %3d \n", ntakeoff);
    printf("Number of planes refused use : %3d \n", nrefuse);
    printf("Number left ready to land : %3d \n", QueueSize(pl));
    printf("Number left ready to takeoff : %3d \n", QueueSize(pt));
    if (endtime > 0) {
        printf("Percentage of time runway idle : %6.2f \n", ((double) idletime / endtime) * 100.0);
    }
    if (nland > 0) {
        printf("Average wait time to land : %6.2f \n", (double) landwait / nland);
    }
    if (ntakeoff > 0) {
        printf("Average wait time to takeoff : %6.2f \n", (double) takeoffwait / ntakeoff);
    }
}

int main(void) {
    
    Queue landing, takeoff;
    Queue *pl = &landing;
    Queue *pt = &takeoff;
    Plane plane;
    int curtime;
    int endtime;
    double expectarrive;
    double expectdepart;
    int i;
    int idletime;
    int landwait;
    int nland;
    int nplanes;
    int nrefuse;
    int ntakeoff;
    int pri;
    int takeoffwait;
    
    CreateQueue(pl);
    CreateQueue(pt);
    nplanes = nland = ntakeoff = nrefuse = 0;
    landwait = takeoffwait = idletime = 0;
    Start(&endtime, &expectarrive, &expectdepart);
    for (curtime = 1; curtime <= endtime; curtime++) {
        pri = PoissonRandom(expectarrive);
        for (i = 1; i <= pri; i++) {
            NewPlane(&plane, &nplanes, curtime, ARRIVE);
            if (QueueFull(pl)) {
                Refuse(plane, &nrefuse, ARRIVE);
            } else {
                Append(plane,pl);
            }
        }
        pri = PoissonRandom(expectdepart);
        for (i = 1; i <= pri; i++) {
            NewPlane(&plane, &nplanes, curtime, DEPART);
            if (QueueFull(pl)) {
                Refuse(plane, &nrefuse, DEPART);
            } else {
                Append(plane,pt);
            }
        }
        if (!QueueEmpty(pl)) { 
            Serve(&plane, pl);
            Land(plane, curtime, &nland, &landwait);
        } else if (!QueueEmpty(pt)) {
            Serve(&plane, pt);
            Fly(plane, curtime, &ntakeoff, &takeoffwait);
        } else {
            ldle(curtime, &idletime);
        }
    }
    Conclude(nplanes, nland, ntakeoff, nrefuse, landwait, takeoffwait, idletime, endtime, pt, pl);
    return 0;
}
