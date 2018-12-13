
#include "MotionProfile.h"

/**
 * ProfileNode is a LinkedList of nodes that the motion profile will hit.
 */
struct ProfileNode {
    float position;             // position of node in revolutions
    unsigned long timestamp;    // timestamp of node, in microseconds from start
    ProfileNode* nextNode; // the next node in the profile
};