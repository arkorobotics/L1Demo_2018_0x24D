#ifndef ALU_H 
#define ALU_H 

#define M_PI 3.141592653589

extern float rotationMatrix[4][4];
extern float inputMatrix[4][1];
extern float outputMatrix[4][1]; 

float radians(uint16_t angle);
int realtoint(float oldval, float oldmin, float oldmax, float newmin, float newmax);

void alu_rot(float x, float y, float z);
void alu_calc_rot_matrix(float angle, float u, float v, float w);

#endif