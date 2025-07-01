#ifndef CARSTATUS_H
#define CARSTATUS_H

// OBD data storage
typedef struct {
  float engineRPM;
  int vehicleSpeed;
  int coolantTemp;
  int engineLoad;
  int fuelLevel;
  int throttlePos;
  int intakeTemp;
  float mafRate;
} OBDData;

void CarStatus_init();
void CarStatus_task(void *);

#endif // CARSTATUS_H
