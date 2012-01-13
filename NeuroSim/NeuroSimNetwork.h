#ifndef NEUROSIM_NETWORK_H
#define NEUROSIM_NETWORK_H

#include "../Library/ExtNetwork/ExtNetwork.h"


ExtNetwork * neurosim_get_ext_network(void);
void neurosim_set_ext_network(ExtNetwork *network);



#endif
