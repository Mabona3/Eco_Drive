#ifndef CARSTATUS_H
#define CARSTATUS_H

void FuelSensor_read(void *pvParameters);
void TempSensor_read(void *pvParameters);
void RPMSensor_read(void *pvParameters);

#endif // CARSTATUS_H
