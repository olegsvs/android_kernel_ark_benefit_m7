#ifndef _EH570_DEFINE_JADEOPC_H
#define _EH570_DEFINE_JADEOPC_H

#define JADE_REGISTER_READ             0x01
#define JADE_REGISTER_WRITE            0x02
#define JADE_GET_ONE_IMG               0x03
#define JADE_SENSOR_RESET              0x04

/* trigger signal initial routine*/
#define INT_TRIGGER_INIT               0xa4
/* trigger signal close routine*/
#define INT_TRIGGER_CLOSE              0xa5
/* read trigger status*/
#define INT_TRIGGER_READ               0xa6
/* polling trigger status*/
#define INT_TRIGGER_POLLING            0xa7
/* polling abort*/
#define INT_TRIGGER_ABORT              0xa8
#endif
